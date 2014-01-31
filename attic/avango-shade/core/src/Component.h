// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#ifndef shade_Component_H
#define shade_Component_H shade_Component_H


#include <shade/Formatter.h>
#include <shade/Type.h>
#include <shade/Shader.h>
#include <shade/ShaderEnvironment.h>
#include <shade/FileAccumulator.h>
#include <shade/PipelineState.h>
#include <shade/GLSLWrapper.h>
#include <string>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>


namespace shade
{

  class Component
  {
  public:

    Component(boost::shared_ptr<Shader> shader);

    bool has_entry(ShaderEnvironment env);

    // Generate source code
    void setup(FileAccumulator& acc, PipelineState ps);
    void compile(Formatter& fmt, const FileAccumulator& acc, ShaderEnvironment env, formatter::Constants::Primitive primitive_type = formatter::Constants::points);
    bool requires_compilation(void);

    // Update index map for used variables
    void link(boost::shared_ptr<GLSLWrapper>);

    // Transfer values to GPU
    void upload(boost::shared_ptr<GLSLWrapper>);
    bool requires_upload(void);

    typedef std::map< const std::type_info*, std::list<int> > Classbins;

  private:

    void output_attribute(Formatter& fmt, const Type&) const;
    void output_property_dispatcher(Formatter& fmt, std::pair<const Type*, ObjectMap::Index>) const;

    boost::shared_ptr<Type::State> get_type_state(const Type*);

    void add_attribute(const Type*, const std::string&, const std::string&, ObjectMap::Index ri);
    void attribute_notify(const Type*);

    boost::shared_ptr<Shader> m_shader;

    bool m_requires_upload;
    bool m_requires_compilation;
    std::set<const shade::Type*> m_modified_attributes;

    struct TypeState
    {
      shade::Type::connection connection;
      std::string obj_id;
      std::string name;
      std::string full_name;
      shade::Type::LinkIndex link_index;
      boost::shared_ptr<Type::State> state;
    };

    typedef std::map<const shade::Type*, TypeState> attribute_map;
    attribute_map m_attributes;

    shade::ObjectMap m_objects;
    Classbins m_classbins;
    PipelineState m_pipeline_state;

    std::string m_sources;
  };

}

#endif /* shade_Component_H */
