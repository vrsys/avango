# -*- Mode:Autoconf -*-

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

AC_DEFUN([AVANGO_ENSEMBLE_SETUP_LINUX], [

  # std-c compiler
  CFLAGS="${CFLAGS}"
  C_DEFAULT_FLAGS="-Wall -funsigned-char"
  C_OPTIMIZE_FLAGS="-O3 -DNDEBUG"
  C_DEBUG_FLAGS="-O0 -g"
  C_WOFFALL_FLAGS="-w"

  # std-c++ compiler
  CXXFLAGS="${CXXFLAGS}"
  CXX_DEFAULT_FLAGS="${C_DEFAULT_FLAGS}"
  CXX_OPTIMIZE_FLAGS="${C_OPTIMIZE_FLAGS}"
  CXX_DEBUG_FLAGS="${C_DEBUG_FLAGS}"
  CXX_WOFFALL_FLAGS="${C_WOFFALL_FLAGS}"

  # linker flags
  LDFLAGS=""

])

AC_DEFUN([AVANGO_ENSEMBLE_SETUP_POST], [

  if test "x${AVANGO_ENSEMBLE_DEBUG}" == "x1"; then
    AC_MSG_RESULT([enabling debug flags...])
    CFLAGS="${C_DEFAULT_FLAGS} ${C_DEBUG_FLAGS} ${CFLAGS}"
    CXXFLAGS="${CXX_DEFAULT_FLAGS} ${CXX_DEBUG_FLAGS} ${CXXFLAGS}"
  else
    AC_MSG_RESULT([enabling optimization flags...])
    CFLAGS="${C_DEFAULT_FLAGS} ${C_OPTIMIZE_FLAGS} ${CFLAGS}"
    CXXFLAGS="${CXX_DEFAULT_FLAGS} ${CXX_OPTIMIZE_FLAGS} ${CXXFLAGS}"
  fi

  AC_MSG_RESULT([setting CFLAGS... ${CFLAGS}])
  AC_MSG_RESULT([setting C_OPTIMIZE_FLAGS... ${C_OPTIMIZE_FLAGS}])
  AC_MSG_RESULT([setting C_DEBUG_FLAGS... ${C_DEBUG_FLAGS}])
  AC_MSG_RESULT([setting C_WOFFALL_FLAGS... ${C_WOFFALL_FLAGS}])

  AC_MSG_RESULT([setting CXXFLAGS... ${CXXFLAGS}])
  AC_MSG_RESULT([setting CXX_OPTIMIZE_FLAGS... ${CXX_OPTIMIZE_FLAGS}])
  AC_MSG_RESULT([setting CXX_DEBUG_FLAGS... ${CXX_DEBUG_FLAGS}])
  AC_MSG_RESULT([setting CXX_WOFFALL_FLAGS... ${CXX_WOFFALL_FLAGS}])

  AC_MSG_RESULT([setting LDFLAGS... ${LDFLAGS}])

  AC_SUBST(C_OPTIMIZE_FLAGS)
  AC_SUBST(C_DEBUG_FLAGS)
  AC_SUBST(C_WOFFALL_FLAGS)

  AC_SUBST(CXX_OPTIMIZE_FLAGS)
  AC_SUBST(CXX_DEBUG_FLAGS)
  AC_SUBST(CXX_WOFFALL_FLAGS)

  AC_SUBST(LDFLAGS)

  AC_MSG_RESULT([])

])
