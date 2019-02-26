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

#include <avango/osg/Capsule.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Capsule"));
}

AV_FC_DEFINE(av::osg::Capsule);
AV_FIELD_DEFINE(av::osg::SFCapsule);
AV_FIELD_DEFINE(av::osg::MFCapsule);

av::osg::Capsule::Capsule() : MatrixTransform(), mGeode(new ::osg::Geode), mRadiusChanged(false), mHeightChanged(false)
{
    AV_FC_ADD_FIELD(Radius, 1.0f);
    AV_FC_ADD_FIELD(Height, 1.0f);

    AV_FC_ADD_FIELD(Color, ::osg::Vec4(1, 1, 1, 1));

    mCapsule = new ::osg::Capsule(::osg::Vec3(), Radius.getValue(), Height.getValue());

    mShapeDrawable = new ::osg::ShapeDrawable(mCapsule.get());
    mShapeDrawable.get()->setColor(Color.getValue());

    mGeode.get()->addDrawable(mShapeDrawable.get());

    getOsgMatrixTransform()->addChild(mGeode.get());
}

av::osg::Capsule::~Capsule() {}

void av::osg::Capsule::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::MatrixTransform::initClass();

        AV_FC_INIT(av::osg::MatrixTransform, av::osg::Capsule, true);

        SFCapsule::initClass("av::osg::SFCapsule", "av::Field");
        MFCapsule::initClass("av::osg::MFCapsule", "av::Field");
    }
}

/* virtual */ void av::osg::Capsule::fieldHasChangedLocalSideEffect(const av::Field& field)
{
    MatrixTransform::fieldHasChangedLocalSideEffect(field);
    AVANGO_LOG(logger, av::logging::TRACE, "fieldHasChangedLocalSideEffect()");

    if(&field == &Radius)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Radius changed to %1%") % Radius.getValue()));
        mRadiusChanged = true;
    }

    if(&field == &Height)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Height changed to %1%") % Height.getValue()));
        mHeightChanged = true;
    }

    if(&field == &Color)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Color changed to %1%") % &Color.getValue()));
        mColorChanged = true;
    }
}

/* virtual */ void av::osg::Capsule::evaluateLocalSideEffect()
{
    MatrixTransform::evaluateLocalSideEffect();
    LOG_TRACE(logger) << "evaluateLocalSideEffect()";

    if(mHeightChanged)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new height %1%") % Height.getValue()));
        mCapsule->setHeight(Height.getValue());
        mShapeDrawable->dirtyDisplayList();
        mShapeDrawable->dirtyBound();
        mHeightChanged = false;
    }

    if(mRadiusChanged)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new radius %1%") % Radius.getValue()));
        mCapsule->setRadius(Radius.getValue());
        mShapeDrawable->dirtyDisplayList();
        mShapeDrawable->dirtyBound();
        mRadiusChanged = false;
    }

    if(mColorChanged)
    {
        AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new Color %1%") % &Color.getValue()));
        mShapeDrawable.get()->setColor(Color.getValue());
        mColorChanged = false;
    }
}

::osg::ref_ptr<::osg::Capsule> av::osg::Capsule::getOsgCapsule() const { return mCapsule; }

::osg::ref_ptr<::osg::ShapeDrawable> av::osg::Capsule::getOsgShapeDrawable() const { return mShapeDrawable; }
