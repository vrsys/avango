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

# Init

AC_PREREQ(2.59)
AC_INIT(avango-ensemble, 1.90.1)
AC_CONFIG_SRCDIR([bootstrap])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config/config.h])
AC_CANONICAL_TARGET
AC_LANG_CPLUSPLUS
AC_DISABLE_STATIC

AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability])

# Versioning

AVANGO_ENSEMBLE_VERSION=AC_PACKAGE_VERSION
AVANGO_ENSEMBLE_VERSION_MAJOR=`echo ${AVANGO_ENSEMBLE_VERSION} | cut -d'.' -f1`
AVANGO_ENSEMBLE_VERSION_MINOR=`echo ${AVANGO_ENSEMBLE_VERSION} | cut -d'.' -f2`
AVANGO_ENSEMBLE_VERSION_MAINT=`echo ${AVANGO_ENSEMBLE_VERSION} | cut -d'.' -f3`
AC_SUBST(AVANGO_ENSEMBLE_VERSION)
AC_SUBST(AVANGO_ENSEMBLE_VERSION_MAJOR)
AC_SUBST(AVANGO_ENSEMBLE_VERSION_MINOR)
AC_SUBST(AVANGO_ENSEMBLE_VERSION_MAINT)

# Build options

AC_ARG_ENABLE(debug,
  AS_HELP_STRING(
    [--enable-debug],
    [enable debug code and compiler flags [[default=no]]]),
  [if test "${enableval}" = "yes"; then
     AVANGO_ENSEMBLE_DEBUG=1
   else
     AVANGO_ENSEMBLE_DEBUG=0
   fi
  ],[AVANGO_ENSEMBLE_DEBUG=0])
AC_SUBST(AVANGO_ENSEMBLE_DEBUG)
AM_CONDITIONAL([AVANGO_ENSEMBLE_DEBUG], [test $AVANGO_ENSEMBLE_DEBUG -eq 1])

# Check platform/compiler support

AC_MSG_RESULT([])

case "${target}" in
  i?86*-linux-* | i?86*-*-linux | x86_64-*-linux-* )
    AVANGO_ENSEMBLE_SETUP_LINUX
  ;;

  *)
    AC_MSG_ERROR([Sorry, ${target} not a supported platform.])
  ;;
esac

# Checks for programs.

AC_PROG_LIBTOOL
AC_PROG_CXX

# Internal checks

AC_MSG_RESULT([])

AVANGO_ENSEMBLE_CHECK_COMPILER_VERSION
AVANGO_ENSEMBLE_SETUP_POST

AVANGO_ENSEMBLE_CHECK_OCAML
AVANGO_ENSEMBLE_SETUP_ENSEMBLE

# Additional macros

CP="cp -f -p"
AC_SUBST(CP)

MKDIR="mkdir -p"
AC_SUBST(MKDIR)

MV="mv -f"
AC_SUBST(MV)

# allow gnu make conditionals in Makefile.am
# usage:
#
# @MK@ifeq(...)
#   ...
# @MK@endif
#
# don't put automake magic variables (things like
# foo_SOURCES, foo_LDFLAGS, etc.) inside ifeq/endif,
# otherwise automake gets confused.
#
MK=
AC_SUBST(MK)

package_version=${PACKAGE_VERSION}
AC_SUBST(package_version)

# Output

AC_CONFIG_FILES([
  Makefile \
  dist/Makefile \
  dist/pkg-config/avango-ensemble.pc \
  dist/pkg-config/Makefile \
  include/Makefile \
  include/avango/Makefile \
  include/avango/ensemble/Config.h \
  include/avango/ensemble/Makefile \
  include/avango/ensemble/hot/Makefile \
  include/avango/ensemble/maestro/Makefile \
  src/Makefile \
  src/avango/Makefile \
  src/avango/ensemble/Makefile \
  src/avango/ensemble/ensemble/Makefile \
  src/avango/ensemble/ensemble/socket/Makefile \
  src/avango/ensemble/ensemble/socket/u/Makefile \
  src/avango/ensemble/ensemble/socket/s/Makefile \
  src/avango/ensemble/hot/Makefile \
  src/avango/ensemble/maestro/Makefile \
  src/avango/ensemble/tests/Makefile \
])
AC_OUTPUT

AC_MSG_RESULT([])
AC_MSG_RESULT([To start the build process run 'make' in here.])
AC_MSG_RESULT([])
AC_MSG_RESULT([Configuration for building ${PACKAGE_NAME}-${PACKAGE_VERSION} on ${target} done.])
AC_MSG_RESULT([])
