You have to compile avango with the following settings in your localdefs.py
DISTRIBUTION_SUPPORT = True 
ZMQ_DISTRIBUTION_SUPPORT = True

The NetMatrixTransform is configured via the Groupname field.
See simpleviewer-clnt.py and simpleviewer-srv.py

Always start the client first.
$ python simpleviewer-clnt.py

Then the server
$ python simpleviewer-srv.py teapot_high.obj
