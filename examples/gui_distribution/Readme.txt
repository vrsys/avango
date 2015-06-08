You have to enable the flag:
DISTRIBUTION_SUPPORT = True 

The NetMatrixTransform is configured via the Groupname field.
See simpleviewer-clnt.py and simpleviewer-srv.py

Create a link:
ln -s ../simple_example/data data

Always start the client first.
$ python simpleviewer-clnt.py

Then the server
$ python simpleviewer-srv.py
