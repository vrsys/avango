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

// $Id

#if !defined(AV_APPLICATION_H)
#define AV_APPLICATION_H

#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>

#include <avango/Singleton.h>

#include "windows_specific.h"

/**
 * \file
 * \ingroup av
 */

namespace av
{

  /**
   * The Application class initializes the Avango runtime system and
   * encapsulates the Avango main loop.
   *
   * There is exactly one Application object per application which can be
   * accessed through the getInstance() function.
   *
   * \ingroup av
   */
  //typedef Singleton<Application>
  class AV_DLL Application /* : public Singleton<Application>*/
  {
    //friend class Singleton<Application>;

    /**
     * Constructor made private to prevent multiple instantiation.
     */
  public:

    Application();
    /**
     * Destructor made private to prevent deletion.
     */
    ~Application();

    /**
     * Triggers Avango object evaluation once.
     * TODO: Why is the pre and post evaluation signal needed? Does it make sense? As far as we know, it is only used in the NetMatrixTransform node.
     */
    void evaluate();

    /**
     * Returns true if the main loop is running.
     */
    bool running();

    /**
     * Starts the main loop
     * Returns true if the main loop has not been running before.
     */
    bool start();

    /**
     * Starts the main loop in a background thread.
     *
     * Note that Avango is not thread-safe in general.
     * Nothing is done for synchronization at this point.
     * Use this only if you know what you are doing.
     */
    void startInBackground();

    /**
     * Stops the mainloop gracefully after current step.
     * Returns true if the main loop has not been stopped already.
     */
    bool stop();

    /**
     * Exits the application gracefully.
     */
    void exit(bool realExit = true);

    using VoidCallbackSignal = boost::signals2::signal<void ()>;
    using VoidCallback = VoidCallbackSignal::slot_type;
    using CallbackHandle = boost::signals2::connection;

    /**
     * add a callback which is called before the evaluation of Avango FieldContainers
     */
    CallbackHandle addPreEvaluationContainerCallback(const VoidCallback& callback);

    /**
     * add a callback which is called after the evaluation of Avango FieldContainers
     */
    CallbackHandle addPostEvaluationContainerCallback(const VoidCallback& callback);

    /**
     * add a render callback which is called after each
     * evaluation of Avango objects
     */
    CallbackHandle addRenderCallback(const VoidCallback& callback);

    /**
     * remove a callback
     */
    static void removeCallback(const CallbackHandle& handle);

  private:

    void disconnectAllFields();
    void cleanExit();

    // Some checks have to be done to allow exit and stop calls from inside evaluate
    // These flags are used for this.

    bool mCleanExit;
    bool mKeepRunning;
    bool mRealExit;
    bool mRunning;

    // why boost::thread::thread?    boost::shared_ptr<boost::thread::thread> mThread;
    boost::shared_ptr<boost::thread> mThread;
    VoidCallbackSignal mPreEvaluateContainerCallbackSignal;
    VoidCallbackSignal mPostEvaluateContainerCallbackSignal;
    VoidCallbackSignal mRenderCallbackSignal;

  };

  using ApplicationInstance = Singleton<Application>;

}

#endif // #if !defined(AV_APPLICATION_H)

// $Id
