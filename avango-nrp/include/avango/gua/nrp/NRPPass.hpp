#ifndef AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP

#include <avango/FieldContainer.h>
#include <avango/gua/Fields.hpp>
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
    NRPPassDescription(::gua::nrp::NRPPassDescription *guapass = new ::gua::nrp::NRPPassDescription());

  private:
    ::gua::nrp::NRPPassDescription *_nrp_pass_ptr;

    NRPPassDescription(const NRPPassDescription &);
    NRPPassDescription &operator=(const NRPPassDescription &);
};

using SFNRPPassDescription = SingleField<Link<NRPPassDescription>>;
using MFNRPPassDescription = MultiField<Link<NRPPassDescription>>;
}
}

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::nrp::NRPPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::nrp::NRPPassDescription>>;
#endif
}

#endif // AVANGO_GUA_NRP_PASS_DESCRIPTION_HPP
