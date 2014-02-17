#!/usr/bin/python

import os
import re
import sys

localSrcDir = os.environ['LOCAL_SRC']
print "local source root dir: " + localSrcDir
#print os.environ['RPI_SRC']
curDir = os.getcwd()
print "current dir: " + curDir


# match to find the subdirectory within the local source root
pattern = re.compile("(" + localSrcDir + ")(.+)")
m = pattern.match(curDir)
subDir = m.group(2)

# create the path on the raspberry pi
rPiPath = os.environ['RPI_SRC'] + subDir
print "rpi path: " + rPiPath

# rsync over to the pi
file = sys.argv[1]
cmd = "rsync -a -e ssh " + curDir + "/ pi@10.0.0.40:" + rPiPath
print "cmd: " + cmd
os.system(cmd)

# execute the remote command
cmd = 'ssh -t pi@10.0.0.40 "' + "cd " + rPiPath + " ; sudo ./" + file + '"'
print cmd
os.system(cmd)