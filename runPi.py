#!/usr/bin/python

import os
import re
import sys

localSrcDir = os.environ['LOCAL_SRC']
curDir = os.getcwd()

# match to find the subdirectory within the local source root
pattern = re.compile("(" + localSrcDir + ")(.+)")
m = pattern.match(curDir)
subDir = m.group(2)

# create the path on the raspberry pi
rPiPath = os.environ['RPI_SRC'] + subDir

# rsync over to the pi
file = sys.argv[1]
cmd = "rsync -a -e ssh " + curDir + "/ pi@10.0.0.40:" + rPiPath
os.system(cmd)

# execute the remote command
cmd = 'ssh -t pi@10.0.0.40 "' + "cd " + rPiPath + " ; sudo ./" + file + '"'
os.system(cmd)