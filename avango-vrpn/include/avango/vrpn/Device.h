#if !defined(AVANGO_DEVICE_H)
#define AVANGO_DEVICE_H

#include "Button.h"
#include "TrackerInformation.h"

#include <avango/FieldContainer.h>
#include <avango/osg/Fields.h>

#include "windows_specific_vrpn.h"

#include <vrpn_Analog.h>
#include <vrpn_Button.h>
#include <vrpn_Tracker.h>

#include <boost/shared_ptr.hpp>

namespace {

}

namespace av
{
  namespace vrpn
  {

    class AV_VRPN_DLL Device : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      Device();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Device();

    public:

      SFString VRPNID;
      MFButton Buttons;
      MFFloat Channels;
      MFTrackerInformation TrackerInfo;


//      /* virtual */ void fieldHasChanged(const av::Field& field);
//      /* virtual */ void evaluate();

      void triggerVRPNMainLoop();

      void handleAnalog(const vrpn_ANALOGCB a);
      void handleButton(const vrpn_BUTTONCB b);
      void handleTracker(const vrpn_TRACKERCB t);

    protected:

      bool vrpnConnected;

      std::string mVRPNID;
      boost::shared_ptr<vrpn_Analog_Remote> mVRPNAnalog;
      boost::shared_ptr<vrpn_Button_Remote> mVRPNButton;
      boost::shared_ptr<vrpn_Tracker_Remote> mVRPNTracker;

      virtual void openVRPNConnections();
      virtual void closeVRPNConnections();

      virtual void getVRPNIDCB(const av::SFString::GetValueEvent& event);
      virtual void setVRPNIDCB(const av::SFString::SetValueEvent& event);

      virtual void triggerVRPNMainLoopTemplateFunc(){};
      virtual void openVRPNConnectionsTemplateFunc(){};
      virtual void closeVRPNConnectionsTemplateFunc(){};
      virtual void handleAnalogTemplFunc(const vrpn_ANALOGCB a){};
      virtual void handleButtonTemplFunc(const vrpn_BUTTONCB b){};
      virtual void handleTrackerTemplFunc(const vrpn_TRACKERCB t){};

    private:



    };

    typedef SingleField<Link<Device> > SFDevice;
    typedef MultiField<Link<Device> > MFDevice;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_VRPN_DLL SingleField<Link<av:vrpn::Device> >;
  template class AV_VRPN_DLL MultiField<Link<av:vrpn::Device> >;
#endif

}

#endif
