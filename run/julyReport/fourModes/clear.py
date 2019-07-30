#! /usr/bin/python3
import os, shutil, re, glob

print("Cleaning test cases...");

for names in glob.glob('*_mesh*'):
    shutil.rmtree(names)
