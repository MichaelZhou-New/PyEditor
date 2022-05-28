'''
Author: dylan
Date: 2022-04-03 15:13:25
LastEditTime: 2022-04-03 15:15:15
LastEditors: dylan
Description: 
FilePath: \Src:\Personal\Workspaces\PyEditor\PyEditor\test\test.py
'''
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

' a test module '

__author__ = 'Michael Liao'

import sys

def test():
    args = sys.argv
    if len(args)==1:
        print('Hello, world!')
    elif len(args)==2:
        print('Hello, %s!' % args[1])
    else:
        print('Too many arguments!')

if __name__=='__main__':
    test()

# test saving file


# test mark of file changed
# asdlkjfklsdf