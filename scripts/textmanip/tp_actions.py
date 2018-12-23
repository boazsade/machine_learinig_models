
from ba_conversion import convert_column
from pyspark.sql.functions import udf, lit, col, instr, concat
import logging


__all__ = [ "handle_actions" ]
##################################################################################
# internal functions

def matchColumn(colid, columns):
    for col in columns:
        if colid == col['colid']:
            return col
    return None

def drop_column(dataframe, columns):
    return dataframe.drop(columns)

def do_join(dataframe, col1, col2, sep, newCol):
    if not col1 in dataframe.columns:
        logging.warning("no such column {}".format(col1))
        return dataframe
    if not col2 in dataframe.columns:
        logging.warning("no such column {}".format(col2))
        return dataframe
    logging.info("joining {0} and  {1} into  {2}".format(col1, col2, newCol))

    df1 = dataframe.withColumn(col1, concat(col1, lit(sep), col2))
    df2 = df1.withColumnRenamed(col1, newCol)
    df1 = df2.drop(col2)
    return df1


#from pyspark.sql.functions import split
from pyspark.sql.types import StringType

def do_split(dataframe, colName, newCol1, newCol2, splitBy):
    if not colName in dataframe.columns:
        logging.warning("cannot split column {} - no such column".format(colName))
        return dataframe
    else:
        logging.info("spliting {0} into {1} and {2}".format(colName, newCol1, newCol2))
    scol1 = udf(lambda s: s[0: s.find(splitBy)] if s is not None else None, StringType())
    scol2 = udf(lambda s: s[s.find(splitBy)+1:] if s is not None else None, StringType())

    dataframe = dataframe.withColumn(newCol1, scol1(colName))
    dataframe = dataframe.withColumn(newCol2, scol2(colName))
    dataframe = dataframe.drop(colName)
    return dataframe

def filterin_streq(dataframe, column, mstr, coltype):
    if coltype is None:
        logging.warning("column {0} type is unknown, cannot filter by string {1}".format(column, mstr))
        return dataframe
    if coltype['dataType'] == 'string':
        logging.info("apply filter equal by {0} on column {1}".format(mstr, column))
        dataframe = dataframe.filter(col(column) == mstr)
    else:
        logging.warning("cannot apply filter by {0} on column {1} not string column".format(mstr, column))
    return dataframe

def filterin_lt(dataframe, column, value, coltype):
    if coltype is None:
        logging.warning("column {0} type is unknown, cannot filter by little than {1}".format(column, value))
        return dataframe
    if coltype['dataType'] == 'number':
        logging.info("apply filter little than by {0} on column {1}".format(value, column))
        dataframe = dataframe.filter(col(column) < value)
    else:
        logging.warning("cannot apply filter by {0} on column {1} not number column".format(value, column))
    return dataframe

def filterin_contains(dataframe, column, substr, coltype):
    if coltype is None:
        logging.warning("column {0} type is unknown, cannot filter by contains {1}".format(column, substr))
        return dataframe
    if coltype['dataType'] == 'string':
        logging.info("apply filter contains by {0} on column {1}".format(substr, column))
        dataframe = dataframe.filter(instr(col(column), substr) != 0)
    return dataframe

def filterin_eq(dataframe, column, value, coltype):
    if coltype is None:
        logging.warning("column {0} type is unknown, cannot filter by equals {1}".format(column, value))
        return dataframe
    if coltype['dataType'] == 'number':
        val = float(value)
        logging.info("apply filter equal by {0} on column {1}".format(value, column))
        dataframe = dataframe.filter(col(column) == val)
    else:
        logging.warning("cannot apply filter by {0} on column {1} not number column".format(value, column))
    return dataframe

def filterin_gt(dataframe, column, value, coltype):
    if coltype is None:
        logging.warning("column {0} type is unknown, cannot filter by greater than {1}".format(column, value))
        return dataframe

    if coltype['dataType'] == 'number':
        logging.info("apply filter greater than by {0} on column {1}".format(value, column))
        dataframe = dataframe.filter(col(column) > value)
    else:
        logging.warning("cannot apply filter by {0} on column {1} not number column".format(value, column))
    return dataframe


def dateRange(df, column, start, end):
    from datetime import datetime
    try:
        startDate = start if isinstance(start, datetime) else datetime.strptime( start, "%Y-%m-%dT%H:%M:%S.%f" )
        endDate = end if isinstance(end, datetime) else datetime.strptime( end, "%Y-%m-%dT%H:%M:%S.%f" )
        return df.filter((col(column) > startDate) & (col(column) < endDate))
    except Exception as e:
        logging.error("failed to apply date range converion for {0} to {1} on column {2}, error {3}".format(start, end, col, e))
        return df


def filterin_inrange(dataframe, column, start, end, coltype):
    if coltype is None:
        logging.warning("column {0} type is unknown, cannot filter by range {1} {2}".format(column, start, end))
        return dataframe
    if coltype['dataType'] == 'date':
        logging.info("filtering column {0} date on values between {1} and {2}".format(column, start, end))
        return dateRange(df=dataframe, column=column, start=start, end=end)
    elif coltype['dataType'] == 'number':
        logging.info("filtering column {0} number on values between {1} and {2}".format(column, start, end))
        dataframe = dataframe.filter((col(column) > start) & (col(column) < end))
    else:
        logging.warning("cannot apply filter by {0} {1} on column {2} not number or date column".format(start, end, column))
    return dataframe


def remove_entry(from_schema, key, value):
    for item in from_schema:
        if item[key] == value:
            from_schema.remove(item)
            return from_schema
    return from_schema


def update_schema(currSchema, entry, value):
    remove_entry(from_schema=currSchema, key=entry, value=value)


def newSchemaEntry(attr, col, dataframe, schema):
    sc = {'colid': col}
    sc.update(attr)
    return convert_column(df=dataframe, schema=sc, newschema=schema)

##################################################################################

def acdrop_column(args, dataframe, sc):
    logging.info("running action drop column {}".format(args['attr']['colid']))
    update_schema(currSchema=sc, entry='colid', value=args['attr']['colid'])
    return drop_column(dataframe=dataframe, columns=args['attr']['colid']), None

def acjoin_colmns(args, dataframe, sc):
    logging.info("running action join columns {0} {1} {2}".format(
        args['attr']['colid']['first'],
        args['attr']['colid']['second'],
        args['attr']['colid']['target']))
    update_schema(currSchema=sc, entry='colid', value=args['attr']['colid']['first'])
    update_schema(currSchema=sc, entry='colid', value=args['attr']['colid']['second'])
    df = do_join(dataframe=dataframe, 
            col1=args['attr']['colid']['first'], 
            col2=args['attr']['colid']['second'], 
            sep=args['attr']['separator'], 
            newCol=args['attr']['colid']['target'])
    return newSchemaEntry(attr=args['attr']['attr'], col=args['attr']['colid']['target'], dataframe=df, schema=sc)

def acsplit_colmns(args, dataframe, sc):
    logging.info("running action split columns {0} {1} {2}".format(
        args['attr']['colid']['first'],
        args['attr']['colid']['second'],
        args['attr']['colid']['source']))
    update_schema(currSchema=sc, entry='colid', value=args['attr']['colid']['source'])
    df = do_split(dataframe=dataframe, 
            newCol1=args['attr']['colid']['first'], 
            newCol2=args['attr']['colid']['second'], 
            splitBy=args['attr']['split']['value'], 
            colName=args['attr']['colid']['source'])
    df, s = newSchemaEntry(attr=args['attr']['attr'], col=args['attr']['colid']['first'], dataframe=df, schema=sc)
    return newSchemaEntry(attr=args['attr']['attr'], col=args['attr']['colid']['second'], dataframe=df, schema=sc)

def acfilter_rows_streq(args, dataframe, sc):
    logging.info("running action filter by string columns {0} {1}".format(
        args['attr']['colid'],
        args['attr']['value']))
    return filterin_streq(dataframe=dataframe, 
                            column=args['attr']['colid'],
                            mstr=args['attr']['value'],
                            coltype=matchColumn(colid=args['attr']['colid'], columns=sc)), None


def acfilter_rows_lt(args, dataframe, sc):
    logging.info("running action filter little than columns {0} {1}".format(
        args['attr']['colid'],
        args['attr']['value']))
    return filterin_lt(dataframe=dataframe,
                         column=args['attr']['colid'],
                         value=args['attr']['value'],
                         coltype=matchColumn(colid=args['attr']['colid'], columns=sc)), None

def acfilter_rows_str_cont(args, dataframe, sc):
    logging.info("running action filter sub string columns {0} {1}".format(
        args['attr']['colid'],
        args['attr']['value']))
    return filterin_contains(dataframe=dataframe,
            column=args['attr']['colid'],
            substr=args['attr']['value'],
            coltype=matchColumn(colid=args['attr']['colid'], columns=sc)), None

def acfilter_row_eq(args, dataframe, sc):
    logging.info("running action filter equal columns {0} {1}".format(
        args['attr']['colid'],
        args['attr']['value']))
    return filterin_eq(dataframe=dataframe,
            column=args['attr']['colid'],
            value=args['attr']['value'],
            coltype=matchColumn(colid=args['attr']['colid'], columns=sc)), None

def acfilter_rows_gt(args, dataframe, sc):
    logging.info("running action filter greater than columns {0} {1}".format(
        args['attr']['colid'],
        args['attr']['value']))
    return filterin_gt(dataframe=dataframe,
            column=args['attr']['colid'],
            value=args['attr']['value'],
            coltype=matchColumn(colid=args['attr']['colid'], columns=sc)), None

def acfilter_rows_in(args, dataframe, sc):
    logging.info("running action filter range columns {0} {1} {2}".format(
        args['attr']['colid'],
        args['attr']['from'],
        args['attr']['to']))
    return filterin_inrange(dataframe=dataframe,
            column=args['attr']['colid'],
            start=args['attr']['from'],
            end=args['attr']['to'],
            coltype=matchColumn(colid=args['attr']['colid'], columns=sc)), None

def acconvert_column(args, dataframe, sc):
    update_schema(currSchema=sc, entry='colid', value=args['attr']['colid'])
    return convert_column(df=dataframe, schema=args['attr'], newschema=sc)

def name_action():
    action_mapping = { "col.drop": acdrop_column, 
            'col.split': acsplit_colmns, 
            'col.type': acconvert_column,
            'col.join': acjoin_colmns,
            'filter.strmatch': acfilter_rows_streq,
            'filter.lt': acfilter_rows_lt,
            'filter.contains': acfilter_rows_str_cont,
            'filter.eq': acfilter_row_eq,
            'filter.gt': acfilter_rows_gt,
            'filter.range': acfilter_rows_in}
    return action_mapping

def apply_action(action, df, oper, schema):
    return oper(args=action, dataframe=df, sc=schema)

##################################################################################
##################################################################################
# axternally accessed functions
##################################################################################
##################################################################################


def handle_actions(dataframe, actions, generatedSchema):
    initStats = {}
    try:
        act = actions['actions']
    except KeyError as e:
        logging.info("no actions were found ({})".format(e))
        return dataframe, None
    if len(actions) == 0:
        return dataframe, None
    else:
        am = name_action()
        for action in act:
            if action['action'] in am:
                logging.info ("running action {}".format(action['action']))
                dataframe, cs = apply_action(action=action, df=dataframe, oper=am[action['action']], schema=generatedSchema)
                logging.info ("updated action is now {}".format(generatedSchema))
                if cs is not None:
                    initStats.update({cs['name']: cs})
            else:
                logging.warning ("action {}, not found in legal action list".format(action['action']))
    return dataframe, initStats

