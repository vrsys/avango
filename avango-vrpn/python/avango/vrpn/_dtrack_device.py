import avango.script
from avango.script import field_has_changed



from _vrpn import *
from _dtrack_device import *

class DTrackTaregtIdName(avango.script.Script):
    
    Id = avango.SFLong()
    Name = avango.SFString()
    
    def __str__(self):
        return str(self.Id.value) + " " + self.Name.value
    

class DTrackDevice(avango.script.Script):
    
    TrackerInfo = MFTrackerInformation()
    Targets = avango.script.MFObject()
    
    TargetIdName = avango.script.MFObject()
    

    def __init__(self):
        self.super(DTrackDevice).__init__()
        
        self.id_name_dict = {}
        
    def populate_interested_target_ids(self, targetIDs, clear):
        if clear:
            self.TargetIdName.value = []
        
        for p in targetIDs:
            assert(len(p)==2)
            
            self.TargetIdName.value.append(DTrackTaregtIdName(Id=p[0],Name=p[1]))
        
    
    def get_name_from_id(self, id):
        for p in self.TargetIdName.value:
            if id == p.Id.value:
                return True, p.Name.value
        return False, ""
    
    def get_target_by_name(self, name):
        for target in self.Targets.value:
            if name == target.TargetName.value:
                return True, target
        return False, None  
    
    @field_has_changed(TrackerInfo)
    def tracker_information_changed(self):
        #loop over all tracker information object (you do not know which one has been changed)
        for trackerInfo in self.TrackerInfo.value:
            trackerNum = trackerInfo.Number.value
            #check if the target is known
            targetFoundInDTrackConfig, targetName = self.get_name_from_id(trackerNum)
            if targetFoundInDTrackConfig :
                #check if the target is in the subscription list
                targetFound, target = get_target_by_name(targetName) 
                if target_found:
                    target.Id = trackerInfo.Number.value
                    target.DTrackMatrix.value = trackerInfo.Matrix
                
            
        