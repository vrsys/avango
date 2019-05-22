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

// includes, project

#include <avango/Link.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

// internal unnamed namespace

namespace
{
TEST(SetValueInUncontainedField)
{
    av::SFInt field;
    field.setValue(42);
    CHECK_EQUAL(42, field.getValue());
}

TEST(CloneSingleField)
{
    av::SFInt field;
    field.setValue(42);

    av::Field* clone = field.clone();
    CHECK_EQUAL(42, dynamic_cast<av::SFInt*>(clone)->getValue());
}

TEST(CloneMultiField)
{
    av::MFInt field;
    std::vector<int> values;
    values.push_back(42);
    values.push_back(21);
    field.setValue(values);

    av::Field* clone = field.clone();
    std::vector<int> clone_values = dynamic_cast<av::MFInt*>(clone)->getValue();
    CHECK_EQUAL(2u, clone_values.size());
    CHECK_EQUAL(42, clone_values[0]);
    CHECK_EQUAL(21, clone_values[1]);
}

TEST(CloneGenericField)
{
    av::Field* field = new av::SFInt;
    dynamic_cast<av::SFInt*>(field)->setValue(42);

    av::Field* clone = field->clone();
    CHECK_EQUAL(42, dynamic_cast<av::SFInt*>(clone)->getValue());
}

} // namespace
