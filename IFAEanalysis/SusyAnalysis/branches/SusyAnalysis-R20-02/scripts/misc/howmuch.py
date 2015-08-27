#!/usr/bin/env python

import os, sys, string, math, re
from subprocess import *

def main():

    try:
        f = open(sys.argv[1])
        lines = f.readlines()
        f.close()
    except IOError:
        print '   ! Not file found.'

    sum=0.

    for l in lines:

        print l,        

        output = Popen(["dq2-ls", "-fH", l.strip()], stdout=PIPE).communicate()[0]

        for item in output.split("\n"):

            if "total files" in item:
                print " n= ",item.split()[2],
            if "size" in item:
                print item.split()[2],' ',item.split()[3]
                factor=1
                if 'MB' in item.split()[3]:
                    factor=0.001
                if 'KB' in item.split()[3]:
                    factor=0.000001
                
                sum += float(item.split()[2])*factor


    print 'Total Size for these ',len(lines),' files = ',str(sum),' GB'

                
if __name__ == '__main__':
    main()

                
