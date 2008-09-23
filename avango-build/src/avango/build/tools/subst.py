# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

def subst(target, source, env):
    t = target[0]
    s = source[0]
    in_file = open(s.srcnode().abspath, 'r')
    out_file = open(t.abspath, 'w')
    out_file.write(in_file.read() % env['SUBST'])
    out_file.close()
    in_file.close()

def generate(env, **kargs):
    action = env.Action(subst, varlist = ['SUBST'])
    builder = env.Builder(action = action, src_suffix = '.in', single_source = 1)
    env['SUBST'] = {}
    env['BUILDERS']['Subst'] = builder

def exists(env):
    return 1
