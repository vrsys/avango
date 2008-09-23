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
