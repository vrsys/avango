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

LIB_HOTI_OBJECTS = \
				hot/hot.o \
				hot/hot_mem.o \
				hot/hot_error.o \
				hot/hot_sys.o \
				hot/hot_msg.o \
				hot/pthread_intf.o \
				hot/hot_inboard_c.o \
				$(NULL)

LIBHOTI_A = $(LIB_HOTI_OBJECTS) $(OCAML_LIBDIR)/libunix.a # $(OCAML_LIBDIR)/libcamlrun.a

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
	$(LIBHOTI_A) \
	-lcamlrun_shared \
	ensemble/socket/libsocket.a \
	maestro/libmaestro.la \
	$(NULL)
