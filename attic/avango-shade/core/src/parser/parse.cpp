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

#include "parse.h"
#include "../../src/parser/FunctionParser.h"
#include "../../src/parser/Scope.h"
#include "../formatter/MarkBuffer.h"
#include <boost/version.hpp>
#if BOOST_VERSION >= 103600
#include <boost/spirit/include/classic_confix.hpp>
#else
#include <boost/spirit/utility/confix.hpp>
#include "boost_spirit_classic.h"
#endif
#include <iostream>

using namespace shade::parser;
using namespace boost::spirit::classic;
using namespace phoenix;

bool shade::parser::parse(const std::string& source, formatter::Generator& generator, const formatter::SelfTranslator& translator)
{
    shade::formatter::MarkBuffer buffer;
    buffer.append(source);

    Function function(generator, translator);
    return parse(buffer.begin(), buffer.end(), *(function) >> end_p, space_p | comment_p("/*", "*/") | comment_p("//")).full;
}
