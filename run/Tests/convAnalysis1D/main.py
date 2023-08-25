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
        sourceDir = baseDir
        targetDir = baseDir
        files = ["/C", "/U"]
        for fl in files:
            sourceFile = sourceDir + fl
            targetFile = targetDir + fl
            # shutil.copyfile(sourceFile, targetFile)
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
    xs, cs = readDataOF(time + "/lineX1_C.xy")
    # plt.plot(xs, cs, ".", label=size+ "(OF)")
    plt.plot(xs, cs, ".", label=size)

    plt.legend()
    plt.xlabel(r"$x$")
    plt.ylabel(r"$c$")

def plotEverything(OFbasePath, cahn, rerun=False):
    plt.clf()
    plotAnalytical(cahn)
    for size in ["fine", "medium", "coarse"]:
        if rerun:
            runOFCase(OFbasePath + "/" + size)
        plotOF(OFbasePath, size)
    plt.savefig("./conv1D.png")

def plotConvergence(basePath, cahn):
    plt.clf()
    DOFs = [20, 40, 80]
    DOFsWithPadding = np.linspace(DOFs[0]*0.9, DOFs[-1]*1.11)
    DOFsNegPadding = np.linspace(DOFs[0]*1.5, DOFs[-1]*0.8) + 1.2
    DOFslog = list(map(math.log2, DOFs))
    errors = list(map(lambda size: getOFError(basePath, size, cahn, False), ["coarse", "medium", "fine"]))
    errorslog = list(map(math.log2, errors))
    def fittingFunc(x, a, b):
        return a + b*x
    result = optimization.curve_fit(fittingFunc, DOFslog, errorslog)
    a = errors[-1]/DOFs[-1] ** (-3)
    errorsExpected= list(map(lambda x: a * x**(-3), DOFsWithPadding))
    plt.loglog()
    plt.xlabel(r"\# of DOFs")
    plt.ylabel(r"$E$")
    plt.plot(DOFs, errors, "ko")

    plt.plot(DOFsWithPadding, errorsExpected, "k--")
    plt.plot(DOFsNegPadding, list(map(lambda x: 3.0 * a * x ** -3, DOFsNegPadding)), "k-")
    plt.text(4.5e1, 2.5e-4, "slope: -1")
    plt.savefig("./convergencePlot1D.png")

def getOFError(basePath, size, cahn, rerun=False):
    casePath = basePath + "/" + size
    if rerun:
        runOFCase(basePath + "/" + size)
    postprocessOFCase(casePath)
    timeDirectories = glob.glob(casePath + '/postProcessing/sets/[0-9]*')
    time = timeDirectories[-1]
    xs, cs = readDataOF(time + "/lineX1_C.xy")
    ys = np.fromiter(map(lambda x: math.tanh(x/(math.sqrt(2) * cahn)), xs), dtype=np.double)
    error = sum(list(map(lambda x: x**2, ys-cs)))
    return error

def getOOC(basePath, cahn, rerun=False):
    DOFs = [20, 40, 80]
    DOFslog = list(map(math.log2, DOFs))
    errors = list(map(lambda size: getOFError(basePath, size, cahn, rerun), ["coarse", "medium", "fine"]))
    errorslog = list(map(math.log2, errors))
    def fittingFunc(x, a, b):
        return a + b*x
    result = optimization.curve_fit(fittingFunc, DOFslog, errorslog)
    ooc = -result[0][1]
    return(ooc)

def main(inp):
    rerun = False
    # rerun = True
    if len(inp) > 1 and inp[1] == "True":
        rerun = True
    cahn = 0.1
    caseDir = os.path.dirname(os.path.realpath(sys.argv[0])) + "/convAnalysis1D/"
    plotEverything(caseDir, cahn, rerun)
    plotConvergence(caseDir, cahn)
    ooc = getOOC(caseDir, cahn, rerun)
    print(ooc)
    return ooc > 2.99

ret = main(sys.argv)
if not ret:
    raise Exception("Convergence order threshold not reached")
