import avango.daemon

head_station0 = avango.daemon.Station('kinect-head-0')

kinect0 = avango.daemon.KinectTrack()
kinect0.port = "7000"
kinect0.server = "127.0.0.1"

kinect0.stations[0] = head_station0

avango.daemon.run([kinect0])
