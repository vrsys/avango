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

#include "diff_dump.h"
#include <sstream>
#include <iostream>
#include <limits>

namespace
{
  const int min_indentation = 0;
  const int max_indentation = 100;

  void indent(std::ostream& stream, int level, int* output_level)
  {
    if (output_level)
    {
      *output_level = level;
      return;
    }

    for (int i = 0; i != level; ++i)
      stream << "  ";
  }

  std::pair<std::string, std::string> get_next_line(const std::string& code, int& level, int* output_level = 0)
  {
    std::ostringstream output;

    std::pair<std::string, std::string> split(shade::formatter::get_next_token(code));
    if (shade::formatter::is_token(split.first))
    {
      if (shade::formatter::is_end_token(split.first))
      {
        if (level > min_indentation) level -= 1;
        indent(output, level, output_level);
      }
      else
      {
        std::pair<std::string, std::string> next_split(shade::formatter::get_next_token(split.second));
        std::pair<std::string, std::string> next_next_split(shade::formatter::get_next_token(next_split.second));
        if ((!shade::formatter::is_token(next_split.first)) && (shade::formatter::is_end_token(next_next_split.first)))
        {
          indent(output, level, output_level);
          output << split.first << next_split.first << next_next_split.first;
          return std::make_pair(output.str(), next_next_split.second);
        }

        indent(output, level, output_level);
        if (level < max_indentation) level += 1;
      }
    }
    else
      indent(output, level, output_level);

    output << split.first;
    return std::make_pair(output.str(), split.second);
  }
}

bool shade::formatter::is_token(const std::string& code)
{
  unsigned int size(code.size());

  if (size < 3)
    return false;

  if (code[0] != '<')
    return false;

  if (code.find('>') != size-1)
    return false;

  return true;
}

bool shade::formatter::is_end_token(const std::string& code)
{
  int size(code.size());

  if (size < 3)
    return false;

  if (code[1] != '/')
    return false;

  return is_token(code);
}


std::pair<std::string, std::string> shade::formatter::get_next_token(const std::string& code)
{
  std::string::size_type token_begin(code.find('<'));
  if (token_begin != 0)
  {
    if (token_begin == std::string::npos)
      return std::make_pair(code, std::string());

    return std::make_pair(std::string(code, 0, token_begin), std::string(code, token_begin, std::string::npos));
  }

  std::string::size_type token_end(code.find('>'));
  if (token_end == std::string::npos)
  {
    return std::make_pair(code, std::string());
  }

  return std::make_pair(std::string(code, 0, token_end+1), std::string(code, token_end+1, std::string::npos));
}

std::vector<std::string> shade::formatter::split_toplevel(const std::string& code)
{
  std::vector<std::string> result;
  std::string accum;
  int level = 0;

  std::string rest(code);
  while (!rest.empty())
  {
    int output_level;
    std::pair<std::string, std::string> split = get_next_line(rest, level, &output_level);
    accum += split.first;
    if (level == 0) 
    {
      result.push_back(accum);
      accum = "";
    }

    rest = split.second;
  }
  if (!accum.empty())
  {
    result.push_back(accum);
  }

  return result;
}

std::string shade::formatter::diff_dump(const std::string& codeA, const std::string& codeB)
{
  int diff_count(0);
  return diff_dump(codeA, codeB, diff_count);
}

std::string shade::formatter::diff_dump(const std::string& codeA, const std::string& codeB, int& diff_count)
{
  std::ostringstream output;
  std::ostringstream tempA;
  std::ostringstream tempB;
  int diff_count_A(0);
  int diff_count_B(0);

  int levelA = 0;
  int levelB = 0;
  bool is_different = false;
  std::string restA = codeA;
  std::string restB = codeB;

  while ( (!restA.empty()) || (!restB.empty()) )
  {
    std::pair<std::string, std::string> splitA(get_next_line(restA, levelA));
    std::pair<std::string, std::string> splitB(get_next_line(restB, levelB));
    if (splitA.first == splitB.first)
    {
      if (is_different)
      {
        is_different = false;
        output << tempA.str();
        output << tempB.str();
        tempA.str("");
        tempB.str("");
      }
      output << " " << splitA.first << std::endl;
    }
    else
    {
      if (!splitA.first.empty())
        tempA << "A" << splitA.first << std::endl;
      if (!splitB.first.empty())
        tempB << "B" << splitB.first << std::endl;
      is_different = true;
      ++diff_count_A;
      ++diff_count_B;
    }

    restA = splitA.second;
    restB = splitB.second;

    while ( (levelA > levelB) && (!restA.empty()) )
    {
      std::pair<std::string, std::string> splitA(get_next_line(restA, levelA));
      tempA << "A" << splitA.first << std::endl;
      restA = splitA.second;
      ++diff_count_A;
    }

    while ( (levelA < levelB) && (!restB.empty()) )
    {
      std::pair<std::string, std::string> splitB(get_next_line(restB, levelB));
      tempB << "B" << splitB.first << std::endl;
      restB = splitB.second;
      ++diff_count_B;
    }

  }

  output << tempA.str();
  output << tempB.str();
  diff_count = std::max(diff_count_A, diff_count_B);
  return output.str();
}

std::string shade::formatter::partial_diff_dump(const std::string& fragment, const std::string& code, bool& match)
{
  std::vector<std::string> code_tops = split_toplevel(code);
  std::vector<std::string> fragment_tops = split_toplevel(fragment);
  if (code_tops.size() < fragment_tops.size())
    return diff_dump(fragment, code);

  int min_index = 0;
  int min_count = std::numeric_limits<int>::max();
  for (unsigned int i = 0; i != (code_tops.size()-fragment_tops.size()+1); ++i)
  {
    int accum_count = 0;
    for (unsigned int j = 0; j != fragment_tops.size(); ++j)
    {
      int count;
      diff_dump(fragment_tops[j], code_tops[i+j], count);
      accum_count += count;
    }
    if(accum_count < min_count)
    {
      min_count = accum_count;
      min_index = i;
    }
  }

  match = (min_count == 0);

  std::string result;
  for (unsigned int j = 0; j != fragment_tops.size(); ++j)
  {
    result += diff_dump(fragment_tops[j], code_tops[min_index+j]);
  }
  return result;
}

bool shade::formatter::check_dump_output(const std::string& expected, const std::string& output)
{
  bool result(expected == output);
  if (!result)
  {
    std::cerr << "=== Error Diff ===" << std::endl;
    std::cerr << diff_dump(expected, output);
  }

  return result;
}

bool shade::formatter::check_partial_dump_output(const std::string& expected, const std::string& output)
{
  if (output.empty())
  {
    std::cerr << "Error: Empty comparsion string" << std::endl;
    return false;
  }

  bool result;
  std::string log(partial_diff_dump(expected, output, result));

  if (!result)
  {
    std::cerr << "=== Error Diff (Partial) ===" << std::endl;
    std::cerr << log;
  }

  return result;
}

std::string shade::formatter::strip_levels(const std::string& code, int levels)
{
  std::ostringstream output;
  std::ostringstream tempA;
  int cur_level(0);

  std::string rest = code;

  while (!rest.empty()) 
  {
    int output_level;
    std::pair<std::string, std::string> split(get_next_line(rest, cur_level, &output_level));

    if (output_level >= levels)
      output << split.first;

    rest = split.second;
  }

  return output.str();
}
