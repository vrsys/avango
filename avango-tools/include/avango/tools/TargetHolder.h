// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_TOOLS_TARGETHOLDER_H)
#define AVANGO_TOOLS_TARGETHOLDER_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/FieldContainer.h>
#include <avango/tools/Config.h>
#include "windows_specific_tools.h"

namespace av
{
namespace tools
{
class TargetHolder;
using SFTargetHolder = SingleField<Link<TargetHolder>>;
using MFTargetHolder = MultiField<Link<TargetHolder>>;

/**
 * TargetHolder base class
 *
 * TargetHolders are created by or passed through Selectors and passed to Tools.
 * They typically hold an av::FieldContainer reference that should be manipulated by a tool.
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL TargetHolder : public av::FieldContainer
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
    SFContainer Target;

    /**
     * May be used to define the creator of this TargetHolder instance, typically a Selector.
     */
    SFContainer Creator;

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

    using FindList = std::list<Link<TargetHolder>>;

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
 * Shortcut for std::find in av::MFContainer values.
 */
inline bool hasObject(const av::MFContainer::ContainerType& container, const av::MFContainer::ValueType& value) { return (std::find(container.begin(), container.end(), value) != container.end()); }

/**
 * Search in MFTargetHolder values.
 */
inline MFTargetHolder::ContainerType::iterator find(MFTargetHolder::ContainerType& holders, const SFContainer::ValueType& target)
{
    MFTargetHolder::ContainerType::iterator holder = holders.begin();
    while(holder != holders.end() && (*holder)->Target.getValue() != target)
        ++holder;
    return holder;
}

inline bool hasTarget(MFTargetHolder::ContainerType& holders, const SFContainer::ValueType& target) { return (find(holders, target) != holders.end()); }

/**
 * Search in const MFTargetHolder values.
 */
inline MFTargetHolder::ContainerType::const_iterator find(const MFTargetHolder::ContainerType& holders, const SFContainer::ValueType& target)
{
    MFTargetHolder::ContainerType::const_iterator holder = holders.begin();
    while(holder != holders.end() && (*holder)->Target.getValue() != target)
        ++holder;
    return holder;
}

inline bool hasTarget(const MFTargetHolder::ContainerType& holders, const SFContainer::ValueType& target) { return (find(holders, target) != holders.end()); }
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::TargetHolder>>;
template class AV_TOOLS_DLL MultiField<Link<tools::TargetHolder>>;
#endif

} // namespace av

#endif
