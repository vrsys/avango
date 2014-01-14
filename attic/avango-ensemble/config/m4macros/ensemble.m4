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
