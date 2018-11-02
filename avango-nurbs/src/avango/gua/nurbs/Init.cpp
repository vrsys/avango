#include <avango/gua/nurbs/scenegraph/NURBSNode.hpp>
#include <avango/gua/nurbs/renderer/NURBSLoader.hpp>

#include <gua/guacamole.hpp>

AV_TYPED_DEFINE_ABSTRACT(av::gua::nurbs::Init);

/* static */ void
av::gua::nurbs::Init::initClass()
{
    if (!isTypeInitialized())
    {
        char** argv;
        ::gua::init(0, argv);

        av::gua::nurbs::NURBSNode::initClass();
        av::gua::nurbs::NURBSLoader::initClass();

        AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::gua::nurbs::Init", true);
    }
}
