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

#if !defined(AVANGO_OSG_PROGRAM_H)
#define AVANGO_OSG_PROGRAM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Program>
#include <avango/osg/Shader.h>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>


namespace av
{
  namespace osg
  {
    typedef ::osg::Program OsgProgram;

    /**
     * Wrapper for ::osg::Program
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Program : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Program is created.
       * Otherwise, the given ::osg::Program is used.
       */
      Program(OsgProgram *osgprogram = new OsgProgram()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Program();

    public:

      MFShader ShaderList;

      /**
       * Get the wrapped ::osg::Program.
       */
      ::osg::Program* getOsgProgram() const;

      void touchFields();

    protected:

      virtual void getShaderListCB(const av::osg::MFShader::GetValueEvent& event);
      virtual void setShaderListCB(const av::osg::MFShader::SetValueEvent& event);

    private:

      ::osg::Program *mOsgProgram;
    };

    typedef SingleField<Link<Program> > SFProgram;
    typedef MultiField<Link<Program> > MFProgram;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Program> >;
  template class AV_OSG_DLL MultiField<Link<osg::Program> >;
#endif

}

#endif
