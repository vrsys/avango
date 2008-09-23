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

AC_DEFUN([AVANGO_ENSEMBLE_CHECK_OCAML], [
  AC_CHECKING([for OCaml])

  AC_ARG_WITH(ocaml,

    AS_HELP_STRING(
      [--with-ocaml=DIR],
      [OCaml home location (needed for distribution support) [[default=/usr]]]),

    [if test "${withval}" != "no"; then
       OCAML_HOME="${withval}"
     else
       AC_MSG_ERROR([ocaml config cannot be suppressed, please add a path name or avoid this option!])
     fi
    ],[OCAML_HOME="/usr"])

  if test "x${OCAML_HOME}" != "x"; then
    AC_MSG_RESULT([setting OCAML_HOME... ${OCAML_HOME}])
  fi

  OCAML_COMPILER=""
  OCAML_DEPEND=""
  OCAML_RUN=""

  if test "x${OCAML_HOME}" != "x"; then
    if ! test -d ${OCAML_HOME}; then
      AC_MSG_ERROR([no such directory '${OCAML_HOME}'])
    elif ! test -d ${OCAML_HOME}/bin; then
      AC_MSG_ERROR([no such directory '${OCAML_HOME}/bin'])
    fi

    if test -x "${OCAML_HOME}/bin/ocamlc" -a -s "${OCAML_HOME}/bin/ocamlc"; then
      OCAML_COMPILER="${OCAML_HOME}/bin/ocamlc"
    else
      OCAML_COMPILER=""
    fi

    if test -x "${OCAML_HOME}/bin/ocamldep" -a -s "${OCAML_HOME}/bin/ocamldep"; then
      OCAML_DEPEND="${OCAML_HOME}/bin/ocamldep"
    else
      OCAML_DEPEND=""
    fi

    if test -x "${OCAML_HOME}/bin/ocamlrun" -a -s "${OCAML_HOME}/bin/ocamlrun"; then
      OCAML_RUN="${OCAML_HOME}/bin/ocamlrun"
    else
      OCAML_RUN=""
    fi
  else
    AC_CHECK_PROG(OCAML_COMPILER, "ocamlc", "ocamlc", "")
    AC_CHECK_PROG(OCAML_DEPEND, "ocamldep", "ocamldep", "")
    AC_CHECK_PROG(OCAML_RUN, "ocamlrun", "ocamlrun", "")
  fi

  if test "x${OCAML_COMPILER}" = "x"; then
    AC_MSG_ERROR([couldn't find 'ocamlc' in ${OCAML_HOME-PATH}])
  elif test "x${OCAML_DEPEND}" = "x"; then
    AC_MSG_ERROR([couldn't find 'ocamldep' in ${OCAML_HOME-PATH}])
  elif test "x${OCAML_RUN}" = "x"; then
    AC_MSG_ERROR([couldn't find 'ocamlrun' in ${OCAML_HOME-PATH}])
  fi

  AC_MSG_RESULT([using OCaml compiler... ${OCAML_COMPILER}])
  AC_MSG_RESULT([using OCaml depend... ${OCAML_DEPEND}])
  AC_MSG_RESULT([using OCaml interpreter... ${OCAML_RUN}])

  OCAML_VERSION=`${OCAML_COMPILER} -v 2>&1 | grep version | cut -d" " -f6-`
  AC_MSG_RESULT([OCaml version... ${OCAML_VERSION}])

  case "${OCAML_VERSION}" in
    [012]?*)
      AC_MSG_ERROR([please upgrade to OCaml compiler version 3.06...])
      ;;
    3?0[12345])
      AC_MSG_ERROR([please upgrade to OCaml compiler version 3.06...])
      ;;
    3.06)
      ;;
    *)
      AC_MSG_WARN([untested OCaml compiler version...])
      ;;
  esac

  OCAML_LIBDIR="`${OCAML_COMPILER} -v 2>&1 | tail -1 | cut -d' ' -f4-`"
  AC_MSG_RESULT([OCaml library directory... ${OCAML_LIBDIR}])

  OCAML_COMPILER_FLAGS="-thread"
  case "${OCAML_VERSION}" in
    3.06)
      OCAML_COMPILER_FLAGS="-w Al ${OCAML_COMPILER_FLAGS}"
      ;;
    3.0[789])
      OCAML_COMPILER_FLAGS="-w Aelxyz ${OCAML_COMPILER_FLAGS}"
      ;;
  esac

  OCAML_OBJSUFFIX="cmo"

  if test "${AVANGO_ENSEMBLE_DEBUG}" == "1"; then
    OCAML_COMPILER_FLAGS="${OCAML_COMPILER_FLAGS} -g"
  #else
    #OCAML_COMPILER_FLAGS="${OCAML_COMPILER_FLAGS} -opt"
    #OCAML_OBJSUFFIX="cmx"
  fi

  AC_MSG_RESULT([using OCAML_COMPILER_FLAGS... ${OCAML_COMPILER_FLAGS}])

  AC_SUBST(OCAML_HOME)
  AC_SUBST(OCAML_COMPILER)
  AC_SUBST(OCAML_DEPEND)
  AC_SUBST(OCAML_RUN)
  AC_SUBST(OCAML_VERSION)
  AC_SUBST(OCAML_LIBDIR)
  AC_SUBST(OCAML_OBJSUFFIX)
  AC_SUBST(OCAML_COMPILER_FLAGS)

  AC_MSG_RESULT([])

])
