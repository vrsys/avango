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

include $(srcdir)/../../ocaml-defines
include $(srcdir)/../../ocaml-rules

SOURCES_MLI = \
	$(NULL)

SOURCES_ML = \
	socket.ml \
	$(NULL)

@MK@ifneq ($(srcdir),.)
$(SOURCES_MLI) $(SOURCES_ML):   % : $(srcdir)/%
	$(CP) $< $@
@MK@endif

socket.mli: $(srcdir)/../socket.mli
	$(CP) $< .

socksupp.mli: $(srcdir)/../socksupp.mli
	$(CP) $< .

OBJECTS_CMI =$(SOURCE_TAILS_MLI) socket.cmi socksupp.cmi
OBJECTS_CMO =$(SOURCE_TAILS_ML)

all: $(OBJECTS_CMO)

$(OBJECTS_CMO): $(OBJECTS_CMI)

clean-local:
	$(RM) $(OBJECTS_CMI) $(OBJECTS_CMO)
	$(RM) socket.mli socksupp.mli
@MK@ifneq ($(srcdir),.)
	$(RM) $(SOURCES_MLI) $(SOURCES_ML)
@MK@endif
	$(RM) $(OBJECTS_CMI:.cmi=.cmi.d) $(OBJECTS_CMO:.cmo=.cmo.d)

EXTRA_DIST = $(SOURCES_ML) $(SOURCES_MLI)
