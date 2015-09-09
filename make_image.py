#!/usr/bin/python

'''
Following instructions will create a disk image with following disk geometry parameters:
* size in megabytes = 64
* amount of cylinders = 130
* amount of headers = 16
* amount of sectors per track = 63
'''

import os, subprocess
import shlex
import sys
import re

class color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def info(c, s):
    print c + s + color.ENDC


def panic(s):
    print color.BOLD + color.FAIL + s + color.ENDC
    sys.exit(1)

def run(cmd):
    if os.system(cmd) != 0:
        panic ("%s executed with error. exit." % cmd)

def exe(cmd):
    proc = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE)
    (out, err) = proc.communicate()
    return out

def grep(s, pattern):
    return '\n'.join(re.findall(r'^.*%s.*?$'%pattern,s,flags=re.M))

info (color.HEADER, 'Building Certikos Image...')

info (color.HEADER, '\ncreating disk...')
run(('dd if=/dev/zero of=certikos.img bs=512 count=%d' % (130 * 16 * 63)))
run('parted -s certikos.img \"mktable msdos mkpart primary 2048s -1s set 1 boot on\"')
info (color.OKGREEN, 'done.')

info (color.HEADER,  '\nwriting mbr...')
run('dd if=obj/boot/boot0 of=certikos.img bs=446 count=1 conv=notrunc')
run('dd if=obj/boot/boot1 of=certikos.img bs=512 count=62 seek=1 conv=notrunc')
info (color.OKGREEN, 'done.')

info (color.HEADER,  '\ncopying kernel files...')
loc = int(grep(exe('fdisk -l certikos.img'), 'certikos.img1').split()[2])
if loc == 0:
    panic ("cannot find valid partition.");

info (color.OKBLUE, 'kernel starts at sector %d' % loc)
run('dd if=obj/kern/kernel of=certikos.img bs=512 seek=%d conv=notrunc' % loc)

info (color.OKGREEN + color.BOLD, '\nAll done.')
sys.exit(0)


