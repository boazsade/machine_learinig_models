import json
import logging

from pyspark.sql.functions import udf, col
from pyspark.sql.types import DoubleType
from datetime import datetime
import sys
import os
import shutil


def columnType(colName, df):
    for c in df.dtypes:
        if c[0] == colName:
            return c[1]
        return None


class StatsEncoder(json.JSONEncoder):
    def default(self, o):
        from datetime import datetime
        if isinstance(o, datetime):
            return o.isoformat()
        return json.JSONEncoder.default(self, o)


def load_files(files, context, sep=',', quote='"', esp='"', hdr='false', guess_schema='false'):
    logging.info("processing input from files {}".format(files))
    return context.read.csv(path=files, header=hdr, inferSchema=guess_schema, sep=sep, quote=quote, escape=esp)


def csvSettings(conf):
    try:
        settings = conf['datasource']['settings']
    except:
        logging.info("no CSV settings from management - using default")
        settings = {'separator': ',', 'escape': '"', 'quote': '"', 'firstRow': 1}
    return settings


def rename_exported(path, source):
    for f in os.listdir(path):
        if f.endswith('.csv'):
            sf = str("{0}/{1}".format(path, f))
            df = str("{0}/{1}".format(path, source))
            logging.info("rename exportted file from {0} --> {1}".format(sf, df))
            os.rename(sf, df)
            return True
    return False


def convertDate2double(date, sd=datetime(1, 1, 1)):
    if date is None:
        return sys.float_info.max

    if not isinstance(date, datetime):
        try:
            date = datetime.fromtimestamp(date)
        except ValueError as ve:
            logging.error('convert to datetime failed: {}'.format(ve))
            raise

    td = date - sd
    return (td.microseconds + (td.seconds + td.days * 86400.0)) / 10 ** 6


def date2numbers(dataframe, colName):
    func = udf(lambda x: convertDate2double(x), DoubleType())
    return dataframe.withColumn(colName, func(col(colName)))


def string2number(dataframe, colName):
    from functools import partial
    from pyspark.sql import functions as F 

    uniq = dataframe.select(F.collect_set(colName).alias(colName)).first()[colName]
    mapping = {k: v for v, k in enumerate(uniq)}

    def my_func(m, x):
        if x is None:
            return sys.float_info.max
        elif x not in m:
            raise RuntimeError('failed to find enty for {}'.format(x))
        else:
            return float(m[x])

    func = udf(partial(my_func, mapping), DoubleType())
    df = dataframe.withColumn(colName, func(col(colName)))
    logging.info("lookup table for {0}: {1}".format(colName, mapping))
    return df, mapping


def number2number(dataframe, colName):
    func = udf(lambda x: x if x is not None else sys.float_info.max, DoubleType())
    return dataframe.withColumn(colName, func(col(colName)))


def numericConversion(df, dclass, dtype, name):
    if dtype == 'date':
        return date2numbers(dataframe=df, colName=name), None
    elif dtype == 'string':
        return string2number(dataframe=df, colName=name)
    elif dtype == 'number':
        return number2number(dataframe=df, colName=name), None
    else:
        return df


# in this function we are converting any column that is not
# numeric and not label to numeric value. note that we are 
# doing so, so that we would use from now on for the next steps 
# the input as numeric
def convert2numeric(sdf, schema):
    lookup_tbl = {}
    for column in schema:
        dc = column['dataClass']
        dt = column['dataType']
        name = column['colid']
        if dc != 'label':
            logging.info("converting {} to numeric column".format(name))
            sdf, lt = numericConversion(df=sdf, dclass=dc, dtype=dt, name=name)
            if lt is not None:
                lookup_tbl[name] = lt
        else:  # dc == 'label':
            logging.warning("dropping column {} - label column".format(name))
            sdf = sdf.drop(name)

    return sdf, lookup_tbl


def make_index_column(df, name):
    from pyspark.sql.window import Window as W
    from pyspark.sql import functions as F
    df = df.withColumn(name, F.monotonically_increasing_id())
    windowSpec = W.orderBy(name)
    return df.withColumn(name, F.row_number().over(windowSpec))


def load_and_index(files, context, sep=',', quote='"', esp='"', hdr='false', guess_schema='false'):
    df = load_files(files=files, context=context, sep=sep, quote=quote, esp=esp, hdr=hdr, guess_schema=guess_schema)
    return make_index_column(df=df, name='index')


def export_column(path, name, df, indexCol):
    if name != indexCol:
        exdf = df.select(name, indexCol)
        output = str("{0}/{1}".format(path, name))
        logging.info("exporting columns {0} and {1} to {2}".format(name, indexCol, output))
        shutil.rmtree(path=output, ignore_errors=True)
        exdf.repartition(1).write.csv(path=output, sep=' ', quote='"', escape='"', mode='overwrite', header='false')
        rename_exported(path=output, source=name)


def export2file(df, conf, schema):
    if conf['format'] == 'csv':
        shutil.rmtree(path=conf['path'], ignore_errors=True)
        logging.warning("exporting to directory {}".format(conf['path']))
#        df2 = make_index_column(df=df, name="index")
        # df.withColumn("index", monotonically_increasing_id())
        # first export all the data to a signle CSV file
        df.repartition(1).write.csv(path=conf['path'], sep=',', quote='"', escape='"', mode='overwrite', header='true')
        df3, lt = convert2numeric(sdf=df, schema=schema)
#        df3 = make_index_column(df=df3, name="index")
        # df3.withColumn('index', monotonically_increasing_id())
        numpath = str("{0}/{1}".format(conf['path'], 'numeric_table'))
        logging.warning("exporting numeric table to {}".format(numpath))
        df3.repartition(1).write.csv(path=numpath, sep=' ', quote='"', escape='"', mode='overwrite', header='true')
        rename_exported(path=numpath, source='numeric_table')
        # then for each of the columns export the data for its own file
        for c in df3.columns:
            export_column(path=conf['path'], name=c, df=df3, indexCol='index')
        logging.warning("successfully finish to export all data to directory {}".format(conf['path']))
        return lt
    else:
        logging.warning("no support for export of type {}".format(conf['format']))
        return None


def loadJson(source):
    try:
        if len(source) < 256:
            ji = open(source).read()
            command = json.loads(ji)
            return command
        else:
            command = json.loads(source)
            return command
    except:
        raise


def getInutFiles(cla):
    try:
        cmd = loadJson(cla)
        return cmd['datasource']['files']
    except:
        return None


def formatError(err, infiles):
    msg = {"error": str(err), "msg": "failed to process command", "files": infiles}
    j = json.dumps({"errors": msg}, sort_keys=True, indent=4, separators=(',', ': '))
    print("\n{}".format(j))

