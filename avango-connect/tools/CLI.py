# -*- Mode:Python -*-

#
# C# builder
#
# based on:
#
#   http://www.scons.org/wiki/CsharpBuilder
#

import os.path
import SCons.Builder
import SCons.Scanner
import SCons.Node.FS
import SCons.Util
import SCons.Script

csccom = "$CSC $CSCFLAGS $_CSCRESOURCES $_CSCLIBPATH $_CSCLIBS $_CSCPKGS -out:${TARGET.abspath} $SOURCES"
csclibcom = "$CSC -t:library $CSCFLAGS $CSCLIBFLAGS $_CSCRESOURCES $_CSCLIBPATH $_CSCPKGS $_CSCLIBS -out:${TARGET.abspath} $SOURCES"


def scan_resources(node, env, path):
    return map(lambda a: SCons.Script.File(a), env['CILRESOURCES'])
ResourceScanner = SCons.Scanner.Scanner(function = scan_resources)

McsBuilder = SCons.Builder.Builder(action = '$CSCCOM',
                                   source_factory = SCons.Node.FS.default_fs.Entry,
                                   suffix = '.exe',
                                   target_scanner = ResourceScanner
                                  )

McsLibBuilder = SCons.Builder.Builder(action = '$CSCLIBCOM',
                                      source_factory = SCons.Node.FS.default_fs.Entry,
                                      suffix = '.dll',
                                      target_scanner = ResourceScanner
                                     )

def generate(env):
    env['BUILDERS']['CLIProgram'] = McsBuilder
    env['BUILDERS']['CLILibrary'] = McsLibBuilder

    def identity(input):
        return input

    def resource_id(items, mapper):
        resources = []
        for res in items:
            try:
                id = mapper(res)
                resname="%s,%s" % (res, id)
            except:
                resname=res
            resources.append(resname)
        return resources
    env['_CILRESOURCEID'] = resource_id

    env['CSC']        = 'gmcs'
    env['_CSCPKGS']    = "${_stripixes('-pkg:', CILPKGS, '', '-r', '', __env__)}"
    env['_CSCLIBS']    = "${_stripixes('-r:', CILLIBS, '', '-r', '', __env__)}"
    env['_CSCLIBPATH'] = "${_stripixes('-lib:', CILLIBPATH, '', '-r', '', __env__)}"
    env['_CSCRESOURCES'] = "${_stripixes('-resource:', _CILRESOURCEID(CILRESOURCES, CILRESOURCEID), '', '-r', '', __env__)}"
    env['CILRESOURCEID']   = identity
    env['CSCFLAGS']   = SCons.Util.CLVar('')
    env['CILRESOURCES']   = SCons.Util.CLVar('')
    env['CSCCOM']     = SCons.Action.Action(csccom)
    env['CSCLIBCOM']  = SCons.Action.Action(csclibcom)

def exists(env):
    return env.Detect('gmcs')
