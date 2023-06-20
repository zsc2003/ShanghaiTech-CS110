#!/usr/bin/python3

import os
import sys


def readlines(file_path):
    with open(file_path, 'r') as f:
        return f.readlines()


def non_code_line(line):
    return line in ['', '{', '}'] or any(
        line.find(i) != -1 for i in [
            '#define',
            '#include',
            '#warn',
            '#error',
            '#if',
            '#else',
            '#endif',
            '#ifdef',
            '#ifndef',
        ])


def check(file_path):
    lines = readlines(file_path)
    lines_len = len(lines)
    i = 0
    NonBlankLine = 0
    while i < lines_len:
        line = lines[i].strip()
        if non_code_line(line):
            pass
        elif line.find('//') != -1:
            NonBlankLine = 0
        elif line.find("/*") != -1:
            NonBlankLine = 0
            while line.find("*/") == -1 and i < lines_len:
                i += 1
                line = lines[i].strip()
        else:
            NonBlankLine += 1
            if NonBlankLine > 6:
                print('No enough comments at line {}.'.format(i + 1))
                return False
        i += 1

    return True


def main(argv):
    if len(argv) != 2:
        print('Usage: python3 {} <filename>'.format(argv[0]))
        return -1

    file = argv[1]
    if not os.path.isfile(file):
        print('no such file: {}'.format(file))
        return -1

    return 0 if check(file) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
