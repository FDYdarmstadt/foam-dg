#!/usr/bin/env python3

from matplotlib import subprocess
import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as optimization
import math
import csv
import glob
import shutil
import subprocess
import os
import sys


plt.rc('font', size=14)
plt.rc('text', usetex = True)
params = {'text.latex.preamble' : [r'\usepackage{siunitx}', r'\usepackage{amsmath}']}

def runOFCase(casePath):
    # cleanup
    timeDirectories = glob.glob(casePath + '/[0-9]*')
    plotFiles = glob.glob(casePath + '/*.plt')
    try:
        shutil.rmtree(casePath + "/postProcessing")
    except FileNotFoundError:
        pass
    for dir in timeDirectories:
        if dir[-2:] != "/0":
            shutil.rmtree(dir)
    for pltFile in plotFiles:
        os.remove(pltFile)

    # run
    subprocess.run(["CahnHilliardFoam"], cwd=casePath)

def prepareOFDir(casePath):
    timeDirectories = glob.glob(casePath + '/[0-9]*')
    for timeDir in timeDirectories:
        baseDir = timeDir
        sourceDir = baseDir + "/fvMesh/"
        targetDir = baseDir
        files = ["/fvC", "/fvU"]
        for fl in files:
            sourceFile = sourceDir + fl
            targetFile = targetDir + fl
            shutil.copyfile(sourceFile, targetFile)
            # Read in the file
            with open(targetFile, 'r') as file :
                filedata = file.read()
            # Replace the target string
            filedata = filedata.replace('boundaryField\n{\n}', 'boundaryField\n{\n top {type            homogenous; value 0;} bottom {type            homogenous; value 0;} left {type            homogenous; value 0;} right {type            homogenous; value 0;} frontAndBack {type            homogenous; value 0;} \n}')
            # Write the file out again
            with open(targetFile, 'w') as file:
                file.write(filedata)

def sample(casePath):
    subprocess.run(["sample"], cwd=casePath, stdout=subprocess.DEVNULL)

def postprocessOFCase(casePath):
    prepareOFDir(casePath)
    sample(casePath)

def readDataOF(path):
    xs = []
    cs = []
    with open(path, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ')
        for row in reader:
            xs.append(float(row[0]))
            cs.append(float(row[1]))
    return (xs, cs)

def plotAnalytical(cahn):
    xs = np.linspace(-1, 1, 100)
    ys = np.fromiter(map(lambda x: math.tanh(x/(math.sqrt(2) * cahn)), xs), dtype=np.cfloat)
    plt.plot(xs, ys, label="analytical")

def plotOF(basePath, size):
    casePath = basePath + "/" + size
    postprocessOFCase(casePath)
    timeDirectories = glob.glob(casePath + '/postProcessing/sets/[0-9]*')
    time = timeDirectories[-1]
    xs, cs = readDataOF(time + "/lineX1_fvC.xy")
    plt.plot(xs, cs, ".", label=size+ "(OF)")

    plt.legend()
    plt.xlabel(r"$x$")
    plt.ylabel(r"$c$")

def plotEverything(OFbasePath, cahn, rerun=False):
    plt.clf()
    plotAnalytical(cahn)
    for size in ["large", "medium", "small"]:
        if rerun:
            runOFCase(OFbasePath + "/" + size)
        plotOF(OFbasePath, size)
    plt.savefig("./plot.pdf")

def getOFError(basePath, size, cahn, rerun=False):
    casePath = basePath + "/" + size
    if rerun:
        runOFCase(basePath + "/" + size)
    postprocessOFCase(casePath)
    timeDirectories = glob.glob(casePath + '/postProcessing/sets/[0-9]*')
    time = timeDirectories[-1]
    xs, cs = readDataOF(time + "/lineX1_fvC.xy")
    ys = np.fromiter(map(lambda x: math.tanh(x/(math.sqrt(2) * cahn)), xs), dtype=np.double)
    error = sum(list(map(lambda x: x**2, ys-cs)))
    return error

def getOOC(basePath, cahn, rerun=False):
    DOFs = [20, 40, 80]
    DOFslog = list(map(math.log2, DOFs))
    errors = list(map(lambda size: getOFError(basePath, size, cahn, rerun), ["small", "medium", "large"]))
    errorslog = list(map(math.log2, errors))
    def fittingFunc(x, a, b):
        return a + b*x
    result = optimization.curve_fit(fittingFunc, DOFslog, errorslog)
    ooc = -result[0][1]
    return(ooc)

def main(inp):
    rerun = False
    if len(inp) > 1 and inp[1] == "True":
        rerun = True
    cahn = 0.1
    caseDir = os.path.dirname(os.path.realpath(sys.argv[0]))
    # plotEverything(caseDir, cahn, rerun)
    ooc = getOOC(caseDir, cahn, rerun)
    print(ooc)
    return ooc > 2.99

ret = main(sys.argv)
if not ret:
    raise Exception("Convergence order threshold not reached")
