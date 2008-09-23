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

#if !defined(AVANGO_OBJECT_H)
#define AVANGO_OBJECT_H

#include <avango/FieldContainer.h>

namespace av
{
  /**
   * Avango object base class
   *
   * This is the base class to extend avango with own classes.
   */
  class AV_DLL Object : public FieldContainer
  {

    AV_FC_DECLARE();

  public:

    Object();

  protected:

    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Object();

  };

  template class AV_DLL SingleField<Link<Object> >;
  template class AV_DLL MultiField<Link<Object> >;

  typedef SingleField<Link<Object> > SFObject;
  typedef MultiField<Link<Object> >  MFObject;

} // namespace av

#endif // #if !defined(AVANGO_OBJECT_H)
