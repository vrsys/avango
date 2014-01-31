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
using System.Globalization;
using System.Collections.Generic;

namespace Connect
 {

  public class MultiVec3Field : Field
   {
    const string typename = "MFVec3";

    double[,] fieldValue;

    public double[,] Value
     {
      get { return fieldValue; }
     }

    public MultiVec3Field(string name, double[,] value_) :
      base(name)
     {
      fieldValue = value_;
     }

    private MultiVec3Field(string name, List<string> tokens) :
      base(name)
     {
      int numVectors = tokens.Count/3;
      fieldValue = new double[numVectors, 3];
      for (int i = 0; i != numVectors; ++i)
       {
        fieldValue[i, 0] = double.Parse(tokens[3*i+0], CultureInfo.InvariantCulture);
        fieldValue[i, 1] = double.Parse(tokens[3*i+1], CultureInfo.InvariantCulture);
        fieldValue[i, 2] = double.Parse(tokens[3*i+2], CultureInfo.InvariantCulture);
       }
     }

    public static Field Create(string name, List<string> tokens)
     {
      return new MultiVec3Field(name, tokens);
     }

    public static void AddField(Connection connection)
     {
      connection.AddFieldCreator(typename, new Connection.CreateField(Create));
     }

    public override void Write(TextWriter writer)
     {
      writer.Write(typename);
      for (int i = 0; i != fieldValue.GetLength(0); ++i)
       {
        writer.Write("\0");
        writer.Write(fieldValue[i, 0].ToString(CultureInfo.InvariantCulture));
        writer.Write("\0");
        writer.Write(fieldValue[i, 1].ToString(CultureInfo.InvariantCulture));
        writer.Write("\0");
        writer.Write(fieldValue[i, 2].ToString(CultureInfo.InvariantCulture));
       }
      writer.Write("\n");
     }
   }

 }
