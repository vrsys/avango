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

#if !defined(AVANGO_OSG_SWITCH_H)
#define AVANGO_OSG_SWITCH_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Switch>
#include <avango/osg/Group.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Switch OsgSwitch;

    /**
     * Wrapper for ::osg::Switch
     *
     * VisibleChild = 0     (all children on)
     * VisibleChild = 1..n  (single child on, others off)
     * VisibleChild < 0     (all children off)
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Switch : public Group
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Switch is created.
       * Otherwise, the given ::osg::Switch is used.
       */
      Switch(OsgSwitch* osgswitch = new OsgSwitch());
      // use defined type to circumvent compiler bug in VS8

      SFInt VisibleChild;

      /**
       * Get the wrapped ::osg::Switch.
       */
      ::osg::Switch* getOsgSwitch() const;

      /* virtual */ void evaluateLocalSideEffect();

    private:

      ::osg::Switch *mOsgSwitch;
      bool mVisibleChildChanged;
      int mVisibleChild;
      void updateVisibleChild();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Switch();

      virtual void getVisibleChildCB(const av::SFInt::GetValueEvent& event);
      virtual void setVisibleChildCB(const av::SFInt::SetValueEvent& event);
      virtual void setChildrenCB(const av::osg::MFNode::SetValueEvent& event);

    };

    typedef SingleField<Link<Switch> > SFSwitch;
    typedef MultiField<Link<Switch> > MFSwitch;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Switch> >;
  template class AV_OSG_DLL MultiField<Link<osg::Switch> >;
#endif

}

#endif
