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

AM_CPPFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = TestEnsemble

TESTS = $(check_PROGRAMS)

TestEnsemble_SOURCES = TestEnsemble.cpp
TestEnsemble_LDADD = $(top_builddir)/src/avango/ensemble/libavangoEnsemble.la
