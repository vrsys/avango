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

#include "MarkBuffer.h"

namespace
{
unsigned int count_line_numbers(const std::string& buffer, std::string::size_type from, std::string::size_type to)
{
    unsigned int result = 1u;

    for(std::string::size_type current = from; current != to; ++current)
    {
        if(buffer[current] == '\n')
            ++result;
    }

    return result;
}
} // namespace

using namespace shade::formatter;

MarkBuffer::MarkBuffer(void) : m_string_cache_valid(false) {}

MarkBuffer::MarkBuffer(const std::string& content) : m_buffer(content), m_string_cache_valid(false) {}

void MarkBuffer::append(const std::string& value)
{
    m_buffer << value;
    m_string_cache_valid = false;
}

void MarkBuffer::update_string_cache(void) const
{
    if(!m_string_cache_valid)
        m_string_cache = m_buffer.str();
}

const std::string& MarkBuffer::str(void) const
{
    update_string_cache();
    return m_string_cache;
}

MarkBuffer& shade::formatter::operator<<(MarkBuffer& buffer, const std::string& value)
{
    buffer.append(value);
    return buffer;
}

void MarkBuffer::set_mark(boost::shared_ptr<Mark> mark)
{
    m_mark_positions[mark] = m_buffer.tellp();
    m_mark_list.push_back(std::make_pair(mark, m_buffer.tellp()));
}

unsigned int MarkBuffer::get_mark_line_number(boost::shared_ptr<Mark> mark) const
{
    MarkPositions::const_iterator position = m_mark_positions.find(mark);
    if(position == m_mark_positions.end())
        return 0;

    return count_line_numbers(m_buffer.str(), 0, position->second);
}

MarkBuffer::iterator MarkBuffer::begin(void) const { return iterator(this, 0); }

MarkBuffer::iterator MarkBuffer::end(void) const
{
    update_string_cache();
    return iterator(this, str().size());
}

MarkBuffer::iterator::iterator(void) : m_cur_file_mark(new FileMark) {}

MarkBuffer::iterator::iterator(const MarkBuffer* buffer, std::string::size_type pos)
    : m_buffer(buffer), m_position(pos), m_line_number(0), m_mark_iter(buffer->m_mark_list.begin()), m_cur_file_mark(new FileMark)
{
    update_mark_iter();
}

char MarkBuffer::iterator::operator*(void)const { return m_buffer->str()[m_position]; }

char MarkBuffer::iterator::operator->(void)const { return m_buffer->str()[m_position]; }

void MarkBuffer::iterator::operator++(void)
{
    if(**this == '\n')
        ++m_line_number;

    ++m_position;

    update_mark_iter();
}

bool MarkBuffer::iterator::operator==(const iterator& other) const { return (m_position == other.m_position); }

bool MarkBuffer::iterator::operator!=(const iterator& other) const { return (m_position != other.m_position); }

bool MarkBuffer::iterator::operator<(const iterator& other) const { return (m_position < other.m_position); }

bool MarkBuffer::iterator::operator>(const iterator& other) const { return (m_position > other.m_position); }

FileMark MarkBuffer::iterator::get_file_mark(void) const { return FileMark(m_cur_file_mark->get_filename(), m_cur_file_mark->get_line_number() + m_line_number); }

void MarkBuffer::iterator::update_mark_iter(void)
{
    while((m_mark_iter != m_buffer->m_mark_list.end()) && (m_mark_iter->second <= m_position))
    {
        boost::shared_ptr<FileMark> cur_file_mark = boost::dynamic_pointer_cast<FileMark>(m_mark_iter->first);
        if(cur_file_mark)
        {
            m_cur_file_mark = cur_file_mark;
            m_line_number = 0;
        }

        ++m_mark_iter;
    }
}
