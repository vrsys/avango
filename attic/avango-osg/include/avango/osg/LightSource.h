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

#if !defined(AVANGO_OSG_LIGHTSOURCE_H)
#define AVANGO_OSG_LIGHTSOURCE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/LightSource>
#include <avango/osg/Fields.h>
#include <avango/osg/Group.h>
#include <avango/osg/Light.h>

namespace av
{
namespace osg
{
typedef ::osg::LightSource OsgLightSource;

/**
 * Wrapper for ::osg::LightSource
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL LightSource : public Group
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::LightSource is created.
     * Otherwise, the given ::osg::LightSource is used.
     */
    LightSource(OsgLightSource* osglightsource = new OsgLightSource());
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~LightSource();

  public:
    av::osg::SFLight Light;

    /**
     * Get the wrapped ::osg::LightSource.
     */
    ::osg::LightSource* getOsgLightSource() const;

    void touchFields();

  protected:
    virtual void getLightCB(const av::osg::SFLight::GetValueEvent& event);
    virtual void setLightCB(const av::osg::SFLight::SetValueEvent& event);

  private:
    ::osg::LightSource* mOsgLightSource;
};

typedef SingleField<Link<LightSource>> SFLightSource;
typedef MultiField<Link<LightSource>> MFLightSource;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::LightSource>>;
template class AV_OSG_DLL MultiField<Link<osg::LightSource>>;
#endif

} // namespace av

#endif
