import avango.daemon

head_station0 = avango.daemon.Station('kinect-head-0')

kinect0 = avango.daemon.KinectTrack()
kinect0.port = "7000"  # tuio port
kinect0.server = "127.0.0.1"

kinect0.stations[0] = head_station0

head_station1 = avango.daemon.Station('kinect-head-1')

kinect1 = avango.daemon.KinectTrack()
kinect1.port = "7001"  # tuio port
kinect1.server = "127.0.0.1"

kinect1.stations[0] = head_station1

#avango.daemon.run([kinect0, kinect1])
avango.daemon.run([kinect0])
