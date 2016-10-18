import avango.daemon

hmd_stations = {}

for i in range(7):
	hmd_stations[i] = avango.daemon.Station('vr-device-{0}'.format(str(i)))


hmdZMQ0 = avango.daemon.HMDTrack()
hmdZMQ0.port = "7770"
hmdZMQ0.server = "141.54.147.35"

	


hmdZMQ0.stations[0] = hmd_stations[0]
for i in range(1,7):
	hmdZMQ0.stations[i] = hmd_stations[i]


avango.daemon.run([hmdZMQ0])