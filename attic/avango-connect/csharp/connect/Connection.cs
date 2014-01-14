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
using System.Threading;
using System.Collections;
using System.Collections.Generic;

namespace Connect
 {

  public class Connection
   {
    public delegate Field CreateField(string name, List<string> tokens);

    TextReader reader;
    TextWriter writer;
    Dictionary<string, CreateField> fields;
    private Queue<Field> queue;
    Thread poller;

    public Connection(TextReader reader, TextWriter writer)
     {
      this.reader = reader;
      this.writer = writer;

      this.queue = new Queue<Field>();
      poller = new Thread(this.PollingRun);

      fields = new Dictionary<string, CreateField>();
      IntField.AddField(this);
      StringField.AddField(this);
      MultiStringField.AddField(this);
      BoolField.AddField(this);
      DoubleField.AddField(this);
      MultiVec2Field.AddField(this);
      MultiVec3Field.AddField(this);
     }

    public void AddFieldCreator(string name, CreateField creator)
     {
      fields[name] = creator;
     }

    void WriteHeader(Field field)
     {
      writer.Write(field.Name);
      writer.Write("\0");
     }

    void WriteBody(Field field)
     {
      field.Write(writer);
     }

    void Flush()
     {
      this.writer.Flush();
     }

    public void Write(Field field)
     {
      WriteHeader(field);
      WriteBody(field);
      Flush();
     }

    public Field Read()
     {
      string line = reader.ReadLine();
      if (line == null)
        return null;
      List<string> tokens = new List<string>(line.Split(new char[]{(char)0}));
      return fields[tokens[1]](tokens[0], tokens.GetRange(2, tokens.Count-2));
     }

    public void PollingRun()
     {
      while (true)
       {
        Field next;
        try
         {
          next = Read();
         }
        catch (System.IO.IOException)
         {
          return;
         }
        if (next == null)
          break;
        lock (((ICollection)queue).SyncRoot)
         {
          queue.Enqueue(next);
         }
       }
     }

    public void StartPolling()
     {
      poller.Start();
     }

    public void JoinPolling()
     {
      poller.Join();
     }

    public Field GetNextField()
     {
      lock (((ICollection)queue).SyncRoot)
       {
        if (queue.Count == 0)
          return null;

        return queue.Dequeue();
       }
     }
   }

 }
