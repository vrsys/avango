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

#if !defined(AVANGO_OSG_FOG_H)
#define AVANGO_OSG_FOG_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Fog>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>


namespace av
{
  namespace osg
  {
    typedef ::osg::Fog OsgFog;

    /**
     * Wrapper for ::osg::Fog
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Fog : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Fog is created.
       * Otherwise, the given ::osg::Fog is used.
       */
      Fog(OsgFog *osgfog = new OsgFog()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Fog();

    public:

      SFFloat Density;
      SFFloat Start;
      SFFloat End;
      SFVec4 Color;

      /**
       * Get the wrapped ::osg::Fog.
       */
      ::osg::Fog* getOsgFog() const;

      void touchFields();

    protected:

      virtual void getDensityCB(const av::SFFloat::GetValueEvent& event);
      virtual void setDensityCB(const av::SFFloat::SetValueEvent& event);
      virtual void getStartCB(const av::SFFloat::GetValueEvent& event);
      virtual void setStartCB(const av::SFFloat::SetValueEvent& event);
      virtual void getEndCB(const av::SFFloat::GetValueEvent& event);
      virtual void setEndCB(const av::SFFloat::SetValueEvent& event);
      virtual void getColorCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setColorCB(const av::osg::SFVec4::SetValueEvent& event);

    private:

      ::osg::Fog *mOsgFog;
    };

    typedef SingleField<Link<Fog> > SFFog;
    typedef MultiField<Link<Fog> > MFFog;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Fog> >;
  template class AV_OSG_DLL MultiField<Link<osg::Fog> >;
#endif

}

#endif
