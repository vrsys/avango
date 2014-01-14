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

using System.IO;
using System.Collections.Generic;

namespace Connect
 {

  public class BoolField : Field
   {
    const string typename = "SFBool";

    bool fieldValue;

    public bool Value
     {
      get { return fieldValue; }
     }

    public BoolField(string name, bool value_) :
      base(name)
     {
      fieldValue = value_;
     }

    private BoolField(string name, List<string> tokens) :
      base(name)
     {
      fieldValue = (0 != int.Parse(tokens[0]));
     }

    public static Field Create(string name, List<string> tokens)
     {
      return new BoolField(name, tokens);
     }

    public static void AddField(Connection connection)
     {
      connection.AddFieldCreator(typename, new Connection.CreateField(Create));
     }

    public override void Write(TextWriter writer)
     {
      writer.Write(typename);
      writer.Write("\0");
      if (fieldValue)
       {
        writer.Write("1");
       }
      else
       {
        writer.Write("0");
       }
      writer.Write("\n");
     }
   }

 }
