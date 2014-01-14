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

SUBDIRS = socket

OCAML_LOCALINCLUDE = \
	-I socket -I util -I mm -I type -I infr -I route \
	-I appl -I trans -I groupd -I layers/trans -I layers/other

include $(srcdir)/ocaml-defines
include $(srcdir)/ocaml-rules

SOURCES_MLI_CORE = \
	socket/socket.mli \
	util/trans.mli \
	util/util.mli \
	util/queuee.mli \
	util/fqueue.mli \
	util/trace.mli \
	util/arraye.mli \
	util/arrayf.mli \
	mm/buf.mli \
	mm/iovec.mli \
	mm/iovecl.mli \
	util/marsh.mli \
	type/security.mli \
	type/shared.mli \
	util/tree.mli \
	type/tdefs.mli \
	util/mrekey_dt.mli \
	util/diamond.mli \
	util/hsys.mli \
	util/lset.mli \
	util/resource.mli \
	util/sched.mli \
	type/time.mli \
	type/addr.mli \
	type/proto.mli \
	type/stack_id.mli \
	type/unique.mli \
	type/endpt.mli \
	type/group.mli \
	type/param.mli \
	type/version.mli \
	type/view.mli \
	type/conn.mli \
	route/route.mli \
	infr/async.mli \
	trans/real.mli \
	type/alarm.mli \
	type/auth.mli \
	type/domain.mli \
	type/event.mli \
	type/property.mli \
	type/appl_intf.mli \
	type/appl_handle.mli \
	type/layer.mli \
	infr/transport.mli \
	infr/glue.mli \
	infr/stacke.mli \
	appl/arge.mli \
	groupd/mutil.mli \
	groupd/manage.mli \
	appl/appl.mli \
	appl/reflect.mli \
	appl/protos.mli \
	infr/hsyssupp.mli \
	appl/timestamp.mli \
	appl/appl_debug.mli \
	util/printe.mli \
	util/queuea.mli \
	util/once.mli \
	util/priq.mli \
	mm/iq.mli \
	route/handler.mli \
	route/unsigned.mli \
	route/signed.mli \
	infr/config_trans.mli \
	trans/ipmc.mli \
	trans/udp.mli \
	route/bypassr.mli \
	trans/netsim.mli \
	trans/tcp.mli \
	appl/pgp.mli \
	util/arrayop.mli \
	appl/debug.mli \
	appl/appl_aggr.mli \
	appl/appl_power.mli \
	appl/appl_lwe.mli \
	appl/appl_multi.mli \
	appl/handle.mli \
	appl/partition.mli \
	groupd/proxy.mli \
	groupd/member.mli \
	groupd/coord.mli \
	groupd/actual.mli \
	util/heap.mli \
	$(NULL)

SOURCES_ML_CORE = \
	util/printe.ml \
	util/trans.ml \
	util/util.ml \
	util/queuee.ml \
	util/trace.ml \
	util/arraye.ml \
	util/arrayf.ml \
	mm/buf.ml \
	mm/iovec.ml \
	mm/iovecl.ml \
	util/hsys.ml \
	util/fqueue.ml \
	util/queuea.ml \
	util/lset.ml \
	util/once.ml \
	util/priq.ml \
	util/resource.ml \
	util/sched.ml \
	mm/iq.ml \
	util/marsh.ml \
	type/time.ml \
	type/addr.ml \
	type/version.ml \
	type/security.ml \
	type/shared.ml \
	util/tree.ml \
	type/tdefs.ml \
	util/mrekey_dt.ml \
	util/diamond.ml \
	type/proto.ml \
	type/stack_id.ml \
	type/unique.ml \
	type/endpt.ml \
	type/group.ml \
	type/param.ml \
	type/view.ml \
	type/conn.ml \
	route/handler.ml \
	route/route.ml \
	infr/async.ml \
	type/alarm.ml \
	type/auth.ml \
	type/domain.ml \
	type/event.ml \
	type/property.ml \
	type/appl_intf.ml \
	type/appl_handle.ml \
	type/layer.ml \
	infr/transport.ml \
	route/unsigned.ml \
	route/signed.ml \
	infr/config_trans.ml \
	infr/glue.ml \
	appl/arge.ml \
	infr/stacke.ml \
	appl/timestamp.ml \
	trans/ipmc.ml \
	trans/udp.ml \
	trans/real.ml \
	route/bypassr.ml \
	infr/hsyssupp.ml \
	trans/netsim.ml \
	trans/tcp.ml \
	appl/pgp.ml \
	util/arrayop.ml \
	appl/debug.ml \
	appl/appl_debug.ml \
	appl/appl_aggr.ml \
	appl/appl_power.ml \
	appl/appl_lwe.ml \
	appl/appl_multi.ml \
	appl/handle.ml \
	appl/partition.ml \
	appl/protos.ml \
	groupd/mutil.ml \
	groupd/proxy.ml \
	groupd/member.ml \
	groupd/coord.ml \
	groupd/actual.ml \
	groupd/manage.ml \
	util/heap.ml \
	appl/appl.ml \
	appl/reflect.ml \
	$(NULL)

SOURCES_MLI_MIN = \
	util/mcredit.mli \
	$(NULL)

SOURCES_ML_MIN = \
	layers/other/top_appl.ml \
	layers/other/top.ml \
	layers/other/partial_appl.ml \
	layers/trans/stable.ml \
	layers/trans/bottom.ml \
	layers/trans/mnak.ml \
	layers/trans/pt2pt.ml \
	layers/vsync/suspect.ml \
	layers/vsync/merge.ml \
	layers/vsync/inter.ml \
	layers/vsync/intra.ml \
	layers/vsync/elect.ml \
	layers/trans/frag.ml \
	layers/trans/frag_abv.ml \
	layers/vsync/leave.ml \
	layers/vsync/sync.ml \
	layers/vsync/vsync.ml \
	layers/vsync/slander.ml \
	layers/gossip/heal.ml \
	layers/flow/pt2ptw.ml \
	layers/flow/pt2ptwp.ml \
	util/mcredit.ml \
	layers/flow/mflow.ml \
	layers/total/sequencer.ml \
	layers/bypass/fpmb.ml \
	$(NULL)

SOURCES_MLI_REST = \
	util/mexchange.mli \
	util/request.mli \
	util/mcausal.mli \
	util/dtbl.mli \
	util/dtblbatch.mli \
	infr/disp.mli \
	$(NULL)

SOURCES_ML_REST = \
	layers/gossip/switch.ml \
	util/mexchange.ml \
	layers/security/exchange.ml \
	layers/security/rekey.ml \
	layers/security/rekey_dt.ml \
	layers/security/rekey_diam.ml \
	layers/security/secchan.ml \
	layers/other/local.ml \
	layers/other/cltsvr.ml \
	layers/other/xfer.ml \
	layers/other/subcast.ml \
	layers/other/migrate.ml \
	layers/security/optrekey.ml \
	layers/security/realkeys.ml \
	layers/security/perfrekey.ml \
	layers/security/encrypt.ml \
	layers/other/primary.ml \
	layers/vsync/present.ml \
	layers/flow/window.ml \
	layers/other/collect.ml \
	util/request.ml \
	layers/total/total.ml \
	layers/total/totem.ml \
	layers/total/seqbb.ml \
	layers/total/tops.ml \
	layers/scale/asym.ml \
	layers/debug/assert.ml \
	layers/debug/delay.ml \
	layers/debug/drop.ml \
	layers/debug/chk_secchan.ml \
	layers/debug/chk_rekey.ml \
	layers/debug/chk_fifo.ml \
	layers/debug/chk_total.ml \
	layers/debug/chk_sync.ml \
	layers/scale/pr_stable.ml \
	layers/scale/pr_suspect.ml \
	layers/scale/gcast.ml \
	layers/scale/pbcast.ml \
	layers/scale/zbcast.ml \
	layers/debug/chk_causal.ml \
	util/mcausal.ml \
	util/diamond_aux.ml \
	layers/trans/causal.ml \
	util/dtbl.ml \
	util/dtblbatch.ml \
	infr/disp.ml \
	layers/debug/dbg.ml \
	layers/debug/dbgbatch.ml \
	$(NULL)

SOURCES_MLI = $(SOURCES_MLI_CORE) $(SOURCES_MLI_MIN) $(SOURCES_MLI_REST)
SOURCES_ML = $(SOURCES_ML_CORE) $(SOURCES_ML_MIN) $(SOURCES_ML_REST)

@MK@ifneq ($(srcdir),.)
$(SOURCES_MLI) $(SOURCES_ML) : % : $(srcdir)/%
	@if ! test -d $(dir $@); then echo $(MKDIR) $(dir $@); $(MKDIR) $(dir $@); fi
	$(CP) $< $@
@MK@endif

OBJECTS_CMI_CORE =$(patsubst %.mli, %.cmi, $(SOURCES_MLI_CORE))
OBJECTS_CMI_MIN =$(patsubst %.mli, %.cmi, $(SOURCES_MLI_MIN))
OBJECTS_CMI_REST =$(patsubst %.mli, %.cmi, $(SOURCES_MLI_REST))

OBJECTS_CMO_CORE =$(patsubst %.ml, %.cmo, $(SOURCES_ML_CORE))
OBJECTS_CMO_MIN =$(patsubst %.ml, %.cmo, $(SOURCES_ML_MIN))
OBJECTS_CMO_REST =$(patsubst %.ml, %.cmo, $(SOURCES_ML_REST))

$(OBJECTS_CMO_CORE): $(OBJECTS_CMI_CORE)
$(OBJECTS_CMO_MIN): $(OBJECTS_CMI_MIN)
$(OBJECTS_CMO_REST): $(OBJECTS_CMI_REST)

OBJECTS_CMI = $(OBJECTS_CMI_CORE) $(OBJECTS_CMI_MIN) $(OBJECTS_CMI_REST)
OBJECTS_CMO = $(OBJECTS_CMO_CORE) $(OBJECTS_CMO_MIN) $(OBJECTS_CMO_REST)

all: libenscore.cma libensmin.cma libensrest.cma

emrg.ml: $(srcdir)/tools/emrg.ml
	$(CP) $< $@

emrg.cmo: emrg.ml
	$(OCAML_COMPILER) -c -I $(srcdir)/tools -o $@ $<

emrg: emrg.cmo
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -o emrg emrg.cmo

ENSEMBLE_MLI_DEPS = $(SOURCES_MLI_CORE)

ensemble.mli: $(ENSEMBLE_MLI_DEPS) emrg
	./emrg -mli $(ENSEMBLE_MLI_DEPS) -o $@

ensemble.ml: $(ENSEMBLE_MLI_DEPS) emrg
	./emrg -ml $(ENSEMBLE_MLI_DEPS) -o $@

ensemble.cmo: ensemble.cmi

LIBENSCORE_CMA_DEPS =$(OBJECTS_CMO_CORE) ensemble.cmo

libenscore.cma:	$(LIBENSCORE_CMA_DEPS)
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -a -o $@ -linkall $(LIBENSCORE_CMA_DEPS)

LIBENSMIN_CMA_DEPS =$(OBJECTS_CMO_MIN)

libensmin.cma:	$(LIBENSMIN_CMA_DEPS)
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -a -o $@ -linkall $(LIBENSMIN_CMA_DEPS)

LIBENSREST_CMA_DEPS =$(OBJECTS_CMO_REST)

libensrest.cma:	$(LIBENSREST_CMA_DEPS)
	$(OCAML_COMPILER) $(OCAML_COMPILER_FLAGS) -a -o $@ -linkall $(LIBENSREST_CMA_DEPS)

clean-local:
	$(RM) $(OBJECTS_CMI) $(OBJECTS_CMO) $(OBJECTS_CMO:.cmo=.cmi)
@MK@ifneq ($(srcdir),.)
	$(RM) -r appl groupd infr layers mm route tools trans type util
@MK@endif
	$(RM) emrg.mli emrg.ml emrg.cmi emrg.cmo emrg
	$(RM) ensemble.cmi ensemble.cmo ensemble.ml ensemble.mli
	$(RM) libenscore.cma
	$(RM) libensmin.cma
	$(RM) libensrest.cma

EXTRA_DIST= \
	$(SOURCES_MLI) \
	$(SOURCES_ML) \
	$(srcdir)/appl/README \
	$(srcdir)/groupd/README \
	$(srcdir)/infr/README \
	$(srcdir)/layers/README \
	$(srcdir)/layers/bypass/bypass.ml \
	$(srcdir)/layers/bypass/bypfifo.ml \
	$(srcdir)/layers/debug/chk_agree.ml \
	$(srcdir)/layers/debug/chk_evs.ml \
	$(srcdir)/layers/debug/chk_hi_causal.ml \
	$(srcdir)/layers/debug/chk_total_fifo.ml \
	$(srcdir)/layers/debug/dbm.ml \
	$(srcdir)/layers/debug/log.ml \
	$(srcdir)/layers/flow/credit.ml \
	$(srcdir)/layers/flow/rate.ml \
	$(srcdir)/mm/README \
	$(srcdir)/route/README \
	$(srcdir)/tools/client.ml \
	$(srcdir)/tools/dist.ml \
	$(srcdir)/tools/emrg.ml \
	$(srcdir)/trans/htk.ml \
	$(srcdir)/trans/htk.mli \
	$(srcdir)/trans/README \
	$(srcdir)/trans/atm/README \
	$(srcdir)/trans/atm/atm.h \
	$(srcdir)/trans/atm/atm.ml \
	$(srcdir)/trans/atm/atm.mli \
	$(srcdir)/trans/atm/atm_ocaml.c \
	$(srcdir)/trans/atm/atm_unet.c \
	$(srcdir)/trans/threaded.ml \
	$(srcdir)/trans/threaded.mli \
	$(srcdir)/trans/mpi/mpic.c \
	$(srcdir)/trans/mpi/mpi.ml \
	$(srcdir)/tools/coord.ml \
	$(srcdir)/tools/coord.mli \
	$(srcdir)/type/README \
	$(srcdir)/util/README \
	$(srcdir)/util/hashtble.ml \
	$(srcdir)/util/hashtble.mli \
	$(NULL)
