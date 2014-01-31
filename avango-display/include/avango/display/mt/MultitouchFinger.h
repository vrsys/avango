// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#if !defined(AV_DISPLAY_MT_MULTITOUCHFINGER_H)
#define AV_DISPLAY_MT_MULTITOUCHFINGER_H

#include <avango/osg/Fields.h>
#include <avango/FieldContainer.h>
#include <avango/display/mt/windows_specific_devices.h>

namespace av
{
  namespace display
  {
    namespace mt
    {
      typedef ::osg::Vec2 OsgVec2;

      class AV_DISPLAY_MT_DLL MultitouchFinger : public FieldContainer
      {
        AV_FC_DECLARE();

      public:
        MultitouchFinger(int _id = -1, OsgVec2 _position = OsgVec2(), float _creationTime= 0.0, float _angle = 0.0, float _width = 0.0, float _height = 0.0, float _area = 0.0, int _user_id = -1);
        virtual ~MultitouchFinger();

        av::SFInt Identifier, UserID;
        av::osg::SFVec2 Position;
		av::SFFloat Angle, Width, Height, Area;
        av::SFFloat CreationTime;

      };

      typedef SingleField<Link<MultitouchFinger> > SFMultitouchFinger;
      typedef MultiField<Link<MultitouchFinger> > MFMultitouchFinger;


	  class AV_DISPLAY_MT_DLL MultitouchUser : public FieldContainer
      {
        AV_FC_DECLARE();

      public:
        MultitouchUser(int _id = -1, float _creationTime= 0.0);
        virtual ~MultitouchUser();

        av::SFInt Identifier;
        av::SFFloat CreationTime;

      };

      typedef SingleField<Link<MultitouchUser> > SFMultitouchUser;
      typedef MultiField<Link<MultitouchUser> > MFMultitouchUser;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_DISPLAY_MT_DLL SingleField<Link<display::mt::MultitouchFinger> >;
template class AV_DISPLAY_MT_DLL MultiField<Link<display::mt::MultitouchFinger> >;

template class AV_DISPLAY_MT_DLL SingleField<Link<display::mt::MultitouchUser> >;
template class AV_DISPLAY_MT_DLL MultiField<Link<display::mt::MultitouchUser> >;
#endif
}

#endif

