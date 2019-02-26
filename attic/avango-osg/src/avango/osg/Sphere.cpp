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

#include <osg/Vec4>
#include <osg/Geode>

#include <avango/Logger.h>
#include <avango/osg/Types.h>

#include <avango/osg/Sphere.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Sphere"));
}

AV_FC_DEFINE(av::osg::Sphere);

AV_FIELD_DEFINE(av::osg::SFSphere);
AV_FIELD_DEFINE(av::osg::MFSphere);

av::osg::Sphere::Sphere() : MatrixTransform(), mHints(new ::osg::TessellationHints), mGeode(new ::osg::Geode), mRadiusChanged(false), mDetailRatioChanged(false)
{
    AV_FC_ADD_FIELD(Radius, 1.0f);
    AV_FC_ADD_FIELD(DetailRatio, 0.5f);
    AV_FC_ADD_FIELD(Color, ::osg::Vec4(1, 1, 1, 1));

    mSphere = new ::osg::Sphere(::osg::Vec3(), Radius.getValue());

    mHints->setDetailRatio(DetailRatio.getValue());

    mShapeDrawable = new ::osg::ShapeDrawable(mSphere.get(), mHints.get());
    mShapeDrawable.get()->setColor(Color.getValue());

    mGeode.get()->addDrawable(mShapeDrawable.get());

    getOsgMatrixTransform()->addChild(mGeode.get());
}

av::osg::Sphere::~Sphere() {}

void av::osg::Sphere::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::MatrixTransform::initClass();

        AV_FC_INIT(av::osg::MatrixTransform, av::osg::Sphere, true);

        SFSphere::initClass("av::osg::SFSphere", "av::Field");
        MFSphere::initClass("av::osg::MFSphere", "av::Field");
    }
}

/* virtual */ void av::osg::Sphere::fieldHasChangedLocalSideEffect(const av::Field& field)
{
    MatrixTransform::fieldHasChangedLocalSideEffect(field);
    AVANGO_LOG(logger, av::logging::TRACE, "fieldHasChangedLocalSideEffect()");

    if(&field == &Radius)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Radius changed to %1%") % Radius.getValue()));
        mRadiusChanged = true;
    }

    if(&field == &DetailRatio)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): DetailRatio changed to %1%") % DetailRatio.getValue()));
        mDetailRatioChanged = true;
    }

    if(&field == &Color)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Color changed to %1%") % &Color.getValue()));
        mColorChanged = true;
    }
}

/* virtual */ void av::osg::Sphere::evaluateLocalSideEffect()
{
    MatrixTransform::evaluateLocalSideEffect();
    LOG_TRACE(logger) << "evaluateLocalSideEffect()";

    if(mRadiusChanged)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new Radius %1%") % Radius.getValue()));
        mSphere->setRadius(Radius.getValue());
        mShapeDrawable->dirtyDisplayList();
        mShapeDrawable->dirtyBound();
        mRadiusChanged = false;
    }

    if(mDetailRatioChanged)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new DetailRatio %1%") % DetailRatio.getValue()));
        mHints->setDetailRatio(DetailRatio.getValue());
        mShapeDrawable->dirtyDisplayList();
        mShapeDrawable->dirtyBound();
        mDetailRatioChanged = false;
    }

    if(mColorChanged)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new Color %1%") % &Color.getValue()));
        mShapeDrawable.get()->setColor(Color.getValue());
        mColorChanged = false;
    }
}

::osg::ref_ptr<::osg::Sphere> av::osg::Sphere::getOsgSphere() const { return mSphere; }

::osg::ref_ptr<::osg::ShapeDrawable> av::osg::Sphere::getOsgShapeDrawable() const { return mShapeDrawable; }
