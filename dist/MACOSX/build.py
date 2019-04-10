#!/usr/bin/env python
import subprocess
import re

# This is a hacked-together script on the way to making an independent app bundle.
# What I want at this time is a bundle that's relatively complete, even in debug mode.
# This includes a redistributable version of many of the dynamically linked libraries
# it depends on.

# Since there don't seem to be that many library release packagers that can handle the way I
# want to organize things (i.e. just a plain old bundle that carries all but the lowest-level
# system bundles that we depend on), and since we depend on a lot of third party libs that 
# have to INTERMINABLY be repackaged, this script will create redistributables that can be
# simply zipped up and packaged along regardless of vendor.

class Otool(object):
    split_block_regex = re.compile("(Load command)")
    block_regex = re.compile(
"(?P<load_key>Load command)\s(?P<load_num>\d+)\n"
"\s+(?P<command_key>cmd)\s(?P<command_str>\w+)\n"
"\s+(?P<size_key>cmdsize)\s(?P<size_num>\d+)\n"
"\s+(?P<name_key>name)\s(?P<name_str>[a-zA-Z\s\/\.]+).*\n"
"\s+(?P<time_stamp_key>time stamp)\s(?P<time_stamp_str>.*)\n"
"\s+(?P<version_key>current version)\s(?P<version_str>[0-9\.]+)\n"
"(?P<compat_key>compatibility version)\s(?P<compat_str>[0-9\.]+)\n"
"(?P<remainder>.*)"
, re.MULTILINE|re.DOTALL
)


    def __init__(self, binary):
        self._binary = binary

    def list_deps(self):
        output = subprocess.check_output(['otool', '-l', self._binary])
        matches = Otool.block_regex.search(output)
        print matches.groups()


if __name__ == "__main__":
    ot = Otool('./Boost.framework/Libraries/libboost_filesystem.dylib')
    ot.list_deps()
