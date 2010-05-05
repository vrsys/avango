#include "../../src/parser/ScopeLayer.h"
#include "../../src/parser/Scope.h"
#include "../../src/parser/ReferenceValue.h"
#include "../../src/parser/ListValue.h"
#include "../../src/parser/ValueList.h"
#include "../../src/parser/Segment.h"
#include "../../src/parser/parse.h"
#include "../../src/formatter/SelfTranslator.h"
#include "../formatter/DumpGenerator.h"
#include "../formatter/diff_dump.h"
#include <boost/bind.hpp>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <UnitTest++.h>

using namespace shade::parser;
using namespace shade::formatter;

namespace
 {
  class SegmentHandler
   {
   public:
    SegmentHandler(Scope& scope) :
      scope_(scope)
     {
     }

    void operator()(const std::string& name,
                    ParameterIterator begin, ParameterIterator end,
                    const std::string& return_type, const std::string& parameters,
                    boost::shared_ptr<Segment> segment)
     {
      name_ = name;
      segment_.push_back(segment);
      return_string_ = return_type;
      parameters_string_ = parameters;
      std::copy(begin, end, std::back_inserter(parameters_));
     }

    std::string get_content(void) const
     {
      return get_content(segment_.size()-1);
     }

    std::string get_content(int index) const
     {
      if (segment_.empty())
        return "";

      std::stringstream error_log;
      shade::formatter::DumpGenerator generator;
      segment_[index]->get_content(generator, scope_, 
                                   boost::bind(&SegmentHandler::mangle_name, this, _1, _2, _3, _4),
                                   error_log);
      return generator.get_code();
     }

    int get_num_functions(void) const
     {
      return segment_.size();
     }

    std::string get_function_name(void) const
     {
      return name_;
     }

    const std::vector<std::string>& get_parameters(void) const
     {
      return parameters_;
     }

    std::string get_return_string(void) const
     {
      return return_string_;
     }

    std::string get_parameters_string(void) const
     {
      return parameters_string_;
     }

   private:

    std::string mangle_name(const std::string& name, const ValueList& values, const ScopeLayer&, std::ostream& error_log) const
     {
      std::ostringstream hout;
      hout << "$" << name << "$";
      for (ValueList::const_iterator i = values.begin(); i != values.end(); ++i)
       {
        const ReferenceValue* ref_value = dynamic_cast<const ReferenceValue*>(i->get());
        if (!ref_value)
         {
          hout << "_X";
         }
        else
         {
          hout << "_" << ref_value->get();
         }
       }
      return hout.str();
     }

    std::string name_;
    std::vector<boost::shared_ptr<Segment> > segment_;
    Scope& scope_;
    std::vector<std::string> parameters_;
    std::string return_string_;
    std::string parameters_string_;
   };
 }

SUITE(TestParser)
{
  struct Fixture
  {
    Fixture(void) :
      scope(global),
      handler(scope)
    {
      scope.add_value("abc", boost::shared_ptr<const Value>(new ReferenceValue(3)));
      scope.add_value("defg", boost::shared_ptr<const Value>(new ReferenceValue(42)));
      scope.add_value("q", boost::shared_ptr<const Value>(new ReferenceValue(7)));
      scope.add_value("lights", boost::shared_ptr<const Value>(new ListValue(3,5)));
    }

    bool check_void_function_block(const std::string& block)
    {
      return shade::formatter::check_dump_output(
        "<funcdef>"
          "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
          "<funcdef_name><id>test</id></funcdef_name>"
          "<funcdef_param_list></funcdef_param_list>"
          "<funcdef_code>"
          +block+
          "</funcdef_code>"
        "</funcdef>"
        , generator.get_code());
    }

    bool check_partial_function_block(const std::string& block)
    {
      std::string result(shade::formatter::strip_levels(generator.get_code(), 2));
      return shade::formatter::check_partial_dump_output(block, result);
    }

    ScopeLayer global;
    Scope scope;
    SegmentHandler handler;
    DumpGenerator generator;
    SelfTranslator translator;
  };


  TEST_FIXTURE(Fixture, VoidFunction)
  {
    CHECK(parse(
      "  void test() {\n"
      "  Test();  "
      "}\n",
      generator, translator
     ));

    CHECK(check_void_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list></func_call_param_list>"
      "</func_call>"
      "<verbatim>;</verbatim>"
      ));
  }

  TEST_FIXTURE(Fixture, DontParse)
  {
    CHECK(!parse(
      "  vec3(0., 0., 0.);  ",
      generator, translator
     ));
  }

  TEST_FIXTURE(Fixture, FunctionWithSimpleArguments)
  {
    CHECK(parse(
      "void test() {\n"
      "  Test(abc\n, defg , q) ; "
      "}\n",
      generator, translator
     ));

    CHECK(check_void_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param><id>abc</id></func_call_param>"
          "<func_call_param><id>defg</id></func_call_param>"
          "<func_call_param><id>q</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      "<verbatim> ;</verbatim>"
      ));
  }

  TEST_FIXTURE(Fixture, FunctionWithExpressionArguments)
  {
    CHECK(parse(
      "void test() {\n"
      "  Test(abc, 1+defg, q) ; "
      "}\n",
      generator, translator
     ));

    CHECK(check_void_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param><id>abc</id></func_call_param>"
          "<func_call_param><verbatim>1+</verbatim><id>defg</id></func_call_param>"
          "<func_call_param><id>q</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      "<verbatim> ;</verbatim>"
      ));
  }

  TEST_FIXTURE(Fixture, FunctionWithParenthesisArguments)
  {
    CHECK(parse(
      "void test() {\n"
      "  Test(((abc)), (xyz+defg), q) ; "
      "}\n",
      generator, translator
     ));

    CHECK(check_void_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param>"
            "<verbatim>(</verbatim>"
            "<verbatim>(</verbatim>"
            "<id>abc</id>"
            "<verbatim>)</verbatim>"
            "<verbatim>)</verbatim>"
          "</func_call_param>"
          "<func_call_param>"
            "<verbatim>(</verbatim>"
            "<id>xyz</id>"
            "<verbatim>+</verbatim>"
            "<id>defg</id>"
            "<verbatim>)</verbatim>"
          "</func_call_param>"
          "<func_call_param><id>q</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      "<verbatim> ;</verbatim>"
      ));
  }

  TEST_FIXTURE(Fixture, FunctionInBlock)
  {
    CHECK(parse(
      "void test() {\n"
      "if (a == 5)\n"
      " {\n"
      "  Test(abc, 1, q)\n"
      " }\n"
      "}\n",
      generator, translator
     ));

    CHECK(check_partial_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param><id>abc</id></func_call_param>"
          "<func_call_param><verbatim>1</verbatim></func_call_param>"
          "<func_call_param><id>q</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      ));
  }

  TEST_FIXTURE(Fixture, TwoFunctions)
  {
    CHECK(parse(
      "void test() {\n"
      "Test(((abc)), (xyz+defg), q);\n"
      "a = b + 1;\n"
      "Test2(1, q);\n"
      "}\n",
      generator, translator
     ));

    CHECK(check_partial_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param>"
            "<verbatim>(</verbatim>"
            "<verbatim>(</verbatim>"
            "<id>abc</id>"
            "<verbatim>)</verbatim>"
            "<verbatim>)</verbatim>"
          "</func_call_param>"
          "<func_call_param>"
            "<verbatim>(</verbatim>"
            "<id>xyz</id>"
            "<verbatim>+</verbatim>"
            "<id>defg</id>"
            "<verbatim>)</verbatim>"
          "</func_call_param>"
          "<func_call_param><id>q</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      ));
    CHECK(check_partial_function_block(
      "<func_call>"
        "<func_call_name><id>Test2</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param><verbatim>1</verbatim></func_call_param>"
          "<func_call_param><id>q</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      ));
  }

  TEST_FIXTURE(Fixture, TwoNestedFunctions)
  {
    CHECK(parse(
      "void test() {\n"
      "Test(abc, Test2(1, q), defg);\n"
      "}\n",
      generator, translator
     ));

    CHECK(check_partial_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
          "<func_call_param><id>abc</id></func_call_param>"
          "<func_call_param>"
            "<func_call>"
              "<func_call_name><id>Test2</id></func_call_name>"
              "<func_call_param_list>"
                "<func_call_param><verbatim>1</verbatim></func_call_param>"
                "<func_call_param><id>q</id></func_call_param>"
              "</func_call_param_list>"
            "</func_call>"
          "</func_call_param>"
          "<func_call_param><id>defg</id></func_call_param>"
        "</func_call_param_list>"
      "</func_call>"
      ));
  }

  TEST_FIXTURE(Fixture, VerfiySimpleParameter)
  {
    CHECK(parse(
      "void test(int value) {\n"
      "}\n",
      generator, translator
     ));

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><verbatim>int</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>value</id></funcdef_param_name>"
        "</funcdef_param_list>"
        "<funcdef_code><verbatim></verbatim></funcdef_code>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(Fixture, VerfiyTwoSimpleParameters)
  {
    CHECK(parse(
      "void test(int value, double number) {\n"
      "}\n",
      generator, translator
     ));

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><verbatim>int</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>value</id></funcdef_param_name>"
          "<funcdef_param_type><verbatim>double</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>number</id></funcdef_param_name>"
        "</funcdef_param_list>"
        "<funcdef_code><verbatim></verbatim></funcdef_code>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(Fixture, VerfiyThreeComplexParameters)
  {
    CHECK(parse(
      "void test(varying int value, invariant centroid varying double number, const sampler2D tex) {\n"
      "}\n",
      generator, translator
     ));

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_qualifier_list>"
            "<funcdef_param_qualifier><verbatim>varying</verbatim></funcdef_param_qualifier>"
          "</funcdef_param_qualifier_list>"
          "<funcdef_param_type><verbatim>int</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>value</id></funcdef_param_name>"
          "<funcdef_param_qualifier_list>"
            "<funcdef_param_qualifier><verbatim>invariant</verbatim></funcdef_param_qualifier>"
            "<funcdef_param_qualifier><verbatim>centroid</verbatim></funcdef_param_qualifier>"
            "<funcdef_param_qualifier><verbatim>varying</verbatim></funcdef_param_qualifier>"
          "</funcdef_param_qualifier_list>"
          "<funcdef_param_type><verbatim>double</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>number</id></funcdef_param_name>"
          "<funcdef_param_qualifier_list>"
            "<funcdef_param_qualifier><verbatim>const</verbatim></funcdef_param_qualifier>"
          "</funcdef_param_qualifier_list>"
          "<funcdef_param_type><verbatim>sampler2D</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>tex</id></funcdef_param_name>"
        "</funcdef_param_list>"
        "<funcdef_code><verbatim></verbatim></funcdef_code>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(Fixture, VerfiyTwoArrayParameters)
  {
    CHECK(parse(
      "void test(int value[7], out double number[124]) {\n"
      "}\n",
      generator, translator
     ));

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type>"
            "<array>"
              "<array_type>"
                "<verbatim>int</verbatim>"
              "</array_type>"
              "<array_size>"
                "<literal_int>7</literal_int>"
              "</array_size>"
            "</array>"
          "</funcdef_param_type>"
          "<funcdef_param_name><id>value</id></funcdef_param_name>"
          "<funcdef_param_qualifier_list>"
            "<funcdef_param_qualifier><verbatim>out</verbatim></funcdef_param_qualifier>"
          "</funcdef_param_qualifier_list>"
          "<funcdef_param_type>"
            "<array>"
              "<array_type>"
                "<verbatim>double</verbatim>"
              "</array_type>"
              "<array_size>"
                "<literal_int>124</literal_int>"
              "</array_size>"
            "</array>"
          "</funcdef_param_type>"
          "<funcdef_param_name><id>number</id></funcdef_param_name>"
        "</funcdef_param_list>"
        "<funcdef_code><verbatim></verbatim></funcdef_code>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(Fixture, VerfiySelfParameter)
  {
    translator.handle_self_begin();
    translator.handle_self_name_begin();
    translator.handle_identifier("Class_SELF");
    translator.handle_self_name_end();
    translator.handle_self_parameter_list_begin();
    translator.handle_self_parameter_type_begin();
    translator.handle_type(shade::formatter::Constants::vec3);
    translator.handle_self_parameter_type_end();
    translator.handle_self_parameter_name_begin();
    translator.handle_identifier("position");
    translator.handle_self_parameter_name_end();
    translator.handle_self_parameter_list_end();
    translator.handle_self_end();

    CHECK(parse(
      "void test(Class_SELF, int value) {\n"
      "}\n",
      generator, translator
     ));

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
          "<funcdef_param_name><id>self</id></funcdef_param_name>"
          "<funcdef_param_type><type>vec3</type></funcdef_param_type>"
          "<funcdef_param_name><id>position</id></funcdef_param_name>"
          "<funcdef_param_type><verbatim>int</verbatim></funcdef_param_type>"
          "<funcdef_param_name><id>value</id></funcdef_param_name>"
        "</funcdef_param_list>"
        "<funcdef_code><verbatim></verbatim></funcdef_code>"
      "</funcdef>"
      , generator.get_code()));
  }

  // TODO Generator does not allow qualifiers in function return
  //TEST_FIXTURE(Fixture, VerifyHeader)
  //{
  //  CHECK(parse(
  //    "varying float test(int value, double number) {\n"
  //    "}\n",
  //    generator, translator
  //   ));

  //  CHECK_EQUAL("varying float", handler.get_return_string());
  //  CHECK_EQUAL("int value, double number", handler.get_parameters_string());
  //}

  TEST_FIXTURE(Fixture, Comments)
  {
    CHECK(parse(
      "void test() /* A comment */ {\n"
      "  Test();\n"
      "  // Ignore Test();\n"
      "  Test();\n"
      "}\n",
      generator, translator
     ));

    CHECK(check_void_function_block(
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
        "</func_call_param_list>"
      "</func_call>"
      "<verbatim>;\n  // Ignore Test();\n  </verbatim>"
      "<func_call>"
        "<func_call_name><id>Test</id></func_call_name>"
        "<func_call_param_list>"
        "</func_call_param_list>"
      "</func_call>"
      "<verbatim>;</verbatim>"
      ));
  }

  TEST_FIXTURE(Fixture, TwoDefinitions)
  {
    CHECK(parse(
          "void test1() {\n"
          "  Test1();  "
          "}\n"
          "void test2() {\n"
          "  Test2();  "
          "}\n",
          generator, translator
          ));

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test1</id></funcdef_name>"
        "<funcdef_param_list>"
        "</funcdef_param_list>"
        "<funcdef_code>"
          "<func_call>"
            "<func_call_name><id>Test1</id></func_call_name>"
            "<func_call_param_list>"
            "</func_call_param_list>"
          "</func_call>"
          "<verbatim>;</verbatim>"
        "</funcdef_code>"
      "</funcdef>"
      "<funcdef>"
        "<funcdef_type><verbatim>void</verbatim></funcdef_type>"
        "<funcdef_name><id>test2</id></funcdef_name>"
        "<funcdef_param_list>"
        "</funcdef_param_list>"
        "<funcdef_code>"
          "<func_call>"
            "<func_call_name><id>Test2</id></func_call_name>"
            "<func_call_param_list>"
            "</func_call_param_list>"
          "</func_call>"
          "<verbatim>;</verbatim>"
        "</funcdef_code>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(Fixture, ForeachLoop)
  {
    CHECK(parse(
      "void test() {\n"
      "  FOREACH(i, lights) {\n"
      "    Test(i);\n"
      "  }\n"
      "}\n",
      generator, translator
      ));

    CHECK(check_void_function_block(
      "<for>"
        "<for_init>"
          "<vardef>"
            "<vardef_type><type>OBJREF</type></vardef_type>"
            "<vardef_name><id>i</id></vardef_name>"
            "<vardef_code>"
              "<func_call>"
                "<func_call_name><id>shade_list_begin</id></func_call_name>"
                "<func_call_param_list>"
                  "<func_call_param><id>lights</id></func_call_param>"
                "</func_call_param_list>"
              "</func_call>"
            "</vardef_code>"
          "</vardef>"
        "</for_init>"
        "<for_condition>"
          "<func_call>"
            "<func_call_name><operator>not_equal</operator></func_call_name>"
            "<func_call_param_list>"
              "<func_call_param><id>i</id></func_call_param>"
              "<func_call_param>"
                "<func_call>"
                  "<func_call_name><id>shade_list_end</id></func_call_name>"
                  "<func_call_param_list>"
                    "<func_call_param><id>lights</id></func_call_param>"
                  "</func_call_param_list>"
                "</func_call>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</for_condition>"
        "<for_increment>"
          "<func_call>"
            "<func_call_name><operator>pre_increment</operator></func_call_name>"
            "<func_call_param_list>"
              "<func_call_param><id>i</id></func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</for_increment>"
        "<for_code>"
          "<func_call>"
            "<func_call_name><id>Test</id></func_call_name>"
            "<func_call_param_list>"
              "<func_call_param><id>i</id></func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
          "<verbatim>;</verbatim>"
        "</for_code>"
      "</for>"
      ));
  }

  TEST_FIXTURE(Fixture, NormalForLoop)
  {
    CHECK(parse(
      "void test() {\n"
      "for(int i = 1; i != 2; ++i)\n"
      "{\n"
      "  j += i;\n"
      "}"
      "}\n",
      generator, translator
      ));

    CHECK(check_void_function_block(
      "<verbatim>"
      "for(int i = 1; i != 2; ++i)\n"
      "{\n"
      "  </verbatim><id>j</id><verbatim> += </verbatim><id>i</id><verbatim>;\n"
      "}"
      "</verbatim>"
      ));
  }
 }
