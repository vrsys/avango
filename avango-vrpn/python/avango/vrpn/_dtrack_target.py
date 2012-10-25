import avango.script
from avango.script import field_has_changed
import avango.osg

import avango.vrpn


def create_dtrack_target(name, receiver_offset, transmitter_offset):
    return DTrackTarget(TargetName=name, ReceiverOffset=receiver_offset, TransmitterOffset = transmitter_offset)

class DTrackTarget(avango.script.Script):
    
    #id of this target. Output field 
    ID = avango.SFLong()
    #name of the target (in the dtrack software)
    TargetName = avango.SFString()
    #receiver offset (premult with target mat)
    ReceiverOffset = avango.osg.SFMatrix()
    #transmitter offset (postmult with target mat)
    TransmitterOffset = avango.osg.SFMatrix()
    #Matrix received and set from the DTrackDevice
    DTrackMatrix = avango.osg.SFMatrix()
    #combined matrix (Output)
    Matrix = avango.osg.SFMatrix()
    
    
    def __init__(self):
        self.super(DTrackTarget).__init__()
        
    @field_has_changed(ReceiverOffset)
    def trigger_changed(self):
        self._update_matrix()
    
    @field_has_changed(TransmitterOffset)
    def trigger_changed(self):
        self._update_matrix()    
        
    @field_has_changed(DTrackMatrix)
    def trigger_changed(self):
        self._update_matrix()
    
    def _update_matrix(self):
        self.Matrix.value = self.ReceiverOffset.value * self.DTrackMatrix.value * self.TransmitterOffset.value 
    
        