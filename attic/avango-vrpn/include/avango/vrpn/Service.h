#if !defined(AVANGO_SERVICE_H)
#define AVANGO_SERVICE_H

#include "Device.h"

#include <avango/FieldContainer.h>

#include "windows_specific_vrpn.h"

namespace av
{
  namespace vrpn
  {

    class AV_VRPN_DLL Service : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      Service();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Service();

    public:

      MFDevice Devices;

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    };

    typedef SingleField<Link<Service> > SFService;
    typedef MultiField<Link<Service> > MFService;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_VRPN_DLL SingleField<Link<av::vrpn::Service> >;
  template class AV_VRPN_DLL MultiField<Link<av::vrpn::Service> >;
#endif

}

#endif
