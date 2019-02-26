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

#include "../../src/parser/MethodInstantiator.h"
#include "../../src/parser/MethodTemplate.h"
#include "../../src/parser/Value.h"
#include "../../src/parser/ReferenceValue.h"
#include "../../src/parser/ValueList.h"
#include "../../src/parser/ScopeLayer.h"
#include "../../src/formatter/ElementGenerator.h"
#include "../formatter/DumpGenerator.h"
#include "../formatter/diff_dump.h"
#include <UnitTest++.h>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

using namespace shade::parser;
using namespace shade::formatter;

namespace
{
struct Fixture
{
    MethodInstantiator instantiator;
    std::ostringstream buffer;
    std::ostringstream error_log;
    ElementGenerator generator;
    DumpGenerator expected_generator;
    DumpGenerator result_generator;
    ScopeLayer global_scope;
};

class Parameter
{
  public:
    virtual void handle(Generator& generator) = 0;
};

class ReferenceParameter : public Parameter
{
  public:
    ReferenceParameter(int ref) : m_ref(ref) {}

    /*virtual*/ void handle(Generator& generator) { generator.handle_literal_objref(m_ref); }

  private:
    int m_ref;
};

boost::shared_ptr<Parameter> make_reference_parameter(int ref) { return boost::shared_ptr<Parameter>(new ReferenceParameter(ref)); }

class IdentifierParameter : public Parameter
{
  public:
    IdentifierParameter(const std::string& id) : m_id(id) {}

    /*virtual*/ void handle(Generator& generator) { generator.handle_identifier(m_id); }

  private:
    const std::string m_id;
};

boost::shared_ptr<Parameter> make_identifier_parameter(const std::string& id) { return boost::shared_ptr<Parameter>(new IdentifierParameter(id)); }

void generate_function_definition_header(Generator& generator, const std::string& name, int num_args = 0)
{
    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(shade::formatter::Constants::void_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    for(int i = 0; i != num_args; ++i)
    {
        generator.handle_function_definition_parameter_type_begin();
        generator.handle_type(Constants::objref);
        generator.handle_function_definition_parameter_type_end();
        generator.handle_function_definition_parameter_name_begin();
        generator.handle_identifier("arg" + boost::lexical_cast<std::string>(i));
        generator.handle_function_definition_parameter_name_end();
    }
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_code_begin();
}

void generate_function_definition_end(Generator& generator)
{
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
}

void generate_static_function_definition(Generator& generator, const std::string& name, int num_args = 0)
{
    generate_function_definition_header(generator, name, num_args);
    generate_function_definition_end(generator);
}

void generate_function_call_begin(Generator& generator, const std::string& name)
{
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
}

void generate_function_call_end(Generator& generator)
{
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();
}

void generate_function_call(Generator& generator, const std::string& name)
{
    generate_function_call_begin(generator, name);
    generate_function_call_end(generator);
}

void generate_function_call(Generator& generator, const std::string& name, boost::shared_ptr<Parameter> arg0)
{
    generate_function_call_begin(generator, name);
    generator.handle_function_call_parameter_begin();
    arg0->handle(generator);
    generator.handle_function_call_parameter_end();
    generate_function_call_end(generator);
}

} // namespace

SUITE(TestMethodInstantiator)
{
    TEST_FIXTURE(Fixture, SimpleStaticMethod)
    {
        generate_static_function_definition(generator, "main");
        instantiator.add_method(generator.get_method_template());

        generate_static_function_definition(expected_generator, "main");

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK_EQUAL(expected_generator.get_code(), result_generator.get_code());
    }

    TEST_FIXTURE(Fixture, DirectCall)
    {
        generate_static_function_definition(generator, "A", 1);
        instantiator.add_method(generator.get_method_template());
        generator.reset();

        generate_function_definition_header(generator, "main");
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());

        generate_static_function_definition(expected_generator, "A", 1);
        generate_function_definition_header(expected_generator, "main");
        generate_function_call(expected_generator, "A", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK_EQUAL(expected_generator.get_code(), result_generator.get_code());
    }

    /*
     * FIXME: Elements from ElementGenerator can not report errors back at the
     * moment ...
     *

     TEST_FIXTURE(Fixture, InfiniteRecursion)
     {
     generate_function_definition_header(generator, "main");
     generate_function_call(generator, "main");
     generate_function_definition_end(generator);
     instantiator.add_method(generator.get_method_template());

     CHECK(!instantiator.build(DumpGeneratorFactory, buffer, error_log));
     }
     */

    TEST_FIXTURE(Fixture, TwoSequentialCallsToSameFunctionButDifferentSignature)
    {
        generate_static_function_definition(generator, "A", 1);
        instantiator.add_method(generator.get_method_template());
        generator.reset();

        generate_function_definition_header(generator, "main");
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_call(generator, "A", make_reference_parameter(2));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());

        generate_static_function_definition(expected_generator, "A", 1);
        generate_static_function_definition(expected_generator, "A1", 1);
        generate_function_definition_header(expected_generator, "main");
        generate_function_call(expected_generator, "A", make_reference_parameter(1));
        generate_function_call(expected_generator, "A1", make_reference_parameter(2));
        generate_function_definition_end(expected_generator);

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK_EQUAL(expected_generator.get_code(), result_generator.get_code());
    }

    TEST_FIXTURE(Fixture, TwoSequentialCallsToSameFunctionAndSameSignature)
    {
        generate_static_function_definition(generator, "A", 1);
        instantiator.add_method(generator.get_method_template());
        generator.reset();

        generate_function_definition_header(generator, "main");
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());

        generate_static_function_definition(expected_generator, "A", 1);
        generate_function_definition_header(expected_generator, "main");
        generate_function_call(expected_generator, "A", make_reference_parameter(1));
        generate_function_call(expected_generator, "A", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK_EQUAL(expected_generator.get_code(), result_generator.get_code());
    }

    TEST_FIXTURE(Fixture, RecursiveCall)
    {
        generate_function_definition_header(generator, "A", 1);
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());
        generator.reset();

        generate_function_definition_header(generator, "main");
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());

        generate_function_definition_header(expected_generator, "A", 1);
        generate_function_call(expected_generator, "", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);
        generate_function_definition_header(expected_generator, "main");
        generate_function_call(expected_generator, "A", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK_EQUAL(expected_generator.get_code(), result_generator.get_code());
    }

    TEST_FIXTURE(Fixture, FunctionWithDifferentSignatureAndRecursiveCall)
    {
        generate_function_definition_header(generator, "A", 1);
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());
        generator.reset();

        generate_function_definition_header(generator, "main");
        generate_function_call(generator, "A", make_reference_parameter(2));
        generate_function_definition_end(generator);
        instantiator.add_method(generator.get_method_template());

        generate_function_definition_header(expected_generator, "A1", 1);
        generate_function_call(expected_generator, "", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);
        generate_function_definition_header(expected_generator, "A", 1);
        generate_function_call(expected_generator, "A1", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);
        generate_function_definition_header(expected_generator, "main");
        generate_function_call(expected_generator, "A", make_reference_parameter(2));
        generate_function_definition_end(expected_generator);

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK(shade::formatter::check_dump_output(expected_generator.get_code(), result_generator.get_code()));
    }

    TEST_FIXTURE(Fixture, Append)
    {
        generate_static_function_definition(generator, "A", 1);
        instantiator.add_method(generator.get_method_template());
        generator.reset();

        MethodInstantiator instantiator_other;
        generate_function_definition_header(generator, "main");
        generate_function_call(generator, "A", make_reference_parameter(1));
        generate_function_definition_end(generator);
        instantiator_other.add_method(generator.get_method_template());
        instantiator.append(instantiator_other);

        generate_static_function_definition(expected_generator, "A", 1);
        generate_function_definition_header(expected_generator, "main");
        generate_function_call(expected_generator, "A", make_reference_parameter(1));
        generate_function_definition_end(expected_generator);

        CHECK(instantiator.build(result_generator, global_scope, error_log));
        CHECK_EQUAL(expected_generator.get_code(), result_generator.get_code());
    }
}
