# -*- Mode:Outline -*-

##########################################################################
#                                                                        #
#  Avango Software, Copyright 2001 - 2008 Fraunhofer Gesellschaft        #
#  zur Foerderung der angewandten Forschung (FhG), Munich, Germany.      #
#                                                                        #
#  Avango Software (both binary and source code - if released) is        #
#  intellectual property owned by FhG and copyrighted by FhG worldwide.  #
#                                                                        #
#  Avango is a trademark owned by FhG.                                   #
#                                                                        #
##########################################################################

The build system uses SCons. Use

  scons --help

to see all configurations options. These options can also be set in a file
called 'localdefs.py' in the top-level directory. Note that this is a python
script with full support for all advanced features of python.

By default the SCons build uses the 'all' build target which will recursively
build all modules with respect to your 'localdefs.py' environment.  For each
Avango NG module different build targets exist. For example to just build the
'avango-core' module use:

  scons core

To execute the unit tests for 'avango-core' use:

  scons check-core

And finally to install the module use:

  scons install-core

Replace 'core' with any other module name (including 'all' for all configured
modules) for other build targets.

Further information about the build system can be found in the python (source)
documentation of avango-build found here:

  avango-build/src/avango/build/__init__.py
