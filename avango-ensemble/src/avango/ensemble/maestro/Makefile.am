# -*- Mode:Makefile -*-

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
