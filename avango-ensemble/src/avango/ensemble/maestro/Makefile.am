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

AM_CPPFLAGS = -I$(top_srcdir)/include $(CXX_WOFFALL_FLAGS) $(ENSEMBLE_CDEFINES)

noinst_LTLIBRARIES = libmaestro.la

libmaestro_la_SOURCES = \
	Maestro_CORBA.c++ \
	Maestro_CSX.c++ \
	Maestro_ClSv.c++ \
	Maestro_ES_ReplicatedUpdates.c++ \
	Maestro_ES_Simple.c++ \
	Maestro_ETC.c++ \
	Maestro_GIOP.c++ \
	Maestro_Group.c++ \
	Maestro_GroupMember.c++ \
	Maestro_IIOPBridge.c++ \
	Maestro_ORB.c++ \
	Maestro_Perf.c++ \
	Maestro_Prim.c++ \
	Maestro_Types.c++ \
	$(NULL)

libmaestro_la_LDFLAGS = -lncurses -lpthread -lrt -lm
