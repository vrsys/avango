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

#if !defined(AV_MOCKFIELDCONTAINER_H)
#define AV_MOCKFIELDCONTAINER_H

#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>

/**
 * MockFieldContainer, illustrates the way of implementing an Avango FieldContainer.
 */
class MockFieldContainer : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor
     */
    MockFieldContainer();

    /**
     * Destructor
     */
    virtual ~MockFieldContainer();

  private:
    av::SFInt AnIntField;
    av::SFDouble ADoubleField;
    av::SFString AStringField;
    av::MFInt AnIntMultiField;
};

#endif // #if !defined(AV_MOCKFIELDCONTAINER_H)
