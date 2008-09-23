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

AC_DEFUN([AVANGO_ENSEMBLE_SETUP_ENSEMBLE], [
  AC_MSG_NOTICE([setting up ensemble...])

  # -DINLINE= defines INLINE to be empty, that's what we want
  ENSEMBLE_CDEFINES="-DINLINE= -D_REENTRANT -DTHREADED_SELECT"

  if test "${AVANGO_ENSEMBLE_DEBUG}" == "0"; then
    ENSEMBLE_CDEFINES="${ENSEMBLE_CDEFINES} -DNDEBUG"
  fi

  AC_MSG_RESULT([using ENSEMBLE_CDEFINES... ${ENSEMBLE_CDEFINES}])

  AC_SUBST(ENSEMBLE_CDEFINES)

  AC_MSG_RESULT([])

])
