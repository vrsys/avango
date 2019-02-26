#ifndef _FINGERLISTENER_H
#define _FINGERLISTENER_H

#include "FingerInfo.h"

class FingerListener
{
  public:
    virtual ~FingerListener(){};
    virtual void fingerAdded(FingerInfo* pFingerInfo) = 0;
    virtual void fingerMoved(FingerInfo* pFingerInfo) = 0;
    virtual void fingerRemoved(FingerInfo* pFingerInfo) = 0;
};

class UserListener
{
  public:
    virtual ~UserListener(){};
    virtual void userAdded(UserInfo* pUserInfo) = 0;
    virtual void userMoved(UserInfo* pUserInfo) = 0;
    virtual void userRemoved(UserInfo* pUserInfo) = 0;
};

#endif //_FINGERLISTENER_H
