#!/usr/bin/python

import sys

translate_array \
 =[['.', "none",           0],
   ['0', "enemy0",         0],
   ['1', "enemy1",         1],
   ['2', "enemy2",         2],
   ['x', "brick0.png",    77],
   ['X', "brick0.png",    77],
   ['y', "brick1.png",    78],
   ['Y', "brick1.png",    78],
   ['A', "box-full.png",  83],
   ['B', "box-full.png",  83],
   ['!', "box-full.png",  83],
   ['a', "box-empty.png", 84],
   ['C', "cloud-00.png",  85],
   ['D', "cloud-00.png",  86],
   ['E', "cloud-00.png",  87],
   ['F', "cloud-00.png",  88],
   ['c', "cloud-00.png",  89],
   ['d', "cloud-00.png",  90],
   ['e', "cloud-00.png",  91],
   ['f', "cloud-00.png",  92],
   ['g', "bkgd-00.png",   93],
   ['h', "bkgd-01.png",   94],
   ['i', "bkgd-02.png",   95],
   ['j', "bkgd-03.png",   96],
   ['G', "bkgd-10.png",   97],
   ['H', "bkgd-11.png",   98],
   ['I', "bkgd-12.png",   99],
   ['J', "bkgd-13.png",  100],
   ['#', "solid0.png",    11],
   ['[', "solid1.png",    13], 
   ['=', "solid2.png",    14],
   [']', "solid3.png",    15],
   ['$', "distro",        82],
   ['^', "water",         76],
   ['*', "poletop.png",   80],
   ['|', "pole.png",      79],
   ['\\',"flag.png",      81],
   ['&', "water.png",     75]]

def translate(c):
    for i in translate_array:
        if c == i[0]:
            return i[2]
    print >>sys.stderr, "Couldn't translate tile %s" % c
    return 0

if len(sys.argv) != 2:
    print "Usage: %s FILENAME" % sys.argv[0]
    sys.exit()

print ";; Converting:", sys.argv[1]

f = open(sys.argv[1], 'r')
mycontent = f.readlines()

content = []

for i in mycontent:
    content.append(i[:-1])

name       = content[0]
theme      = content[1]
time       = content[2]
music      = content[3]
background = content[4]
red        = content[5]
green      = content[6]
blue       = content[7]
width      = content[8]
height     = "15"
gravity    = content[9]

print "(supertux-level"
print "  (name \"%s\")" % name
print "  (theme \"%s\")" % theme
print "  (music \"%s\")" % music
print "  (background \"%s\")" % background
print "  (bkgd_red   %s)" % red
print "  (bkgd_green %s)" % green
print "  (bkgd_blue  %s)" % blue
print "  (time %s)" % time
print "  (width %s)" % width
print "  (gravity %s)" % gravity
print "  (interactive-tm "
for i in content[10:]:
    for x in i:
        sys.stdout.write("%3d" % translate(x))
        sys.stdout.write(" ")
    print ""
print "   )"
print ")"

# EOF #
