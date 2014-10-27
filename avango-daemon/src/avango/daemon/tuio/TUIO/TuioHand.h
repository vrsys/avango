/*
 TUIO C++ Library - part of the reacTIVision project
 http://reactivision.sourceforge.net/

 Part of avango for Guacamlole.

 Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>
               2014 Janek Bevendorff <janek.bevendorff@uni-weimar.de>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_TUIOHAND_H
#define INCLUDED_TUIOHAND_H

#include <math.h>
#include <array>
#include "TuioContainer.h"

namespace TUIO {

    /**
     * The TuioFinger class encapsulates /tuio/_Finger TUIO fingers.
     *
     * @author Janek Bevendorff
     * @version 1.4.01
     */
    class TuioHand: public TuioContainer {

    public:
        /**
          * The class of the  hand.
          */
        enum Class { UNKNOWN = 0, LEFT = 1, RIGHT = 2 };

        /**
         * This constructor takes a TuioTime argument and assigns it along with the provided
         * Session ID, Hand ID, X and Y coordinate to the newly created TuioFinger
         *
         * @param	ttime	the TuioTime to assign
         * @param	si	the Session ID  to assign
         * @param	hc	the hand class to assign (UNKNOWN, LEFT,RIGHT)
         * @param	f1	first finger session ID
         * @param	f2	second finger session ID
         * @param	f3	third finger session ID
         * @param	f4	fourth finger session ID
         * @param	f5	fifth finger session ID
         */
        TuioHand (TuioTime ttime,
                  long si, Class hc,
                  long f1, long f2, long f3, long f4, long f5) :
            TuioContainer(ttime,si,-1,-1),
            hand_id(si), hand_class(hc),
            fingers({f1, f2, f3, f4, f5})
        {
        }

        /**
         * This constructor takes a TuioTime argument and assigns it along with the provided
         * Session ID, Hand ID, X and Y coordinate to the newly created TuioFinger
         *
         * @param	si	the Session ID  to assign
         * @param	hc	the hand class to assign (UNKNOWN, LEFT,RIGHT)
         * @param	f1	first finger session ID
         * @param	f2	second finger session ID
         * @param	f3	third finger session ID
         * @param	f4	fourth finger session ID
         * @param	f5	fifth finger session ID
         */
        TuioHand (long si, Class hc,
                  long f1, long f2, long f3, long f4, long f5) :
            TuioContainer(si,-1,-1),
            hand_id(si), hand_class(hc),
            fingers({f1, f2, f3, f4, f5})
        {
        }

        /**
          * Get the class  of this hand.
          * @return the hand class
          */
        Class getHandClass() {
            return hand_class;
        }

        /**
          * Get an array of fingers for this hand.
          * @return the finger Session IDs
          */
        std::array<long, 5> getFingerIDs() {
            return fingers;
        }

        /**
         * Returns the Hand ID of this TuioHand.
         * @return	the Hand ID of this TuioHand
         */
        int getHandID() {
            return hand_id;
        }

    protected:
        /**
         * The individual finger ID number that is assigned to each TuioFinger.
         */
        int hand_id;
        Class hand_class;
        std::array<long, 5> fingers;

    };
}
#endif
