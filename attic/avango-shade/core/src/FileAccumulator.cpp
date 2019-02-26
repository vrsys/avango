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

#include <shade/FileAccumulator.h>
#include <shade/io.h>
#include <map>
#include <sstream>
#include <boost/lexical_cast.hpp>

shade::FileAccumulator::FileAccumulator(const FileResolver& resolver) : m_resolver(resolver), m_glsl_version(0), m_requires_gpu_shader4_ext(false) {}

void shade::FileAccumulator::add_file(const std::string& file)
{
    if(file.empty())
        return;

    if(m_seen_files.find(file) == m_seen_files.end())
    {
        std::string content = m_resolver.load(file);
        m_seen_files.insert(file);

        typedef std::map<std::string, std::string> Keywords;
        Keywords keywords;
        std::istringstream input(content);
        m_content += io::parse_input(input, keywords);

        Keywords::const_iterator glsl_version = keywords.find("glsl_version");
        if(glsl_version != keywords.end())
        {
            m_glsl_version = std::max(m_glsl_version, boost::lexical_cast<int>(glsl_version->second));
        }

        Keywords::const_iterator gpu_shader4_ext = keywords.find("GL_EXT_gpu_shader4");
        if(gpu_shader4_ext != keywords.end())
        {
            if(gpu_shader4_ext->second == "require")
                m_requires_gpu_shader4_ext = true;
        }
    }
}

std::string shade::FileAccumulator::get_str() const { return m_content; }

int shade::FileAccumulator::get_glsl_version(void) const { return m_glsl_version; }

bool shade::FileAccumulator::requires_gpu_shader4_ext(void) const { return m_requires_gpu_shader4_ext; }
