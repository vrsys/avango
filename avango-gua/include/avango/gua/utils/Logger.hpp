#ifndef AVANGO_GUA_LOGGER_HPP
#define AVANGO_GUA_LOGGER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/utils/Logger.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Logger
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Logger : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Logger is created.
       * Otherwise, the given ::gua::Logger is used.
       */
      Logger();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
     // virtual ~Logger();

    public:


      SFBool EnableDebug;
      SFBool EnableMessage;
      SFBool EnableWarning;
      SFBool EnableError;

      /**
       * Get the wrapped ::gua::Logger.
       */
      // std::shared_ptr< ::gua::Logger> getGuaNode() const;

    public:

      virtual void getEnableDebugCB(const SFBool::GetValueEvent& event);
      virtual void setEnableDebugCB(const SFBool::SetValueEvent& event);

      virtual void getEnableMessageCB(const SFBool::GetValueEvent& event);
      virtual void setEnableMessageCB(const SFBool::SetValueEvent& event);

      virtual void getEnableWarningCB(const SFBool::GetValueEvent& event);
      virtual void setEnableWarningCB(const SFBool::SetValueEvent& event);

      virtual void getEnableErrorCB(const SFBool::GetValueEvent& event);
      virtual void setEnableErrorCB(const SFBool::SetValueEvent& event);


    private:

      // std::shared_ptr< ::gua::Logger> m_guaNode;

      Logger(const Logger&);
      Logger& operator=(const Logger&);
    };

    using SFLogger = SingleField<Link<Logger> >;
    using MFLogger = MultiField<Link<Logger> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Logger> >;
  template class AV_GUA_DLL MultiField<Link<gua::Logger> >;
#endif

}

#endif //AVANGO_GUA_LOGGER_HPP
