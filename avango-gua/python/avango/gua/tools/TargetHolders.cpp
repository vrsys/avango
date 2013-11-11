#include "TargetHolders.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>

#include <avango/gua/tools/TargetHolder.hpp>

using namespace boost::python;
using namespace av::python;

namespace
{
  list
  TargetHolderFind(av::gua::TargetHolder& holder, av::gua::TargetHolder* type)
  {
    list py_holders;

    if (type != 0)
    {
      av::gua::TargetHolder::FindList found_holders = holder.find(type->getTypeId());
      for (av::gua::TargetHolder::FindList::iterator found_holder = found_holders.begin();
           found_holder != found_holders.end(); ++found_holder)
      {
        py_holders.append(*found_holder);
      }
    }

    return py_holders;
  }
}

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      typedef T type;
    };
  }
}

void init_TargetHolders()
{

  register_field<av::gua::SFTargetHolder>("SFTargetHolder");
  register_multifield<av::gua::MFTargetHolder>("MFTargetHolder");

  class_<av::gua::TargetHolder, av::Link<av::gua::TargetHolder>, bases<av::FieldContainer>, boost::noncopyable >("TargetHolder", "TargetHolder base class", no_init)
    .def("find", TargetHolderFind)
    ;
}

