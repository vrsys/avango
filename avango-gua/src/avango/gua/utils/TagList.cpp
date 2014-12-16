#include <avango/gua/utils/TagList.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TagList);

AV_FIELD_DEFINE(av::gua::SFTagList);
AV_FIELD_DEFINE(av::gua::MFTagList);

av::gua::TagList::TagList(std::shared_ptr< ::gua::utils::TagList> guaTagList)
  : m_guaTagList(guaTagList)
{

  AV_FC_ADD_ADAPTOR_FIELD(Tags,
                      boost::bind(&TagList::getTagsCB, this, _1),
                      boost::bind(&TagList::setTagsCB, this, _1));
}

//av::gua::TagList::~TagList()
//{}

void
av::gua::TagList::initClass()
{
  if (!isTypeInitialized())
  {
      av::FieldContainer::initClass();

      AV_FC_INIT(av::FieldContainer, av::gua::TagList, true);

      SFTagList::initClass("av::gua::SFTagList", "av::Field");
      MFTagList::initClass("av::gua::MFTagList", "av::Field");

      sClassTypeId.setDistributable(true);
  }
}


void
av::gua::TagList::getTagsCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTagList->get_strings();
}

void
av::gua::TagList::setTagsCB(const MFString::SetValueEvent& event)
{
  m_guaTagList->add_tags(event.getValue());
}

std::shared_ptr< ::gua::utils::TagList>
av::gua::TagList::getGuaTagList() const {
  return m_guaTagList;
}
