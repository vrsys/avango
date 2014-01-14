#if !defined(AVANGO_BUTTON_H)
#define AVANGO_BUTTON_H

#include <avango/FieldContainer.h>

#include "windows_specific_vrpn.h"

namespace av
{
  namespace vrpn
  {

    class AV_VRPN_DLL Button : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      Button();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Button();

    public:

      //Number of the button (Given by the VRPN numbering)
      SFInt Number;
      //Trigger state of the button
      SFBool State;

    };

    typedef SingleField<Link<Button> > SFButton;
    typedef MultiField<Link<Button> > MFButton;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_VRPN_DLL SingleField< Link<av::vrpn::Button> >;
  template class AV_VRPN_DLL MultiField< Link<av::vrpn::Button> >;
#endif

}

#endif
