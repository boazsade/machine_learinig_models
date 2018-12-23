#!/usr/bin/env python

import logging

from pyspark.sql import SparkSession

from tp_actions import handle_actions
from tp_conversion import rename_columns
from tp_logging import init_log
from tp_utilities import *


def column_type(dataframe, column):
    result = [element[1] for element in  dataframe.dtypes if element[0] == column]
    if result[0] is not None:
        return result[0]
    else:
        return None


def format_output(dataframe):
    return {'rows': []}


def do_export(dataframe, settings, schema):
    try:
        export2file(df=dataframe, conf=settings['export'], schema=schema)
    except Exception as e:
        logging.warning("no export is required or failed to export ({})".format(e))
        pass


# we are expecting to get a command here after
# we parse it with json parser - if this failed
# we would raise an error that this is not valid
def process_command(command):
    if command is None:
        raise ValueError("invalid command from front end")

    spark = SparkSession.builder.appName("tp_dataprep").master("local[16]").getOrCreate()
    
    settings = csvSettings(command)
    df = load_files(files=command['datasource']['files'], 
                    context=spark, sep=settings['separator'],
                    esp=settings['escape'], quote=settings['quote'],
                    hdr=settings['firstRow'] > 0)
    names = []
    for cn in command['schema']['cols']:
        names.append(cn['colid'])
    logging.info("processing columns {}".format(names))
    df = rename_columns(dataset=df, new_names=names)
    df = make_index_column(df=df, name='index')
    newSchema = []
    df, stats = handle_actions(dataframe=df, actions=command, generatedSchema=newSchema)
    logging.info("the schema after running actions is {}".format(newSchema))
    output = format_output(dataframe=df)
    if len(newSchema) == 0:
        logging.warning("we don't have a schema to return back - this is a serious issue!!")
    j = json.dumps({'schema': output}, sort_keys=True, indent=4, separators=(',', ': '), cls=StatsEncoder)
    do_export(dataframe=df, settings=command, schema=newSchema)
    print ("\n{}".format(j))
    return True


def process(arg):
    # if the input is short assumes file
    logging.info("starting data prep with command input\n{}".format(arg))
    command=loadJson(arg)
    process_command(command)


def main(argv=sys.argv[1:]):
    if len(argv) != 1:
        err=ValueError("invalid command line args: missing input")
        formatError(err=err, infiles=None)
        return -1
    try:
        init_log(name="model_prep")
        logging.warning("\n========================\n========================\n\tstarting data prep\n========================\n========================\n")
    except Exception as e:
        formatError(err=e, infiles=None)
        return -1
#    process(argv[0])
#    return 0
    try:
        process(argv[0])
        logging.warning("\n========================\n========================\n\tfinish data prep\n========================\n========================\n")
        return 0
    except Exception as e:
        logging.error("processing failed with error {}".format(e))
        inf = getInutFiles(argv[0])
        formatError(err=e, infiles=inf)
        return -1


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

