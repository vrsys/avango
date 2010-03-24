#ifndef _FINGERLISTENER_H
#define _FINGERLISTENER_H


#include "FingerInfo.h"


class FingerListener
{
public:
  virtual void fingerAdded(FingerInfo *pFingerInfo) = 0;
  virtual void fingerMoved(FingerInfo *pFingerInfo) = 0;
  virtual void fingerRemoved(FingerInfo *pFingerInfo) = 0;
};


#endif //_FINGERLISTENER_H
