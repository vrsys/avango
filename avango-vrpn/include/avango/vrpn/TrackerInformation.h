#if !defined(AVANGO_TRACKERINFORMATION_H)
#define AVANGO_TRACKERINFORMATION_H

#include <avango/FieldContainer.h>

#include <avango/osg/Types.h>
#include <avango/osg/Quad.h>

#include "windows_specific_vrpn.h"

namespace av
{
  namespace vrpn
  {

    class AV_VRPN_DLL TrackerInformation : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      TrackerInformation();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TrackerInformation();

    public:

      //Number of the Sensor
      SFLong Number;
      //Position
      ::av::osg::SFVec3 Pos;
      //Quat
      ::av::osg::SFQuat Quat;
      //Matrix (is calculated by using the pos vector and the quat. Convenience.)
      ::av::osg::SFMatrix Matrix;


    private:

      ::osg::Vec3 mPos;
      ::osg::Quat mQuat;

      void getPOSCB(const av::osg::SFVec3::GetValueEvent& event);
      void setPOSCB(const av::osg::SFVec3::SetValueEvent& event);

      void getQUATCB(const av::osg::SFQuat::GetValueEvent& event);
      void setQUATCB(const av::osg::SFQuat::SetValueEvent& event);

      void updateMatrix();

    };

    typedef SingleField<Link<TrackerInformation> > SFTrackerInformation;
    typedef MultiField<Link<TrackerInformation> > MFTrackerInformation;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_VRPN_DLL SingleField<Link<av:vrpn::TrackerInformation> >;
  template class AV_VRPN_DLL MultiField<Link<av:vrpn::TrackerInformation> >;
#endif

}

#endif
