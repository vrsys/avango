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

#include <shade/io.h>
#include <sstream>
#include <boost/tokenizer.hpp>


std::string shade::io::parse_input(std::istream& hin, std::map<std::string, std::string>& keywords)
{
  std::ostringstream contents;
  while (hin.good())
  {
    std::string line;
    std::getline(hin, line);
    contents << line;
    if (line.size()) contents << "\n";

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" ");
    tokenizer tokens(line, sep);
    tokenizer::iterator tok_iter = tokens.begin();
    if (    (tok_iter != tokens.end()) && (*(tok_iter++) == "//")
        &&  (tok_iter != tokens.end()) && (*(tok_iter++) == "SHADE") )
    {
      for (; tok_iter != tokens.end(); ++tok_iter)
      {
        std::string::size_type split = tok_iter->find(':');
        std::string key = tok_iter->substr(0, split);
        std::string value;
        if (split != std::string::npos)
        {
          value = tok_iter->substr(split+1, std::string::npos);
        }
        keywords[key] = value;
      }
    }
  }

  return contents.str();
}
