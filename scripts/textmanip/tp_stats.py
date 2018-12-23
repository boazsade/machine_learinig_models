import logging
from functools import partial

from pyspark.sql.functions import col, udf, lag
from pyspark.sql.types import DoubleType
from pyspark.sql.window import Window

__all__ = ["columnStats"]


###############################################################################
###############################################################################

def population_hist(dataframe, column, lim):
    logging.info("frequency histogram for column {0} with {1} entries".format(column, lim))
    tdf = dataframe.select(column).na.drop()
    return tdf.groupby(column).count().limit(lim).collect()


def column_summary(dataframe, col, pstats):
    ddf = dataframe.select(col)
    c1 = ddf.count()
    ddf2 = ddf.dropna()
    c2 = ddf2.count()
    missing = c1 - c2
    errors = 0
    valid = c2
    if pstats is not None:
        if pstats['errors'] < missing:
            errors = pstats['errors']
            missing = missing - errors

    ret = {'total': c1, 'missing': missing, 'errors': errors, 'valid': valid, 'unique': ddf2.distinct().count()}
    logging.info("column summey {}".format(ret))
    return ret


def numerical_stats(dataframe, column, dclass, datat):
    if dclass == 'numeric':
        ns = dataframe.describe(column).collect()
        m = [element[1] for element in ns if element[0] == 'mean']
        s = [element[1] for element in ns if element[0] == 'stddev']
        if m[0] is None and s[0] is None:
            return None
        else:
            logging.info("numerical status for column {0} {1}".format(column, ns))
            return ns
    else:
        return None


def binsCount(schema):
    try:
        return schema['attr']['bin_count']
    except:
        return 0


def uniqueLimit(scheme):
    try:
        return scheme['attr']['unique_limit']
    except:
        return 0


def categorial_stats(dataframe, column, dclass, lim):
    if dclass == 'numeric':
        return None
    elif dclass == 'interval':
        return None
    elif dclass == 'label':
        return None
    else:
        if lim > 0:
            return population_hist(dataframe, column, lim)
        else:
            return None


def deltaStatsPrep(df, column, newColumn):
    my_window = Window.partitionBy().orderBy(column)
    df = df.withColumn(newColumn, lag(col(column)).over(my_window))
    return df


def numericDiff(fact, curr, prev):
    if prev is None:
        return 0
    if curr is None:
        return 0
    return round((curr - prev), fact)


def numericDeltasStats(df, column, setup):
    df = deltaStatsPrep(df=df, column=column, newColumn="prev_value")
    func = partial(numericDiff, int(setup))
    f = udf(func, DoubleType())
    df = df.withColumn("diff", f(col(column), df.prev_value))
    s = population_hist(dataframe=df, column="diff", lim=100)
    numStats = numerical_stats(dataframe=df, column='diff', dclass='numeric', datat='number')
    return s, numStats


def calcualteDeltaTime(factor, end, start):
    if start is None:
        return None
    if end is None:
        return None
    return float(factor((end - start).total_seconds()))


res_factor = {'ms': lambda x: round(x, 3),
              'seconds': lambda x: round(x, 0),
              'minutes': lambda x: round(x, 0) / 60.0,
              'hours': lambda x: round(x, 0) / 3600.0,
              'days': lambda x: round(x, 0) / 86400.0}


def dateDeltasStats(df, column, resolution):
    df2 = df.select(column)
    df2 = deltaStatsPrep(df=df2, column=column, newColumn="prev_value")
    fact = res_factor[resolution]
    func = partial(calcualteDeltaTime, fact)
    f = udf(func, DoubleType())
    df2 = df2.withColumn("diff", f(col(column), df2.prev_value))
    dateStats = population_hist(dataframe=df2, column="diff", lim=100)
    numStats = numerical_stats(dataframe=df2, column='diff', dclass='numeric', datat='number')
    return dateStats, numStats


def deltaStats(df, column, schema):
    #colName = schema['colid']
    dataType = schema['dataType']
    dataClass = schema['dataClass']
    attr = schema['attr']

    if dataClass == 'interval':
        if dataType == 'date':
            return dateDeltasStats(df=df.select(column), column=column, resolution=attr['resolution'])
        elif dataType == 'number':
            return numericDeltasStats(df=df.select(column), column=column, setup=attr['resolution'])
        else:
            return None, None
    else:
        return None, None


def make_histogram(dataframe, column, bins):
    return dataframe.select(column).rdd.flatMap(lambda x: x).histogram(bins)


###############################################################################
##########      use functions under this
###############################################################################

def columnStats(colSchema, df, colStats):
    names = df.schema.names
    colName = colSchema['colid']
    dataType = colSchema['dataType']
    dataClass = colSchema['dataClass']
    logging.info("building stats for column {}".format(colName))
    cs = None
    if colName not in names:
        return cs
    if dataClass == 'label':
        return cs
    if colName in colStats:
        cs = colStats[colName]
    missing = column_summary(dataframe=df, col=colName, pstats=cs)
    stats = {'colid': colName, 'summary': missing}
    ns = numerical_stats(dataframe=df, column=colName, dclass=dataClass, datat=dataType)
    if ns is not None:
        stats.update({'stats': ns})
        stats.update({'histogram': make_histogram(dataframe=df, column=colName, bins=binsCount(schema=colSchema))})
    cs = categorial_stats(dataframe=df, column=colName, dclass=dataClass, lim=uniqueLimit(scheme=colSchema))
    if cs is not None:
        stats.update({'stats': cs})
    ds, ns = deltaStats(df=df, column=colName, schema=colSchema)
    if ds is not None:
        stats.update({'freq': ds})
    if ns is not None:
        stats.update({'stats': ns})
    return stats
