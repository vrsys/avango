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

.NOTPARALLEL:

# remove $(srcdir) from targets search path to allow .ml? copying
VPATH=

SUBDIRS = u s

AM_CPPFLAGS = \
	-I$(srcdir)/s -I$(srcdir)/s/unix -I$(OCAML_LIBDIR) \
	$(ENSEMBLE_CDEFINES) $(C_WOFFALL_FLAGS)

noinst_LIBRARIES = libsocket.a

noinst_HEADERS = \
	$(srcdir)/s/e_iovec.h \
	$(srcdir)/s/mm.h \
	$(srcdir)/s/mm_basic.h \
	$(srcdir)/s/mm_so.h \
	$(srcdir)/s/prefix_suffix.h \
	$(srcdir)/s/select_common.h \
	$(srcdir)/s/skt.h \
	$(srcdir)/s/unix/e_iovec.h \
	$(srcdir)/s/unix/platform.h \
	$(srcdir)/s/unix/sockfd.h \
	$(NULL)

libsocket_a_SOURCES = \
	$(srcdir)/s/mm_basic.c \
	$(srcdir)/s/mm.c \
	$(srcdir)/s/md5c.c \
	$(srcdir)/s/heapc.c \
	$(srcdir)/s/unix/sendrecv.c \
	$(srcdir)/s/unix/gettimeofday.c \
	$(srcdir)/s/unix/miscsupp.c \
	$(srcdir)/s/unix/multicasts.c \
	$(srcdir)/s/unix/sockfd.c \
	$(srcdir)/s/unix/sockopt.c \
	$(srcdir)/s/unix/select.c \
	$(NULL)

include $(srcdir)/../ocaml-defines
include $(srcdir)/../ocaml-rules

SOURCES_MLI = \
	socket.mli \
	socksupp.mli \
	$(NULL)

SOURCES_ML = \
	socksupp.ml \
	$(NULL)

@MK@ifneq ($(srcdir),.)
$(SOURCES_MLI) $(SOURCES_ML):   % : $(srcdir)/%
	$(CP) $< $@
@MK@endif

OBJECTS_CMI =$(SOURCE_TAILS_MLI)
OBJECTS_CMO =$(SOURCE_TAILS_ML)

all: $(OBJECTS_CMO) usocket.cma ssocket.cma

$(OBJECTS_CMO): $(OBJECTS_CMI)

usocket.cma: $(OBJECTS_CMO) $(SUBDIRS)
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -a -o usocket.cma \
	  -linkall socksupp.cmo u/socket.cmo

ssocket.cma: $(OBJECTS_CMO) $(SUBDIRS)
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -a -o ssocket.cma \
	  -linkall socksupp.cmo s/natiov_impl.cmo s/common_impl.cmo s/comm_impl.cmo s/socket.cmo

clean-local:
	$(RM) $(OBJECTS_CMI) $(OBJECTS_CMO)
@MK@ifneq ($(srcdir),.)
	$(RM) $(SOURCES_MLI) $(SOURCES_ML)
@MK@endif
	$(RM) usocket.cma ssocket.cma
	$(RM) $(OBJECTS_CMI:.cmi=.cmi.d) $(OBJECTS_CMO:.cmo=.cmo.d)

EXTRA_DIST = \
	$(SOURCES_ML) \
	$(SOURCES_MLI) \
	$(srcdir)/README-socket \
	$(NULL)
