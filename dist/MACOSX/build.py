#!/usr/bin/env python
import subprocess
import re
import os.path
from shutil import copyfile
import errno

# This is a hacked-together script on the way to making an independent app bundle.
# What I want at this time is a bundle that's relatively complete, even in debug mode.
# This includes a redistributable version of many of the dynamically linked libraries
# it depends on.

# The ultimate goal here is to reduce the number of dependencies that have to be installed
# for the executable to run (minus the ones that are all but guaranteed to be on the target
# computer.) In other words, a clean install of the OS should be able to run the binary,
# even if there's no pacakage-managed depdendents installed.

# Therefore, any of the top-level dependencies should depend only (mostly) on the downstream
# dependencies (which aren't accessed directly in the source code).

import pdb

class RootLibrary(object):
    """
    This class is responsible for keeping track of the root of the whole dependency tree.
    """
    def __init__(self, location):
        self._location = location

    @property
    def location(self):
        return self._location

    def execute_rewrite(self):
        """
        Treat the top level dependency as special - it's not being copied in.
        """
        ot = Otool(self.location + '/libopencv_world.4.1.0.dylib')
        self.recurse(ot)
    
    def copy_dependency(self, dependency):
        destination = self._location + dependency
        print("Copy " + dependency + " to " + destination)
        try:
            os.makedirs(os.path.dirname(destination))
        except OSError as exc:
            if exc.errno == errno.EEXIST and os.path.isdir(os.path.dirname(destination)):
                pass
            else:
                raise
        if not os.path.isfile(destination):
            copyfile(dependency, destination)
        return destination

    
    def rpath(self, subdirectory):
        return "@rpath" + subdirectory[1:]
#        # Calculate the relative path after the root
#        absolute_lib_path = os.path.abspath(self._library)
#        absolute_dep_path = os.path.abspath(self._new_location)
#        # This prefix doesn't necessarily split on a directory boundary.
#        # Many libraries will start with lib***, and those match too.
#        common_prefix = os.path.commonprefix((absolute_lib_path, absolute_dep_path))
#        if not os.path.isdir(common_prefix):
#            # Reconstruct what this would be if it were a dir.
#            common_prefix = os.path.dirname(common_prefix) + "/"
#        relative_suffix = "@rpath/" + absolute_dep_path[len(common_prefix):]
#        return relative_suffix

    def recurse(self, otool):
        dependencies = otool.deps_to_rewrite()
        # I find it easier to read the output as breadth-first
        next_layer = []
        for dep in dependencies:
            # Move the dependency
            new_location = self.copy_dependency(dep)
            name_tool = InstallNameTool(otool.binary, dep, self.rpath(new_location))
            name_tool.run_tool()
            next_layer.append(Otool(new_location))
        for lib in next_layer:
            self.recurse(lib)


class Otool(object):
    split_block_regex = re.compile("(Load command)")

    block_regex = re.compile(
"(?P<load_key>Load command)\s(?P<load_num>\d+)\n"
"\s+(?P<command_key>cmd)\s(?P<command_str>\w+)\n"
"\s+(?P<size_key>cmdsize)\s(?P<size_num>\d+)\n"
"\s+(?P<name_key>name)\s(?P<name_str>[a-zA-Z\s\/\.0-9]+).*\n"
"\s+(?P<time_stamp_key>time stamp)\s(?P<time_stamp_str>.*)\n"
"\s+(?P<version_key>current version)\s(?P<version_str>[0-9\.]+)\n"
"(?P<compat_key>compatibility version)\s(?P<compat_str>[0-9\.]+)\n"
"(?P<remainder>.*)"
, re.MULTILINE|re.DOTALL
)

    skip_rewrite_prefix = [
            "/System/Library/Frameworks",
            "/usr/lib",
            "/usr/local/lib"
            ]

    def __init__(self, binary):
        self._binary = binary

    @property
    def binary(self):
        return self._binary

    def split_load_commands(self):
        output = self.run_tool()
        # Split on the split regex
        blocks = []
        parsed_block_text = Otool.split_block_regex.split(output)
        parsed_block_text.pop(0)
        for i,k in zip(parsed_block_text[0::2], parsed_block_text[1::2]):
            blocks.append(i + k)
        return blocks

    def list_deps(self):
        blocks = self.split_load_commands()
        matches = []
        for block in blocks:
            match = Otool.block_regex.search(block)
            if match and match.groupdict().has_key('name_key'):
                matches.append(match)
        return matches

    def deps_to_rewrite(self):
        print("### Performing on " + self._binary)
        matches = self.list_deps()
        recursions = []
        for match in matches:
            # Only rewriting load commands
            if match.groupdict()['command_str'].strip() != "LC_LOAD_DYLIB":
                continue
            lib_path = match.groupdict()['name_str'].strip()
            if any(lib_path.startswith(prefix) for prefix in Otool.skip_rewrite_prefix):
                #print("Skipping " + lib_path)
                pass
            else:
                recursions.append(lib_path)
        return recursions

    def run_tool(self):
        return subprocess.check_output(['otool', '-l', self._binary])

# OK, it's important to understand how this should flow:

# All files should be copied first, then their installation deps updated in a second pass.
# This way there won't be any re-copying.
# To ensure this, we don't copyfile if the file does already exist.
class InstallNameTool(object):

    class DependencyDoesNotExist(Exception):
        pass

    def __init__(self, library, old_location, rpath):
        self._library = library
        self._old_location = old_location
        self._rpath = rpath

    def run_tool(self):
        return subprocess.check_output(self.build_command_array())


    def build_command_array(self):
        return ['install_name_tool', '-change', self._old_location, self._rpath, self._library]

    def __str__(self):
        return str(self.build_command_array())



if __name__ == "__main__":
    rl = RootLibrary('./OpenCV.framework/Libraries')
    rl.execute_rewrite()
