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

using System;
using System.IO;
using System.Text;
using NUnit.Framework;

[TestFixture]
public class FieldTest
 {
  StringReader reader;
  StringWriter writer;
  Connect.Connection connection;

  [SetUp]
  public void Init()
   {
    Init("");
   }

  public void Init(string content)
   {
    reader = new StringReader(content);
    writer = new StringWriter();
    connection = new Connect.Connection(reader, writer);
   }

  [Test]
  public void WriteInteger()
   {
    int value_ = 42;
    connection.Write(new Connect.IntField("A", value_));
    Assert.AreEqual("A\0SFInt\042\n", writer.ToString());
   }

  [Test]
  public void ReadInteger()
   {
    Init("A\0SFInt\042\n");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.IntField), field);
    Assert.AreEqual(42, ((Connect.IntField)field).Value);
   }

  [Test]
  public void WriteString()
   {
    string value_ = "ATest";
    connection.Write(new Connect.StringField("A", value_));
    Assert.AreEqual("A\0SFString\0ATest\n", writer.ToString());
   }

  [Test]
  public void ReadString()
   {
    Init("A\0SFString\0ATest");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.StringField), field);
    Assert.AreEqual("ATest", ((Connect.StringField)field).Value);
   }

  [Test]
  public void WriteMultiString()
   {
    string[] value_ = { "ATest", "Another Test" };
    connection.Write(new Connect.MultiStringField("A", value_));
    Assert.AreEqual("A\0MFString\0ATest\0Another Test\n", writer.ToString());
   }

  [Test]
  public void ReadMultiString()
   {
    Init("A\0MFString\0ATest\0Another Test\n");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.MultiStringField), field);
    string[] value_ = { "ATest", "Another Test" };
    Assert.AreEqual(value_, ((Connect.MultiStringField)field).Value);
   }

  [Test]
  public void ReadFieldsFromFile()
   {
    StreamReader reader = new StreamReader(Environment.GetEnvironmentVariable("TEST_DATA_PATH")+"/fielddata", new ASCIIEncoding());
    Connect.Connection connection = new Connect.Connection(reader, writer);

    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.IntField), field);
    Assert.AreEqual(42, ((Connect.IntField)field).Value);

    field = connection.Read();
    Assert.AreEqual("B", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.StringField), field);
    Assert.AreEqual("Another one.", ((Connect.StringField)field).Value);

    field = connection.Read();
    Assert.AreEqual("C", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.MultiStringField), field);
    string[] value_ = { "Another one.", "Two" };
    Assert.AreEqual(value_, ((Connect.MultiStringField)field).Value);
   }

  [Test]
  public void WriteBool()
   {
    connection.Write(new Connect.BoolField("A", true));
    Assert.AreEqual("A\0SFBool\01\n", writer.ToString());
   }

  [Test]
  public void ReadBool()
   {
    Init("A\0SFBool\01\n");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.BoolField), field);
    Assert.AreEqual(true, ((Connect.BoolField)field).Value);
   }

  [Test]
  public void WriteDouble()
   {
    double value_ = 1.2;
    connection.Write(new Connect.DoubleField("A", value_));
    // FIXME use an almost check here
    Assert.AreEqual("A\0SFDouble\01.2\n", writer.ToString());
   }

  [Test]
  public void ReadDouble()
   {
    Init("A\0SFDouble\01.2\n");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.DoubleField), field);
    Assert.AreEqual(1.2, ((Connect.DoubleField)field).Value, 0.00001);
   }

  [Test]
  public void WriteMultiVec2()
   {
    double[,] value_ = { {1.1, 1.2}, {2.1, 2.2} };
    connection.Write(new Connect.MultiVec2Field("A", value_));
    Assert.AreEqual("A\0MFVec2\01.1\01.2\02.1\02.2\n", writer.ToString());
   }

  public void AssertVectorsAreEqual(double[,] a, double[,] b)
   {
    Assert.AreEqual(a.GetLength(0), b.GetLength(0));
    Assert.AreEqual(a.GetLength(1), b.GetLength(1));
    for (int i = 0; i != a.GetLength(0); ++i)
     {
      for (int j = 0; j != a.GetLength(1); ++j)
       {
        Assert.AreEqual(a[i,j], b[i,j]);
       }
     }
   }

  [Test]
  public void ReadMultiVec2()
   {
    Init("A\0MFVec2\01.1\01.2\02.1\02.2\n");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.MultiVec2Field), field);
    double[,] value_ = { {1.1, 1.2}, {2.1, 2.2} };
    AssertVectorsAreEqual(value_, ((Connect.MultiVec2Field)field).Value);
   }

  [Test]
  public void WriteMultiVec3()
   {
    double[,] value_ = { {1.1, 1.2, 1.3}, {2.1, 2.2, 2.3} };
    connection.Write(new Connect.MultiVec3Field("A", value_));
    Assert.AreEqual("A\0MFVec3\01.1\01.2\01.3\02.1\02.2\02.3\n", writer.ToString());
   }

  [Test]
  public void ReadMultiVec3()
   {
    Init("A\0MFVec3\01.1\01.2\01.3\02.1\02.2\02.3\n");
    Connect.Field field = connection.Read();
    Assert.AreEqual("A", field.Name);
    Assert.IsInstanceOfType(typeof(Connect.MultiVec3Field), field);
    double[,] value_ = { {1.1, 1.2, 1.3}, {2.1, 2.2, 2.3} };
    AssertVectorsAreEqual(value_, ((Connect.MultiVec3Field)field).Value);
   }

 }
