import avango.daemon

head_station = avango.daemon.Station('head')

kinect = avango.daemon.KinectTrack()
kinect.port = "7000"  # tuio port
kinect.server = "127.0.0.1"

kinect.stations[0] = head_station

avango.daemon.run([kinect])
