#!/usr/bin/env python
import numpy as np
import sys

def outliers_modified_z_score(ys):
    threshold = 4.5

    median_y = np.median(ys)
    median_absolute_deviation_y = np.median([np.abs(y - median_y) for y in ys])
    modified_z_scores = [0.6745 * (y - median_y) / median_absolute_deviation_y for y in ys]
    return np.where(np.abs(modified_z_scores) > threshold)

def main(argv=sys.argv[1:]):
    try:
        with open(argv[0]) as f:
            lines = f.read().splitlines()
            outliers = outliers_modified_z_score(ys=np.asarray(lines, dtype=np.float32))
            print ("the outliers we have in file {0} ".format(argv[0]))
            print(" outliers:\n{0}".format(outliers))
    except Exception as e:
        print ("fail to find outliers {}".format(e))


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

