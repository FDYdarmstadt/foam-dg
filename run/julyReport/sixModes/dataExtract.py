#! /usr/bin/python3
import os, shutil, fileinput, glob, subprocess, string
from shutil import copyfile

# Look for test cases
foldernames = []

for x in os.listdir('.'):
    if os.path.isdir(x):
        foldernames.append(x)

curMod = str(6)

absDir = os.path.abspath('.')


failedN = 0
caseNumber = 0

print('\n\n\nCopying postprocessing files.\n')
# Copy test cases a number of times based on number of mesh sizes
for case in foldernames:
    if case[0].isdigit():
        if "laplace" in case:
#            print('Printing only laplace cases: ' + case)
            csvPath = case + '/1/ptsInt.csv'
        if "convection" in case:
#            print('Printing only convection cases: ' + case)
            csvPath = case + '/3/ptsInt.csv'
        newPath =\
'/home/gregor/foam/gregor-4.0/run/discontinuousGalerkin/report/July/images/xmg/newDat/'
        newFile = case[1:]
        if case[1].isdigit():
            newFile = case[2:]
        newFile = newPath + curMod + newFile + ".dat"
#        print('OLD PATH: ' + csvPath)
#        print('New PATH: ' + newFile)
        copyfile(csvPath, newFile)



#        caseNumber += 1
#        newCase = str(caseNumber) + "-" + case
#        newPath = os.path.join(absDir,newCase) + "_mesh" + mesh
#        newSize = 'background (' + mesh + ' 1 1)'
#        newAllrun = newPath + '/Allrun'
#        newLogLap = newPath + '/log.dgLaplacianFoam'
#        newLogConv = newPath + '/log.dgScalarTransport'
##        newLog = newAllrun
#        blockmeshdict = newPath + '/blockMeshDict'
##        print(case)
##       Copy/create new cases
#        shutil.copytree(case, newPath)
##       Set number of cells blockMeshDict
#        with fileinput.FileInput(blockmeshdict, inplace=True, backup='.bak') as file:
#            for line in file:
#                print(line.replace('background (10 1 1)', newSize), end='')
#        os.chdir(newPath)
##        print("DIRECTORY",os.path.abspath('.'))
#        subprocess.call(newAllrun)
#        failed = True
#        if os.path.exists(newLogConv):
#            with fileinput.FileInput(newLogConv) as file:
#                for line in file:
#                    if "successfully" in line:
#                       print(' - Test case', newCase + '_mesh' + mesh,'successfully '\
#                       'finished.')
#                       failed = False
#        if os.path.exists(newLogLap):
#            with fileinput.FileInput(newLogLap) as file:
#                for line in file:
#                    if "successfully" in line:
#                       print(' - Test case', newCase + '_mesh' + mesh,'successfully '\
#                       'finished.')
#                       failed = False
#        if failed:
#            statement =' - Test case '+newCase+'_mesh'+mesh+' failed.'
#            print('\033[91m' + statement + '\033[0m')
#            failedN += 1
#        os.chdir(absDir)
#        print("EXITING DIR TO",os.path.abspath('.'))





# End
print('\n\nDone.')
