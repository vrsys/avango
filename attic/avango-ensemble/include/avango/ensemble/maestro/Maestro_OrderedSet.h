// $Header$

#ifndef __MAESTRO_ORDERED_SET_H__
#define __MAESTRO_ORDERED_SET_H__

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  February 95                            *
 *                                                                  *
 * Contents:  Implementation of Ordered Set macro                   *
 *                                                                  *
 ********************************************************************/

// TODO:  Make it efficient.

/********************** OrderedSet implementation ***********************/

const int MAESTRO_MESSAGE_MAGIC_ORDERED_SET = 204987;

#define Maestro_OrderedSet(T)                                    \
                                                                 \
class Maestro_OrderedSet ## T : virtual public Maestro_Base {    \
                                                                 \
public:                                                          \
                                                                 \
  Maestro_OrderedSet ## T (Maestro_ErrorHandler *error_handler = 0) : \
    Maestro_Base(error_handler) {                                \
    list = 0;                                                    \
    listSize = 0;                                                \
  }                                                              \
                                                                 \
  Maestro_OrderedSet ## T (T &t, Maestro_ErrorHandler *error_handler = 0) : \
    Maestro_Base(error_handler) {                                \
    list = new T*[1];                                            \
    list[0] = new T(t);                                          \
    listSize = 1;                                                \
  }                                                              \
                                                                 \
  Maestro_OrderedSet ## T (Maestro_OrderedSet ## T &l,           \
                           Maestro_ErrorHandler *error_handler = 0) : \
    Maestro_Base(error_handler) {                                \
    listSize = l.size();                                         \
    if (listSize) {                                              \
      list = new T*[listSize];                                   \
      for (unsigned i = 0; i < listSize; i++)                    \
        list[i] = new T(l[i]);                                   \
    }                                                            \
    else                                                         \
      list = 0;                                                  \
  }                                                              \
                                                                 \
  Maestro_OrderedSet ## T (T *tp, unsigned nelem,                \
			   Maestro_ErrorHandler *error_handler = 0) : \
    Maestro_Base(error_handler) {                                \
    listSize = nelem;                                            \
    if (listSize) {                                              \
      list = new T*[listSize];                                   \
      for (unsigned i = 0; i < listSize; i++)                    \
        list[i] = new T(tp[i]);                                  \
    }                                                            \
    else                                                         \
      list = 0;                                                  \
  }                                                              \
                                                                 \
  virtual ~Maestro_OrderedSet ## T () {                          \
     clear();                                                    \
                                                                 \
  }                                                              \
                                                                 \
  virtual Maestro_OrderedSet ## T & operator += (T &t) {         \
    unsigned int i;                                                       \
    for (i = 0; i < listSize; i++)                               \
      if (t == *list[i])                                         \
        return *this;                                            \
                                                                 \
    if (!listSize) {                                             \
      assert(list == 0);                                         \
      list = new T*[1];                                          \
    }                                                            \
    else {                                                       \
      assert(list);                                              \
    T **tmp = new T*[listSize + 1];                              \
      for (i = 0; i < listSize; i++)                             \
        tmp[i] = list[i];                                        \
      delete [] list;                                            \
      list = tmp;                                                \
    }                                                            \
                                                                 \
    list[listSize++] = new T(t);                                 \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual Maestro_OrderedSet ## T& operator -= (T &t) {          \
    if (!listSize)                                               \
      return *this;                                              \
    T **tmp = new T*[listSize];                                  \
    unsigned int index = 0;                                               \
    for (unsigned i = 0; i < listSize; i++)                      \
      if (*list[i] == t)                                         \
        delete list[i];                                          \
      else                                                       \
        tmp[index++] = list[i];                                  \
    listSize = index;                                            \
    delete [] list;                                              \
    list = tmp;                                                  \
    if (!listSize) {                                             \
      delete [] list;                                            \
      list = 0;                                                  \
    }                                                            \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual Maestro_OrderedSet ## T& operator = (Maestro_OrderedSet ## T &l) { \
    if (this != &l) {                                            \
      clear();                                                   \
      listSize = l.size();                                       \
      if (listSize) {                                            \
        list = new T*[listSize];                                 \
        for (unsigned i = 0; i < listSize; i++)                  \
	  list[i] = new T(l[i]);                                 \
      }                                                          \
    }                                                            \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual Maestro_OrderedSet ## T& operator += (Maestro_OrderedSet ## T &l) { \
    if (!l.size() || (this == &l))                               \
      return *this;                                              \
    T **tmp = new T*[listSize + l.listSize];                     \
    unsigned i, index = listSize;                                \
    for (i = 0; i < listSize; i++)                               \
      tmp[i] = list[i];                                          \
    for (i = 0; i < l.listSize; i++)                             \
      if (!contains(l[i]))                                       \
        tmp[index++] = new T(l[i]);                              \
    listSize = index;                                            \
    delete [] list;                                              \
    list = tmp;                                                  \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual Maestro_OrderedSet ## T& operator -= (Maestro_OrderedSet ## T &l) { \
    if (!l.size() || !listSize)                                  \
      return *this;                                              \
    if (this == &l) {                                            \
      clear();                                                   \
      return *this;                                              \
    }                                                            \
    T **tmp = new T*[listSize];                                  \
    unsigned int index = 0;                                               \
    for (unsigned i = 0; i < listSize; i++)                      \
      if (!l.contains(*list[i]))                                 \
        tmp[index++] = list[i];                                  \
      else                                                       \
        delete list[i];                                          \
    listSize = index;                                            \
    delete [] list;                                              \
    list = tmp;                                                  \
    if (!listSize) {                                             \
      delete [] list;                                            \
      list = 0;                                                  \
    }                                                            \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual Maestro_OrderedSet ## T& operator &= (Maestro_OrderedSet ## T &l) { \
    if (!l.size() || !listSize) {                                \
      clear();                                                   \
      return *this;                                              \
    }                                                            \
    if (this == &l)                                              \
      return *this;                                              \
    T **tmp = new T*[listSize];                                  \
    unsigned int index = 0;                                               \
    for (unsigned i = 0; i < listSize; i++)                      \
      if (l.contains(*list[i]))                                  \
        tmp[index++] = list[i];                                  \
      else                                                       \
        delete list[i];                                          \
    listSize = index;                                            \
    delete [] list;                                              \
    list = tmp;                                                  \
    if (!listSize) {                                             \
      delete [] list;                                            \
      list = 0;                                                  \
    }                                                            \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual void operator <<= (Maestro_OrderedSet ## T &l) {       \
    if (this != &l) {                                            \
      clear();                                                   \
      listSize = l.listSize;                                     \
      l.listSize = 0;                                            \
      list = l.list;                                             \
      l.list = 0;                                                \
    }                                                            \
  }                                                              \
                                                                 \
  virtual int operator [] (T &t) const {                         \
    for (unsigned int i = 0; i < listSize; i++)                      \
      if (*list[i] == t)                                         \
        return i;                                                \
    return -1;                                                   \
  }                                                              \
                                                                 \
  virtual T& operator [] (unsigned i) const {                    \
    assert(i < listSize);                                        \
    return *list[i];                                             \
  }                                                              \
                                                                 \
  virtual int contains(T &t) const {                             \
    for (unsigned i = 0; i < listSize; i++)                      \
      if (*list[i] == t)                                         \
        return 1;                                                \
    return 0;                                                    \
  }                                                              \
                                                                 \
  virtual void clear() {                                         \
    unsigned int i;                                                       \
    for (i = 0; i < listSize; i++)                               \
      delete list[i];                                            \
    delete [] list;                                              \
    list = 0;                                                    \
    listSize = 0;                                                \
  }                                                              \
                                                                 \
  virtual unsigned size() const { return listSize; }             \
                                                                 \
  void operator >> (Maestro_Message& msg) {                      \
    unsigned int i;                                                       \
    for (i = 0; i < listSize; i++)                               \
      msg << *(list[i]);                                         \
    msg << listSize;                                             \
  }                                                              \
                                                                 \
  void operator << (Maestro_Message& msg) {                      \
    clear();                                                     \
    assert(listSize == 0 && list == 0);                          \
                                                                 \
    msg >> (int&) listSize;                                      \
    if (listSize) {                                              \
      list = new T*[listSize];                                   \
      int i;                                                     \
      for (i = listSize - 1; i >= 0; i--) {                      \
	list[i] = new T;                                         \
	msg >> *(list[i]);                                       \
      }                                                          \
    }                                                            \
  }                                                              \
                                                                 \
  int messageMagic() const {                                     \
    return MAESTRO_MESSAGE_MAGIC_ORDERED_SET;                    \
  }                                                              \
                                                                 \
  void operator >> (std::ostream& out) const {                   \
    unsigned int i;                                              \
    for (i = 0; i < listSize; i++)                               \
      out << *(list[i]);                                         \
  }                                                              \
                                                                 \
protected:                                                       \
                                                                 \
  T **list;                                                      \
  unsigned int listSize;                                             \
}

#endif // __MAESTRO_ORDERED_SET_H__

