#ifndef AVANGO_GUA_LOD_AUXVIEW_HPP
#define AVANGO_GUA_LOD_AUXVIEW_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Aux.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::AuxView
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL AuxView : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::AuxView is created.
         * Otherwise, the given ::gua::AuxView is used.
         */
        AuxView(std::shared_ptr< ::gua::Aux::view> guanode =
            std::shared_ptr< ::gua::Aux::view>(new ::gua::Aux::view()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~AuxView();

      public:
        /**
         * Get the wrapped ::gua::AuxView.
         */
        std::shared_ptr< ::gua::Aux::view> getGuaAuxView() const;


        SFUInt CameraId;
        SFVec3 Position;
        SFMatrix4 Transform;
        SFFloat FocalLength;
        SFFloat Distortion;
        SFUInt ImageWidth;
        SFUInt ImageHeight;
        SFUInt AtlasTileId;
        SFString ImageFile;


        virtual void getCameraIdCB(const SFUInt::GetValueEvent& event);
        virtual void getPositionCB(const SFVec3::GetValueEvent& event);
        virtual void getTransformCB(const SFMatrix4::GetValueEvent& event);
        virtual void getFocalLengthCB(const SFFloat::GetValueEvent& event);
        virtual void getDistortionCB(const SFFloat::GetValueEvent& event);
        virtual void getImageWidthCB(const SFUInt::GetValueEvent& event);
        virtual void getImageHeightCB(const SFUInt::GetValueEvent& event);
        virtual void getAtlasTileIdCB(const SFUInt::GetValueEvent& event);
        virtual void getImageFileCB(const SFString::GetValueEvent& event);


        inline uint32_t getCameraId() {
          if (m_guaAuxView)
            return m_guaAuxView->camera_id_;
          else
            return 0;
        }

        inline ::gua::math::vec3 getPosition() {
          if (m_guaAuxView)
            return ::gua::math::vec3(m_guaAuxView->position_);
          else
            return ::gua::math::vec3();
        }

        inline ::gua::math::mat4 getTransform() {
          if (m_guaAuxView)
            return ::gua::math::mat4(m_guaAuxView->transform_);
          else
            return ::gua::math::mat4::identity();
        }
        inline float getFocalLength() {
          if (m_guaAuxView)
            return m_guaAuxView->focal_length_;
          else
            return 1.0f;
        }
        inline float getDistortion() {
          if (m_guaAuxView)
            return m_guaAuxView->distortion_;
          else
            return 1.0f;
        }
        inline uint32_t getImageWidth() {
          if (m_guaAuxView)
            return m_guaAuxView->image_width_;
          else
            return 0;
        }
        inline uint32_t getImageHeight() {
          if (m_guaAuxView)
            return m_guaAuxView->image_height_;
          else
            return 0;
        }
        inline uint32_t getAtlasTileId() {
          if (m_guaAuxView)
            return m_guaAuxView->atlas_tile_id_;
          else
            return 0;
        }

        inline std::string getImageFile() {
          if (m_guaAuxView)
            return m_guaAuxView->image_file_;
          else
            return "";
        }


      private:
        std::shared_ptr< ::gua::Aux::view> m_guaAuxView;

        AuxView(const AuxView&);
        AuxView& operator=(const AuxView&);
      };

      typedef SingleField<Link<AuxView> > SFAuxView;
      typedef MultiField<Link<AuxView> > MFAuxView;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::AuxView> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::AuxView> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUXVIEW_HPP
