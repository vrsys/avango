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
using NUnit.Framework;

[TestFixture]
public class ThreadsTest
 {

  [Test]
  public void RunPollingTest()
   {
    StringReader reader = new StringReader("A\0SFInt\042\n");
    StringWriter writer = new StringWriter();
    Connect.Connection connection = new Connect.Connection(reader, writer);

    Assert.AreEqual(null, connection.GetNextField());

    connection.StartPolling();
    connection.JoinPolling();

    Assert.AreEqual(42, ((Connect.IntField)connection.GetNextField()).Value);
   }

 }
