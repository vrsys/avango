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

namespace TUIO
{
/**
 * The TuioFinger class encapsulates /tuio/_Finger TUIO fingers.
 *
 * @author Janek Bevendorff
 * @version 1.4.01
 */
class TuioHand : public TuioContainer
{
  public:
    /**
     * The class of the  hand.
     */
    enum Class
    {
        UNKNOWN = 0,
        LEFT = 1,
        RIGHT = 2
    };

    using FingerArray = std::array<long, 5>;

    /**
     * This constructor takes a TuioTime argument and assigns it along with the provided
     * Session ID, Hand ID, X and Y coordinate to the newly created TuioFinger
     *
     * @param	ttime	the TuioTime to assign
     * @param	si	the Session ID  to assign
     * @param   xp  the X coordinate to assign
     * @param   yp  the Y coordinate to assign
     * @param	f1	first finger session ID
     * @param	f2	second finger session ID
     * @param	f3	third finger session ID
     * @param	f4	fourth finger session ID
     * @param	f5	fifth finger session ID
     * @param	hc	the hand class to assign (UNKNOWN, LEFT,RIGHT)
     * @param   xe  the bounding ellipse center X coordinate
     * @param   ye  the bounding ellipse center Y coordinate
     * @param   mi  the bounding ellipse minor axis
     * @param   ma  the bounding ellipse major axis
     * @param   in  the bounding ellipse inclination
     */
    TuioHand(TuioTime ttime, long si, float xp, float yp, long f1, long f2, long f3, long f4, long f5, Class hc, float xe, float ye, float mi, float ma, float in)
        : TuioContainer(ttime, si, xp, yp), hand_id(si), x_pos(xp), y_pos(yp), fingers({f1, f2, f3, f4, f5}), hand_class(hc), ellipse_centerX(xe), ellipse_centerY(ye), ellipse_minor_axis(mi),
          ellipse_major_axis(ma), ellipse_inclination(in)
    {
    }

    /**
     * This constructor takes a TuioTime argument and assigns it along with the provided
     * Session ID, Hand ID, X and Y coordinate to the newly created TuioFinger
     *
     * @param	si	the Session ID  to assign
     * @param   xp  the X coordinate to assign
     * @param   yp  the Y coordinate to assign
     * @param	f1	first finger session ID
     * @param	f2	second finger session ID
     * @param	f3	third finger session ID
     * @param	f4	fourth finger session ID
     * @param	f5	fifth finger session ID
     * @param	hc	the hand class to assign (UNKNOWN, LEFT,RIGHT)
     * @param   xe  the bounding ellipse center X coordinate
     * @param   ye  the bounding ellipse center Y coordinate
     * @param   mi  the bounding ellipse minor axis
     * @param   ma  the bounding ellipse major axis
     * @param   in  the bounding ellipse inclination
     */
    TuioHand(long si, float xp, float yp, long f1, long f2, long f3, long f4, long f5, Class hc, float xe, float ye, float mi, float ma, float in)
        : TuioContainer(si, xp, yp), hand_id(si), x_pos(xp), y_pos(yp), fingers({f1, f2, f3, f4, f5}), hand_class(hc), ellipse_centerX(xe), ellipse_centerY(ye), ellipse_minor_axis(mi),
          ellipse_major_axis(ma), ellipse_inclination(in)
    {
    }

    /**
     * Update finger IDs of the hand.
     *
     * @param xp   the X coordinate to assign
     * @param yp   the Y coordinate to assign
     * @param f1   Session ID of the first finger
     * @param f2   Session ID of the second finger
     * @param f3   Session ID of the third finger
     * @param f4   Session ID of the fourth finge
     * @param f5   Session ID of the fifth finger
     * @param hc   the hand class to assign (UNKNOWN, LEFT,RIGHT)
     * @param xe   the bounding ellipse center X coordinate
     * @param ye   the bounding ellipse center Y coordinate
     * @param mi   the bounding ellipse minor axis
     * @param ma   the bounding ellipse major axis
     * @param in   the bounding ellipse inclination
     */
    void update(float xp, float yp, long f1, long f2, long f3, long f4, long f5, Class hc, float xe, float ye, float mi, float ma, float in)
    {
        x_pos = xp;
        y_pos = yp;
        fingers[0] = f1;
        fingers[1] = f2;
        fingers[2] = f3;
        fingers[3] = f4;
        fingers[4] = f5;
        hand_class = hc;
        ellipse_centerX = xe;
        ellipse_centerY = ye;
        ellipse_minor_axis = mi;
        ellipse_major_axis = ma;
        ellipse_inclination = in;
    }

    /**
     * Get the class of this hand.
     * @return the hand class
     */
    Class getHandClass() { return hand_class; }

    /**
     * Get a std::array of finger IDs for this hand.
     * @return the finger Session IDs
     */
    FingerArray getFingerIDs() { return fingers; }

    /**
     * Returns the Hand ID of this TuioHand.
     * @return	the Hand ID of this TuioHand
     */
    int getHandID() { return hand_id; }

    /**
     * Returns the X position of this TuioHand.
     * @return the X position of this TuioHand
     */
    float getXPos() const { return x_pos; }

    /**
     * Returns the Y position of this TuioHand.
     * @return the Y position of this TuioHand
     */
    float getYPos() const { return y_pos; }

    /**
     * Return the hand ellipse center X ccordinate.
     * @return the hand ellipse center X ccordinate
     */
    float getEllipseX() { return ellipse_centerX; }

    /**
     * Return the hand ellipse center Y ccordinate.
     * @return the hand ellipse center Y ccordinate
     */
    float getEllipseY() { return ellipse_centerY; }

    /**
     * Return the hand ellipse center major axis.
     * @return the hand ellipse center major axis
     */
    float getEllipseMajor() { return ellipse_major_axis; }

    /**
     * Return the hand ellipse center minor axis.
     * @return the hand ellipse center minor axis
     */
    float getEllipseMinor() { return ellipse_minor_axis; }

    /**
     * Return the hand ellipse inclination.
     * @return the hand ellipse inclination
     */
    float getEllipseInclination() { return ellipse_inclination; }

  protected:
    /**
     * The individual hand ID number that is assigned to each TuioHand.
     */
    int hand_id;
    float x_pos;
    float y_pos;
    FingerArray fingers;
    Class hand_class;

    float ellipse_centerX;
    float ellipse_centerY;
    float ellipse_minor_axis;
    float ellipse_major_axis;
    float ellipse_inclination;
};
} // namespace TUIO
#endif
