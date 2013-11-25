// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_SINGLETON_H)
#define AVANGO_SINGLETON_H

#include <avango/Assert.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <stdexcept>
#include <cstdlib>

namespace av
{
  template<class T>
  class DefaultLifeTime
  {
  public:
    static void onDeadReference() { throw std::logic_error("Access to dead singleton"); }
    static void scheduleDestruction(T*, void (*pFun)()) { std::atexit(pFun); }
  };

  template<class T> class CreateUsingNew //do not use for arrays
  {
  public:
    static T* create() { return new T(); }
    static void destroy(T* inType) { delete inType; inType = 0; } //@note: this is not intended to be used on arrays!
  protected:
    ~CreateUsingNew();//do not allow CreateUsingNew<Foo>* pCreator = &foo; delete pCreator->undef behave
  };

  template<class T>
  class SingleThreaded
  {
  public:
    class Lock //fake lock
    {
    public:
      Lock() { ; }
    };
    typedef T VolatileType; //ensure optimal variant for single threaded environment
  };

  template<class T>
  class MultiThreaded
  {
  public:
    class Lock
    {
    public:
      typedef boost::mutex::scoped_lock ScopeLock;
      Lock();
    private:
      ScopeLock mGuard;
    };
    typedef volatile T VolatileType; //ensure compiler does get hints what we are trying to do
    static boost::mutex sConstructionMutex;
  };

  template<class T> MultiThreaded<T>::Lock::Lock() :
    mGuard(sConstructionMutex)
    {
    }

  template<class T> boost::mutex MultiThreaded<T>::sConstructionMutex;

  template<typename T, template <class> class CreationPolicy = CreateUsingNew,
  template <class> class LifeTimePolicy = DefaultLifeTime,
  template <class> class ThreadingModel = SingleThreaded>
  class Singleton
  {
  public:
    typedef typename ThreadingModel<T*>::VolatileType InstanceTypePtr; //T or volatile T, depending on threading model
    typedef T Type;

    static Type& get() //no exposing of raw pointer to assure no delete call on ValueType
    {
      // FIXME This is potentially not thread-safe if access to sInstance is
      // non-atomic. Better use boost::call_once().
      if ( !sInstance)
      {
        typename ThreadingModel<T>::Lock lock;
        if ( !sInstance)
        {
          if (sIsDead)
          {
            LifeTimePolicy<Type>::onDeadReference();
            sIsDead = false;
          }
          sInstance = CreationPolicy<Type>::create();
          LifeTimePolicy<T>::scheduleDestruction(sInstance,
                                                 &destroySingleton);
        }
      }
      return *sInstance;
    }

    static boost::shared_ptr<Type> getAsSharedPtr()
    {
      return boost::shared_ptr<Type>(&get(), NullDeleter());
    }
  private:

    static void destroySingleton();

    Singleton();
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&); //no assignment

    // boost::shared_ptr must not delete the singleton
    class NullDeleter
    {
    public:
      void operator()(void const *) const {}
    };

    static InstanceTypePtr sInstance;
    static bool sIsDead;
  };

  template <
  typename T,
  template <class> class C,
  template <class> class L,
  template <class> class M
  >
  bool Singleton<T, C, L, M>::sIsDead = false;

  template <
  typename T,
  template <class> class C,
  template <class> class L,
  template <class> class M
  >
  typename Singleton<T, C, L, M >::InstanceTypePtr Singleton<T, C, L, M >::sInstance = 0;

  template
  <
  class T,
  template <class> class CreationPolicy,
  template <class> class L,
  template <class> class M
  >
  void Singleton<T, CreationPolicy, L, M>::destroySingleton()
  {
    AV_ASSERT(!sIsDead);
    CreationPolicy<T>::destroy(sInstance);
    sInstance = 0;
    sIsDead = true;
  }

} // namespace av

#endif // #if !defined(AVANGO_SINGLETON_H)
