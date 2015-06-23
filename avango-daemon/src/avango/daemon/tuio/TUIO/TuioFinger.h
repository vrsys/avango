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

namespace TUIO {

    /**
     * The TuioFinger class encapsulates /tuio/_Finger TUIO fingers.
     *
     * @author Janek Bevendorff
     * @version 1.4.01
     */
    class TuioFinger: public TuioContainer {

    public:
        /**
         * This constructor takes a TuioTime argument and assigns it along with the provided
         * Session ID, Finger ID, X and Y coordinate to the newly created TuioFinger
         *
         * @param ttime the TuioTime to assign
         * @param si the Session ID  to assign
         * @param xp the X coordinate to assign
         * @param yp the Y coordinate to assign
         * @param xv the X velocity to assign
         * @param yv the Y velocity to assign
         * @param xe the bounding ellipse center X coordinate
         * @param ye the bounding ellipse center Y coordinate
         * @param mi the bounding ellipse minor axis
         * @param ma the bounding ellipse major axis
         * @param in the bounding ellipse inclination
         */
        TuioFinger (TuioTime ttime,
                    long si,
                    float xp, float yp,
                    float xv, float yv,
                    float xe, float ye,
                    float mi, float ma,
                    float in) :
            TuioContainer(ttime,si,xp,yp),
            finger_id(si),
            x_speed(xv), y_speed(yv),
            x_ellipse_center(xe), y_ellipse_center(ye),
            ellipse_minor_axis(mi), ellipse_major_axis(ma),
            ellipse_inclination(in)
        {
        }

        /**
         * This constructor takes the provided Session ID, Cursor ID, X and Y coordinate
         * and assigs these values to the newly created TuioCursor.
         *
         * @param	si	the Session ID  to assign
         * @param	xp	the X coordinate to assign
         * @param	yp	the Y coordinate to assign
         * @param   xs  the X velocity to assign
         * @param   ys  the Y velocity to assign
         * @param   xe  the bounding ellipse center X coordinate
         * @param   ye  the bounding ellipse center Y coordinate
         * @param   mi  the bounding ellipse minor axis
         * @param   ma  the bounding ellipse major axis
         * @param   in  the bounding ellipse inclination
         */
        TuioFinger (long si,
                    float xp, float yp,
                    float xs, float ys,
                    float xe, float ye,
                    float mi, float ma,
                    float in) :
            TuioContainer(si,xp,yp),
            finger_id(si),
            x_speed(xs), y_speed(ys),
            x_ellipse_center(xe), y_ellipse_center(ye),
            ellipse_minor_axis(mi), ellipse_major_axis(ma),
            ellipse_inclination(in)
        {
        }

        /**
          * Update finger and corresponding 2DCursors.
          *
          * @param  xp	the X coordinate to assign
          * @param  yp	the Y coordinate to assign
          * @param  xs  the X velocity to assign
          * @param  ys  the Y velocity to assign
          * @param  xe  the bounding ellipse center X coordinate
          * @param  ye  the bounding ellipse center Y coordinate
          * @param  mi  the bounding ellipse minor axis
          * @param  ma  the bounding ellipse major axis
          * @param  in  the bounding ellipse inclination
          */
        void update(float xp, float yp,
                    float xs, float ys,
                    float xe, float ye,
                    float mi, float ma,
                    float in)
        {
            TuioContainer::update(TuioTime::getSessionTime(), xp, yp);
            x_speed = xs;
            y_speed = ys;
            x_ellipse_center = xe;
            y_ellipse_center = ye;
            ellipse_minor_axis = mi;
            ellipse_major_axis = ma;
            ellipse_inclination = in;
        }

        /**
         * Returns the Finger ID of this TuioFinger.
         * @return	the Finger ID of this TuioFinger
         */
        int getFingerID() {
            return finger_id;
        }

        /**
         * Returns the X velocity of this TuioFinger.
         * @return	the X velocity of this TuioFinger
         */
        float getXSpeed() {
            return x_speed;
        }

        /**
         * Returns the Y velocity of this TuioFinger.
         * @return	the Y velocity of this TuioFinger
         */
        float getYSpeed() {
            return y_speed;
        }

        /**
          * Return the finger ellipse center X ccordinate.
          * @return the finger ellipse center X ccordinate
          */
        float getEllipseX() {
            return x_ellipse_center;
        }

        /**
          * Return the finger ellipse center Y ccordinate.
          * @return the finger ellipse center Y ccordinate
          */
        float getEllipseY() {
            return y_ellipse_center;
        }

        /**
          * Return the finger ellipse center major axis.
          * @return the finger ellipse center major axis
          */
        float getEllipseMajor() {
            return ellipse_major_axis;
        }

        /**
          * Return the finger ellipse center minor axis.
          * @return the finger ellipse center minor axis
          */
        float getEllipseMinor() {
            return ellipse_minor_axis;
        }

        /**
          * Return the finger ellipse inclination.
          * @return the finger ellipse inclination
          */
        float getEllipseInclination() {
            return ellipse_inclination;
        }


    protected:
        /**
         * The individual finger ID number that is assigned to each TuioFinger.
         */
        int finger_id;
        float x_speed;
        float y_speed;
        float x_ellipse_center;
        float y_ellipse_center;
        float ellipse_minor_axis;
        float ellipse_major_axis;
        float ellipse_inclination;
    };
}
#endif
