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

#if !defined(AVANGO_TOOLS_TARGETHOLDER_H)
#define AVANGO_TOOLS_TARGETHOLDER_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/Object.h>

#include <avango/tools/Config.h>

namespace av
{
  namespace tools
  {
    class TargetHolder;
    typedef SingleField<Link<TargetHolder> > SFTargetHolder;
    typedef MultiField<Link<TargetHolder> > MFTargetHolder;

    /**
     * TargetHolder base class
     *
     * TargetHolders are created by or passed through Selectors and passed to Tools.
     * They typically hold an av::Object reference that should be manipulated by a tool.
     *
     * \ingroup av_tools
     */
    class TargetHolder : public av::Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TargetHolder();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TargetHolder();

    public:

      /**
       * Defines the target that should be used by a Selector or Tool.
       */
      SFObject Target;

      /**
       * May be used to define the creator of this TargetHolder instance, typically a Selector.
       */
      SFObject Creator;

      /**
       * Selectors put incoming TargetHolders here, if they want to replace the Targetholder
       * with a new one and the Keep value is true.
       */
      MFTargetHolder ParentTargets;

      /**
       * If false, this TargetHolder may be replaced with another one, which is default.
       * If true, it is kept as a parent from the new TargetHolder.
       */
      SFBool Keep;

      typedef std::list<Link<TargetHolder> > FindList;

      /**
       * Searches self and recursively all parents for the given or derived types of target holders.
       */
      FindList find(Type type);

      /**
       * Returns true if this target holder should be kept, when a new one with a matching
       * target is created.
       */
      virtual bool keep();

    private:

      void find(Type type, FindList& foundHolders);
    };

    /**
     * Shortcut for std::find in av::MFObject values.
     */
    inline bool
    hasObject(const av::MFObject::ContainerType& container, const av::MFObject::ValueType& value)
    {
      return (std::find(container.begin(), container.end(), value) != container.end());
    }

    /**
     * Search in MFTargetHolder values.
     */
    inline MFTargetHolder::ContainerType::iterator
    find(MFTargetHolder::ContainerType& holders, const SFObject::ValueType& target)
    {
      MFTargetHolder::ContainerType::iterator holder = holders.begin();
      while (holder != holders.end() && (*holder)->Target.getValue() != target)
        ++holder;
      return holder;
    }

    inline bool
    hasTarget(MFTargetHolder::ContainerType& holders, const SFObject::ValueType& target)
    {
      return (find(holders, target) != holders.end());
    }

    /**
     * Search in const MFTargetHolder values.
     */
    inline MFTargetHolder::ContainerType::const_iterator
    find(const MFTargetHolder::ContainerType& holders, const SFObject::ValueType& target)
    {
      MFTargetHolder::ContainerType::const_iterator holder = holders.begin();
      while (holder != holders.end() && (*holder)->Target.getValue() != target)
        ++holder;
      return holder;
    }

    inline bool
    hasTarget(const MFTargetHolder::ContainerType& holders, const SFObject::ValueType& target)
    {
      return (find(holders, target) != holders.end());
    }
  }
}

#endif
