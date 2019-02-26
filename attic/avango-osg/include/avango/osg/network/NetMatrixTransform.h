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

#if !defined(AVANGO_OSG_NETMATRIXTRANSFORM_H)
#define AVANGO_OSG_NETMATRIXTRANSFORM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/MatrixTransform.h>
#include <avango/NetNode.h>
#include <avango/Application.h>

#ifdef __GNUC__ // GNU C++ stores TR1 headers differently
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

namespace av
{
namespace osg
{
class FragmentGroup;
class SharedContainerHolder;
/**
 *  \container_doc
 *   this works like fpDCS exept that the children are sorted by creator.
 *   each creator has an own group node here which is well known.
 *  \end_container_doc
 */
class NetMatrixTransform : public MatrixTransform, public NetNode
{
    AV_FC_DECLARE();

  public:
    typedef std::tr1::unordered_map<std::string, Link<FragmentGroup>> EIDGrpMap;
    NetMatrixTransform();
    virtual ~NetMatrixTransform();

    // the fields for the network part

    /**
     *  The Groupname field specifies the string identifier for the distribution
     *  group this node will represent. The empty string will result in no distribution.
     */
    SFString Groupname; ///< distribution group name

    /**
     *  The Members field gives the endponit identifiers for all processes
     *  currently participating in this distribution group.
     */
    MFString Members;             ///< distribution group members
    MFString NewMembers;          ///< newly added members since last update
    MFString DepartedMembers;     ///< departed members since last update
    SFString NetId;               ///< endpoint id of the current process
    MFContainer SharedContainers; ///< list of containers shared by all members

    // tell our clients we joined ( virtual from NetNode )
    /* virtual */ void _join(const std::string& fragment);

    // overloaded from NetNode to react to view changes
    /* virtual */ void _acceptNewView(const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&);

    // overloaded to participate in state transfers
    /* virtual */ void _getStateFragment(const std::string& fragment, Msg& stateMsg);
    /* virtual */ void _setStateFragment(const std::string& fragment, Msg& stateMsg);
    /* virtual */ void _removeStateFragment(const std::string& fragment);

    // notification uplink from fragment groups
    void fragmentChildrenChanged();
    // notification uplink from shared containers node
    void sharedContainersChanged();

  protected:
    /// function description
    /* virtual */ void fieldHasChangedLocalSideEffect(const Field&);

    /// function description
    /* virtual */ void evaluate();

    // virtual from av::osg::Group
    /* virtual */ void getChildrenCB(const av::osg::MFNode::GetValueEvent& event);
    /* virtual */ void setChildrenCB(const av::osg::MFNode::SetValueEvent& event);

  protected:
    // reimplemented as no-op
    /* virtual */ void refImpl();
    /* virtual */ void unrefImpl();
    /* virtual */ int refCountImpl();

    virtual void getSharedContainersCB(const MFContainer::GetValueEvent& event);
    virtual void setSharedContainersCB(const MFContainer::SetValueEvent& event);

  private:
    // the current view, meaning the current set of participants in this group
    EIDGrpMap mGroupMap;

    // all nodes which are not distributed are added here
    FragmentGroup* mLocalGroups;
    typedef std::tr1::unordered_map<std::string, Link<SharedContainerHolder>> SharedContainerMap;
    SharedContainerMap mSharedContainerMap;

    Application::CallbackHandle mPreEvalHandle;
    Application::CallbackHandle mPostEvalHandle;
};

typedef SingleField<Link<NetMatrixTransform>> SFNetMatrixTransform;
typedef MultiField<Link<NetMatrixTransform>> MFNetMatrixTransform;

} // namespace osg

} // namespace av

#endif
