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

#ifndef INCLUDED_TUIOFINGER_H
#define INCLUDED_TUIOFINGER_H

#include <cmath>
#include "TuioContainer.h"

namespace TUIO
{
/**
 * The TuioFinger class encapsulates /tuio/_Finger TUIO fingers.
 *
 * @author Janek Bevendorff
 * @version 1.4.01
 */
class TuioFinger : public TuioContainer
{
  public:
    /**
     * The class of the Finger.
     */
    enum Class
    {
        UNKNOWN = 0,
        THUMB = 1,
        INDEX = 2,
        MIDDLE = 3,
        RING = 4,
        LITTLE = 5
    };

    /**
     * This constructor takes a TuioTime argument and assigns it along with the provided
     * Session ID, Finger ID, X and Y coordinate to the newly created TuioFinger
     *
     * @param ttime the TuioTime to assign
     * @param si the Session ID  to assign
     * @param xp the X coordinate to assign
     * @param yp the Y coordinate to assign
     * @param h_id the hand id to assign
     * @param f_class the fing class to assign (UNKNOWN, THUMB, INDEX, MIDDLE, RING, LITTLE)
     */
    TuioFinger(TuioTime ttime, long si, float xp, float yp, int h_id, Class f_class) : TuioContainer(ttime, si, xp, yp), finger_id(si), x_pos(xp), y_pos(yp), hand_id(h_id), finger_class(f_class) {}

    /**
     * This constructor takes the provided Session ID, Cursor ID, X and Y coordinate
     * and assigs these values to the newly created TuioCursor.
     *
     * @param si the Session ID  to assign
     * @param xp the X coordinate to assign
     * @param yp the Y coordinate to assign
     * @param h_id the hand id to assign
     * @param f_class the fing class to assign (UNKNOWN, THUMB, INDEX, MIDDLE, RING, LITTLE)
     */
    TuioFinger(long si, float xp, float yp, int h_id, Class f_class) : TuioContainer(si, xp, yp), finger_id(si), x_pos(xp), y_pos(yp), hand_id(h_id), finger_class(f_class) {}

    /**
     * Update finger and corresponding 2DCursors.
     *
     * @param xp the X coordinate to assign
     * @param yp the Y coordinate to assign
     * @param h_id the hand id to assign
     * @param f_class the fing class to assign (UNKNOWN, THUMB, INDEX, MIDDLE, RING, LITTLE)
     */
    void update(float xp, float yp, int h_id, Class f_class)
    {
        TuioContainer::update(TuioTime::getSessionTime(), xp, yp);
        x_pos = xp;
        y_pos = yp;
        hand_id = h_id;
        finger_class = f_class;
    }

    /**
     * Returns the Finger ID of this TuioFinger.
     * @return	the Finger ID of this TuioFinger
     */
    int getFingerID() { return finger_id; }

    /**
     * Returns the X position of this TuioFinger.
     * @return	the X position of this TuioFinger
     */
    float getXpos() const { return x_pos; }

    /**
     * Returns the Y position of this TuioFinger.
     * @return	the Y position of this TuioFinger
     */
    float getYpos() const { return y_pos; }

    /**
     * Returns the Class of this TuioFinger.
     * @return	the Hand ID of this TuioFinger
     */
    Class getFinger_class() const { return finger_class; }

    /**
     * Returns the Hand ID of this TuioFinger.
     * @return	the Hand ID of this TuioFinger
     */
    int getHand_id() const { return hand_id; }

  protected:
    /**
     * The individual finger ID number that is assigned to each TuioFinger.
     */
    int finger_id;
    float x_pos;
    float y_pos;
    int hand_id;
    Class finger_class;
};
} // namespace TUIO
#endif
