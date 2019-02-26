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

#if !defined(AVANGO_OSG_SHADER_H)
#define AVANGO_OSG_SHADER_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Shader>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
namespace osg
{
typedef ::osg::Shader OsgShader;

/**
 * Wrapper for ::osg::Shader
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Shader : public Object
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::Shader is created.
     * Otherwise, the given ::osg::Shader is used.
     */
    Shader(OsgShader* osgshader = new OsgShader()); // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Shader();

  public:
    SFInt Type;
    SFString ShaderSource;
    SFString FileName;

    /**
     * Get the wrapped ::osg::Shader.
     */
    ::osg::Shader* getOsgShader() const;

    void touchFields();

  protected:
    virtual void getTypeCB(const av::SFInt::GetValueEvent& event);
    virtual void setTypeCB(const av::SFInt::SetValueEvent& event);
    virtual void getShaderSourceCB(const av::SFString::GetValueEvent& event);
    virtual void setShaderSourceCB(const av::SFString::SetValueEvent& event);
    virtual void getFileNameCB(const av::SFString::GetValueEvent& event);
    virtual void setFileNameCB(const av::SFString::SetValueEvent& event);

  private:
    ::osg::Shader* mOsgShader;
};

typedef SingleField<Link<Shader>> SFShader;
typedef MultiField<Link<Shader>> MFShader;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Shader>>;
template class AV_OSG_DLL MultiField<Link<osg::Shader>>;
#endif

} // namespace av

#endif
