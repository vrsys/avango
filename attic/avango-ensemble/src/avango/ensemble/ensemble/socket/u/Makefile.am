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
