#ifndef AVANGO_GUA_NAMED_SHARED_MEMORY_CONTROLLER_HPP
#define AVANGO_GUA_NAMED_SHARED_MEMORY_CONTROLLER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/math/math.hpp>
#include <gua/utils/NamedSharedMemoryController.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::Ray
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL NamedSharedMemoryController : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::Ray is created.
     * Otherwise, the given ::gua::Ray is used.
     */

    NamedSharedMemoryController(/*std::shared_ptr< ::gua::NamedSharedMemoryController> guaNamedSharedMemoryController =
                                  nullptr*/);

  public:
    //  Get the wrapped ::gua::NamedSharedMemoryController.

    std::shared_ptr<::gua::NamedSharedMemoryController> getGuaNamedSharedMemoryController() const;

  public:
#if not defined(__WIN32__) && not defined(_WIN32) && not defined(_WIN64)
    void add_memory_segment(std::string const& segment_name, uint64_t size_in_byte);
    void add_read_only_memory_segment(std::string const& segment_name);

    void register_remotely_constructed_object_on_segment(std::string const& segment_name, std::string const& object_name);

    template <typename INTERNAL_TYPE>
    void construct_named_object_on_segment(std::string const& segment_name, std::string const& object_name)
    {
        m_guaNamedSharedMemoryController->construct_named_object_on_segment<INTERNAL_TYPE>(segment_name, object_name);
    }

    template <typename INTERNAL_TYPE>
    void destroy_named_object(std::string const& object_name)
    {
        m_guaNamedSharedMemoryController->construct_named_object_on_segment<INTERNAL_TYPE>(object_name);
    }

    template <typename INTERNAL_TYPE, typename EXTERNAL_TYPE>
    void set_value_for_named_object(std::string const& object_name, EXTERNAL_TYPE const& value)
    {
        m_guaNamedSharedMemoryController->set_value_for_named_object<INTERNAL_TYPE, EXTERNAL_TYPE>(object_name, value);
    }

    template <typename INTERNAL_TYPE, typename EXTERNAL_TYPE>
    EXTERNAL_TYPE get_value_from_named_object(std::string const& object_name)
    {
        return m_guaNamedSharedMemoryController->get_value_from_named_object<INTERNAL_TYPE, EXTERNAL_TYPE>(object_name);
    }
#endif

  private:
    static std::shared_ptr<::gua::NamedSharedMemoryController> m_guaNamedSharedMemoryController;

    NamedSharedMemoryController(const NamedSharedMemoryController&);
    NamedSharedMemoryController& operator=(const NamedSharedMemoryController&);
};

using SFNamedSharedMemoryController = SingleField<Link<NamedSharedMemoryController>>;
using MFNamedSharedMemoryController = MultiField<Link<NamedSharedMemoryController>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::NamedSharedMemoryController>>;
template class AV_GUA_DLL MultiField<Link<gua::NamedSharedMemoryController>>;
#endif

} // namespace av

#endif // AVANGO_GUA_NAMED_SHARED_MEMORY_CONTROLLER_HPP
