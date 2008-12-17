import avango.osg.viewer
import avango.daemon
from math import *

class Device(avango.script.Script):
    Active = avango.SFBool()
    Matrix = avango.osg.SFMatrix()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    Button3 = avango.SFBool()
    Button4 = avango.SFBool()
    Button5 = avango.SFBool()

    def __init__(self):
        self.Active.value = True

osg2perf = avango.osg.make_rot_mat(radians(90), 1, 0, 0)
perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)


device_service = avango.daemon.DeviceService()
def create_glasses(name, receiver_offset):
    sensor = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                              Station = name)
    sensor.ReceiverOffset.value = avango.osg.make_trans_mat(receiver_offset)
    sensor.TransmitterOffset.value = perf2osg
    return sensor


unknown_glasses1 = create_glasses("ve-dtrack-head2", avango.osg.Vec3( 0.076, -0.016, 0.025))
view1_grey_glasses = create_glasses("ve-dtrack-head8", avango.osg.Vec3(-0.073, -0.016, 0.045))
view1_yellow_glasses = create_glasses("ve-dtrack-head4", avango.osg.Vec3(-0.074, -0.018, 0.025))

unknown_glasses3 = create_glasses("ve-dtrack-head1", avango.osg.Vec3(-0.089, 0.015, -0.040))
view2_green_glasses = create_glasses("ve-dtrack-head5", avango.osg.Vec3( 0.077, -0.017, 0.0 ))
view2_blue_glasses = create_glasses("ve-dtrack-head3", avango.osg.Vec3(-0.073, -0.016, 0.025))

pda_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                            Station = "ve-dtrack-pda2")
pda_sensor.TransmitterOffset.value = perf2osg
pda_sensor.ReceiverOffset.value = avango.osg.make_trans_mat(0.076, -0.016, 0.025)


wiimote_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                                    Station = "wiimote1")
wiimote_actuator = avango.daemon.nodes.WiimoteActuator(DeviceService = device_service,
                                                        Station = "wiimote1")

wiimote_actuator.LED0.value = True
wiimote_atek_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = device_service, Station = "ve-dtrack-atek")
wiimote_atek_sensor.ReceiverOffset.value = osg2perf * avango.osg.make_trans_mat(avango.osg.Vec3(0.045, 0.0,  0.022))
wiimote_atek_sensor.TransmitterOffset.value = perf2osg

# get glasses to connect with view
head_sensor_1 = view1_yellow_glasses
head_sensor_2 = view2_blue_glasses