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

#if !defined(AVANGO_OSG_UNIFORM_H)
#define AVANGO_OSG_UNIFORM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Uniform>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>


namespace av
{
  namespace osg
  {
    typedef ::osg::Uniform OsgUniform;

    /**
     * Wrapper for ::osg::Uniform
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Uniform : public Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Uniform is created.
       * Otherwise, the given ::osg::Uniform is used.
       */
      Uniform(OsgUniform *osguniform = new OsgUniform()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Uniform();

    public:

      SFInt Type;
      SFString UniformName;
      MFFloat Values;

      /**
       * Get the wrapped ::osg::Uniform.
       */
      ::osg::Uniform* getOsgUniform() const;

      void touchFields();

    protected:

      virtual void getTypeCB(const av::SFInt::GetValueEvent& event);
      virtual void setTypeCB(const av::SFInt::SetValueEvent& event);
      virtual void getValuesCB(const av::MFFloat::GetValueEvent& event);
      virtual void setValuesCB(const av::MFFloat::SetValueEvent& event);
      virtual void getUniformNameCB(const av::SFString::GetValueEvent& event);
      virtual void setUniformNameCB(const av::SFString::SetValueEvent& event);

    private:

      ::osg::Uniform *mOsgUniform;
    };

    typedef SingleField<Link<Uniform> > SFUniform;
    typedef MultiField<Link<Uniform> > MFUniform;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Uniform> >;
  template class AV_OSG_DLL MultiField<Link<osg::Uniform> >;
#endif

}

#endif
