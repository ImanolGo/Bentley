import sys
import glob
import dxfgrabber
import time
from itertools import chain


DIRS = (r"D:\Source\dxftest\CADKitSamples\*.dxf",
r"D:\Source\dxftest\*.dxf",
r"D:\Source\dxftest\R12_test_files\*.dxf")

all_files = chain(*[glob.glob(d)for d in DIRS])


def stress_test():
    overall_time = 0
    ok = 0
    error = 0

    print('Cython acceleration is removed since version 0.8.0')
    print("Stress test:")

    for filename in all_files:
        try:
            start_time = time.time()
            dwg = dxfgrabber.readfile(filename)
            end_time = time.time()
            run_time = end_time - start_time
        except Exception:
            error += 1
            print('errors at opening: {}'.format(filename))
        else:
            ok += 1
            overall_time += run_time
            print('ok ({:.2f} seconds for opening: {})'.format(run_time, filename))

    print("{:.2f} seconds for {} files".format(overall_time, ok))
    print("{} errors".format(error))
    print('success' if error == 0 else 'failed')


if __name__ == "__main__":
    stress_test()
