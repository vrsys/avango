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

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/CoordinateSystemNode>

#include <osg/Group>
#include <osg/NodeCallback>
#include <osg/ref_ptr>

#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <avango/Application.h>
#include <avango/Type.h>
#include <avango/osg/Node.h>

#include <boost/python.hpp>

using namespace boost::python;

namespace
{
class EvaluateCallback : public osg::NodeCallback
{
    /*virtual*/ void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        av::ApplicationInstance::get().evaluate();
        traverse(node, nv);
    }
};

osg::ref_ptr<osgViewer::Viewer> viewer(0);

void setupViewer(osgViewer::Viewer* viewer, av::osg::Node* root)
{
    // camera manipulator
    viewer->setCameraManipulator(new osgGA::TrackballManipulator());

    // add the state manipulator
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

    // add the thread model handler
    viewer->addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer->addEventHandler(new osgViewer::WindowSizeHandler);

    // add the stats handler
    viewer->addEventHandler(new osgViewer::StatsHandler);

    // add the record camera path handler
    viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

    // add root node to scene graph
    osg::Group* viewer_root = new osg::Group();
    viewer->setSceneData(viewer_root);

    // hint to tell viewer to request stencil buffer when setting up window
    osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

    // set up window dimensions
    viewer->setUpViewInWindow(-1, -1, 400, 400);

    // add Avango root not to our root
    viewer_root->addChild(root->getOsgNode());
}

class EmbeddedViewer
{
  public:
    EmbeddedViewer(av::osg::Node* root, int x, int y, int width, int height)
    {
        mViewer = new osgViewer::Viewer;
        mViewer->setSceneData(root->getOsgNode());
        mGraphicsWindow = mViewer->setUpViewerAsEmbeddedInWindow(x, y, width, height);
    }

    void resized(int x, int y, int width, int height)
    {
        if(!mGraphicsWindow)
            return;

        mGraphicsWindow->resized(x, y, width, height);
    }

    void frame(void) { mViewer->frame(); }

  private:
    osg::ref_ptr<osgViewer::Viewer> mViewer;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
};
} // namespace

void run(av::osg::Node* root)
{
    viewer = new osgViewer::Viewer;
    setupViewer(viewer.get(), root);
    EvaluateCallback* ec = new EvaluateCallback();
    viewer->getSceneData()->setUpdateCallback(ec);
    viewer->run();
}

void realize(av::osg::Node* root)
{
    viewer = new osgViewer::Viewer;
    setupViewer(viewer.get(), root);
    EvaluateCallback* ec = new EvaluateCallback();
    viewer->getSceneData()->setUpdateCallback(ec);
    viewer->realize();
}

void frame()
{
    if(viewer.valid() && !viewer->done())
    {
        viewer->frame();
    }
}

bool done() { return viewer->done(); }

BOOST_PYTHON_MODULE(_simpleviewer)
{
    def("run", run);
    def("realize", realize);
    def("frame", frame);
    def("done", done);

    class_<EmbeddedViewer>("EmbeddedViewer", init<av::osg::Node*, int, int, int, int>()).def("resized", &EmbeddedViewer::resized).def("frame", &EmbeddedViewer::frame);
}
