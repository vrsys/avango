import avango.daemon

head_station0 = avango.daemon.Station('oculus-head-0')

oculusZMQ0 = avango.daemon.OculusTrack()
oculusZMQ0.port = "7701"
oculusZMQ0.server = "141.54.147.35"

oculusZMQ0.stations[0] = head_station0

avango.daemon.run([oculusZMQ0])
