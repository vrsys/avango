#include <avango/gua/utils/Mask.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::Mask);

AV_FIELD_DEFINE(av::gua::SFMask);
AV_FIELD_DEFINE(av::gua::MFMask);

av::gua::Mask::Mask(std::shared_ptr< ::gua::Mask> guaMask)
  : m_guaMask(guaMask)
{

  AV_FC_ADD_ADAPTOR_FIELD(WhiteList,
                      boost::bind(&Mask::getWhiteListCB, this, _1),
                      boost::bind(&Mask::setWhiteListCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BlackList,
                      boost::bind(&Mask::getBlackListCB, this, _1),
                      boost::bind(&Mask::setBlackListCB, this, _1));


}

//av::gua::Mask::~Mask()
//{}

void
av::gua::Mask::initClass()
{
  if (!isTypeInitialized())
  {
      av::FieldContainer::initClass();

      AV_FC_INIT(av::FieldContainer, av::gua::Mask, true);

      SFMask::initClass("av::gua::SFMask", "av::Field");
      MFMask::initClass("av::gua::MFMask", "av::Field");

      sClassTypeId.setDistributable(true);
  }
}


void
av::gua::Mask::getWhiteListCB(const SFTagList::GetValueEvent& event)
{
  if (m_guaMask->whitelist.get_user_data()) {
    *(event.getValuePtr()) = *static_cast<av::Link<av::gua::TagList>*>(m_guaMask->whitelist.get_user_data());
  }
}

void
av::gua::Mask::setWhiteListCB(const SFTagList::SetValueEvent& event)
{
  m_guaMask->whitelist = *event.getValue()->getGuaTagList();
  m_guaMask->whitelist.set_user_data(new av::Link<av::gua::TagList>(
    new av::gua::TagList(
      std::shared_ptr< ::gua::utils::TagList>(&m_guaMask->whitelist)
    )
  ));
}

void
av::gua::Mask::getBlackListCB(const SFTagList::GetValueEvent& event)
{
  if (m_guaMask->blacklist.get_user_data()) {
    *(event.getValuePtr()) = *static_cast<av::Link<av::gua::TagList>*>(m_guaMask->blacklist.get_user_data());
  }
}

void
av::gua::Mask::setBlackListCB(const SFTagList::SetValueEvent& event)
{
  m_guaMask->blacklist = *event.getValue()->getGuaTagList();
  m_guaMask->blacklist.set_user_data(new av::Link<av::gua::TagList>(
    new av::gua::TagList(
      std::shared_ptr< ::gua::utils::TagList>(&m_guaMask->blacklist)
    )
  ));
}

std::shared_ptr< ::gua::Mask>
av::gua::Mask::getGuaMask() const {
  return m_guaMask;
}
