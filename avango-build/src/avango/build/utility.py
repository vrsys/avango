# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import os.path
import py_compile
import SCons.Script as scons
import oshelper

def _python_compile(target, source, env):
    py_compile.compile(source[0].path, target[0].path)
    return None

def install_python(dir, sources):
    basedir = dir
    scons.Mkdir(basedir)
    for file in sources:
        pyc = basedir+'/'+os.path.splitext(os.path.basename(file))[0]+'.pyc'
        scons.Command(pyc, file, _python_compile)

def make_vcproject(env, name, target_name):
    if not oshelper.os_is_windows():
        return

    sources =  env.FindSourceFiles()
    sourceList = []
    prepend_string = ''

    if env['DEBUG']:
        mode = 'Debug'
    else:
        mode = 'Release'

    for s in sources:
        sourceList.append('..\\' + s.srcnode().path)

    build_target_list = []
    build_target_list.append('check-'+target_name)
    build_target_list.append(target_name)

    variant_list = []
    variant_list.append('Check|Win32')
    variant_list.append(mode+'|Win32')

    project = env.MSVSProject(target = name + env['MSVSPROJECTSUFFIX'],
                              auto_build_solution=0,
                              srcs =  sourceList,
                              #incs = includes,
                              buildtarget = build_target_list,
                              variant = variant_list)
    scons.Alias('vcprojects', project)
