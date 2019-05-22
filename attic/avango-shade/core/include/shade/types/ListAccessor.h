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

#ifndef shade_types_ListAccessor_H
#define shade_types_ListAccessor_H shade_types_ListAccessor_H

#include "TypeAccessor.h"
#include <boost/shared_ptr.hpp>
#include <utility>
#include <exception>

namespace shade
{
class Shader;

namespace types
{
class ListAccessor : public TypeAccessor
{
  protected:
    class InIteratorImpl
    {
      public:
        virtual ~InIteratorImpl(void) {}
        virtual InIteratorImpl* clone(void) = 0;
        virtual const boost::shared_ptr<Shader> dereference(void) const = 0;
        virtual void increment(void) = 0;
        virtual bool equals(const InIteratorImpl& other) const = 0;
    };

    class OutIteratorImpl
    {
      public:
        virtual ~OutIteratorImpl(void) {}
        virtual OutIteratorImpl* clone(void) = 0;
        virtual boost::shared_ptr<Shader>& dereference(void) = 0;
        virtual void increment(void) = 0;
    };

  public:
    class InIterator
    {
      public:
        InIterator(InIteratorImpl* impl) : m_impl(impl) {}

        InIterator(const InIterator& other) : m_impl(other.m_impl->clone()) {}

        const InIterator& operator=(const InIterator& other)
        {
            m_impl = boost::shared_ptr<InIteratorImpl>(other.m_impl->clone());
            return *this;
        }

        const boost::shared_ptr<Shader> operator*(void)const { return m_impl->dereference(); }

        void operator++(void) { return m_impl->increment(); }

        bool operator==(const InIterator& other) { return m_impl->equals(*other.m_impl); }

        bool operator!=(const InIterator& other) { return !m_impl->equals(*other.m_impl); }

      private:
        boost::shared_ptr<InIteratorImpl> m_impl;
    };

    typedef std::pair<InIterator, InIterator> Range;

    class OutIterator
    {
      public:
        OutIterator(OutIteratorImpl* impl) : m_impl(impl) {}

        OutIterator(const OutIterator& other) : m_impl(other.m_impl->clone()) {}

        const OutIterator& operator=(const OutIterator& other)
        {
            m_impl = boost::shared_ptr<OutIteratorImpl>(other.m_impl->clone());
            return *this;
        }

        boost::shared_ptr<Shader>& operator*(void) { return m_impl->dereference(); }

        void operator++(void) { m_impl->increment(); }

      private:
        boost::shared_ptr<OutIteratorImpl> m_impl;
    };

    virtual OutIterator set(void) = 0;
    virtual Range get(void) const = 0;

    class BadCast : public std::bad_cast
    {
      public:
        /*virtual*/ const char* what(void) const throw();
    };

    /*virtual*/ HashType hash(void) const;

  private:
    static HashValue m_hash;
};
} // namespace types
} // namespace shade

#endif /* shade_types_ListAccessor_H */
