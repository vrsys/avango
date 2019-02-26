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

#if !defined(AVANGO_SHADE_STATE_H)
#define AVANGO_SHADE_STATE_H

/**
 * \file
 * \ingroup av_shade
 */

#include <avango/shade/Types.h>
#include <avango/FieldContainer.h>
#include <boost/shared_ptr.hpp>
#include "windows_specific_shade.h"

namespace osg
{
class Node;
}

namespace shade
{
class Program;

namespace osg
{
class Wrapper;
}
} // namespace shade

namespace av
{
namespace shade
{
typedef SingleField<::osg::Object::DataVariance> SFDataVariance;

/**
 * A State represents one compiled shader program
 *
 * \ingroup av_shade
 */
class AV_SHADE_DLL State : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    State();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~State();

  public:
    /**
     * Root shader
     */
    av::SFContainer Shader;

    /**
     * Data Variance of OSG StateSet
     */
    SFDataVariance DataVariance;

    /**
     * Apply contained ::osg::Shader to given node
     */
    void applyState(::osg::Node* node);

    /**
     * Compile shader and upload data
     */
    void update(void);

    /* virtual */ void fieldHasChanged(const av::Field& field);

  private:
    void getDataVarianceCB(const SFDataVariance::GetValueEvent& event);
    void setDataVarianceCB(const SFDataVariance::SetValueEvent& event);

    boost::shared_ptr<::shade::Program> mProgram;
    ::osg::ref_ptr<::osg::StateSet> mOsgStateSet;
    boost::shared_ptr<::shade::osg::Wrapper> mState;
};

typedef SingleField<Link<State>> SFState;
typedef MultiField<Link<State>> MFState;
} // namespace shade

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_SHADE_DLL SingleField<::osg::Object::DataVariance>;
template class AV_SHADE_DLL SingleField<Link<shade::State>>;
template class AV_SHADE_DLL MultiField<Link<shade::State>>;
#endif
} // namespace av

#endif
