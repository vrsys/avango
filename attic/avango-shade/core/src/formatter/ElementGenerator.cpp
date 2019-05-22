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

#include "ElementGenerator.h"
#include "../parser/Element.h"
#include "../parser/Scope.h"
#include "../parser/ReferenceValue.h"
#include "../parser/ListValue.h"
#include "../parser/IntValue.h"
#include "../parser/IntConvertableValue.h"
#include "../parser/ValueList.h"
#include "../parser/MethodTemplate.h"
#include <boost/shared_ptr.hpp>
#include <iterator>
#include <algorithm>
using namespace shade::formatter;
using namespace shade::parser;

namespace
{
class ContentState
{
  public:
    ContentState(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : m_generator(generator), m_scope(scope), m_call(call), m_error_log(error_log) {}

  protected:
    Generator& m_generator;
    Scope& m_scope;
    const FunctionCall& m_call;
    std::ostream& m_error_log;
};

class IdentifierElement : public Element
{
  public:
    IdentifierElement(std::string name) : m_name(name) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_identifier(m_name); }

    /*virtual*/ boost::shared_ptr<const Value> evaluate(const Scope& scope) const { return scope.get_value(m_name); }

    std::string value(void) const { return m_name; }

  private:
    std::string m_name;
};

class VerbatimElement : public Element
{
  public:
    VerbatimElement(std::string code) : m_code(code) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_verbatim(m_code); }

  private:
    std::string m_code;
};

class LiteralObjrefElement : public Element
{
  public:
    LiteralObjrefElement(ReferenceValue::Index ref) : m_ref(ref) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_objref(m_ref); }

    /*virtual*/ boost::shared_ptr<const Value> evaluate(const Scope& scope) const { return boost::shared_ptr<const Value>(new ReferenceValue(m_ref)); }

    ReferenceValue::Index value(void) const { return m_ref; }

  private:
    ReferenceValue::Index m_ref;
};

class LiteralListElement : public Element
{
  public:
    LiteralListElement(ReferenceValue::Index begin, ReferenceValue::Index end) : m_begin(begin), m_end(end) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_list(m_begin, m_end); }

    /*virtual*/ boost::shared_ptr<const Value> evaluate(const Scope& scope) const { return boost::shared_ptr<const Value>(new ListValue(m_begin, m_end)); }

  private:
    ReferenceValue::Index m_begin;
    ReferenceValue::Index m_end;
};

class LiteralIntElement : public Element
{
  public:
    LiteralIntElement(int value) : m_value(value) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_int(m_value); }

    /*virtual*/ boost::shared_ptr<const Value> evaluate(const Scope& scope) const { return boost::shared_ptr<const Value>(new IntValue(m_value)); }

  private:
    int m_value;
};

class LiteralFloatElement : public Element
{
  public:
    LiteralFloatElement(float value) : m_value(value) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_float(m_value); }

  private:
    float m_value;
};

class LiteralVec2Element : public Element
{
  public:
    LiteralVec2Element(float x, float y) : m_x(x), m_y(y) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_vec2(m_x, m_y); }

  private:
    float m_x;
    float m_y;
};

class LiteralVec3Element : public Element
{
  public:
    LiteralVec3Element(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_vec3(m_x, m_y, m_z); }

  private:
    float m_x;
    float m_y;
    float m_z;
};

class LiteralVec4Element : public Element
{
  public:
    LiteralVec4Element(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_literal_vec4(m_x, m_y, m_z, m_w); }

  private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

class TypeElement : public Element
{
  public:
    TypeElement(Constants::Type type) : m_type(type) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_type(m_type); }

    Constants::Type value(void) const { return m_type; }

  private:
    Constants::Type m_type;
};

class ArrayElement : public Element
{
  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_array_begin();
        generator.handle_array_type_begin();
        m_type->get_content(generator, scope, call, error_log);
        generator.handle_array_type_end();
        generator.handle_array_size_begin();
        m_size->get_content(generator, scope, call, error_log);
        generator.handle_array_size_end();
        generator.handle_array_end();
    }

    void set_type(boost::shared_ptr<Element> type) { m_type = type; }

    void set_size(boost::shared_ptr<Element> size) { m_size = size; }

  private:
    boost::shared_ptr<Element> m_type;
    boost::shared_ptr<Element> m_size;
};

class QualifierElement : public Element
{
  public:
    QualifierElement(Constants::Qualifier qualifier) : m_qualifier(qualifier) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_qualifier(m_qualifier); }

  private:
    Constants::Qualifier m_qualifier;
};

class OperatorElement : public Element
{
  public:
    OperatorElement(Constants::Operator op) : m_op(op) {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const { generator.handle_operator(m_op); }

    Constants::Operator value(void) const { return m_op; }

  private:
    Constants::Operator m_op;
};

class FunctionDefinitionElement : public Element, public MethodTemplate
{
    typedef boost::shared_ptr<Element> ElementPtr;
    typedef std::pair<ElementPtr, ElementPtr> ElementPairPtr;

  public:
    FunctionDefinitionElement(const std::string& name) : MethodTemplate(name), m_code_block_enabled(false) {}

    /*virtual*/ bool
    instantiate(const std::string& name, const ValueList& values, Generator& generator, const ScopeLayer& global_scope, std::ostream& error_log, shade::parser::FunctionCall call) const
    {
        Scope scope(global_scope);
        if(values.size() != m_parameters.size())
        {
            error_log << "Internal Error: Non-matching number of parameters in method instantiation for '";
            error_log << name << "' (Expected " << m_parameters.size() << ", got " << values.size() << ")";
            error_log << std::endl;
            return false;
        }

        int i = 0;
        for(ValueList::const_iterator value_iter = values.begin(); value_iter != values.end(); ++value_iter)
        {
            const boost::shared_ptr<const Value>& value(*value_iter);
            if(!m_parameters[i].second)
            {
                error_log << "Internal Error: Invalid parameter name." << std::endl;
                return false;
            }
            IdentifierElement* identifier(dynamic_cast<IdentifierElement*>(m_parameters[i].second.get()));
            if(!identifier)
            {
                error_log << "Internal Error: Invalid parameter name." << std::endl;
                return false;
            }

            scope.add_value(identifier->value(), value);
            ++i;
        }

        get_named_content(generator, scope, call, error_log, name);
        return true;
    }

    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const { get_named_content(generator, scope, call, error_log, get_name()); }

    void set_type(boost::shared_ptr<Element> type) { m_type = type; }

    void set_parameter_type(boost::shared_ptr<Element> type) { m_parameter_type = type; }

    void set_parameter_name(boost::shared_ptr<Element> name)
    {
        m_parameters.push_back(std::make_pair(m_parameter_type, name));
        m_parameter_type.reset();
    }

    void enable_code_block(void) { m_code_block_enabled = true; }
    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

  private:
    class FunctionParameterGenerator : public ContentState
    {
      public:
        FunctionParameterGenerator(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : ContentState(generator, scope, call, error_log) {}

        void operator()(const ElementPairPtr& param)
        {
            if((!param.first) || (!param.second))
            {
                m_error_log << "Internal Error: Incomplete function call parameter" << std::endl;
                return;
            }
            m_generator.handle_function_definition_parameter_type_begin();
            param.first->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_function_definition_parameter_type_end();
            m_generator.handle_function_definition_parameter_name_begin();
            param.second->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_function_definition_parameter_name_end();
        }
    };

    void get_named_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log, const std::string name) const
    {
        if(!m_type)
        {
            error_log << "Internal Error: Incomplete function call" << std::endl;
            return;
        }

        generator.handle_function_definition_begin();
        generator.handle_function_definition_return_type_begin();
        m_type->get_content(generator, scope, call, error_log);
        generator.handle_function_definition_return_type_end();
        generator.handle_function_definition_name_begin();
        generator.handle_identifier(name);
        generator.handle_function_definition_name_end();
        generator.handle_function_definition_parameter_list_begin();
        std::for_each(m_parameters.begin(), m_parameters.end(), FunctionParameterGenerator(generator, scope, call, error_log));
        generator.handle_function_definition_parameter_list_end();
        if(m_code_block_enabled)
        {
            generator.handle_function_definition_code_begin();
            if(m_code)
            {
                m_code->get_content(generator, scope, call, error_log);
            }
            generator.handle_function_definition_code_end();
        }
        generator.handle_function_definition_end();
    }

    ElementPtr m_type;
    ElementPtr m_parameter_type;
    std::vector<std::pair<ElementPtr, ElementPtr>> m_parameters;
    ElementPtr m_code;
    bool m_code_block_enabled;
};

class FunctionCallElement : public Element
{
    typedef boost::shared_ptr<Element> ElementPtr;

  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        if(!m_name)
        {
            error_log << "Internal Error: Incomplete function call" << std::endl;
            return;
        }

        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        IdentifierElement* name(dynamic_cast<IdentifierElement*>(m_name.get()));
        if(name)
        {
            ValueList value_list;
            std::for_each(m_parameters.begin(), m_parameters.end(), ValueListBuilder(value_list, scope));

            std::string translated_name(call(name->value(), value_list, scope.get_global(), error_log));
            generator.handle_identifier(translated_name);
        }
        else
        {
            m_name->get_content(generator, scope, call, error_log);
        }
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        std::for_each(m_parameters.begin(), m_parameters.end(), FunctionCallParameterGenerator(generator, scope, call, error_log));
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }

    /*virtual*/ boost::shared_ptr<const Value> evaluate(const Scope& scope) const
    {
        boost::shared_ptr<const Value> noresult(new Value());
        IdentifierElement* function(dynamic_cast<IdentifierElement*>(m_name.get()));
        if(!function)
            return noresult;

        if(function->value() == "shade_list_begin")
        {
            if(m_parameters.size() != 1)
                return noresult;
            boost::shared_ptr<const Value> value(m_parameters.front()->evaluate(scope));

            const ListValue* list = dynamic_cast<const ListValue*>(value.get());
            if(!list)
                return noresult;

            return boost::shared_ptr<const ReferenceValue>(new ReferenceValue(list->begin()));
        }
        else if(function->value() == "shade_list_end")
        {
            if(m_parameters.size() != 1)
                return noresult;
            boost::shared_ptr<const Value> value(m_parameters.front()->evaluate(scope));

            const ListValue* list = dynamic_cast<const ListValue*>(value.get());
            if(!list)
                return noresult;

            return boost::shared_ptr<const ReferenceValue>(new ReferenceValue(list->end()));
        }
        else if(function->value() == "shade_get_base_object")
        {
            if(m_parameters.size() != 1)
                return noresult;
            boost::shared_ptr<const Value> value(m_parameters.front()->evaluate(scope));

            const ReferenceValue* reference = dynamic_cast<const ReferenceValue*>(value.get());
            if(!reference)
                return noresult;

            boost::shared_ptr<const IntValue> num_classes(boost::dynamic_pointer_cast<const IntValue>(scope.get_value("shade_num_classes")));
            if(!num_classes)
                return noresult;

            ReferenceValue::Index result(get_base_object(reference->get(), num_classes->get()));

            return boost::shared_ptr<const ReferenceValue>(new ReferenceValue(result));
        }
        else if(function->value() == "shade_get_object_index")
        {
            if(m_parameters.size() != 1)
                return noresult;
            boost::shared_ptr<const Value> value(m_parameters.front()->evaluate(scope));

            const ReferenceValue* reference = dynamic_cast<const ReferenceValue*>(value.get());
            if(!reference)
                return noresult;

            boost::shared_ptr<const IntValue> num_classes(boost::dynamic_pointer_cast<const IntValue>(scope.get_value("shade_num_classes")));
            if(!num_classes)
                return noresult;

            int result(get_object_index(reference->get(), num_classes->get()));

            return boost::shared_ptr<const IntValue>(new IntValue(result));
        }
        else if(function->value() == "shade_get_linked_object")
        {
            if(m_parameters.size() != 2)
                return noresult;
            boost::shared_ptr<const Value> value1(m_parameters.front()->evaluate(scope));
            boost::shared_ptr<const Value> value2(m_parameters.back()->evaluate(scope));

            const ReferenceValue* reference = dynamic_cast<const ReferenceValue*>(value1.get());
            if(!reference)
                return noresult;

            const ReferenceValue* index = dynamic_cast<const ReferenceValue*>(value2.get());
            if(!index)
                return noresult;

            boost::shared_ptr<const IntValue> num_classes(boost::dynamic_pointer_cast<const IntValue>(scope.get_value("shade_num_classes")));
            if(!num_classes)
                return noresult;

            ReferenceValue::Index result(get_object(index->get(), get_object_index(reference->get(), num_classes->get()), num_classes->get()));

            return boost::shared_ptr<const ReferenceValue>(new ReferenceValue(result));
        }
        else if(function->value() == "shade_get_object")
        {
            if(m_parameters.size() != 2)
                return noresult;
            boost::shared_ptr<const Value> value1(m_parameters.front()->evaluate(scope));
            boost::shared_ptr<const Value> value2(m_parameters.back()->evaluate(scope));

            const ReferenceValue* reference = dynamic_cast<const ReferenceValue*>(value1.get());
            if(!reference)
                return noresult;

            const IntValue* index = dynamic_cast<const IntValue*>(value2.get());
            if(!index)
                return noresult;

            boost::shared_ptr<const IntValue> num_classes(boost::dynamic_pointer_cast<const IntValue>(scope.get_value("shade_num_classes")));
            if(!num_classes)
                return noresult;

            ReferenceValue::Index result(get_object(reference->get(), index->get(), num_classes->get()));

            return boost::shared_ptr<const ReferenceValue>(new ReferenceValue(result));
        }

        return noresult;
    }

    void set_name(boost::shared_ptr<Element> name) { m_name = name; }

    boost::shared_ptr<Element> get_name(void) { return m_name; }

    void set_parameter(boost::shared_ptr<Element> name) { m_parameters.push_back(name); }

    int get_num_parameters(void) const { return m_parameters.size(); }

    boost::shared_ptr<const Element> get_parameter(int index) { return m_parameters[index]; }

  private:
    ReferenceValue::Index get_base_object(ReferenceValue::Index reference, int num_classes) const { return reference % num_classes; }

    int get_object_index(ReferenceValue::Index reference, int num_classes) const { return (reference - get_base_object(reference, num_classes)) / num_classes; }

    ReferenceValue::Index get_object(ReferenceValue::Index reference, int index, int num_classes) const { return get_base_object(reference, num_classes) + num_classes * index; }

    class ValueListBuilder
    {
      public:
        ValueListBuilder(ValueList& value_list, Scope& scope) : m_value_list(value_list), m_scope(scope) {}

        void operator()(const ElementPtr& param)
        {
            if(!param)
                return;

            m_value_list.add(param->evaluate(m_scope));
        }

      private:
        ValueList& m_value_list;
        Scope& m_scope;
    };

    class FunctionCallParameterGenerator : public ContentState
    {
      public:
        FunctionCallParameterGenerator(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : ContentState(generator, scope, call, error_log) {}

        void operator()(const ElementPtr& param)
        {
            if(!param)
            {
                m_error_log << "Internal Error: Incomplete function parameter." << std::endl;
                return;
            }
            m_generator.handle_function_call_parameter_begin();
            param->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_function_call_parameter_end();
        }
    };

    ElementPtr m_name;
    std::vector<ElementPtr> m_parameters;
};

class VarDefinitionElement : public Element
{
    typedef boost::shared_ptr<Element> ElementPtr;

  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        if((!m_type) || (!m_name))
        {
            error_log << "Internal Error: Incomplete function call" << std::endl;
            return;
        }

        generator.handle_var_definition_begin();
        if(!m_qualifiers.empty())
        {
            generator.handle_var_definition_qualifier_list_begin();
            std::for_each(m_qualifiers.begin(), m_qualifiers.end(), VarDefQualifierGenerator(generator, scope, call, error_log));
            generator.handle_var_definition_qualifier_list_end();
        }
        generator.handle_var_definition_type_begin();
        m_type->get_content(generator, scope, call, error_log);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        m_name->get_content(generator, scope, call, error_log);
        generator.handle_var_definition_name_end();
        if(m_code)
        {
            generator.handle_var_definition_code_begin();
            m_code->get_content(generator, scope, call, error_log);
            generator.handle_var_definition_code_end();

            const IdentifierElement* identifier = dynamic_cast<const IdentifierElement*>(m_name.get());
            if(identifier)
            {
                scope.add_value(identifier->value(), m_code->evaluate(scope));
            }
        }
        generator.handle_var_definition_end();
    }

    void set_type(boost::shared_ptr<Element> type) { m_type = type; }

    boost::shared_ptr<const Element> get_type(void) const { return m_type; }

    void set_name(boost::shared_ptr<Element> name) { m_name = name; }

    boost::shared_ptr<const Element> get_name(void) const { return m_name; }

    void set_qualifier(boost::shared_ptr<Element> qualifier) { m_qualifiers.push_back(qualifier); }

    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

    boost::shared_ptr<const Element> get_code(void) const { return m_code; }

  private:
    class VarDefQualifierGenerator : public ContentState
    {
      public:
        VarDefQualifierGenerator(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : ContentState(generator, scope, call, error_log) {}

        void operator()(const ElementPtr& qualifier)
        {
            m_generator.handle_var_definition_qualifier_begin();
            qualifier->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_var_definition_qualifier_end();
        }
    };

    std::vector<ElementPtr> m_qualifiers;
    ElementPtr m_type;
    ElementPtr m_name;
    ElementPtr m_code;
};

class ForElement : public Element
{
  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        VarDefinitionElement* init(dynamic_cast<VarDefinitionElement*>(m_init.get()));
        if((!init) || (!init->get_type() || (!init->get_name())) || (!init->get_code()))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Element> generic_type(init->get_type());
        const TypeElement* type(dynamic_cast<const TypeElement*>(generic_type.get()));
        if((!type) || (type->value() != Constants::objref))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Element> generic_name(init->get_name());
        const IdentifierElement* name(dynamic_cast<const IdentifierElement*>(generic_name.get()));
        if(!name)
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Value> generic_list_begin(init->get_code()->evaluate(scope));
        const ReferenceValue* list_begin = dynamic_cast<const ReferenceValue*>(generic_list_begin.get());
        if(!list_begin)
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        FunctionCallElement* condition(dynamic_cast<FunctionCallElement*>(m_condition.get()));
        if((!condition) || (!condition->get_name() || (condition->get_num_parameters() != 2)) || (!condition->get_parameter(0)) || (!condition->get_parameter(1)))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Element> generic_condition_operator(condition->get_name());
        const OperatorElement* condition_operator(dynamic_cast<const OperatorElement*>(generic_condition_operator.get()));
        if((!condition_operator) || (condition_operator->value() != Constants::not_equal))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Element> generic_condition_name(condition->get_parameter(0));
        const IdentifierElement* condition_name(dynamic_cast<const IdentifierElement*>(generic_condition_name.get()));
        if((!condition_name) || (condition_name->value() != name->value()))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Value> generic_list_end(condition->get_parameter(1)->evaluate(scope));
        const ReferenceValue* list_end = dynamic_cast<const ReferenceValue*>(generic_list_end.get());
        if(!list_end)
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        FunctionCallElement* increment(dynamic_cast<FunctionCallElement*>(m_increment.get()));
        if((!increment) || (!increment->get_name() || (increment->get_num_parameters() != 1)) || (!increment->get_parameter(0)))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Element> generic_increment_operator(increment->get_name());
        const OperatorElement* increment_operator(dynamic_cast<const OperatorElement*>(generic_increment_operator.get()));
        if((!increment_operator) || (increment_operator->value() != Constants::pre_increment))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        boost::shared_ptr<const Element> generic_increment_name(increment->get_parameter(0));
        const IdentifierElement* increment_name(dynamic_cast<const IdentifierElement*>(generic_increment_name.get()));
        if((!increment_name) || (increment_name->value() != name->value()))
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        get_optimized_content(generator, scope, call, error_log, name->value(), list_begin->get(), list_end->get());
    }

    void set_init(boost::shared_ptr<Element> init) { m_init = init; }

    void set_condition(boost::shared_ptr<Element> condition) { m_condition = condition; }

    void set_increment(boost::shared_ptr<Element> increment) { m_increment = increment; }

    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

  private:
    void get_literal_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_for_begin();
        generator.handle_for_init_begin();
        if(m_init)
            m_init->get_content(generator, scope, call, error_log);
        generator.handle_for_init_end();
        generator.handle_for_condition_begin();
        if(m_condition)
            m_condition->get_content(generator, scope, call, error_log);
        generator.handle_for_condition_end();
        generator.handle_for_increment_begin();
        if(m_increment)
            m_increment->get_content(generator, scope, call, error_log);
        generator.handle_for_increment_end();
        generator.handle_for_code_begin();
        if(m_code)
            m_code->get_content(generator, scope, call, error_log);
        generator.handle_for_code_end();
        generator.handle_for_end();
    }

    void get_optimized_content(Generator& generator,
                               const Scope& scope,
                               const FunctionCall& call,
                               std::ostream& error_log,
                               const std::string& var_name,
                               ReferenceValue::Index list_begin,
                               ReferenceValue::Index list_end) const
    {
        for(ReferenceValue::Index ref = list_begin; ref != list_end; ++ref)
        {
            generator.handle_block_begin();

            generator.handle_statement_begin();
            generator.handle_var_definition_begin();
            generator.handle_var_definition_type_begin();
            generator.handle_type(Constants::objref);
            generator.handle_var_definition_type_end();
            generator.handle_var_definition_name_begin();
            generator.handle_identifier(var_name);
            generator.handle_var_definition_name_end();
            generator.handle_var_definition_code_begin();
            generator.handle_literal_objref(ref);
            generator.handle_var_definition_code_end();
            generator.handle_var_definition_end();
            generator.handle_statement_end();

            Scope inner_scope(scope);
            inner_scope.add_value(var_name, boost::shared_ptr<Value>(new ReferenceValue(ref)));
            m_code->get_content(generator, inner_scope, call, error_log);

            generator.handle_block_end();
        }
    }

    boost::shared_ptr<Element> m_init;
    boost::shared_ptr<Element> m_condition;
    boost::shared_ptr<Element> m_increment;
    boost::shared_ptr<Element> m_code;
};

class StatementElement : public Element
{
  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_statement_begin();
        if(m_code)
        {
            m_code->get_content(generator, scope, call, error_log);
        }
        generator.handle_statement_end();
    }

    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

  private:
    boost::shared_ptr<Element> m_code;
};

class BlockElement : public Element
{
  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_block_begin();
        if(m_code)
        {
            Scope inner_scope(scope);
            m_code->get_content(generator, inner_scope, call, error_log);
        }
        generator.handle_block_end();
    }

    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

  private:
    boost::shared_ptr<Element> m_code;
};

class SwitchElement : public Element
{
    typedef boost::shared_ptr<Element> ElementPtr;
    typedef std::pair<ElementPtr, ElementPtr> ElementPairPtr;

  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        if(!m_value)
        {
            error_log << "Internal Error: Incomplete switch value." << std::endl;
            return;
        }

        boost::shared_ptr<const Value> generic_switch_value(m_value->evaluate(scope));
        const IntConvertableValue* switch_value = dynamic_cast<const IntConvertableValue*>(generic_switch_value.get());
        if(!switch_value)
        {
            get_literal_content(generator, scope, call, error_log);
            return;
        }

        for(std::vector<ElementPairPtr>::const_iterator case_iter = m_cases.begin(); case_iter != m_cases.end(); ++case_iter)
        {
            const ElementPairPtr& case_(*case_iter);
            if((!case_.first) || (!case_.second))
            {
                error_log << "Internal Error: Incomplete switch case." << std::endl;
                continue;
            }

            boost::shared_ptr<const Value> generic_switch_case(case_.first->evaluate(scope));
            const IntConvertableValue* switch_case = dynamic_cast<const IntConvertableValue*>(generic_switch_case.get());
            if(!switch_case)
            {
                get_literal_content(generator, scope, call, error_log);
                return;
            }
            if(switch_value->get_int() != switch_case->get_int())
                continue;

            generator.handle_block_begin();
            Scope inner_scope(scope);
            case_.second->get_content(generator, inner_scope, call, error_log);
            generator.handle_block_end();
            return;
        }

        // All switch values could be evaluated to objrefs but none matched.
        // Thus output default if it exists.

        if(!m_default)
            return;

        generator.handle_block_begin();
        Scope inner_scope(scope);
        m_default->get_content(generator, inner_scope, call, error_log);
        generator.handle_block_end();
    }

    void set_value(boost::shared_ptr<Element> value) { m_value = value; }

    void set_case_value(boost::shared_ptr<Element> value) { m_case_value = value; }

    void set_case_code(boost::shared_ptr<Element> code)
    {
        m_cases.push_back(std::make_pair(m_case_value, code));
        m_case_value.reset();
    }

    void set_default(boost::shared_ptr<Element> default_) { m_default = default_; }

  private:
    class SwitchCaseGenerator : public ContentState
    {
      public:
        SwitchCaseGenerator(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : ContentState(generator, scope, call, error_log) {}

        void operator()(const ElementPairPtr& case_)
        {
            if((!case_.first) || (!case_.second))
            {
                m_error_log << "Internal Error: Incomplete switch case." << std::endl;
                return;
            }
            m_generator.handle_switch_case_begin();
            case_.first->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_switch_case_end();
            m_generator.handle_switch_code_begin();
            case_.second->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_switch_code_end();
        }
    };

    void get_literal_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        m_value->get_content(generator, scope, call, error_log);
        generator.handle_switch_value_end();
        std::for_each(m_cases.begin(), m_cases.end(), SwitchCaseGenerator(generator, scope, call, error_log));
        if(m_default)
        {
            generator.handle_switch_default_begin();
            m_default->get_content(generator, scope, call, error_log);
            generator.handle_switch_default_end();
        }
        generator.handle_switch_end();
    }

    ElementPtr m_value;
    ElementPtr m_case_value;
    std::vector<ElementPairPtr> m_cases;
    ElementPtr m_default;
};

class SelfElement : public Element
{
    typedef boost::shared_ptr<Element> ElementPtr;
    typedef std::pair<ElementPtr, ElementPtr> ElementPairPtr;

  public:
    SelfElement(ElementPtr name) : m_name(name) {}

    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_self_begin();
        generator.handle_self_name_begin();
        m_name->get_content(generator, scope, call, error_log);
        generator.handle_self_name_end();
        generator.handle_self_parameter_list_begin();
        std::for_each(m_parameters.begin(), m_parameters.end(), SelfParameterGenerator(generator, scope, call, error_log));
        generator.handle_self_parameter_list_end();
        generator.handle_self_end();
    }

    void set_parameter_type(boost::shared_ptr<Element> type) { m_parameter_type = type; }

    void set_parameter_name(boost::shared_ptr<Element> name)
    {
        m_parameters.push_back(std::make_pair(m_parameter_type, name));
        m_parameter_type.reset();
    }

  private:
    class SelfParameterGenerator : public ContentState
    {
      public:
        SelfParameterGenerator(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : ContentState(generator, scope, call, error_log) {}

        void operator()(const ElementPairPtr& param)
        {
            if((!param.first) || (!param.second))
            {
                m_error_log << "Internal Error: Incomplete function call parameter" << std::endl;
                return;
            }
            m_generator.handle_self_parameter_type_begin();
            param.first->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_self_parameter_type_end();
            m_generator.handle_self_parameter_name_begin();
            param.second->get_content(m_generator, m_scope, m_call, m_error_log);
            m_generator.handle_self_parameter_name_end();
        }
    };

    ElementPtr m_name;
    ElementPtr m_parameter_type;
    std::vector<std::pair<ElementPtr, ElementPtr>> m_parameters;
};

class AccumulateElement : public Element
{
  public:
    template <class Iter>
    AccumulateElement(Iter begin, Iter end)
    {
        std::copy(begin, end, std::back_inserter(m_elements));
    }

    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        std::for_each(m_elements.begin(), m_elements.end(), SubelementGenerator(generator, scope, call, error_log));
    }

    /*virtual*/ boost::shared_ptr<const Value> evaluate(const Scope& scope) const
    {
        if(m_elements.size() == 0)
            return boost::shared_ptr<Value>(new Value());

        return m_elements.back()->evaluate(scope);
    }

  private:
    class SubelementGenerator : public ContentState
    {
      public:
        SubelementGenerator(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) : ContentState(generator, scope, call, error_log) {}

        void operator()(const boost::shared_ptr<Element>& element) { element->get_content(m_generator, m_scope, m_call, m_error_log); }
    };

    std::vector<boost::shared_ptr<Element>> m_elements;
};

class NoopElement : public Element
{
  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const {}
};

class ReturnElement : public Element
{
  public:
    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        generator.handle_return_begin();
        if(m_code)
        {
            m_code->get_content(generator, scope, call, error_log);
        }
        generator.handle_return_end();
    }

    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

  private:
    boost::shared_ptr<Element> m_code;
};

class EnvironmentElement : public Element
{
  public:
    EnvironmentElement(shade::ShaderEnvironment env) : m_env(env) {}

    /*virtual*/ void get_content(Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        if((generator.handle_environment_begin(m_env)) && (m_code))
        {
            m_code->get_content(generator, scope, call, error_log);
        }
        generator.handle_environment_end();
    }

    void set_code(boost::shared_ptr<Element> code) { m_code = code; }

  private:
    shade::ShaderEnvironment m_env;
    boost::shared_ptr<Element> m_code;
};

} // namespace

ElementGenerator::ElementGenerator(void) { init_elements(); }

void ElementGenerator::reset(void)
{
    m_elements.clear();
    init_elements();
}

boost::shared_ptr<Element> ElementGenerator::get_element(void) const
{
    boost::shared_ptr<parser::Element> result(get_accumulated_element());
    return result;
}

boost::shared_ptr<MethodTemplate> ElementGenerator::get_method_template(void) const { return boost::dynamic_pointer_cast<MethodTemplate>(get_element()); }

boost::shared_ptr<Element> ElementGenerator::get_last_element(void)
{
    if(m_elements.back().size() == 0)
        return boost::shared_ptr<Element>(new NoopElement());

    return m_elements.back().back();
}

boost::shared_ptr<Element> ElementGenerator::get_accumulated_element(void) const
{
    if(m_elements.back().size() == 1)
        return m_elements.back().back();

    return boost::shared_ptr<Element>(new AccumulateElement(m_elements.back().begin(), m_elements.back().end()));
}

void ElementGenerator::add_element(boost::shared_ptr<parser::Element> element) { m_elements.back().push_back(element); }

void ElementGenerator::save_elements(void) { m_elements.push_back(std::vector<boost::shared_ptr<Element>>()); }

void ElementGenerator::restore_elements(void) { m_elements.pop_back(); }

void ElementGenerator::init_elements(void) { save_elements(); }

void ElementGenerator::handle_identifier(const std::string& name) { add_element(boost::shared_ptr<parser::Element>(new IdentifierElement(name))); }

void ElementGenerator::handle_verbatim(const std::string& code) { add_element(boost::shared_ptr<parser::Element>(new VerbatimElement(code))); }

void ElementGenerator::handle_type(Constants::Type type) { add_element(boost::shared_ptr<parser::Element>(new TypeElement(type))); }

void ElementGenerator::handle_qualifier(Constants::Qualifier qualifier) { add_element(boost::shared_ptr<parser::Element>(new QualifierElement(qualifier))); }

void ElementGenerator::handle_operator(Constants::Operator op) { add_element(boost::shared_ptr<parser::Element>(new OperatorElement(op))); }

void ElementGenerator::handle_literal_objref(ObjectMap::Index ref) { add_element(boost::shared_ptr<parser::Element>(new LiteralObjrefElement(ref))); }

void ElementGenerator::handle_literal_list(ObjectMap::Index begin, ObjectMap::Index end) { add_element(boost::shared_ptr<parser::Element>(new LiteralListElement(begin, end))); }

void ElementGenerator::handle_literal_int(int value) { add_element(boost::shared_ptr<parser::Element>(new LiteralIntElement(value))); }

void ElementGenerator::handle_literal_float(float value) { add_element(boost::shared_ptr<parser::Element>(new LiteralFloatElement(value))); }

void ElementGenerator::handle_literal_vec2(float x, float y) { add_element(boost::shared_ptr<parser::Element>(new LiteralVec2Element(x, y))); }

void ElementGenerator::handle_literal_vec3(float x, float y, float z) { add_element(boost::shared_ptr<parser::Element>(new LiteralVec3Element(x, y, z))); }

void ElementGenerator::handle_literal_vec4(float x, float y, float z, float w) { add_element(boost::shared_ptr<parser::Element>(new LiteralVec4Element(x, y, z, w))); }

void ElementGenerator::handle_array_begin(void) { add_element(boost::shared_ptr<parser::Element>(new ArrayElement())); }

void ElementGenerator::handle_array_type_begin(void) { save_elements(); }

void ElementGenerator::handle_array_type_end(void)
{
    boost::shared_ptr<parser::Element> type(get_accumulated_element());
    restore_elements();

    ArrayElement* array(dynamic_cast<ArrayElement*>(get_last_element().get()));
    if(!array)
        return;

    array->set_type(type);
}

void ElementGenerator::handle_array_size_begin(void) { save_elements(); }

void ElementGenerator::handle_array_size_end(void)
{
    boost::shared_ptr<parser::Element> size(get_accumulated_element());
    restore_elements();

    ArrayElement* array(dynamic_cast<ArrayElement*>(get_last_element().get()));
    if(!array)
        return;

    array->set_size(size);
}

void ElementGenerator::handle_array_end(void) {}

void ElementGenerator::handle_switch_begin(void) { add_element(boost::shared_ptr<parser::Element>(new SwitchElement())); }

void ElementGenerator::handle_switch_value_begin(void) { save_elements(); }

void ElementGenerator::handle_switch_value_end(void)
{
    boost::shared_ptr<parser::Element> value(get_accumulated_element());
    restore_elements();

    SwitchElement* switch_(dynamic_cast<SwitchElement*>(get_last_element().get()));
    if(!switch_)
        return;

    switch_->set_value(value);
}

void ElementGenerator::handle_switch_case_begin(void) { save_elements(); }

void ElementGenerator::handle_switch_case_end(void)
{
    boost::shared_ptr<parser::Element> value(get_accumulated_element());
    restore_elements();

    SwitchElement* switch_(dynamic_cast<SwitchElement*>(get_last_element().get()));
    if(!switch_)
        return;

    switch_->set_case_value(value);
}

void ElementGenerator::handle_switch_code_begin(void) { save_elements(); }

void ElementGenerator::handle_switch_code_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    SwitchElement* switch_(dynamic_cast<SwitchElement*>(get_last_element().get()));
    if(!switch_)
        return;

    switch_->set_case_code(code);
}

void ElementGenerator::handle_switch_default_begin(void) { save_elements(); }

void ElementGenerator::handle_switch_default_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    SwitchElement* switch_(dynamic_cast<SwitchElement*>(get_last_element().get()));
    if(!switch_)
        return;

    switch_->set_default(code);
}

void ElementGenerator::handle_switch_end(void) {}

void ElementGenerator::handle_for_begin(void) { add_element(boost::shared_ptr<parser::Element>(new ForElement())); }

void ElementGenerator::handle_for_init_begin(void) { save_elements(); }

void ElementGenerator::handle_for_init_end(void)
{
    boost::shared_ptr<parser::Element> init(get_accumulated_element());
    restore_elements();

    ForElement* forloop(dynamic_cast<ForElement*>(get_last_element().get()));
    if(!forloop)
        return;

    forloop->set_init(init);
}

void ElementGenerator::handle_for_condition_begin(void) { save_elements(); }

void ElementGenerator::handle_for_condition_end(void)
{
    boost::shared_ptr<parser::Element> condition(get_accumulated_element());
    restore_elements();

    ForElement* forloop(dynamic_cast<ForElement*>(get_last_element().get()));
    if(!forloop)
        return;

    forloop->set_condition(condition);
}

void ElementGenerator::handle_for_increment_begin(void) { save_elements(); }

void ElementGenerator::handle_for_increment_end(void)
{
    boost::shared_ptr<parser::Element> increment(get_accumulated_element());
    restore_elements();

    ForElement* forloop(dynamic_cast<ForElement*>(get_last_element().get()));
    if(!forloop)
        return;

    forloop->set_increment(increment);
}

void ElementGenerator::handle_for_code_begin(void) { save_elements(); }

void ElementGenerator::handle_for_code_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    ForElement* forloop(dynamic_cast<ForElement*>(get_last_element().get()));
    if(!forloop)
        return;

    forloop->set_code(code);
}

void ElementGenerator::handle_for_end(void) {}

void ElementGenerator::handle_var_definition_begin(void) { add_element(boost::shared_ptr<parser::Element>(new VarDefinitionElement())); }

void ElementGenerator::handle_var_definition_qualifier_list_begin(void) {}

void ElementGenerator::handle_var_definition_qualifier_begin(void) { save_elements(); }

void ElementGenerator::handle_var_definition_qualifier_end(void)
{
    boost::shared_ptr<parser::Element> qualifier(get_accumulated_element());
    restore_elements();

    VarDefinitionElement* vardef(dynamic_cast<VarDefinitionElement*>(get_last_element().get()));
    if(!vardef)
        return;

    vardef->set_qualifier(qualifier);
}

void ElementGenerator::handle_var_definition_qualifier_list_end(void) {}

void ElementGenerator::handle_var_definition_type_begin(void) { save_elements(); }

void ElementGenerator::handle_var_definition_type_end(void)
{
    boost::shared_ptr<parser::Element> type(get_accumulated_element());
    restore_elements();

    VarDefinitionElement* vardef(dynamic_cast<VarDefinitionElement*>(get_last_element().get()));
    if(!vardef)
        return;

    vardef->set_type(type);
}

void ElementGenerator::handle_var_definition_name_begin(void) { save_elements(); }

void ElementGenerator::handle_var_definition_name_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    VarDefinitionElement* vardef(dynamic_cast<VarDefinitionElement*>(get_last_element().get()));
    if(!vardef)
        return;

    vardef->set_name(name);
}

void ElementGenerator::handle_var_definition_code_begin(void) { save_elements(); }

void ElementGenerator::handle_var_definition_code_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    VarDefinitionElement* vardef(dynamic_cast<VarDefinitionElement*>(get_last_element().get()));
    if(!vardef)
        return;

    vardef->set_code(code);
}

void ElementGenerator::handle_var_definition_end(void) {}

void ElementGenerator::handle_statement_begin(void)
{
    add_element(boost::shared_ptr<parser::Element>(new StatementElement()));
    save_elements();
}

void ElementGenerator::handle_statement_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    StatementElement* block(dynamic_cast<StatementElement*>(get_last_element().get()));
    if(!block)
        return;

    block->set_code(code);
}

void ElementGenerator::handle_block_begin(void)
{
    add_element(boost::shared_ptr<parser::Element>(new BlockElement()));
    save_elements();
}

void ElementGenerator::handle_block_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    BlockElement* block(dynamic_cast<BlockElement*>(get_last_element().get()));
    if(!block)
        return;

    block->set_code(code);
}

void ElementGenerator::handle_function_definition_begin(void) {}

void ElementGenerator::handle_function_definition_return_type_begin(void) { save_elements(); }

void ElementGenerator::handle_function_definition_return_type_end(void)
{
    m_function_definition_return_type = get_accumulated_element();
    restore_elements();
}

void ElementGenerator::handle_function_definition_name_begin(void) { save_elements(); }

void ElementGenerator::handle_function_definition_name_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    IdentifierElement* identifier(dynamic_cast<IdentifierElement*>(name.get()));
    if(!identifier)
        return;

    boost::shared_ptr<FunctionDefinitionElement> function(new FunctionDefinitionElement(identifier->value()));
    function->set_type(m_function_definition_return_type);

    add_element(function);
}

void ElementGenerator::handle_function_definition_parameter_list_begin(void) {}

void ElementGenerator::handle_function_definition_parameter_type_begin(void) { save_elements(); }

void ElementGenerator::handle_function_definition_parameter_qualifier_list_begin(void) {}

void ElementGenerator::handle_function_definition_parameter_qualifier_begin(void) {}

void ElementGenerator::handle_function_definition_parameter_qualifier_end(void) {}

void ElementGenerator::handle_function_definition_parameter_qualifier_list_end(void) {}

void ElementGenerator::handle_function_definition_parameter_type_end(void)
{
    boost::shared_ptr<parser::Element> type(get_accumulated_element());
    restore_elements();

    FunctionDefinitionElement* function(dynamic_cast<FunctionDefinitionElement*>(get_last_element().get()));
    if(!function)
        return;

    function->set_parameter_type(type);
}

void ElementGenerator::handle_function_definition_parameter_name_begin(void) { save_elements(); }

void ElementGenerator::handle_function_definition_parameter_name_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    FunctionDefinitionElement* function(dynamic_cast<FunctionDefinitionElement*>(get_last_element().get()));
    if(!function)
        return;

    function->set_parameter_name(name);
}

void ElementGenerator::handle_function_definition_parameter_list_end(void) {}

void ElementGenerator::handle_function_definition_code_begin(void)
{
    FunctionDefinitionElement* function(dynamic_cast<FunctionDefinitionElement*>(get_last_element().get()));
    if(function)
    {
        function->enable_code_block();
    }

    save_elements();
}

void ElementGenerator::handle_function_definition_code_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    FunctionDefinitionElement* function(dynamic_cast<FunctionDefinitionElement*>(get_last_element().get()));
    if(!function)
        return;

    function->set_code(code);
}

void ElementGenerator::handle_function_definition_end(void) {}

void ElementGenerator::handle_function_call_begin(void) { add_element(boost::shared_ptr<parser::Element>(new FunctionCallElement())); }

void ElementGenerator::handle_function_call_name_begin(void) { save_elements(); }

void ElementGenerator::handle_function_call_name_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    FunctionCallElement* function(dynamic_cast<FunctionCallElement*>(get_last_element().get()));
    if(!function)
        return;

    function->set_name(name);
}

void ElementGenerator::handle_function_call_parameter_list_begin(void) {}

void ElementGenerator::handle_function_call_parameter_begin(void) { save_elements(); }

void ElementGenerator::handle_function_call_parameter_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    FunctionCallElement* function(dynamic_cast<FunctionCallElement*>(get_last_element().get()));
    if(!function)
        return;

    function->set_parameter(name);
}

void ElementGenerator::handle_function_call_parameter_list_end(void) {}

void ElementGenerator::handle_function_call_end(void) {}

void ElementGenerator::handle_return_begin(void)
{
    add_element(boost::shared_ptr<parser::Element>(new ReturnElement()));
    save_elements();
}

void ElementGenerator::handle_return_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    ReturnElement* return_(dynamic_cast<ReturnElement*>(get_last_element().get()));
    if(!return_)
        return;

    return_->set_code(code);
}

void ElementGenerator::handle_self_begin(void) {}

void ElementGenerator::handle_self_name_begin(void) { save_elements(); }

void ElementGenerator::handle_self_name_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    boost::shared_ptr<SelfElement> self(new SelfElement(name));
    add_element(self);
}

void ElementGenerator::handle_self_parameter_list_begin(void) {}

void ElementGenerator::handle_self_parameter_type_begin(void) { save_elements(); }

void ElementGenerator::handle_self_parameter_type_end(void)
{
    boost::shared_ptr<parser::Element> type(get_accumulated_element());
    restore_elements();

    SelfElement* self(dynamic_cast<SelfElement*>(get_last_element().get()));
    if(!self)
        return;

    self->set_parameter_type(type);
}

void ElementGenerator::handle_self_parameter_name_begin(void) { save_elements(); }

void ElementGenerator::handle_self_parameter_name_end(void)
{
    boost::shared_ptr<parser::Element> name(get_accumulated_element());
    restore_elements();

    SelfElement* self(dynamic_cast<SelfElement*>(get_last_element().get()));
    if(!self)
        return;

    self->set_parameter_name(name);
}

void ElementGenerator::handle_self_parameter_list_end(void) {}

void ElementGenerator::handle_self_end(void) {}

bool ElementGenerator::handle_environment_begin(ShaderEnvironment env)
{
    add_element(boost::shared_ptr<parser::Element>(new EnvironmentElement(env)));
    save_elements();
    return true;
}

void ElementGenerator::handle_environment_end(void)
{
    boost::shared_ptr<parser::Element> code(get_accumulated_element());
    restore_elements();

    EnvironmentElement* env(dynamic_cast<EnvironmentElement*>(get_last_element().get()));
    if(!env)
        return;

    env->set_code(code);
}

void ElementGenerator::append(const Generator& generator)
{
    const ElementGenerator* other = dynamic_cast<const ElementGenerator*>(&generator);
    if(!other)
        return;

    add_element(other->get_accumulated_element());
}

boost::shared_ptr<Generator> ElementGenerator::construct(void) const { return boost::shared_ptr<Generator>(new ElementGenerator); }
