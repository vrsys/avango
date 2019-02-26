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

#ifndef shade_formatter_diff_dump_H
#define shade_formatter_diff_dump_H shade_formatter_diff_dump_H

#include <string>
#include <utility>
#include <vector>

namespace shade
{
namespace formatter
{
bool is_token(const std::string& code);
bool is_end_token(const std::string& code);

std::pair<std::string, std::string> get_next_token(const std::string& code);

std::vector<std::string> split_toplevel(const std::string& code);

std::string diff_dump(const std::string& codeA, const std::string& codeB);
std::string diff_dump(const std::string& codeA, const std::string& codeB, int& diff_count);

std::string partial_diff_dump(const std::string& fragment, const std::string& code, bool& match);

bool check_dump_output(const std::string& expected, const std::string& output);
bool check_partial_dump_output(const std::string& expected, const std::string& output);

std::string strip_levels(const std::string& code, int levels);
} // namespace formatter
} // namespace shade

#endif /* shade_formatter_diff_dump_H */
