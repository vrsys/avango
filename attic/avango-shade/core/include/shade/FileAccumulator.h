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

#ifndef shade_FileAccumulator_H
#define shade_FileAccumulator_H shade_FileAccumulator_H

#include "FileResolver.h"
#include <string>
#include <set>


namespace shade
{
  class FileAccumulator
  {
  public:
    FileAccumulator(const FileResolver& resolver);

    void add_file(const std::string& file);
    std::string get_str() const;

    int get_glsl_version(void) const;
    bool requires_gpu_shader4_ext(void) const;

  private:
    const FileResolver& m_resolver;
    std::set<std::string> m_seen_files;
    std::string m_content;
    int m_glsl_version;
    bool m_requires_gpu_shader4_ext;
  };
}

#endif /* shade_FileAccumulator_H */
