// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#ifndef AV_NETLOCK_H
#define AV_NETLOCK_H

#include <avango/FieldContainer.h>

namespace av
{
/**
 * This class implements a building block for locking over the distribution group.
 * The process requesting the network lock writes a unique id into the "Request" field.
 * If the same name shows up in the "Granted" field, this process owns the lock.
 * It can then release the lock by writing the same unique id into the "Release" field,
 * which resets the "Granted" field to the empty string.
 * Short: This is cooperative locking, you only have the lock when your id shows up in the "Granted" field.
 */
class AV_DLL NetLock : public FieldContainer
{
    AV_FC_DECLARE();

  public:
    NetLock();

  protected:
    /* virtual */ ~NetLock();

  public:
    /**
     * Request the lock. Write an id unique for this process in here, preferably the EID
     * (\see av::NetNode).
     */
    SFString Request;
    /**
     * Output field. This contains the id of the process that is holding the lock.
     */
    SFString Granted;
    /**
     * Release the Lock. You have to put the same id in here that you used for the request, and
     * your process has to have acquired the lock ( your id is in the "Granted" field )
     */
    SFString Release;

    void fieldHasChanged(const Field& field);
    void evaluate();

  private:
    bool mRelease;
    bool mRequest;
};
} // namespace av

#endif /*AV_NETLOCK_H*/
