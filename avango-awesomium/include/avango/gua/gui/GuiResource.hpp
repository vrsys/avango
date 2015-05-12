#ifndef AVANGO_GUA_GUI_RESOURCE_HPP
#define AVANGO_GUA_GUI_RESOURCE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/gui/GuiResource.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Application.h>

namespace av
{

namespace gua
{

  namespace gui
  {
    /**
     * Wrapper for ::gua::GuiResource
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL GuiResource : public av::gua::TransformNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::GuiResource is created.
       * Otherwise, the given ::gua::GuiResource is used.
       */
      GuiResource(std::shared_ptr< ::gua::GuiResource> guaGuiResource =
                  std::shared_ptr< ::gua::GuiResource>(new ::gua::GuiResource()));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~GuiResource();

    public:

      SFString TextureName;
      SFString URL;
      SFVec2   Size;
      SFBool   Interactive;


      /**
       * Get the wrapped ::gua::GuiResource.
       */
      std::shared_ptr< ::gua::GuiResource> getGuaGuiResource() const;
      unsigned     getUserDataHandle() const;


    public:

      virtual void getTextureNameCB(const SFString::GetValueEvent& event);
      virtual void setTextureNameCB(const SFString::SetValueEvent& event);

      virtual void getURLCB(const SFString::GetValueEvent& event);
      virtual void setURLCB(const SFString::SetValueEvent& event);

      virtual void getSizeCB(const SFVec2::GetValueEvent& event);
      virtual void setSizeCB(const SFVec2::SetValueEvent& event);

      virtual void getInteractiveCB(const SFBool::GetValueEvent& event);
      virtual void setInteractiveCB(const SFBool::SetValueEvent& event);


      void on_loaded(std::function<void()> const& callback) const;

      void on_javascript_callback(std::function<void(
                                    std::string const&,
                                    std::vector<std::string> const& )> const& callback) const;

      void add_javascript_callback(std::string const& name);
      void add_javascript_getter(std::string const& name, std::function<std::string()> callback);

      void go_forward();
      void go_back();
      void go_to_history_offset(int offset);

      void reload();
      void focus();

      void inject_keyboard_event(int key, int scancode, int action, int mods) const;
      void inject_char_event(unsigned c) const;

      void inject_mouse_position_relative(::gua::math::vec2 const& position) const;
      void inject_mouse_position(::gua::math::vec2 const& position) const;
      void inject_mouse_button(int button, int action, int mods) const;
      void inject_mouse_wheel(::gua::math::vec2 const& direction) const;

      void call_javascript(std::string const& method, std::vector<std::string> const& args) const;

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);

    private:

      std::shared_ptr< ::gua::GuiResource> m_guaGuiResource;

      bool              m_initialized;
      bool              m_distributed;
      std::string       m_textureName;
      ::gua::math::vec2 m_size;

      Application::CallbackHandle m_clearCallbackHandle;

      mutable MFVec2 m_networkMousePositions;
      mutable MFVec2 m_networkMousePositionsRelative;
      mutable MFVec3i m_networkMouseButtons;
      mutable MFVec2 m_networkMouseWheelDirections;

      bool check_completeness() const;

      void init();
      void clearCallback();
      /*virtual*/ void fieldHasChangedLocalSideEffect(Field const& field);

      GuiResource(const GuiResource&);
      GuiResource& operator=(const GuiResource&);

    };

    typedef SingleField<Link<GuiResource> > SFGuiResource;
    typedef MultiField<Link<GuiResource> > MFGuiResource;

  } // namespace gui

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::gui::GuiResource> >;
  template class AV_GUA_DLL MultiField<Link<gua::gui::GuiResource> >;
#endif

} // namespace av

#endif //AVANGO_GUA_GUI_RESOURCE_HPP
