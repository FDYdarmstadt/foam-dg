import glob
import os

files = list(filter(os.path.isfile, glob.glob('plot*.plt')))
files.sort(key=lambda x: os.path.getmtime(x))

i = 0
for fl in files:
    os.rename(fl, "plot" + str(i) + ".plt")
    i += 1
