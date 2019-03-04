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

#ifndef shade_Type_H
#define shade_Type_H shade_Type_H

#include "ShaderEnvironment.h"
#include "Formatter.h"
#include "PipelineState.h"
#include "GLSLWrapper.h"
#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>

namespace shade
{
class Shader;
class ObjectMap;

class Type
{
  public:
    Type(formatter::Constants::Type type);
    virtual ~Type(void);

    Type(const Type& t);
    Type& operator=(const Type& t);

    /**
     * Returns a cloned copy of itself.
     * @todo Do we require a copy or just a factory function?
     */
    virtual boost::shared_ptr<Type> clone(void) const = 0;

    /**
     * @name State
     */
    //@{

    /**
     * Helper class for state used per Component
     */
    class State
    {
      public:
        virtual ~State(void) {}
    };

    virtual boost::shared_ptr<State> create_state(const PipelineState& ps) const;

    //@}

    /**
     * @name Traits
     */
    //@{

    /**
     * Does the type require an initialization?
     * E.g. because it has a const qualifier.
     */
    virtual bool requires_initializer(void) const;

    /**
     * Is this type writable from the shading language?
     * @param se Environment the type is used in
     */
    virtual bool is_writable(const ShaderEnvironment& se) const;

    /**
     * Can this type exist in a certain environment?
     * E.g. a attribute qualified type does not exist in a fragment shader.
     * @param se Environment to be tested
     */
    virtual bool exists(const ShaderEnvironment& se) const;

    /**
     * Returns GLSL code that to be used in constructor of type
     * This variant gives a default value without a reference to a real state.
     */
    virtual std::string get_constructor_str(void) const;

    /**
     * Returns GLSL code that to be used in constructor of type
     * @param state State object created for specific instance of Component
     */
    virtual std::string get_constructor_str(boost::shared_ptr<Type::State> state) const;

    /**
     * Returns the Formatter Type
     */
    formatter::Constants::Type get_formatter_type(void) const;

    /**
     * Returns a unique id of the type a string usable as a variable
     * in the shading language
     */
    virtual std::string get_uniq_id(void) const;

    /**
     * Generates a constructor of the current value
     */
    virtual void generate_constructor(formatter::Generator& generator, boost::shared_ptr<Type::State> state) const;

    /**
     * Generates a constructor of the current value
     * This variant gives a default value without a reference to a real state.
     */
    virtual void generate_constructor(formatter::Generator& generator) const;

    //@}

    /**
     * @name Invocation
     */
    //@{

    /**
     * Transport a function call via a type object.
     * This is used in @ref ShaderBase derived proxy classes which have methods with
     * return types of Type.
     * @param func Name of function to invoke
     * @param ti Type-info of class to invoke this function on
     * @param env Shader-Environment in which this method exists
     */
    void invoke(const std::string& func, const std::type_info* ti);

    /**
     * Transport a inline method via a type object.
     * This is used in @ref ShaderBase derived proxy classes which have methods with
     * return types of Type.
     * @param generator Function that generates the body of the method
     * @param ti Type-info of class to invoke this function on
     * @param env Shader-Environment in which this method exists
     */
    void invoke_inline(boost::function<void(formatter::Generator&)> generator, const std::type_info* ti);

    /**
     * Generates a previously set method body.
     */
    void generate_inline(formatter::Generator&) const;

    /**
     * Returns a previously set function call name.
     */
    std::string get_function_name(void) const;

    /**
     * Returns a previously set type-info for the class performing the
     * invocation.
     */
    const std::type_info* get_function_type(void) const;

    /**
     * Returns whether a function call was set
     */
    bool has_function(void) const;

    /**
     * Returns whether a method body was set
     */
    bool has_inline_function(void) const;
    //@}

    /**
     * @name Traversing and Collecting
     * Methods used by the compilation stage to find and enumerate all
     * referenced @ref Shader objects.
     */
    virtual void collect_references(boost::shared_ptr<Type::State> state, boost::function<void(Shader*)>) const;
    virtual void insert_references(boost::shared_ptr<Type::State>, ObjectMap&) const;

    //@}

    /**
     * @name OpenGL bindings
     */
    //@{

    typedef GLSLWrapper::Handle LinkProgram;
    typedef GLSLWrapper::Handle LinkIndex;
    virtual LinkIndex get_link_index(boost::shared_ptr<GLSLWrapper>, const std::string& name) const;
    virtual void upload(boost::shared_ptr<GLSLWrapper>, LinkIndex) const;

    //@}

    /**
     * @name Output to formatter
     */
    //@{

    virtual void output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const;
    virtual bool output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const;
    virtual void output_insert_property_dispatcher(boost::shared_ptr<Type::State>, Formatter* fmt, ObjectMap::Index index, const std::string& obj) const;

    //@}

    /**
     * @name Observer
     */
    //@{

    typedef boost::signal<void(const Type*)> signal;
    typedef boost::signals::connection connection;
    shade::Type::connection register_notify(signal::slot_function_type) const;

  protected:
    void touch(void);

    virtual void upload_uniform(boost::shared_ptr<GLSLWrapper>, LinkIndex) const;

    //@}

  protected:
    formatter::Constants::Type m_type;

    std::string join_names(const std::string& a, const std::string& b) const;

  private:
    std::string m_invoke;
    boost::function<void(formatter::Generator&)> m_invoke_generator;
    bool m_invoke_is_inline;
    const std::type_info* m_invoke_ti;
    mutable signal m_modified_cb;
};

} // namespace shade

#endif /* shade_Type_H */
