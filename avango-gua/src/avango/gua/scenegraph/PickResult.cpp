#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>

AV_FC_DEFINE(av::gua::PickResult);

AV_FIELD_DEFINE(av::gua::SFPickResult);
AV_FIELD_DEFINE(av::gua::MFPickResult);

av::gua::PickResult::PickResult(::gua::PickResult const& result)
{
    av::Link<av::gua::Node> node(nullptr);
    if(result.object && result.object->get_user_data(0))
    {
        node = static_cast<av::gua::Node*>(result.object->get_user_data(0));
    }

    AV_FC_ADD_FIELD(Distance, result.distance);
    AV_FC_ADD_FIELD(Object, node);
    AV_FC_ADD_FIELD(Position, result.position);
    AV_FC_ADD_FIELD(WorldPosition, result.world_position);
    AV_FC_ADD_FIELD(Normal, result.normal);
    AV_FC_ADD_FIELD(WorldNormal, result.world_normal);
    AV_FC_ADD_FIELD(TextureCoords, result.texture_coords);
}

av::gua::PickResult::~PickResult() {}

void av::gua::PickResult::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::PickResult, true);

        SFPickResult::initClass("av::gua::SFPickResult", "av::Field");
        MFPickResult::initClass("av::gua::MFPickResult", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
