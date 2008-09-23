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
