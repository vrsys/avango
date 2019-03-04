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

#include <boost/bind.hpp>
#include <avango/osg/Geometry.h>
#include <avango/Logger.h>
#include <osg/Version>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Geometry"));
}

AV_FC_DEFINE(av::osg::Geometry);

AV_FIELD_DEFINE(av::osg::SFGeometry);
AV_FIELD_DEFINE(av::osg::MFGeometry);

av::osg::Geometry::Geometry(::osg::Geometry* osggeometry) : Drawable(osggeometry), mOsgGeometry(osggeometry)
{
    AV_FC_ADD_ADAPTOR_FIELD(VertexArray, boost::bind(&Geometry::getVertexArrayCB, this, _1), boost::bind(&Geometry::setVertexArrayCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(NormalArray, boost::bind(&Geometry::getNormalArrayCB, this, _1), boost::bind(&Geometry::setNormalArrayCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(ColorArray, boost::bind(&Geometry::getColorArrayCB, this, _1), boost::bind(&Geometry::setColorArrayCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(TexCoordArray, boost::bind(&Geometry::getTexCoordArrayCB, this, _1), boost::bind(&Geometry::setTexCoordArrayCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(NormalBinding, boost::bind(&Geometry::getNormalBindingCB, this, _1), boost::bind(&Geometry::setNormalBindingCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(ColorBinding, boost::bind(&Geometry::getColorBindingCB, this, _1), boost::bind(&Geometry::setColorBindingCB, this, _1));

    AV_FC_ADD_FIELD(Mode, DISABLED);
    AV_FC_ADD_FIELD(PrimitiveIndices, MFUInt::ContainerType());
    AV_FC_ADD_FIELD(PrimitiveLengths, MFInt::ContainerType());
}

av::osg::Geometry::~Geometry() {}

void av::osg::Geometry::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Drawable::initClass();

        AV_FC_INIT(av::osg::Drawable, av::osg::Geometry, true);

        SFGeometry::initClass("av::osg::SFGeometry", "av::Field");
        MFGeometry::initClass("av::osg::MFGeometry", "av::Field");
    }
}

::osg::Geometry* av::osg::Geometry::getOsgGeometry() const { return mOsgGeometry; }

/* virtual */ void av::osg::Geometry::evaluateLocalSideEffect()
{
    av::osg::Drawable::evaluateLocalSideEffect();

    const unsigned int mode = Mode.getValue();
    if(mode < DISABLED && mOsgGeometry->getVertexArray() != 0)
    {
#if OPENSCENEGRAPH_MAJOR_VERSION == 2 && OPENSCENEGRAPH_MINOR_VERSION == 2
        // workaround for a bug in OSG
        MFUInt::ContainerType& prim_inds = const_cast<MFUInt::ContainerType&>(PrimitiveIndices.getValue());
#else
        const MFUInt::ContainerType& prim_inds = PrimitiveIndices.getValue();
#endif

        MFInt::ContainerType prim_lengths = PrimitiveLengths.getValue();
        ::osg::Geometry::PrimitiveSetList prim_sets;

        if(prim_inds.empty())
        {
            if(prim_lengths.empty())
            {
                const GLsizei num_vertices = static_cast<GLsizei>(mOsgGeometry->getVertexArray()->getNumElements());
                prim_sets.push_back(new ::osg::DrawArrays(mode, 0, num_vertices));
            }
            else
            {
                const unsigned int num_lengths = static_cast<unsigned int>(prim_lengths.size());
                prim_sets.push_back(new ::osg::DrawArrayLengths(mode, 0, num_lengths, &prim_lengths[0]));
            }
        }
        else
        {
            if(prim_lengths.empty())
            {
                const unsigned int num_indices = static_cast<unsigned int>(prim_inds.size());
                prim_sets.push_back(new ::osg::DrawElementsUInt(mode, num_indices, &prim_inds[0]));
            }
            else
            {
                int cur_index = 0;
                for(MFInt::ContainerType::const_iterator prim_length = prim_lengths.begin(); prim_length != prim_lengths.end(); ++prim_length)
                {
                    if(cur_index + *prim_length <= static_cast<int>(prim_inds.size()))
                    {
                        const unsigned int num_indices = static_cast<unsigned int>(*prim_length);
                        prim_sets.push_back(new ::osg::DrawElementsUInt(mode, num_indices, &prim_inds[cur_index]));
                        cur_index += *prim_length;
                    }
                    else
                    {
                        AVANGO_LOG(logger, av::logging::WARN, "not enough indices for given primitive lengths");
                        break;
                    }
                }
            }
        }

        mOsgGeometry->setPrimitiveSetList(prim_sets);
        mOsgGeometry->computeFastPathsUsed();
    }
}

/* virtual */ void av::osg::Geometry::getVertexArrayCB(const MFVec3::GetValueEvent& event)
{
    ::osg::ref_ptr<::osg::Vec3Array> array = dynamic_cast<::osg::Vec3Array*>(mOsgGeometry->getVertexArray());
    if(array.valid())
    {
        const ::osg::Vec3* data_pointer = static_cast<const ::osg::Vec3*>(array->getDataPointer());
        MFVec3::ContainerType tmpContainer(data_pointer, data_pointer + array->getNumElements());
        event.getValuePtr()->swap(tmpContainer);
    }
    else
        event.getValuePtr()->clear();
}

/* virtual */ void av::osg::Geometry::setVertexArrayCB(const MFVec3::SetValueEvent& event)
{
    if(!event.getValue().empty())
    {
        ::osg::ref_ptr<::osg::Vec3Array> array = dynamic_cast<::osg::Vec3Array*>(mOsgGeometry->getVertexArray());
        if(array.valid())
        {
            array->clear();
            array->assign(event.getValue().begin(), event.getValue().end());
            array->dirty();
        }
        else
        {
            array = new ::osg::Vec3Array(event.getValue().begin(), event.getValue().end());
            mOsgGeometry->setVertexArray(array.get());
        }
    }
    else if(mOsgGeometry->getVertexArray() != 0)
    {
        mOsgGeometry->setVertexArray(0);
    }
}

/* virtual */ void av::osg::Geometry::getNormalArrayCB(const MFVec3::GetValueEvent& event)
{
    ::osg::ref_ptr<::osg::Vec3Array> array = dynamic_cast<::osg::Vec3Array*>(mOsgGeometry->getNormalArray());
    if(array.valid())
    {
        const ::osg::Vec3* data_pointer = static_cast<const ::osg::Vec3*>(array->getDataPointer());
        MFVec3::ContainerType tmpContainer(data_pointer, data_pointer + array->getNumElements());
        event.getValuePtr()->swap(tmpContainer);
    }
    else
        event.getValuePtr()->clear();
}

/* virtual */ void av::osg::Geometry::setNormalArrayCB(const MFVec3::SetValueEvent& event)
{
    if(!event.getValue().empty())
    {
        ::osg::ref_ptr<::osg::Vec3Array> array = dynamic_cast<::osg::Vec3Array*>(mOsgGeometry->getNormalArray());
        if(array.valid())
        {
            array->clear();
            array->assign(event.getValue().begin(), event.getValue().end());
            array->dirty();
        }
        else
        {
            array = new ::osg::Vec3Array(event.getValue().begin(), event.getValue().end());
            mOsgGeometry->setNormalArray(array.get());
        }
    }
    else if(mOsgGeometry->getNormalArray() != 0)
        mOsgGeometry->setNormalArray(0);
}

/* virtual */ void av::osg::Geometry::getColorArrayCB(const MFVec4::GetValueEvent& event)
{
    ::osg::ref_ptr<::osg::Vec4Array> array = dynamic_cast<::osg::Vec4Array*>(mOsgGeometry->getColorArray());
    if(array.valid())
    {
        const ::osg::Vec4* data_pointer = static_cast<const ::osg::Vec4*>(array->getDataPointer());
        MFVec4::ContainerType tmpContainer(data_pointer, data_pointer + array->getNumElements());
        event.getValuePtr()->swap(tmpContainer);
    }
    else
        event.getValuePtr()->clear();
}

/* virtual */ void av::osg::Geometry::setColorArrayCB(const MFVec4::SetValueEvent& event)
{
    if(!event.getValue().empty())
    {
        ::osg::ref_ptr<::osg::Vec4Array> array = dynamic_cast<::osg::Vec4Array*>(mOsgGeometry->getColorArray());
        if(array.valid())
        {
            array->clear();
            array->assign(event.getValue().begin(), event.getValue().end());
            array->dirty();
        }
        else
        {
            array = new ::osg::Vec4Array(event.getValue().begin(), event.getValue().end());
            mOsgGeometry->setColorArray(array.get());
        }
    }
    else if(mOsgGeometry->getColorArray() != 0)
        mOsgGeometry->setColorArray(0);
}

/* virtual */ void av::osg::Geometry::getTexCoordArrayCB(const MFVec2::GetValueEvent& event)
{
    ::osg::ref_ptr<::osg::Vec2Array> array = dynamic_cast<::osg::Vec2Array*>(mOsgGeometry->getTexCoordArray(0u));
    if(array.valid())
    {
        const ::osg::Vec2* data_pointer = static_cast<const ::osg::Vec2*>(array->getDataPointer());
        MFVec2::ContainerType tmpContainer(data_pointer, data_pointer + array->getNumElements());
        event.getValuePtr()->swap(tmpContainer);
    }
    else
        event.getValuePtr()->clear();
}

/* virtual */ void av::osg::Geometry::setTexCoordArrayCB(const MFVec2::SetValueEvent& event)
{
    if(!event.getValue().empty())
    {
        ::osg::ref_ptr<::osg::Vec2Array> array = dynamic_cast<::osg::Vec2Array*>(mOsgGeometry->getTexCoordArray(0u));
        if(array.valid())
        {
            array->clear();
            array->assign(event.getValue().begin(), event.getValue().end());
            array->dirty();
        }
        else
        {
            array = new ::osg::Vec2Array(event.getValue().begin(), event.getValue().end());
            mOsgGeometry->setTexCoordArray(0u, array.get());
        }
    }
    else if(mOsgGeometry->getTexCoordArray(0u) != 0)
        mOsgGeometry->setTexCoordArray(0u, 0);
}

/* virtual */ void av::osg::Geometry::getNormalBindingCB(const SFInt::GetValueEvent& event) { *event.getValuePtr() = static_cast<int>(mOsgGeometry->getNormalBinding()); }

/* virtual */ void av::osg::Geometry::setNormalBindingCB(const SFInt::SetValueEvent& event) { mOsgGeometry->setNormalBinding(static_cast<::osg::Geometry::AttributeBinding>(event.getValue())); }

/* virtual */ void av::osg::Geometry::getColorBindingCB(const SFInt::GetValueEvent& event) { *event.getValuePtr() = static_cast<int>(mOsgGeometry->getColorBinding()); }

/* virtual */ void av::osg::Geometry::setColorBindingCB(const SFInt::SetValueEvent& event) { mOsgGeometry->setColorBinding(static_cast<::osg::Geometry::AttributeBinding>(event.getValue())); }
