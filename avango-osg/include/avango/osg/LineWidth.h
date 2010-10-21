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

#if !defined(AVANGO_OSG_LINEWIDTH_H)
#define AVANGO_OSG_LINEWIDTH_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/LineWidth>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::LineWidth OsgLineWidth;

    /**
     * Wrapper for ::osg::LineWidth
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL LineWidth : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::LineWidth is created.
       * Otherwise, the given ::osg::LineWidth is used.
       */
      LineWidth(OsgLineWidth* osglinewidth = new OsgLineWidth());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LineWidth();

    public:

      SFFloat Width;
      /**
       * Get the wrapped ::osg::LineWidth.
       */
      ::osg::LineWidth* getOsgLineWidth() const;

    protected:

      virtual void getWidthCB(const av::SFFloat::GetValueEvent& event);
      virtual void setWidthCB(const av::SFFloat::SetValueEvent& event);

    private:

      ::osg::LineWidth *mOsgLineWidth;
    };

    typedef SingleField<Link<LineWidth> > SFLineWidth;
    typedef MultiField<Link<LineWidth> > MFLineWidth;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::LineWidth> >;
  template class AV_OSG_DLL MultiField<Link<osg::LineWidth> >;
#endif

}

#endif
