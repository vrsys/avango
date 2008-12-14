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

#if !defined(AVANGO_OSG_TEXTURE_H)
#define AVANGO_OSG_TEXTURE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Object.h>
#include <osg/Texture>
#include <avango/osg/Fields.h>
#include <avango/StandardFields.h>

namespace av
{
  namespace osg
  {
    /**
     * Abstract Wrapper for ::osg::Texture
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Texture : public Object
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor.
       */
      Texture(::osg::Texture* osgtexture);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Texture();

    public:

      SFInt MinFilter;
      SFInt MagFilter;
      SFFloat MaxAnisotropy;

      /**
       * Get the wrapped ::osg::Texture object.
       * \return a pointer to the texture object
       */
      ::osg::Texture* getOsgTexture() const;

    private:

      ::osg::Texture* mOsgTexture;

    protected:

      virtual void getMinFilterCB(const av::SFInt::GetValueEvent& event);
      virtual void setMinFilterCB(const av::SFInt::SetValueEvent& event);
      virtual void getMagFilterCB(const av::SFInt::GetValueEvent& event);
      virtual void setMagFilterCB(const av::SFInt::SetValueEvent& event);
      virtual void getMaxAnisotropyCB(const av::SFFloat::GetValueEvent& event);
      virtual void setMaxAnisotropyCB(const av::SFFloat::SetValueEvent& event);

    };

    typedef SingleField<Link<Texture> > SFTexture;
    typedef MultiField<Link<Texture> > MFTexture;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Texture> >;
  template class AV_OSG_DLL MultiField<Link<osg::Texture> >;
#endif

} // namespace av

#endif
