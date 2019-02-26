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

#include <shade/FileResolver.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace boost::lambda;

void shade::FileResolver::append_path(const std::string& path) { m_path.push_back(path); }

void shade::FileResolver::prepend_path(const std::string& path) { m_path.insert(m_path.begin(), path); }

std::string shade::FileResolver::load(const std::string& name) const
{
    boost::filesystem::path filename(name);

    for(std::list<std::string>::const_iterator i = m_path.begin(); i != m_path.end(); ++i)
    {
        boost::filesystem::ifstream file(*i / filename);
        if(file)
        {
            std::ostringstream contents;
            contents << file.rdbuf();
            return contents.str();
        }
    }
    throw FileNotFound();
}

namespace shade
{
void append_env_path(FileResolver& resolver, const std::string& env_path)
{
    boost::escaped_list_separator<char> sep('\\', ':', '\"');
    boost::tokenizer<boost::escaped_list_separator<char>> token_line(env_path, sep);
    std::for_each(token_line.begin(), token_line.end(), bind(&FileResolver::append_path, boost::ref(resolver), _1));
}

} // namespace shade
