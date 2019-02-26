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

#if !defined(AVANGO_OSG_VIEWER_VIEW_H)
#define AVANGO_OSG_VIEWER_VIEW_H

/**
 * \file
 * \ingroup av_osg_viewer
 */

#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include <avango/osg/Object.h>
#include <avango/osg/Node.h>
#include <avango/osg/viewer/Camera.h>
#include <avango/osg/viewer/windows_specific_osg_viewer.h>

namespace av
{
namespace osg
{
namespace viewer
{
/**
 * Wrapper for ::osgViewer::View.
 *
 * \ingroup av_osg_viewer
 */
class AV_OSG_VIEWER_DLL View : public av::osg::Object
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osgViewer::View is created.
     * Otherwise, the given ::osgViewer::View is used.
     */
    View(::osgViewer::View* osgview = new ::osgViewer::View);

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~View();

  public:
    /**
     * So called master camera (see ::osg::View documentation)
     */
    SFCamera MasterCamera;

    /**
     * Slave cameras that can render the scene from another position.
     */
    MFCamera SlaveCameras;

    /**
     * Scene to render. Required for the opening of the windows.
     */
    SFNode Scene;

    /**
     * Sets the displayed statistics mode:
     * 0 = No statistics
     * 1 = Frame rate only
     * 2 = Full statistics
     *
     * See StatsHandler::StatsType in OpenSceneGraph/include/osgViewer/ViewerEventHandlers
     */
    SFInt StatsMode;

    /* virtual */ void fieldHasChangedLocalSideEffect(const Field& field);
    /* virtual */ void evaluateLocalSideEffect();

    /**
     * Get the wrapped ::osgViewer::View.
     */
    ::osgViewer::View* getOsgView() const;

    virtual void getMasterCameraCB(const SFCamera::GetValueEvent& event);
    virtual void setMasterCameraCB(const SFCamera::SetValueEvent& event);

    virtual void getSceneCB(const SFNode::GetValueEvent& event);
    virtual void setSceneCB(const SFNode::SetValueEvent& event);

  private:
    ::osgViewer::View* mOsgView;
    ::osg::ref_ptr<::osgGA::StateSetManipulator> mOsgStateSetManipulator;
    ::osg::ref_ptr<::osgViewer::StatsHandler> mOsgStatsHandler;
    ::osg::ref_ptr<::osgGA::GUIEventAdapter> mOsgStatsToggle;

    int mStatsMode;
};

typedef SingleField<Link<View>> SFView;
typedef MultiField<Link<View>> MFView;

} // namespace viewer

} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_VIEWER_DLL SingleField<Link<osg::viewer::View>>;
template class AV_OSG_VIEWER_DLL MultiField<Link<osg::viewer::View>>;
#endif

} // namespace av

#endif
