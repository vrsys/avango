# -*- Mode:Makefile -*-

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

SUBDIRS = \
	ensemble \
	hot \
	maestro \
	tests \
	$(NULL)

hot: ensemble
maestro: hot

EXTRA_DIST = \
	ENSEMBLE.1.40 \
	LICENSE \
	$(NULL)

lib_LTLIBRARIES = libavangoEnsemble.la

libavangoEnsemble_la_SOURCES =

libavangoEnsemble_la_LIBADD = \
	hot/libhoti.a \
	maestro/libmaestro.la \
	$(NULL)
