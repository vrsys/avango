#include <avango/vrpn/TrackerInformation.h>

#include <osg/Vec4>
#include <osg/Geode>

#include <boost/bind.hpp>

#include <iostream>

#include <avango/Logger.h>
#include <avango/osg/Types.h>


namespace
{
  av::Logger& logger(av::getLogger("av::vrpn::TrackerInformation"));
}

AV_FC_DEFINE(av::vrpn::TrackerInformation);

AV_FIELD_DEFINE(av::vrpn::SFTrackerInformation);
AV_FIELD_DEFINE(av::vrpn::MFTrackerInformation);

av::vrpn::TrackerInformation::TrackerInformation() : ::av::FieldContainer()
{
  AV_FC_ADD_FIELD(Number, -1);
  AV_FC_ADD_FIELD(Matrix, ::osg::Matrix());
  AV_FC_ADD_ADAPTOR_FIELD( Pos,
                            boost::bind(&av::vrpn::TrackerInformation::getPOSCB, this, _1),
                            boost::bind(&av::vrpn::TrackerInformation::setPOSCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD( Quat,
                              boost::bind(&av::vrpn::TrackerInformation::getQUATCB, this, _1),
                              boost::bind(&av::vrpn::TrackerInformation::setQUATCB, this, _1));

  mPos = ::osg::Vec3();
  mQuat = ::osg::Quat();
}

av::vrpn::TrackerInformation::~TrackerInformation()
{}

void
av::vrpn::TrackerInformation::initClass()
{
  if (!isTypeInitialized())
  {
    ::av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::vrpn::TrackerInformation, true);

    SFTrackerInformation::initClass("av::vrpn::SFTrackerInformation", "av::Field");
    MFTrackerInformation::initClass("av::vrpn::MFTrackerInformation", "av::Field");
  }
}

void
av::vrpn::TrackerInformation::updateMatrix() {
  ::osg::Matrixf pos_mat;
  pos_mat.makeTranslate(Pos.getValue());

  ::osg::Matrixf rot_mat(Quat.getValue());
  pos_mat.preMult(rot_mat);

  Matrix.setValue(pos_mat);
}

void
av::vrpn::TrackerInformation::getPOSCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mPos;
}
void
av::vrpn::TrackerInformation::setPOSCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mPos = (event.getValue());
  updateMatrix();
}

void
av::vrpn::TrackerInformation::getQUATCB(const av::osg::SFQuat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mQuat;
}
void
av::vrpn::TrackerInformation::setQUATCB(const av::osg::SFQuat::SetValueEvent& event)
{
  mQuat = (event.getValue());
  updateMatrix();
}
