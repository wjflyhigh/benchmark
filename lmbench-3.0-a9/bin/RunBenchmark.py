#!/usr/bin/env python

import sys
import re
import argparse
import commands
from multiprocessing import Process

def Color_Print(color,*params):
    pList = []
    for param in params:
        pList.append(str(param)) ### turn tuple(params) into list(pList) ###
    colorDict = { 'red':'\033[01;31m', 'green':'\033[01;32m', 'yellow':'\033[01;33m', 'blue':'\033[01;34m', 'magenta':'\033[01;35m', 'cyan':'\033[01;36m', 'white':'\033[01;37m' }
    try:
        if color in colorDict:
            print colorDict[color],' '.join(params),"\033[0m"
    except:
        print str(color)+' '.join(params)

def getSizeList(minSizeValue, maxSizeValue):
    unitDict = {}
    unitDict['K'] = 'M'
    unitDict['M'] = 'G'
    #unitReverseDict = {}
    #unitReverseDict['G'] = 'M'
    #unitReverseDict['M'] = 'K'
    sizeValueList = []
    curSizeValue = minSizeValue
    while True:
        curSize = re.search('\d*', curSizeValue).group(0)
        unit = curSizeValue[-1]
        ### when bigger the maxSizeValue, break loop
        #print 'curSizeValue is %s' % (curSizeValue)
        if unit == maxSizeValue[-1]:
            maxSize = re.search('\d*', maxSizeValue).group(0)
            if int(curSize) >= int(maxSize):
                sizeValueList.append(maxSizeValue)
                break
        nextSize = int(curSize) * 2
        sizeValueList.append(curSizeValue)
        if nextSize >= 1024:
            unit = unitDict[unit]
            curSizeValue = '1%s' % unit
        else:
            curSizeValue = '%s%s' % (nextSize, unit)
    return sizeValueList

def runCommand(curCommand):
    #TODO: confirm the output ends with "xxxx mb xxxxx mb/secs"
    curResult = commands.getoutput(curCommand).split('\n')[-1].split()[2]
    print 'curResult=[%s]' % curResult

def RunACommandManyTimes(func, arg, runTimes):
    proc_record = []
    Color_Print('yellow', '===================%s=============== %s Times' % (arg, runTimes))
    for i in range(runTimes):
        p = Process(target = func, args = (arg,))
        p.start()
        proc_record.append(p)
    for p in proc_record:
        #Wait until child process terminates
        p.join()

### ./script.py --time=8 --Warmup=3 --Number=100 --Size="4G" --Operation="rd"
### eg:./bw_mem -W 3 -N 100 4G rd
params = argparse.ArgumentParser(description = 'A script used for auto-pass parameters and get the test result')
params.add_argument('-t', '--time', help = 'The times of the script running in background', required = True)
params.add_argument('-W', '--Warmup', help = 'Warm up', required = True)
params.add_argument('-N', '--Number', help = 'The number of executing', required = True)
params.add_argument('-S', '--Size', help = 'The Size of operation data', default="4G")
params.add_argument('-O', '--Operation', help = 'The type of Operation', choices = ['rd', 'wr', 'cp', 'fwr', 'fcp', 'bzero', 'bcopy'], required = True)
args = params.parse_args()

warmupTimes = args.Warmup
numberValue = args.Number
maxSizeValue = args.Size
operationType = args.Operation
timeForRun = args.time

Color_Print('yellow', 'Warm UP(-W) = [%s]' % warmupTimes)
Color_Print('yellow', 'Number(-N)  = [%s]' % numberValue)
Color_Print('yellow', 'Data Size   = [%s]' % maxSizeValue)
Color_Print('yellow', 'Operation   = [%s]' % operationType)
Color_Print('yellow', 'Times for run each command: %s' % timeForRun)



#TODO: please ensure the minSizeValue is 1024 times
#minSizeValue = '2G'
#minSizeValue = '256K'
minSizeValue = '32K'

##########################
"""
timeForRun = 8
maxSizeValue = '4G'
warmupTimes = 3
numberValue = 100 
operationType = 'rd'
"""
##########################

initCommand = './bw_mem -W %s -N %s %s %s ' % (warmupTimes, numberValue, maxSizeValue, operationType)
#initCommand = "echo '#./bw_mem -W %s -N %s %s %s & ;' ; time sleep %s ; echo 'Hello world How%s are You'" % (warmupTimes, numberValue, maxSizeValue, operationType, warmupTimes, numberValue)

sizeValueList = getSizeList(minSizeValue, maxSizeValue)
orginalSizeValueList = __import__('copy').deepcopy(sizeValueList)
sizeValueList.reverse()

curWarmupTimes = warmupTimes
curNumberValue = numberValue

allCommandList = []
allCommandList.append(initCommand)
for curMaxSizeValue in sizeValueList[1:]:
    curWarmupTimes = int(curWarmupTimes) * 2
    curNumberValue = int(curNumberValue) * 2
    curCommand = './bw_mem -W %s -N %s %s %s ' % (curWarmupTimes, curNumberValue, curMaxSizeValue, operationType) 
    #curCommand = "echo '#./bw_mem -W %s -N %s %s %s & ;' ; time sleep %s ; echo 'Hello world How%s are You'" % (curWarmupTimes, curNumberValue, curMaxSizeValue, operationType, curWarmupTimes, curNumberValue) 
    allCommandList.append(curCommand)
for i in allCommandList:
    Color_Print('yellow', 'I will run [%s] %s Times' % (i, timeForRun))

answer = raw_input('Above all are the commands you will run(yes or no)? ')
if 'y' in answer.lower():
    pass
else:
    sys.exit(1)


### 'warmup is 3.\nrepetitions is 100.\nsize is 4000000000.\nStart to execute benchmp_for_mambo, args: warmup = 3, repetitions = 100.\nSum is 0.\nSum is 0.\nTime used is 28830817.\nresult = 28830817\n4000.00 mb 13874.04 mb/secs'

for curCommand in allCommandList:
    RunACommandManyTimes(runCommand, curCommand, int(timeForRun))

