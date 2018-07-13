#include "NamedSharedMemoryController.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/utils/NamedSharedMemoryController.hpp>

#include <array>

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      using type = T;
     };
   }
 }


void addMemorySegment(av::gua::NamedSharedMemoryController& memory_controller,
                      std::string const& segment_name,
                      int memory_chunk_size_in_bytes
                      ) {

  memory_controller.add_memory_segment(segment_name,
                                       memory_chunk_size_in_bytes);
}

void addReadOnlyMemorySegment(av::gua::NamedSharedMemoryController& memory_controller,
                              std::string const& segment_name
                             ) {

  memory_controller.add_read_only_memory_segment(segment_name);
}

void constructNamedAtomicIntOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                      std::string const& segment_name,
                                      std::string const& object_name) {
  memory_controller.construct_named_object_on_segment<std::atomic_int>(segment_name, object_name);
}

void constructNamed2KBCharBufferOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                          std::string const& segment_name,
                                          std::string const& object_name) {
  memory_controller.construct_named_object_on_segment<std::array<char, ::gua::MemAllocSizes::KB2> >(segment_name, object_name);
}

void constructNamed64KBCharBufferOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                           std::string const& segment_name,
                                           std::string const& object_name) {
  memory_controller.construct_named_object_on_segment<std::array<char, ::gua::MemAllocSizes::KB64> >(segment_name, object_name);
}

void constructNamed1MBCharBufferOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                          std::string const& segment_name,
                                          std::string const& object_name) {
  memory_controller.construct_named_object_on_segment<std::array<char, ::gua::MemAllocSizes::MB1> >(segment_name, object_name);
}

void constructNamed16MBCharBufferOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                           std::string const& segment_name,
                                           std::string const& object_name) {
  memory_controller.construct_named_object_on_segment<std::array<char, ::gua::MemAllocSizes::MB16> >(segment_name, object_name);
}

void setValueForNamedAtomicIntOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                        std::string const& object_name,
                                        int value) {
  // first template parameter: internal_type, second template parameter: external_type
  memory_controller.set_value_for_named_object<std::atomic_int, int>(object_name, value);
}

int getValueFromNamedAtomicIntOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                        std::string const& object_name) {
  // first template parameter: internal_type, second template parameter: external_type
  return memory_controller.get_value_from_named_object<std::atomic_int, int>(object_name);
}

void registerRemotelyConstructedObjectOnSegment(av::gua::NamedSharedMemoryController& memory_controller,
                                                     std::string const& segment_name, std::string const& object_name) {
  memory_controller.register_remotely_constructed_object_on_segment(segment_name, object_name);
}

void init_NamedSharedMemoryController()
 {




  register_ptr_to_python<av::Link<av::gua::NamedSharedMemoryController> >();
  register_field<av::gua::SFNamedSharedMemoryController>("SFNamedSharedMemoryController");
  register_multifield<av::gua::MFNamedSharedMemoryController>("MFNamedSharedMemoryController");
  class_<av::gua::NamedSharedMemoryController, av::Link<av::gua::NamedSharedMemoryController>, bases<av::FieldContainer>,
         boost::noncopyable >("NamedSharedMemoryController", "docstring", no_init)
  ;


  class_<av::gua::NamedSharedMemoryController,
         av::Link<av::gua::NamedSharedMemoryController>,
         bases<av::FieldContainer>, boost::noncopyable> ("NamedSharedMemoryController", "docstring", no_init)
         .def("add_memory_segment", &addMemorySegment)
         .def("add_read_only_memory_segment", &addReadOnlyMemorySegment)
         .def("construct_named_atomic_int",  &constructNamedAtomicIntOnSegment)
         .def("construct_named_2KB_char_buffer", &constructNamed2KBCharBufferOnSegment)
         .def("construct_named_64KB_char_buffer", &constructNamed64KBCharBufferOnSegment)
         .def("construct_named_1MB_char_buffer",  &constructNamed1MBCharBufferOnSegment)
         .def("construct_named_16MB_char_buffer", &constructNamed16MBCharBufferOnSegment)
         .def("set_value_for_named_object",  &setValueForNamedAtomicIntOnSegment)
         .def("get_value_from_named_object", &getValueFromNamedAtomicIntOnSegment)
         .def("register_remotely_constructed_object_on_segment", &registerRemotelyConstructedObjectOnSegment)
         ;

 }