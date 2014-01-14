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

AC_DEFUN([AVANGO_ENSEMBLE_CHECK_COMPILER_VERSION], [
  AC_MSG_NOTICE([setting up compiler...])

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS

  AC_EGREP_CPP(notok, [
#if (__GNUC__ < 4)
    notok
#endif
  ], AC_MSG_ERROR([please upgrade to gcc 4.x]),
     [version="`${CXX} -v 2>&1 | tail -1 | cut -d' ' -f3-`"
      AC_MSG_RESULT(${version})]
  )

  AC_EGREP_CPP(isok, [
#if (__GNUC__ >= 4)
    isok
#endif
  ], [
      add_flags_all="-fmessage-length=0"
      add_flags_dbg="-fno-inline"
      add_flags_opt=""
      C_DEBUG_FLAGS="${C_DEBUG_FLAGS} ${add_flags_all} ${add_flags_dbg}"
      AC_MSG_RESULT([updating C_DEBUG_FLAGS... ${C_DEBUG_FLAGS}])
      C_OPTIMIZE_FLAGS="${C_OPTIMIZE_FLAGS} ${add_flags_all} ${add_flags_opt}"
      AC_MSG_RESULT([updating C_OPTIMIZE_FLAGS... ${C_OPTIMIZE_FLAGS}])
      CXX_DEBUG_FLAGS="${CXX_DEBUG_FLAGS} ${add_flags_all} ${add_flags_dbg}"
      AC_MSG_RESULT([updating CXX_DEBUG_FLAGS... ${CXX_DEBUG_FLAGS}])
      CXX_OPTIMIZE_FLAGS="${CXX_OPTIMIZE_FLAGS} ${add_flags_all} ${add_flags_opt}"
      AC_MSG_RESULT([updating CXX_OPTIMIZE_FLAGS... ${CXX_OPTIMIZE_FLAGS}])],
  )

  AC_LANG_RESTORE
])
