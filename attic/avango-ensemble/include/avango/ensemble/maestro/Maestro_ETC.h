// $Header$
//
// Support for publishing IOR's.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#ifndef __MAESTRO_ETC_H__
#define __MAESTRO_ETC_H__

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_CORBA.h>
#include <avango/ensemble/maestro/Maestro_GIOP.h>

/******************************* ETC ********************************/

const char* const MAESTRO_ETC = "MAESTRO_ETC";

// Maestro_Etc:  install/lookup text strings/octet sequences in the Maestro
// etc directory.  Data strings/sequences must be alphanumerical.
class Maestro_Etc : virtual public Maestro_CORBA_Serializable
{
  public:
    virtual void install(Maestro_String& name, Maestro_CORBA_String& data);

    virtual void install(Maestro_String& name, Maestro_CORBA_OctetSequence& data);

    virtual void lookup(Maestro_String& name, Maestro_CORBA_String& data, unsigned long maxSize = 1024);

    virtual void lookup(Maestro_String& name, Maestro_CORBA_OctetSequence& data, unsigned long maxSize = 1024);
};

extern Maestro_Etc Maestro_DefaultEtc;

#endif // __MAESTRO_ETC_H__
