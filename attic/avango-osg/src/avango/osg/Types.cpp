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

#include <avango/osg/Types.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Types"));
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

#include <rpc/rpc.h>
#include <avango/Msg.h>

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Matrixf& buf)
{
  ::osg::Matrixf b;
  XDR xdr;

  xdrmem_create(&xdr, reinterpret_cast<caddr_t>( &b) , sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*)&buf , 16, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push( &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Matrixf& buf)
{
  ::osg::Matrixf b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 16, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Matrixd& buf)
{
  ::osg::Matrixd b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 16, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Matrixd& buf)
{
  ::osg::Matrixd b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 16, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Vec2f& buf)
{
  ::osg::Vec2f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Vec2f& buf)
{
  ::osg::Vec2f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Vec2d& buf)
{
  ::osg::Vec2d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Vec2d& buf)
{
  ::osg::Vec2d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Vec3f& buf)
{
  ::osg::Vec3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Vec3f& buf)
{
  ::osg::Vec3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Vec3d& buf)
{
  ::osg::Vec3d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Vec3d& buf)
{
  ::osg::Vec3d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Vec4f& buf)
{
  ::osg::Vec4f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Vec4f& buf)
{
  ::osg::Vec4f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Vec4d& buf)
{
  ::osg::Vec4d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Vec4d& buf)
{
  ::osg::Vec4d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Quat& buf)
{
  ::osg::Vec4d vec = buf.asVec4();
  av_pushMsg(netMsg, vec);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Quat& buf)
{
  ::osg::Vec4d vec;
  av_popMsg(netMsg, vec);
  buf.set(vec);
}


#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Matrixf& mat)
{
  const ::osg::Matrixf::value_type *mat_vals = mat.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) mat_vals, sizeof(mat_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        (std::ostream&) os << mat(i, j) << ' ';
      }
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Matrixf& mat)
{
  ::osg::Matrixf::value_type *mat_vals = mat.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) mat_vals, sizeof(mat_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        (std::istream&) is >> mat(i, j);
      }
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Matrixd& mat)
{
  const ::osg::Matrixd::value_type *mat_vals = mat.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) mat_vals, sizeof(mat_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        (std::ostream&) os << mat(i, j) << ' ';
      }
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Matrixd& mat)
{
  ::osg::Matrixd::value_type *mat_vals = mat.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) mat_vals, sizeof(mat_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        (std::istream&) is >> mat(i, j);
      }
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Vec2f& vec)
{
  const ::osg::Vec2f::value_type *vec_vals = vec.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 2; i++)
    {
      (std::ostream&) os << vec[i] << ' ';
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Vec2f& vec)
{
  ::osg::Vec2f::value_type *vec_vals = vec.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 2; i++)
    {
      (std::istream&) is >> vec[i];
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Vec2d& vec)
{
  const ::osg::Vec2d::value_type *vec_vals = vec.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 2; i++)
    {
      (std::ostream&) os << vec[i] << ' ';
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Vec2d& vec)
{
  ::osg::Vec2d::value_type *vec_vals = vec.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 2; i++)
    {
      (std::istream&) is >> vec[i];
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Vec3f& vec)
{
  const ::osg::Vec3f::value_type *vec_vals = vec.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 3; i++)
    {
      (std::ostream&) os << vec[i] << ' ';
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Vec3f& vec)
{
  ::osg::Vec3f::value_type *vec_vals = vec.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 3; i++)
    {
      (std::istream&) is >> vec[i];
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Vec3d& vec)
{
  const ::osg::Vec3d::value_type *vec_vals = vec.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 3; i++)
    {
      (std::ostream&) os << vec[i] << ' ';
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Vec3d& vec)
{
  ::osg::Vec3d::value_type *vec_vals = vec.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 3; i++)
    {
      (std::istream&) is >> vec[i];
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Vec4f& vec)
{
  const ::osg::Vec4f::value_type *vec_vals = vec.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      (std::ostream&) os << vec[i] << ' ';
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Vec4f& vec)
{
  ::osg::Vec4f::value_type *vec_vals = vec.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      (std::istream&) is >> vec[i];
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Vec4d& vec)
{
  const ::osg::Vec4d::value_type *vec_vals = vec.ptr();

  if (os.isBinaryEnabled()) {
    os.write((const char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      (std::ostream&) os << vec[i] << ' ';
    }
  }

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Vec4d& vec)
{
  ::osg::Vec4d::value_type *vec_vals = vec.ptr();

  if (is.isBinaryEnabled()) {
    is.read((char*) vec_vals, sizeof(vec_vals));
  } else {
    for (int i = 0; i < 4; i++)
    {
      (std::istream&) is >> vec[i];
    }
  }

  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Quat& vec)
{
  return os << vec.asVec4();
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Quat& vec)
{
  ::osg::Vec4d tmp;

  is >> tmp;
  vec.set(tmp);
  return is;
}
