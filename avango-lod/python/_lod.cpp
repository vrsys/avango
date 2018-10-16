// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/
#include "LodLoader.hpp"
#include "MLodPassDescription.hpp"
#include "PLodPassDescription.hpp"
#include "MLodNode.hpp"
#include "PLodNode.hpp"
#include "Aux.hpp"
#include "OctreeNode.hpp"
#include "AuxAtlas.hpp"
#include "AuxAtlasTile.hpp"
#include "AuxFeature.hpp"
#include "AuxSparsePoint.hpp"
#include "AuxView.hpp"
#include <avango/gua/lod/renderer/LodLoader.hpp>
#include <avango/gua/lod/renderer/MLodPassDescription.hpp>
#include <avango/gua/lod/renderer/PLodPassDescription.hpp>
#include <avango/gua/lod/scenegraph/MLodNode.hpp>
#include <avango/gua/lod/scenegraph/PLodNode.hpp>
#include <avango/gua/lod/renderer/Aux.hpp>
#include <avango/gua/lod/renderer/OctreeNode.hpp>
#include <avango/gua/lod/renderer/AuxAtlas.hpp>
#include <avango/gua/lod/renderer/AuxAtlasTile.hpp>
#include <avango/gua/lod/renderer/AuxFeature.hpp>
#include <avango/gua/lod/renderer/AuxSparsePoint.hpp>
#include <avango/gua/lod/renderer/AuxView.hpp>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

BOOST_PYTHON_MODULE(_lod)
{
  av::gua::lod::LodLoader::initClass();
  av::gua::lod::MLodPassDescription::initClass();
  av::gua::lod::PLodPassDescription::initClass();
  av::gua::lod::MLodNode::initClass();
  av::gua::lod::PLodNode::initClass();
  av::gua::lod::Aux::initClass();
  av::gua::lod::OctreeNode::initClass();
  av::gua::lod::AuxAtlas::initClass();
  av::gua::lod::AuxAtlasTile::initClass();
  av::gua::lod::AuxFeature::initClass();
  av::gua::lod::AuxSparsePoint::initClass();
  av::gua::lod::AuxView::initClass();
  init_MLodNode();
  init_PLodNode();
  init_MLodPassDescription();
  init_PLodPassDescription();
  init_LodLoader();
  init_Aux();
  init_OctreeNode();
  init_AuxAtlas();
  init_AuxAtlasTile();
  init_AuxFeature();
  init_AuxSparsePoint();
  init_AuxView();
}
