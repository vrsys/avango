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

#ifndef shade_formatter_MarkBuffer
#define shade_formatter_MarkBuffer shade_formatter_MarkBuffer

#include "Mark.h"
#include "FileMark.h"
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <utility>
#include <boost/shared_ptr.hpp>

namespace shade
{
namespace formatter
{
class MarkBuffer
{
  public:
    MarkBuffer(void);
    MarkBuffer(const std::string& content);

    void append(const std::string& value);

    // The returned reference is valid as long a nothing is appended
    const std::string& str(void) const;

    void set_mark(boost::shared_ptr<Mark> mark);
    unsigned int get_mark_line_number(boost::shared_ptr<Mark> mark) const;

  private:
    void update_string_cache(void) const;

    std::stringstream m_buffer;
    mutable std::string m_string_cache;
    mutable bool m_string_cache_valid;
    typedef std::map<boost::shared_ptr<Mark>, std::string::size_type> MarkPositions;
    MarkPositions m_mark_positions;
    typedef std::list<std::pair<boost::shared_ptr<Mark>, std::string::size_type>> MarkList;
    MarkList m_mark_list;

  public:
    class iterator
    {
      public:
        iterator(void);

      private:
        iterator(const MarkBuffer* buffer, std::string::size_type pos);
        friend class MarkBuffer;

      public:
        typedef std::forward_iterator_tag iterator_category;
        typedef char value_type;
        typedef std::string::size_type difference_type;
        typedef char pointer;
        typedef char reference;

        char operator*(void)const;
        char operator->(void)const;
        void operator++(void);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
        bool operator<(const iterator&) const;
        bool operator>(const iterator&) const;

        FileMark get_file_mark(void) const;

      private:
        void update_mark_iter(void);

        const MarkBuffer* m_buffer;
        std::string::size_type m_position;
        unsigned int m_line_number;
        MarkList::const_iterator m_mark_iter;
        boost::shared_ptr<FileMark> m_cur_file_mark;
    };

    iterator begin(void) const;
    iterator end(void) const;
};

MarkBuffer& operator<<(MarkBuffer& buffer, const std::string& value);
} // namespace formatter
} // namespace shade

#endif /* shade_formatter_MarkBuffer */
