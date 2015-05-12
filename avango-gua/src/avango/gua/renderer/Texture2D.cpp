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

#include <avango/gua/renderer/Texture2D.hpp>
#include <avango/Logger.h>
#include <boost/bind.hpp>
#include <cstring>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Texture2D"));
}

AV_FC_DEFINE(av::gua::Texture2D);

AV_FIELD_DEFINE(av::gua::SFTexture2D);
AV_FIELD_DEFINE(av::gua::MFTexture2D);

av::gua::Texture2D::Texture2D(
  std::shared_ptr< ::gua::Texture2D> const& guaTexture) :
      //Texture(guaTexture),
      m_guaTexture2D(guaTexture)
{
  AV_FC_ADD_ADAPTOR_FIELD(Bytes,
                          boost::bind(&Texture2D::getBytesCB, this, _1),
                          boost::bind(&Texture2D::setBytesCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Width,
                          boost::bind(&Texture2D::getWidthCB, this, _1),
                          boost::bind(&Texture2D::setWidthCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Height,
                          boost::bind(&Texture2D::getHeightCB, this, _1),
                          boost::bind(&Texture2D::setHeightCB, this, _1));
}

/* static */ void
av::gua::Texture2D::initClass()
{
  if (!isTypeInitialized()) {
    av::FieldContainer::initClass();

    //AV_FC_INIT(av::gua::Texture, av::gua::Texture2D, true);
    AV_FC_INIT(av::FieldContainer, av::gua::Texture2D, true);

    SFTexture2D::initClass("av::gua::SFTexture2D", "av::Field");
    MFTexture2D::initClass("av::gua::MFTexture2D", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

std::shared_ptr< ::gua::Texture2D> const&
av::gua::Texture2D::getGuaTexture2D() const
{
  return m_guaTexture2D;
}

/* virtual */ void
av::gua::Texture2D::getBytesCB(const av::SFString::GetValueEvent& event)
{
  char const* data = static_cast<char const*>(m_guaTexture2D->base_layer_data());
  std::size_t n = m_guaTexture2D->base_layer_size();
  if (nullptr != data && 0 < n) {
    *(event.getValuePtr()) = std::string(data, data + n);
  }
}

/* virtual */ void
av::gua::Texture2D::setBytesCB(const av::SFString::SetValueEvent& event)
{
  // copy
  std::size_t n = event.getValue().size();
  char* p = new char[n];
  std::memcpy(p, event.getValue().c_str(), n);
  char* old = static_cast<char*>(m_guaTexture2D->base_layer_data());
  if (old) {
    delete [] old;
  }
  m_guaTexture2D->base_layer_data(p);
}

/* virtual */ void
av::gua::Texture2D::getWidthCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTexture2D->width();
}

/* virtual */ void
av::gua::Texture2D::setWidthCB(const av::SFInt::SetValueEvent& event)
{
  m_guaTexture2D->width(event.getValue());
}

/* virtual */ void
av::gua::Texture2D::getHeightCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTexture2D->height();
}

/* virtual */ void
av::gua::Texture2D::setHeightCB(const av::SFInt::SetValueEvent& event)
{
  m_guaTexture2D->height(event.getValue());
}
