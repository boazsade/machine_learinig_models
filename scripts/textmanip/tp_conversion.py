import logging
from functools import partial

from pyspark.sql.functions import col, udf
from pyspark.sql.types import TimestampType, DoubleType

__all__ = ["rename_columns", "do_convertion", "convert_column"]

###############################################################################
###############################################################################
#####       private functions
###############################################################################
###############################################################################
from datetime import datetime


def get_missing_list(att):
    try:
        if not att:
            return None
        if att['missing_list']:
            return att['missing_list']
        else:
            return None
    except:
        return None


def date_convert(fmt, dstr):
    ret = datetime.strptime(dstr, fmt)
    if ret is None:
        raise ValueError("failed to convert")
    return ret


def convert2date(fmt, missing, istr):
    if istr is None:
        return None
    if missing:
        if istr in missing:
            return None
    try:
        val = date_convert(fmt=fmt, dstr=istr)
        return val
    except Exception as e:
        logging.warning("converting failed for {0} with format {1} error {2}".format(istr, fmt, e))
        return None


def convert2double(missing, istr):
    def is_missing(miss, v):
        if miss is None:
            return False
        elif str(v) in miss:
            return True
        else:
            return False

    if is_missing(missing, istr):
        return None
    try:
        val = float(istr)
        return val
    except:
        return None


def convertdate2number(frmt, missing, istr):
    import time
    date = convert2date(fmt=frmt, missing=missing, istr=istr)
    if date is not None:
        return float(time.mktime(date.timetuple()))
    else:
        return None


def date2number(df, column):
    import time

    def num_convert(x):
        if x:
            return float(time.mktime(x.timetuple()))
        else:
            return None

    f = udf(lambda x: num_convert(x), DoubleType())
    total = df.select(column).count()
    good1 = df.select(column).na.drop().count()
    df = df.withColumn(column, f(col(column)))
    good2 = df.select(column).na.drop().count()
    total_mis = total - good1
    err = total - (good1 - good2)
    valid = good2
    stats = {'name': column, 'missing': total_mis, 'errors': err, 'valid': valid}
    return df, stats


def convert_date(dataframe, column, conversion):
    from pyspark.sql.types import DateType

    dataClass = conversion['dataClass']
    t = dataframe.schema[str(column)].dataType
    if isinstance(t, TimestampType) or isinstance(t, DateType):
        if dataClass == 'numeric':
            return date2number(df=dataframe, column=column)
        else:
            logging.info("column {} is date column, not applying any conversion".format(column))
            return no_action(dataframe=dataframe, column=column, conversion=conversion)

    if dataClass == 'numeric':
        return convert_numeric(column=column, dataframe=dataframe, conversion=conversion)
    total = dataframe.select(column).count()
    good1 = dataframe.select(column).na.drop().count()
    tf = conversion['attr']['format']
    miss = get_missing_list(conversion['attr'])
    logging.info("converting date column with format {}".format(tf))
    func = udf(lambda x, fmt=tf: convert2date(fmt, miss, x), TimestampType())
    df = dataframe.withColumn(column, func(col(column)))
    good2 = df.select(column).na.drop().count()
    total_mis = total - good1
    err = total - (good1 - good2)
    valid = good2
    stats = {'name': column, 'missing': total_mis, 'errors': err, 'valid': valid}

    return df, stats


def no_action(column, dataframe, conversion):
    return dataframe, None


def conver2numeric(column, dataframe, conversion):
    missing = conversion['attr']['missing_list']
    func = udf(lambda x, tf=missing: convert2double(tf, x), DoubleType())
    return dataframe.withColumn(column, func(col(column)))


def convert2numeric_date(dataframe, column, conversion):
    frt = conversion['attr']['format']
    missing = get_missing_list(att = conversion['attr'])
    f = partial(convertdate2number, frt, missing)
    func = udf(f, DoubleType())
    return dataframe.withColumn(column, func(col(column)))


def convert_numeric(column, dataframe, conversion):
    total = dataframe.select(column).count()
    good1 = dataframe.select(column).na.drop().count()
    if conversion['dataType'] == 'date':
        logging.info("converting column to date type {}".format(column))
        dataframe = convert2numeric_date(column=column, dataframe=dataframe, conversion=conversion)
    else:
        logging.info("converting column to number type {}".format(column))
        dataframe = conver2numeric(column=column, dataframe=dataframe, conversion=conversion)

    good2 = dataframe.select(column).na.drop().count()
    mis = total - good1
    err = total - (good1 - good2)
    valid = good2
    stats = {'name': column, 'missing': mis, 'errors': err, 'valid': valid}
    return dataframe, stats


def string_convertion(column, dataframe, conversion):
    from pyspark.sql.types import StringType

    def apply_missing(miss, entry):
        if not entry:
            return None
        else:
            s = str(entry)
            if miss:
                if s in miss:
                    return None
            return s

    logging.info("running string conversion on column {}".format(column))
    missing = get_missing_list(conversion['attr'])
    if missing:
        logging.info("missing values for string column {} is {}".format(column, missing))
    else:
        logging.info("no missing list for string column {}".format(column))
        try:
            if isinstance(dataframe.schema[str(column)].dataType, StringType):
                logging.info("column {} is string column, not applying any changes".format(column))
                return no_action(column=column, dataframe=dataframe, conversion=conversion)
        except Exception as e:
            logging.error("failed to get information about the type of the column: {}".format(e))
            raise e
    f = partial(apply_missing, missing)
    func = udf(f, StringType())
    return dataframe.withColumn(column, func(col(column))), None


def apply_conversion(column, dataframe, action, schema):
    return action(column=column, dataframe=dataframe, conversion=schema)


conversion_action = {'string': string_convertion, 'number': convert_numeric, 'date': convert_date, 'label': no_action}


###############################################################################
###############################################################################
###############     public functions
###############################################################################
###############################################################################

def convert_column(df, schema, newschema):
    colname = schema['colid']
    datatype = schema['dataType']
    if datatype in conversion_action:
        df, st = apply_conversion(column=colname, dataframe=df, action=conversion_action[datatype], schema=schema)
        logging.info("converting column {0} and we have new entry {1}".format(colname, schema))
        newschema.append(schema)
        logging.info("the updated schema is now {}".format(newschema))
        return df, st
    else:
        return df, None


def rename_columns(dataset, new_names):
    oldColumns = dataset.schema.names
    if len(oldColumns) != len(new_names):
        logging.error("number of columns in schema {0} do not match the actual number {1}".format(len(oldColumns),
                                                                                                  len(new_names)))
        raise ValueError("number of columns do no match actual number of columns in data set")
    else:
        logging.info("apply new names {}".format(new_names))
    df = reduce(lambda df, idx: df.withColumnRenamed(oldColumns[idx], new_names[idx]),
                xrange(len(oldColumns)), dataset)
    return df


def do_convertion(dataframe, schema):
    initStats = {}
    newschema = []
    for colSchema in schema:
        dataframe, st = convert_column(schema=colSchema, df=dataframe, newschema=newschema)
        if st is not None:
            initStats.update({st['name']: st})
    # dataframe.show()
    return dataframe, initStats
