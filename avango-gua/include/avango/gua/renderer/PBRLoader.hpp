// #ifndef AVANGO_GUA_PBR_LOADER_HPP
// #define AVANGO_GUA_PBR_LOADER_HPP

// /**
//  * \file
//  * \ingroup av_gua
//  */

// #include <gua/renderer/PBRLoader.hpp>

// #include <avango/gua/Fields.hpp>
// #include <avango/gua/scenegraph/TriMeshNode.hpp>
// #include <avango/FieldContainer.h>

// #include <avango/gua/windows_specific_gua.hpp>


// namespace av
// {
//   namespace gua
//   {
//     /**
//      * Wrapper for ::gua::PBRLoader
//      *
//      * \ingroup av_gua
//      */
//     class AV_GUA_DLL PBRLoader : public av::FieldContainer
//     {
//       AV_FC_DECLARE();

//     public:

//       /**
//        * Constructor. When called without arguments, a new ::gua::PBRLoader is created.
//        * Otherwise, the given ::gua::PBRLoader is used.
//        */
//       PBRLoader(::gua::PBRLoader* guaPBRLoader = new ::gua::PBRLoader());

//       av::Link<av::gua::Node> load( std::string const& nodename,
//                                     std::string const& fileName) const;

//       bool is_supported(std::string const& fileName) const;
//     protected:

//       /**
//        * Destructor made protected to prevent allocation on stack.
//        */
// //      virtual ~PBRLoader();

//     public:

//       /**
//        * Get the wrapped ::gua::PBRLoader.
//        */
//       ::gua::PBRLoader* getGuaPBRLoader() const;


//     private:

//       ::gua::PBRLoader *m_guaPBRLoader;

//       av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

//       PBRLoader(const PBRLoader&);
//       PBRLoader& operator=(const PBRLoader&);
//     };

//     typedef SingleField<Link<PBRLoader> > SFPBRLoader;
//     typedef MultiField<Link<PBRLoader> > MFPBRLoader;

//   }

// #ifdef AV_INSTANTIATE_FIELD_TEMPLATES
//   template class AV_GUA_DLL SingleField<Link<gua::PBRLoader> >;
//   template class AV_GUA_DLL MultiField<Link<gua::PBRLoader> >;
// #endif

// }

// #endif //AVANGO_GUA_PBR_LOADER_HPP
