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

#include <avango/gua/Types.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Types"));
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

#include <rpc/rpc.h>
#include <avango/Msg.h>

namespace av {

void
av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4d& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4d & buf): not implemented yet.");
  ::gua::math::mat4d b;
  XDR xdr;

  xdrmem_create(&xdr, reinterpret_cast<caddr_t>( &b) , sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*)&buf , 16, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push( &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::mat4d& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::mat4d & buf): not implemented yet.");
  ::gua::math::mat4d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 16, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void
av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4f& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4f & buf): not implemented yet.");
  ::gua::math::mat4f b;
  XDR xdr;

  xdrmem_create(&xdr, reinterpret_cast<caddr_t>( &b) , sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*)&buf , 16, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push( &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::mat4f& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::mat4f & buf): not implemented yet.");
  ::gua::math::mat4f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 16, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::mat3d& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::mat3d & buf): not implemented yet.");
  ::gua::math::mat3d b;
  XDR xdr;

  xdrmem_create(&xdr, reinterpret_cast<caddr_t>( &b) , sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*)&buf , 9, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push( &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::mat3d& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::mat3d & buf): not implemented yet.");
  ::gua::math::mat3d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 9, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::mat3f& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::mat3f & buf): not implemented yet.");
  ::gua::math::mat3f b;
  XDR xdr;

  xdrmem_create(&xdr, reinterpret_cast<caddr_t>( &b) , sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*)&buf , 9, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push( &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::mat3f& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::mat3f & buf): not implemented yet.");
  ::gua::math::mat3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 9, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::quatd& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::quatd & buf): not implemented yet.");
  ::gua::math::quatd b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::quatd& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::quatd & buf): not implemented yet.");
  ::gua::math::quatd b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}


void av_pushMsg(av::Msg& netMsg, const::gua::math::quatf& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::quatf & buf): not implemented yet.");
  ::gua::math::quatf b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::quatf& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::quatf & buf): not implemented yet.");
  ::gua::math::quatf b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4d& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4d & buf): not implemented yet.");
  ::gua::math::vec4d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4d& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4d & buf): not implemented yet.");
  ::gua::math::vec4d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4f& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4f & buf): not implemented yet.");
  ::gua::math::vec4f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4f& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4f & buf): not implemented yet.");
  ::gua::math::vec4f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4i & buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4i & buf): not implemented yet.");
  ::gua::math::vec4i b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(int), (xdrproc_t) xdr_int);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4i & buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4i & buf): not implemented yet.");
  ::gua::math::vec4i b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(int), (xdrproc_t) xdr_int);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4ui & buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4ui & buf): not implemented yet.");
  ::gua::math::vec4ui b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(unsigned int), (xdrproc_t) xdr_u_int);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4ui & buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4ui & buf): not implemented yet.");
  ::gua::math::vec4ui b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 4, sizeof(unsigned int), (xdrproc_t) xdr_u_int);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3d& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3d & buf): not implemented yet.");
  ::gua::math::vec3d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3d& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3d & buf): not implemented yet.");
  ::gua::math::vec3d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}


void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3f& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3f & buf): not implemented yet.");
  ::gua::math::vec3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3f& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3f & buf): not implemented yet.");
  ::gua::math::vec3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3i & buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3i & buf): not implemented yet.");
  ::gua::math::vec3i b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(int), (xdrproc_t) xdr_int);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3i & buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3i & buf): not implemented yet.");
  ::gua::math::vec3i b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(int), (xdrproc_t) xdr_int);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3ui & buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3ui & buf): not implemented yet.");
  ::gua::math::vec3ui b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(unsigned int), (xdrproc_t) xdr_u_int);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3ui & buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3ui & buf): not implemented yet.");
  ::gua::math::vec3ui b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(unsigned int), (xdrproc_t) xdr_u_int);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2d& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2d & buf): not implemented yet.");
  ::gua::math::vec2d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(double), (xdrproc_t) xdr_double);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg, ::gua::math::vec2d& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2d & buf): not implemented yet.");
  ::gua::math::vec2d b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(double), (xdrproc_t) xdr_double);
  xdr_destroy(&xdr);
}


void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2f& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2f & buf): not implemented yet.");
  ::gua::math::vec2f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg, ::gua::math::vec2f& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2f & buf): not implemented yet.");
  ::gua::math::vec2f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec2i & buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2i & buf): not implemented yet.");
  ::gua::math::vec2i b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(int), (xdrproc_t) xdr_int);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec2i& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2i & buf): not implemented yet.");
  ::gua::math::vec2i b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(int), (xdrproc_t) xdr_int);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec2ui & buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2ui & buf): not implemented yet.");
  ::gua::math::vec2ui b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(unsigned int), (xdrproc_t) xdr_u_int);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec2ui& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2ui & buf): not implemented yet.");
  ::gua::math::vec2ui b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 2, sizeof(unsigned int), (xdrproc_t) xdr_u_int);
  xdr_destroy(&xdr);
}

void av_pushMsg(av::Msg& netMsg, const::gua::utils::Color3f& buf)
{
  //throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::utils::Color3f & buf): not implemented yet.");
  ::gua::utils::Color3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_ENCODE);
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(float), (xdrproc_t) xdr_float);
  netMsg.push((void*) &b, sizeof(b));
  xdr_destroy(&xdr);
}

void av_popMsg(av::Msg& netMsg,::gua::utils::Color3f& buf)
{
  //throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::utils::Color3f & buf): not implemented yet.");
  ::gua::utils::Color3f b;
  XDR xdr;

  xdrmem_create(&xdr, (caddr_t) &b, sizeof(b), XDR_DECODE);
  netMsg.pop((void*) &b, sizeof(b));
  xdr_vector(&xdr, (char*) &buf, 3, sizeof(float), (xdrproc_t) xdr_float);
  xdr_destroy(&xdr);
}

#if 0
void av_pushMsg(av::Msg& netMsg, const::gua::physics::CollisionShape & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::physics::CollisionShape & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::physics::CollisionShape & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::physics::CollisionShape & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::physics::Constraint & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::physics::Constraint & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::physics::Constraint & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::physics::Constraint & buf): not implemented yet.");
}

//void av_pushMsg(av::Msg& netMsg, const ::gua::Camera& buf) { throw std::runtime_error("av_pushMsg(av::Msg& netMsg, : not implemented yet."); }
//void av_popMsg(av::Msg& netMsg, ::gua::Camera& buf) { throw std::runtime_error("av_pushMsg(av::Msg& netMsg, : not implemented yet."); }

void av_pushMsg(av::Msg& netMsg, const::gua::RayNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::RayNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::RayNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::RayNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::ScreenNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::ScreenNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::ScreenNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::ScreenNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::TexturedQuadNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::TexturedQuadNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::TexturedQuadNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::TexturedQuadNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::TriMeshNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::TriMeshNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::TriMeshNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::TriMeshNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::CubemapNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::CubemapNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::CubemapNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::CubemapNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::SkeletalAnimationNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::SkeletalAnimationNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::SkeletalAnimationNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::SkeletalAnimationNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::Video3DNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::Video3DNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::Video3DNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::Video3DNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::NURBSNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::NURBSNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::NURBSNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::NURBSNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::VolumeNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::VolumeNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::VolumeNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::VolumeNode & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::SceneGraph & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::SceneGraph & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::SceneGraph & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::SceneGraph & buf): not implemented yet.");
}
#endif // #if 0

} // namespace gua

#endif// #if defined(AVANGO_DISTRIBUTION_SUPPORT)

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::mat4d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::mat4d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::mat3d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::mat3d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::quatd &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::quatd &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec4d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec4d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec3d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec3d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec2d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec2d &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::mat4f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::mat4f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::mat3f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::mat3f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::quatf &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::quatf &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec4f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec4f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec4i &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec4i &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec4ui &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::
				 vec4ui &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec3f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec3f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec3i &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec3i &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec3ui &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::
				 vec3ui &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec2f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec2f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec2i &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec2i &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec2ui &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::
				 vec2ui &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::utils::Color3f &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::utils::
				 Color3f &)
{
    throw std::runtime_error("not implemented yet.");
}

#if defined(AVANGO_PHYSICS_SUPPORT)
av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::physics::CollisionShape &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::physics::
				 CollisionShape &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::physics::Constraint &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::physics::
				 Constraint &)
{
    throw std::runtime_error("not implemented yet.");
}
#endif

//av::OutputStream& av::operator<<(av::OutputStream& os, const ::gua::Camera&) { throw std::runtime_error("not implemented yet."); }
//av::InputStream& av::operator>>(av::InputStream& is, ::gua::Camera&) { throw std::runtime_error("not implemented yet."); }

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::RayNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::RayNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::ScreenNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::ScreenNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::TexturedQuadNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::
				 TexturedQuadNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::TriMeshNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::TriMeshNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
          const::gua::CubemapNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::CubemapNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const ::gua::SkeletalAnimationNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::SkeletalAnimationNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::Video3DNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::Video3DNode &)
{
    throw std::runtime_error("not implemented yet.");
}

#if defined(AVANGO_PBR_SUPPORT)

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::PBRNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::PBRNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::PLODNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::PLODNode &)
{
    throw std::runtime_error("not implemented yet.");
}

#endif

av::OutputStream & av::operator<<(av::OutputStream & os,
                                  const::gua::NURBSNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::NURBSNode &)
{
    throw std::runtime_error("not implemented yet.");
}


av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::VolumeNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::VolumeNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::SceneGraph &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::SceneGraph &)
{
    throw std::runtime_error("not implemented yet.");
}
