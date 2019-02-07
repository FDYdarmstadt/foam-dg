#! /usr/bin/python3
import os, shutil, fileinput, glob, subprocess

print("Starting test harness...");


# Clear previously run test harness
clearCases = input('\n--> Do you want to clear old test cases?\n')

if clearCases in ['true', '1', 't', 'y', 'yes', 'TRUE', 'True']:
    print("\nCleaning test cases...");
    for names in glob.glob('*_mesh*'):
        shutil.rmtree(names)


# Look for test cases
foldernames = []

for x in os.listdir('.'):
    if os.path.isdir(x):
        foldernames.append(x)

#print(foldernames)


# Ask for mesh sizes to be used
mesh_sizes = input("\n--> Enter mesh sizes to be used, separated by space:  ")
meshList = mesh_sizes.split()

#for i, mesh in enumerate(meshList):
#    print("Mesh size", i + 1, "is", mesh, len(meshList))

absDir = os.path.abspath('.')


print('\n\n\nRunning test cases:\n')
# Copy test cases a number of times based on number of mesh sizes
for i, mesh in enumerate(meshList):
    for case in foldernames:
        newPath = os.path.join(absDir,case) + "_mesh" + mesh
        newSize = 'background (' + mesh + ' 1 1)'
        newAllrun = newPath + '/Allrun'
        newLog = newPath + '/log.dgLaplacianFoam'
#        newLog = newAllrun
        blockmeshdict = newPath + '/blockMeshDict'
#        print(case)
#       Copy/create new cases
        shutil.copytree(case, newPath)
#       Set number of cells blockMeshDict
        with fileinput.FileInput(blockmeshdict, inplace=True, backup='.bak') as file:
            for line in file:
                print(line.replace('background (10 1 1)', newSize), end='')
        os.chdir(newPath)
#        print("DIRECTORY",os.path.abspath('.'))
        subprocess.call(newAllrun)
        failed = True
        with fileinput.FileInput(newLog) as file:
            for line in file:
                if "successfully" in line:
                   print(' - Test case', case + '_mesh' + mesh,'successfully '\
                   'finished.')
                   failed = False
        if failed:
            statement =' - Test case '+case+'_mesh'+mesh+' failed.'
            print('\033[91m' + statement + '\033[0m')
        os.chdir(absDir)
#        print("EXITING DIR TO",os.path.abspath('.'))


# End
print('\n\nDone.')
