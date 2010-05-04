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

.NOTPARALLEL:

# remove $(srcdir) from targets search path to allow .ml? copying
VPATH=

AM_CPPFLAGS = \
	-I$(top_srcdir)/include/avango/ensemble/hot \
	-I$(srcdir)/../ensemble/socket/s \
	-I$(srcdir)/../ensemble/socket/s/unix -I$(OCAML_LIBDIR) \
	$(ENSEMBLE_CDEFINES) \
	$(C_WOFFALL_FLAGS)

OCAML_LOCALINCLUDE = -I ../ensemble

include $(srcdir)/../ensemble/ocaml-defines
include $(srcdir)/../ensemble/ocaml-rules

SOURCES_MLI = \
	hot_util.mli \
	hot_appl.mli \
	$(NULL)

SOURCES_ML = \
	hot_util.ml \
	hot_appl.ml \
	hot_inboard.ml \
	$(NULL)

ENSEMBLE_LIBS = \
	../ensemble/socket/ssocket.cma \
	../ensemble/libenscore.cma \
	../ensemble/libensmin.cma \
	../ensemble/libensrest.cma \
	$(NULL)

# to create .o from .c files
noinst_LIBRARIES = libdummy.a

libdummy_a_SOURCES = \
	$(srcdir)/hot_mem.c \
	$(srcdir)/hot_error.c \
	$(srcdir)/hot_sys.c \
	$(srcdir)/hot_msg.c \
	$(srcdir)/pthread_intf.c \
	$(srcdir)/hot_inboard_c.c \
	$(srcdir)/hot_outboard_c.c \
	$(NULL)

LIB_HOTI_OBJECTS = \
	hot.o \
	hot_mem.o \
	hot_error.o \
	hot_sys.o \
	hot_msg.o \
	pthread_intf.o \
	hot_inboard_c.o \
	$(NULL)

LIB_HOTO_OBJECTS = \
	hot_mem.o \
	hot_error.o \
	hot_sys.o \
	hot_msg.o \
	pthread_intf.o \
	hot_outboard_c.o \
	$(NULL)

@MK@ifneq ($(srcdir),.)
$(SOURCES_MLI) $(SOURCES_ML) : % : $(srcdir)/%
	$(CP) $< $@
@MK@endif

OBJECTS_CMI =$(SOURCE_TAILS_MLI)
OBJECTS_CMO =$(SOURCE_TAILS_ML)

$(OBJECTS_CMO): $(OBJECTS_CMI)

hot.o: $(OBJECTS_CMI) $(OBJECTS_CMO)
	-$(RM) hot.o hot.c
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -output-obj -o $@ -custom unix.cma \
	$(ENSEMBLE_LIBS) -thread threads.cma -cclib -lthreads $(OBJECTS_CMO)

libhoti.o: $(LIB_HOTI_OBJECTS)
	$(LD) -r -o $@  $(LIB_HOTI_OBJECTS) ../ensemble/socket/libsocket.a \
	$(OCAML_LIBDIR)/libunix.a $(OCAML_LIBDIR)/libcamlrun.a

libhoti.a: libhoti.o
	ar cr $@ $<

libhoto.a: $(LIB_HOTO_OBJECTS)
	ar cr $@ $<

all: libhoti.a libhoto.a

clean-local:
@MK@ifneq ($(srcdir),.)
	$(RM) $(SOURCES_MLI) $(SOURCES_ML)
@MK@endif
	$(RM) $(OBJECTS_CMI) $(OBJECTS_CMO) $(OBJECTS_CMO:.cmo=.cmi)
	$(RM) hot.o libhoti.o libhoto.o libhoti.a libhoto.a

EXTRA_DIST= \
	$(SOURCES_MLI) \
	$(SOURCES_ML) \
	$(srcdir)/hot_outboard.ml \
	$(srcdir)/hot_ping.c \
	$(srcdir)/hpux_thread.c \
	$(srcdir)/ntthread.c \
	$(srcdir)/solaris_thread.c \
	$(NULL)
