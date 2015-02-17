// #ifndef AVANGO_GUA_VIDEO3D_NODE_HPP
// #define AVANGO_GUA_VIDEO3D_NODE_HPP

// /**
//  * \file
//  * \ingroup av_gua
//  */

// #include <gua/node/Video3DNode.hpp>
// #include <gua/math/math.hpp>

// #include <avango/gua/scenegraph/GeometryNode.hpp>

// namespace av
// {
//   namespace gua
//   {
//     /**
//      * Wrapper for ::gua::Video3DNode
//      *
//      * \ingroup av_gua
//      */
//     class AV_GUA_DLL Video3DNode : public av::gua::GeometryNode
//     {
//       AV_FC_DECLARE();

//     public:

//       /**
//        * Constructor. When called without arguments, a new ::gua::Video3DNode
//        * is created.
//        * Otherwise, the given ::gua::Video3DNode is used.
//        */
//       Video3DNode(std::shared_ptr< ::gua::node::Video3DNode> guanode =
//           std::shared_ptr< ::gua::node::Video3DNode>(new ::gua::node::Video3DNode("")));

//     protected:

//       /**
//        * Destructor made protected to prevent allocation on stack.
//        */
// //      virtual ~Video3DNode();

//     public:

//       /**
//        * Get the wrapped ::gua::Video3DNode.
//        */
//       //std::shared_ptr< ::gua::Video3DNode> getGuaNode() const;

//     private:

//       Video3DNode(const Video3DNode&);
//       Video3DNode& operator=(const Video3DNode&);
//     };

//     typedef SingleField<Link<Video3DNode> > SFVideo3DNode;
//     typedef MultiField<Link<Video3DNode> > MFVideo3DNode;

//   }

// #ifdef AV_INSTANTIATE_FIELD_TEMPLATES
//   template class AV_GUA_DLL SingleField<Link<gua::Video3DNode> >;
//   template class AV_GUA_DLL MultiField<Link<gua::Video3DNode> >;
// #endif

// }

// #endif //AVANGO_GUA_VIDEO3D_NODE_HPP
