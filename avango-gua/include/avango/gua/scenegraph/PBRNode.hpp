// #ifndef AVANGO_GUA_PBR_NODE_HPP
// #define AVANGO_GUA_PBR_NODE_HPP

// /**
//  * \file
//  * \ingroup av_gua
//  */

// #include <gua/node/PBRNode.hpp>
// #include <gua/math/math.hpp>

// #include <avango/gua/scenegraph/GeometryNode.hpp>

// namespace av
// {
//   namespace gua
//   {
//     /**
//      * Wrapper for ::gua::PBRNode
//      *
//      * \ingroup av_gua
//      */
//     class AV_GUA_DLL PBRNode : public av::gua::GeometryNode
//     {
//       AV_FC_DECLARE();

//     public:

//       /**
//        * Constructor. When called without arguments, a new ::gua::PBRNode
//        * is created.
//        * Otherwise, the given ::gua::PBRNode is used.
//        */
//       PBRNode(std::shared_ptr< ::gua::node::PBRNode> guanode =
//           std::shared_ptr< ::gua::node::PBRNode>(new ::gua::node::PBRNode("")));

//     protected:

//       /**
//        * Destructor made protected to prevent allocation on stack.
//        */
// //      virtual ~PBRNode();

//     public:

//       /**
//        * Get the wrapped ::gua::PBRNode.
//        */
//       //std::shared_ptr< ::gua::PBRNode> getGuaNode() const;

//     private:

//       PBRNode(const PBRNode&);
//       PBRNode& operator=(const PBRNode&);
//     };

//     typedef SingleField<Link<PBRNode> > SFPBRNode;
//     typedef MultiField<Link<PBRNode> > MFPBRNode;

//   }

// #ifdef AV_INSTANTIATE_FIELD_TEMPLATES
//   template class AV_GUA_DLL SingleField<Link<gua::PBRNode> >;
//   template class AV_GUA_DLL MultiField<Link<gua::PBRNode> >;
// #endif

// }

// #endif //AVANGO_GUA_PBR_NODE_HPP
