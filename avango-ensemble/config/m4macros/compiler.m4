# -*- Mode:Autoconf -*-

##########################################################################
#                                                                        #
#  Avango Software, Copyright 2001 - 2007 Fraunhofer Gesellschaft        #
#  zur Foerderung der angewandten Forschung (FhG), Munich, Germany.      #
#                                                                        #
#  Avango Software (both binary and source code - if released) is        #
#  intellectual property owned by FhG and copyrighted by FhG worldwide.  #
#                                                                        #
#  Avango is a trademark owned by FhG.                                   #
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
