#ifndef AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP

#include <gua/nrp/nrp_pass.hpp>
#include <avango/gua/renderer/PipelinePassDescription.hpp>

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
    NRPPassDescription(::gua::nrp::NRPPassDescription *guapass = new ::gua::nrp::NRPPassDescription());

  private:
    ::gua::nrp::NRPPassDescription *_nrp_pass_ptr;
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
