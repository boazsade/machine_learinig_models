import logging
import os


def init_log(name, level=logging.INFO):
    cwd = os.path.dirname(os.getcwd())
    logfile = cwd + "/logs/" + name + ".log"
    logging.basicConfig(filename=logfile, level=level, format='%(levelname)s %(asctime)s : %(message)s')


