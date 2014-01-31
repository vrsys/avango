// $Header$
// 
// Support for publishing IOR's.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#pragma implementation
#include "avango/ensemble/maestro/Maestro_Types.h"
#include "avango/ensemble/maestro/Maestro_ETC.h"

#include <fstream>
#include <strstream>

/******************************* ETC ********************************/

void 
Maestro_Etc::install(Maestro_String &name,
		     Maestro_CORBA_String &data) 
{
  Maestro_CORBA_OctetSequence seq;
  seq.size(data.size);
  memcpy(seq.data(), data.s, data.size);
  install(name, seq);
}

void 
Maestro_Etc::install(Maestro_String &name,
		     Maestro_CORBA_OctetSequence &data) 
{
  char *etc;
  Maestro_ErrorHandler err;
  
  if ((etc = getenv("MAESTRO_ETC")) == NULL)
    err.panic("Maestro_Etc:  environment variable MAESTRO_ETC must be set");
  
  unsigned size = name.size + strlen(etc) + 1;
  char *p = new char[size];
  memset(p, 0, size);
  std::ostrstream fname(p, size);
  fname << etc << "/" << name.s;
  
  std::ofstream ofs(p);
  if (!ofs) err.panic("Maestro_Etc:install: could not open %s", p);
  ofs << data.data();
  ofs.close();
}

void 
Maestro_Etc::lookup(Maestro_String &name, 
		    Maestro_CORBA_String &data,
		    unsigned long maxSize)
{
  Maestro_CORBA_OctetSequence seq;
  lookup(name, seq, maxSize);
  data = (char*) seq.data();
}

void 
Maestro_Etc::lookup(Maestro_String &name, 
		    Maestro_CORBA_OctetSequence &data,
		    unsigned long maxSize)
{
  char *etc;
  Maestro_ErrorHandler err;
  
  if ((etc = getenv("MAESTRO_ETC")) == NULL)
    err.panic("Maestro_Etc:  environment variable MAESTRO_ETC must be set");
  
  unsigned size = name.size + strlen(etc) + 1;
  char *p = new char[size];
  memset(p, 0, size);
  std::ostrstream fname(p, size);
  fname << etc << "/" << name.s;
  
  char *buf = new char[maxSize];
  std::ifstream ifs(p);
  if (!ifs) err.panic("Maestro_Etc:lookup: could not open %s", p);
  ifs.getline(buf, maxSize); 
  ifs.close();

  data.size(strlen(buf));
  memcpy(data.data(), buf, strlen(buf));
  delete [] buf;
}

Maestro_Etc Maestro_DefaultEtc;
