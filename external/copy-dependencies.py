#!/usr/bin/env python3
import os
import sys
import glob
import shutil
import argparse
from pathlib import Path

if __name__ == "__main__":
    # Handle arguments
    parser = argparse.ArgumentParser(description='Copy external library files into build directory')
    parser.add_argument('source_dir', help='Source directory to find library files (*.dll, *.so)')
    parser.add_argument('target_dir', help='Target directory')
    args = parser.parse_args()

    source = Path(args.source_dir)
    target = Path(args.target_dir)
    notexist = False
    if not os.path.exists(source):
        print('  Source directory is not exist !')
        notexist = True
    if not os.path.exists(target):
        print('  Target directory is not exist !')
        notexist = True
    if notexist:
        parser.print_usage()
        exit(1)

    for ext in ['*.dll', '*.so']:
        for f in glob.glob(f'{source}/{ext}'):
            f_src = Path(f)
            f_trg = Path(f'{target}/')
            print(f'  Copy [{f_src}]')
            print(f'    -> [{f_trg}]')
            shutil.copy2(f_src, f_trg)
