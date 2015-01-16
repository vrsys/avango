#include <avango/gua/utils/Mask.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::Mask);

AV_FIELD_DEFINE(av::gua::SFMask);
AV_FIELD_DEFINE(av::gua::MFMask);

av::gua::Mask::Mask( ::gua::Mask* guaMask)
  : m_guaMask(guaMask)
{

  m_whiteList = av::Link<av::gua::TagList>(new av::gua::TagList(&m_guaMask->whitelist));
  m_blackList = av::Link<av::gua::TagList>(new av::gua::TagList(&m_guaMask->blacklist));

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
  if (m_whiteList.isValid()) {
    *(event.getValuePtr()) = m_whiteList;
  }
}

void
av::gua::Mask::setWhiteListCB(const SFTagList::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_whiteList = event.getValue();
    m_guaMask->whitelist = *m_whiteList->getGuaTagList();
  }
}

void
av::gua::Mask::getBlackListCB(const SFTagList::GetValueEvent& event)
{
  if (m_blackList.isValid()) {
    *(event.getValuePtr()) = m_blackList;
  }
}

void
av::gua::Mask::setBlackListCB(const SFTagList::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_blackList = event.getValue();
    m_guaMask->blacklist = *m_blackList->getGuaTagList();
  }
}

::gua::Mask*
av::gua::Mask::getGuaMask() const {
  return m_guaMask;
}
