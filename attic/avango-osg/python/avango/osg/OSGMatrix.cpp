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

#include <boost/python.hpp>
#include <boost/format.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/Fields.h>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>

using namespace boost::python;
using namespace av::python;

namespace
{
  osg::Matrix::value_type MatrixGetElement(const osg::Matrix& m, int row, int col)
  {
    return m(row, col);
  }

  void MatrixSetElement(osg::Matrix& m, int row, int col, osg::Matrix::value_type value)
  {
    m(row, col) = value;
  }

  osg::Vec3 MatrixGetTranslate(const osg::Matrix& m)
  {
    return m.getTrans();
  }

  osg::Vec3 MatrixGetScale(const osg::Matrix& m)
  {
    return m.getScale();
  }

  osg::Matrix MatrixMakeScale(osg::Matrix::value_type v)
  {
    return osg::Matrix::scale(v, v, v);
  }

  std::string MatrixString(const osg::Matrix& m)
  {
    std::stringstream ss;
    ss << boost::format("<Matrix\n");
    ss << boost::format(" a11 a12 a13 a14: %11.6f %11.6f %11.6f %11.6f \n") % m(0,0) % m(0,1) % m(0,2) % m(0,3);
    ss << boost::format(" a21 a22 a23 a24: %11.6f %11.6f %11.6f %11.6f \n") % m(1,0) % m(1,1) % m(1,2) % m(1,3);
    ss << boost::format(" a31 a32 a33 a34: %11.6f %11.6f %11.6f %11.6f \n") % m(2,0) % m(2,1) % m(2,2) % m(2,3);
    ss << boost::format(" a41 a42 a43 a44: %11.6f %11.6f %11.6f %11.6f \n") % m(3,0) % m(3,1) % m(3,2) % m(3,3);
    ss << boost::format(">");
    return ss.str();
  }

  //taken from delta3D
  osg::Matrix HprToMatrix(const osg::Vec3& hpr )
  {
     //implementation converted from plib's sg.cxx
     //PLIB - A Suite of Portable Game Libraries
     //Copyright (C) 1998,2002  Steve Baker
     //For further information visit http://plib.sourceforge.net

      osg::Matrix rotation;

     double ch, sh, cp, sp, cr, sr, srsp, crsp, srcp ;

     // this can't be smart for both 32 and 64 bit types.
     ///\todo find a preprocessor way to assign this constant different for the different precision types.
     const osg::Vec3::value_type magic_epsilon = 0.00001;

     if ( osg::equivalent(hpr[0],(osg::Vec3::value_type)0.0,magic_epsilon) )
     {
        ch = 1.0 ;
        sh = 0.0 ;
     }
     else
     {
        sh = sinf(hpr[0]);
        ch = cosf(hpr[0]);
     }

     if ( osg::equivalent(hpr[1],(osg::Vec3::value_type)0.0,magic_epsilon) )
     {
        cp = 1.0 ;
        sp = 0.0 ;
     }
     else
     {
        sp = sinf(hpr[1]);
        cp = cosf(hpr[1]);
     }

     if ( osg::equivalent(hpr[2],(osg::Vec3::value_type)0.0,magic_epsilon) )
     {
        cr   = 1.0 ;
        sr   = 0.0 ;
        srsp = 0.0 ;
        srcp = 0.0 ;
        crsp = sp ;
     }
     else
     {
        sr   = sinf(hpr[2]);
        cr   = cosf(hpr[2]);
        srsp = sr * sp ;
        crsp = cr * sp ;
        srcp = sr * cp ;
     }

     rotation(0, 0) = (  ch * cr - sh * srsp ) ;
     rotation(1, 0) = ( -sh * cp ) ;
     rotation(2, 0) = (  sr * ch + sh * crsp ) ;

     rotation(0, 1) = ( cr * sh + srsp * ch ) ;
     rotation(1, 1) = ( ch * cp ) ;
     rotation(2, 1) = ( sr * sh - crsp * ch ) ;

     rotation(0, 2) = ( -srcp ) ;
     rotation(1, 2) = (  sp ) ;
     rotation(2, 2) = (  cr * cp ) ;

     rotation(3, 0) =  0.0;  // x trans
     rotation(3, 1) =  0.0;  // y trans
     rotation(3, 2) =  0.0;  // z trans

     rotation(0, 3) =  0.0;
     rotation(1, 3) =  0.0;
     rotation(2, 3) =  0.0;
     rotation(3, 3) =  1.0;

     return rotation;
  }

  float ClampUnity( float x )
  {
     if ( x >  1.0f ) return  1.0f;
     if ( x < -1.0f ) return -1.0f;
     return x ;
  }

  //taken from delta3D
  osg::Vec3 MatrixToHpr( osg::Matrix& rotation )
  {
     //implementation converted from plib's sg.cxx
     //PLIB - A Suite of Portable Game Libraries
     //Copyright (C) 1998,2002  Steve Baker
     //For further information visit http://plib.sourceforge.net

     osg::Vec3 hpr;

     osg::Matrix mat;

     osg::Vec3 col1(rotation(0, 0), rotation(0, 1), rotation(0, 2));
     double s = col1.length();

     const double magic_epsilon = 0.00001;
     if ( s <= magic_epsilon )
     {
        hpr.set(0.0f, 0.0f, 0.0f);
        return osg::Vec3();
     }


     double oneOverS = 1.0f / s;
     for( int i = 0; i < 3; i++ )
        for( int j = 0; j < 3; j++ )
           mat(i, j) = rotation(i, j) * oneOverS;


     double sin_pitch = ClampUnity(mat(1, 2));
     double pitch = asin(sin_pitch);
     hpr[1] = pitch;

     double cp = cos(pitch);

     if ( cp > -magic_epsilon && cp < magic_epsilon )
     {
        double cr = ClampUnity(-mat(2,1));
        double sr = ClampUnity(mat(0,1));

        hpr[0] = 0.0f;
        hpr[2] = atan2(sr,cr);
     }
     else
     {
        double one_over_cp = 1.0 / cp ;
        double sr = ClampUnity(-mat(0,2) * one_over_cp);
        double cr = ClampUnity(mat(2,2) * one_over_cp);
        double sh = ClampUnity(-mat(1,0) * one_over_cp);
        double ch = ClampUnity(mat(1,1) * one_over_cp);

        if ( ( osg::equivalent(sh,0.0,magic_epsilon) && osg::equivalent(ch,0.0,magic_epsilon) ) ||
             ( osg::equivalent(sr,0.0,magic_epsilon) && osg::equivalent(cr,0.0,magic_epsilon) ) )
        {
           cr = ClampUnity(-mat(2,1));
           sr = ClampUnity(mat(0,1));;

           hpr[0] = 0.0f;
        }
        else
        {
          hpr[0] = atan2(sh, ch);
        }

        hpr[2] = atan2(sr, cr);
     }
     return hpr;
  }

}

void init_OSGMatrix(void)
 {
  // introducing member function pointers for overloaded function of osg::Matrix::MakeScale
  void (osg::Matrix::*mscale1)(const osg::Vec3&) = &osg::Matrix::makeScale;
  void (osg::Matrix::*mscale2)(osg::Matrix::value_type, osg::Matrix::value_type, osg::Matrix::value_type) = &osg::Matrix::makeScale;

  // introducing member function pointers for overloaded function of osg::Matrix::MakeTranslate
  void (osg::Matrix::*mtrans1)(const osg::Vec3&) = &osg::Matrix::makeTranslate;
  void (osg::Matrix::*mtrans2)(osg::Matrix::value_type, osg::Matrix::value_type, osg::Matrix::value_type) = &osg::Matrix::makeTranslate;

  // introducing member function pointers for overloaded function of osg::Matrix::MakeRotate
  void (osg::Matrix::*mrot1)(const osg::Vec3&, const osg::Vec3&) = &osg::Matrix::makeRotate;
  void (osg::Matrix::*mrot2)(osg::Matrix::value_type, const osg::Vec3&) = &osg::Matrix::makeRotate;
  void (osg::Matrix::*mrot3)(osg::Matrix::value_type, osg::Matrix::value_type,
        osg::Matrix::value_type, osg::Matrix::value_type) = &osg::Matrix::makeRotate;
  void (osg::Matrix::*mrot4)(const osg::Quat&) = &osg::Matrix::makeRotate;
  void (osg::Matrix::*mrot5)(osg::Matrix::value_type, const osg::Vec3&,
                             osg::Matrix::value_type, const osg::Vec3&,
                             osg::Matrix::value_type, const osg::Vec3&) = &osg::Matrix::makeRotate;

  // introducing member function pointers for overloaded function of osg::Matrix::makeOrtho2D
  void (osg::Matrix::*makeOrtho)(double,double,double,double,double,double) = &osg::Matrix::makeOrtho;
  void (osg::Matrix::*makeOrtho2D)(double,double,double,double) = &osg::Matrix::makeOrtho2D;

  // introducing member function pointers for overloaded function of osg::Matrix::setTranslate
  void (osg::Matrix::*setTrans1)(const osg::Vec3&) = &osg::Matrix::setTrans;
  void (osg::Matrix::*setTrans2)(osg::Matrix::value_type,osg::Matrix::value_type,osg::Matrix::value_type) = &osg::Matrix::setTrans;

  // introducing member function pointers for static osg::Matrix::translate functions
  osg::Matrix (*makeTrans1)(const osg::Vec3&) = &osg::Matrix::translate;
  osg::Matrix (*makeTrans2)(osg::Matrix::value_type,osg::Matrix::value_type,osg::Matrix::value_type) = &osg::Matrix::translate;

  // introducing member function pointers for static osg::Matrix::rotate functions
  osg::Matrix (*makeRot1)(const osg::Vec3&, const osg::Vec3&) = &osg::Matrix::rotate;
  osg::Matrix (*makeRot2)(osg::Matrix::value_type, const osg::Vec3&) = &osg::Matrix::rotate;
  osg::Matrix (*makeRot3)(osg::Matrix::value_type, osg::Matrix::value_type,
                          osg::Matrix::value_type, osg::Matrix::value_type) = &osg::Matrix::rotate;
  osg::Matrix (*makeRot4)(const osg::Quat&) = &osg::Matrix::rotate;
  osg::Matrix (*makeRot5)(osg::Matrix::value_type, const osg::Vec3&,
                          osg::Matrix::value_type, const osg::Vec3&,
                          osg::Matrix::value_type, const osg::Vec3&) = &osg::Matrix::rotate;

  // introducing member function pointers for static osg::Matrix::scale functions
  osg::Matrix (*makeScale1)(const osg::Vec3&) = &osg::Matrix::scale;
  osg::Matrix (*makeScale2)(osg::Matrix::value_type,osg::Matrix::value_type,osg::Matrix::value_type) = &osg::Matrix::scale;

  // introducing member function pointers for static osg::Matrix::transform3x3 functions
  osg::Vec3 (*transform1)(const osg::Vec3&, const osg::Matrix&) = &osg::Matrix::transform3x3;
  osg::Vec3 (*transform2)(const osg::Matrix&, const osg::Vec3&) = &osg::Matrix::transform3x3;

  // introducing member function pointers for static osg::Matrix::lookAt
  osg::Matrix (*makeLookAt)(const osg::Vec3&, const osg::Vec3&, const osg::Vec3&) = &osg::Matrix::lookAt;

  // introducing member function pointers for static osg::Matrix::perspective
  //osg::Matrix (*makePerspective)(double, double, double, double) = &osg::Matrix::perspective;


  // wrapping osg::Matrix functionality
  class_<osg::Matrix>("Matrix")
    .def(init<osg::Matrix>())
    .def(self == other<osg::Matrix>())
    .def(self != other<osg::Matrix>())
    .def(self *= other<osg::Matrix>())
    .def(self * other<osg::Matrix>())
    .def(self * other<osg::Vec3>())
    .def(self * other<osg::Vec4>())
    .def(other<osg::Vec3>() * self)
    .def(other<osg::Vec4>() * self)
    .def("get_element", MatrixGetElement)
    .def("set_element", MatrixSetElement)
    .def("is_identity", &osg::Matrix::isIdentity)
    .def("make_identity", &osg::Matrix::makeIdentity)
    .def("make_scale", mscale1)
    .def("make_scale", mscale2)
    .def("make_translate", mtrans1)
    .def("make_translate", mtrans2)
    .def("make_rotate", mrot1)
    .def("make_rotate", mrot2)
    .def("make_rotate", mrot3)
    .def("make_rotate", mrot4)
    .def("make_rotate", mrot5)
    .def("make_ortho", makeOrtho)
    .def("make_ortho_2d", makeOrtho2D)
    .def("invert", &osg::Matrix::invert)
    .def("mult", &osg::Matrix::mult)
    .def("valid", &osg::Matrix::valid)
    .def("is_nan", &osg::Matrix::isNaN)
    .def("get_scale", MatrixGetScale)
    .def("get_translate", MatrixGetTranslate)
    .def("get_rotate", &osg::Matrix::getRotate)
    .def("set_rotate", &osg::Matrix::setRotate)
    .def("set_translate", setTrans1)
    .def("set_translate", setTrans2)
    .def("__repr__", MatrixString)
    ;

  // defining methods for creating translation, rotation, scale matrices (using static member functions of osg::Matrix)
  def("make_ident_mat", osg::Matrix::identity);
  def("make_trans_mat", makeTrans1);
  def("make_trans_mat", makeTrans2);
  def("make_rot_mat", makeRot1);
  def("make_rot_mat", makeRot2);
  def("make_rot_mat", makeRot3);
  def("make_rot_mat", makeRot4);
  def("make_rot_mat", makeRot5);
  def("make_scale_mat", makeScale1);
  def("make_scale_mat", makeScale2);
  def("make_scale_mat", MatrixMakeScale);
  def("make_inverse_mat", osg::Matrix::inverse);
  def("transform3x3", transform1);
  def("transform3x3", transform2);
  def("make_lookat_mat", makeLookAt);
  def("make_perspective_mat", ::osg::Matrix::perspective);
  def("make_frustum_mat", ::osg::Matrix::frustum);
  def("make_ortho_mat", ::osg::Matrix::ortho);
  def("hpr_to_matrix", HprToMatrix);
  def("matrix_to_hpr", MatrixToHpr);





  // register as a field
  register_field<av::osg::SFMatrix>("SFMatrix");
  register_multifield<av::osg::MFMatrix>("MFMatrix");

 }
