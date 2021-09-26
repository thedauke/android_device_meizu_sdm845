#!/usr/bin/env python
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2021 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from hashlib import sha1
import sys


target_list = {
    'sdm845': {
        'file': 'proprietary-files-sdm845.txt',
        'device': 'sdm845'
    },
    'm1882': {
        'file': 'm1882/proprietary-files-m1882.txt',
        'device': 'm1882'
    },
    'm1892': {
        'file': 'm1892/proprietary-files-m1892.txt',
        'device': 'm1892'
    }
}
needSHA1 = False


def query_yes_no(question, default="yes"):
    """Ask a yes/no question via raw_input() and return their answer.

    "question" is a string that is presented to the user.
    "default" is the presumed answer if the user just hits <Enter>.
            It must be "yes" (the default), "no" or None (meaning
            an answer is required of the user).

    The "answer" return value is True for "yes" or False for "no".

    https://stackoverflow.com/a/3041990
    """
    valid = {"yes": True, "y": True, "ye": True, "no": False, "n": False}
    if default is None:
        prompt = " [y/n] "
    elif default == "yes":
        prompt = " [Y/n] "
    elif default == "no":
        prompt = " [y/N] "
    else:
        raise ValueError("invalid default answer: '%s'" % default)

    while True:
        sys.stdout.write(question + prompt)
        choice = input().lower()
        if default is not None and choice == "":
            return valid[default]
        elif choice in valid:
            return valid[choice]
        else:
            sys.stdout.write("Please respond with 'yes' or 'no' " "(or 'y' or 'n').\n")


def cleanup():
    for index, line in enumerate(lines):
        # Remove '\n' character
        line = line[:-1]

        # Skip empty or commented lines
        if len(line) == 0 or line[0] == '#':
            continue

        # Drop SHA1 hash, if existing
        if '|' in line:
            line = line.split('|')[0]
            lines[index] = '%s\n' % line


def update():
    for index, line in enumerate(lines):
        # Remove '\n' character
        line = line[:-1]

        # Skip empty lines
        if len(line) == 0:
            continue

        # Check if we need to set SHA1 hash for the next files
        if line[0] == '#':
            needSHA1 = (' - from' in line)
            continue

        if needSHA1:
            # Remove existing SHA1 hash
            line = line.split('|')[0]
            filePath = line.split(':')[1] if len(line.split(':')) == 2 else line

            if filePath[0] == '-':
                file = open('%s/%s' % (vendorPath, filePath[1:]), 'rb').read()
            else:
                file = open('%s/%s' % (vendorPath, filePath), 'rb').read()

            hash = sha1(file).hexdigest()
            lines[index] = '%s|%s\n' % (line, hash)


for target in target_list:
    if not query_yes_no('Update sha1sum for ' + target_list[target]['file'] + '?'):
        continue
    lines = [line for line in open(target_list[target]['file'], 'r')]
    vendorPath = '../../../vendor/meizu/' + target_list[target]['device'] + '/proprietary'

    if len(sys.argv) == 2 and sys.argv[1] == '-c':
        cleanup()
    else:
        update()
    
    with open(target_list[target]['file'], 'w') as file:
        for line in lines:
            file.write(line)
