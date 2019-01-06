#ifndef AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP

#include <avango/gua/renderer/PipelinePassDescription.hpp>
#include <gua/nrp/nrp_pass.hpp>

namespace av
{
namespace gua
{
namespace nrp
{
class AV_GUA_DLL NRPPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    NRPPassDescription(std::shared_ptr<::gua::nrp::NRPPassDescription> const &NRPPassDescription = std::shared_ptr<::gua::nrp::NRPPassDescription>(new ::gua::nrp::NRPPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::PipelinePassDescription.
     */
    std::shared_ptr<::gua::nrp::NRPPassDescription> const &getGuaPipelinePassDescription() const;

  private:
    std::shared_ptr<::gua::nrp::NRPPassDescription> m_guaPipelinePassDescription;

    NRPPassDescription(const NRPPassDescription &);
    NRPPassDescription &operator=(const NRPPassDescription &);
};

using SFNRPPassDescription = av::SingleField<av::Link<av::gua::nrp::NRPPassDescription>>;
using MFNRPPassDescription = av::MultiField<av::Link<av::gua::nrp::NRPPassDescription>>;
}
}

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL av::SingleField<av::Link<gua::nrp::NRPPassDescription>>;
template class AV_GUA_DLL av::MultiField<av::Link<gua::nrp::NRPPassDescription>>;
#endif
}

#endif // AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP
