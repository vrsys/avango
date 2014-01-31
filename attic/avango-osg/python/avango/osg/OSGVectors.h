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


#ifndef AV_PYTHON_OSG_VECTORS
#define AV_PYTHON_OSG_VECTORS

// helper functions to get and set x, y, z, w properties on Vec2, Vec3, Vec4, Quat
template <class T> float getX(T* self) { return self->_v[0]; }
template <class T> float getY(T* self) { return self->_v[1]; }
template <class T> float getZ(T* self) { return self->_v[2]; }
template <class T> float getW(T* self) { return self->_v[3]; }

template <class T> void  setX(T* self, typename T::value_type value) { self->_v[0] = value; }
template <class T> void  setY(T* self, typename T::value_type value) { self->_v[1] = value; }
template <class T> void  setZ(T* self, typename T::value_type value) { self->_v[2] = value; }
template <class T> void  setW(T* self, typename T::value_type value) { self->_v[3] = value; }

void init_OSGVec2(void);
void init_OSGVec3(void);
void init_OSGVec4(void);

#endif
