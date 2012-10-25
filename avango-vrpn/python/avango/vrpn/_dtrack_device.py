import avango.script

import avango.vrpn

class DTrackDevice(avango.script.Script):
    
    TrackerInformation = avango.vrpn.MFTrackerInformation()

    def __init__(self):
        self.super(DTrackDevice).__init__()
        