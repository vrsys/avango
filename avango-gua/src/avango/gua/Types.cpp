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
av_pushMsg(av::Msg& netMsg, const::gua::math::mat4 & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::mat4 & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::mat4 & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::mat4 & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::mat3 & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::mat3 & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::mat3 & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::mat3 & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::quat & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, const::gua::math::quat & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::quat & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::quat & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4 & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4 & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4 & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4 & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4i & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4i & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4i & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4i & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec4ui & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec4ui & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec4ui & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec4ui & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3 & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3 & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3 & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3 & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3i & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3i & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3i & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3i & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec3ui & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec3ui & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec3ui & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec3ui & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec2 & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2 & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec2 & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2 & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec2i & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2i & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec2i & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2i & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::math::vec2ui & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::math::vec2ui & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::math::vec2ui & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::math::vec2ui & buf): not implemented yet.");
}

void av_pushMsg(av::Msg& netMsg, const::gua::utils::Color3f & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::utils::Color3f & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::utils::Color3f & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::utils::Color3f & buf): not implemented yet.");
}

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

void av_pushMsg(av::Msg& netMsg, const::gua::GeometryNode & buf) {
    throw std::runtime_error("av_pushMsg(av::Msg& netMsg, ::gua::GeometryNode & buf): not implemented yet.");
}

void av_popMsg(av::Msg& netMsg,::gua::GeometryNode & buf) {
    throw std::runtime_error("av_popMsg(av::Msg& netMsg, ::gua::GeometryNode & buf): not implemented yet.");
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

#endif// #if defined(AVANGO_DISTRIBUTION_SUPPORT)

} // namespace gua

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::mat4 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::mat4 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::mat3 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::mat3 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::quat &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::quat &)
{
    throw std::runtime_error("not implemented yet.");
}

av::OutputStream & av::operator<<(av::OutputStream & os,
				  const::gua::math::vec4 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec4 &)
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
				  const::gua::math::vec3 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec3 &)
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
				  const::gua::math::vec2 &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::math::vec2 &)
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
				  const::gua::GeometryNode &)
{
    throw std::runtime_error("not implemented yet.");
}

av::InputStream & av::operator>>(av::InputStream & is,::gua::
				 GeometryNode &)
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
