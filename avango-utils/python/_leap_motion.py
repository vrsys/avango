#leap motion sdk
import Leap

#avango 
import avango.osg

#python 
import Queue

keep_alive = []

class AVLeapPointable(avango.script.Script):
    
    ID = avango.SFInt()
    TipPosition = avango.osg.SFVec3()
    TipVelocity = avango.osg.SFVec3()
    Direction = avango.osg.SFVec3()
    Width = avango.SFFloat()
    Length = avango.SFFloat()
    IsTool = avango.SFBool()
    IsFinger = avango.SFBool()
    IsValid = avango.SFBool()
    TouchZone = avango.SFBool()
    TouchDistance = avango.SFFloat()
    TipPositionStabilized = avango.osg.SFVec3()
    
    def __init__(self):
        self.super(AVLeapPointable).__init__();
    
    def init_with_leap_data(self, pointable):
        self.ID.value = pointable.id
        self.TipPosition.value = leap_av_vec(pointable.tip_position)
        self.TipVelocity.value = leap_av_vec(pointable.tip_velocity)
        self.Direction.value = leap_av_vec(pointable.direction)
        self.Width.value = pointable.width
        self.Length.value = pointable.length
        self.IsTool.value = pointable.is_tool
        self.IsFinger.value = pointable.is_finger
        self.IsValid.value = pointable.is_valid
        self.TouchZone.value = pointable.touch_zone
        self.TouchDistance.value = pointable.touch_distance
        self.TipPositionStabilized.value = leap_av_vec(pointable.stabilized_tip_position)
        
    def __str__(self):
                
        return "id: " + str(self.ID.value)  \
                + " tip_position: " +  str(self.TipPosition.value) \
                + " tip_velocity: " +  str(self.TipVelocity.value) \
                + " direction: " +  str(self.Direction.value) \
                + " width: " +  str(self.Width.value) \
                + " length: " +  str(self.Length.value) \
                + " is_tool: " +  str(self.IsTool.value) \
                + " is_finger: " +  str(self.IsFinger.value) \
                + " is_valid: " +  str(self.IsValid.value) \
                + " touch_zone: " +  str(self.TouchZone.value) \
                + " touch_distance: " +  str(self.TouchDistance.value) \
                + " stabilized_tip_position: " +  str(self.TipPositionStabilized.value) 


class AVLeapFinger(AVLeapPointable):
    def __init__(self):
        self.super(AVLeapFinger).__init__();
    
class AVLeapTool(AVLeapPointable):
    def __init__(self):
        self.super(AVLeapTool).__init__();    
        
        
class AVLeapGesture(avango.script.Script):
    
    ID = avango.SFInt()
    State = avango.SFInt()
    Type = avango.SFInt()
    Duration = avango.SFInt()
    DurationSeconds = avango.SFFloat()
    Pointables = avango.script.MFObject()
    Hands = avango.script.MFObject()

    def __init__(self):
        self.super(AVLeapGesture).__init__();
    
    def init_with_leap_data(self, gesture):
        self.ID.value = gesture.id
        self.State.value = gesture.state
        self.Type.value = gesture.type
        self.Duration.value = gesture.duration
        self.DurationSeconds.value = gesture.duration_seconds
        
        self.Pointables.value = create_av_leap_data_factory(gesture.pointables, AVLeapPointable)
        self.Hands.value = create_av_leap_data_factory(gesture.hands, AVLeapHand)
    
    def __str__(self):
        buffer = ""
        buffer += "id: " + str(self.ID.value) + "\n"
        buffer += "type: "
        if self.Type.value == LeapGestureType.TYPE_SWIPE:
            buffer += "TYPE_SWIPE" + "\n"
        elif self.Type.value == LeapGestureType.TYPE_CIRCLE:
            buffer += "TYPE_CIRCLE" + "\n"
        elif self.Type.value == LeapGestureType.TYPE_SCREEN_TAP:
            buffer += "TYPE_SCREEN_TAP" + "\n"
        elif self.Type.value == LeapGestureType.TYPE_KEY_TAP:
            buffer += "TYPE_KEY_TAP" + "\n"
        else:
            buffer += "STATE_INVALID" + "\n"
            
        buffer += "state: "
        if self.State.value == LeapGestureState.STATE_START:
            buffer += "STATE_START" + "\n"
        elif self.State.value == LeapGestureState.STATE_UPDATE:
            buffer += "STATE_UPDATE" + "\n"
        elif self.State.value == LeapGestureState.STATE_STOP:
            buffer += "STATE_STOP" + "\n"
        else:
            buffer += "STATE_INVALID" + "\n"
            
        buffer += "duration: " + str(self.Duration.value) + "\n"
        buffer += "duration_seconds: " + str(self.DurationSeconds.value) + "\n"
        
        return buffer
 
class AVLeapHand(avango.script.Script):
    
    ID = avango.SFInt()
    Pointables = avango.script.MFObject()
    Fingers = avango.script.MFObject()
    Tools = avango.script.MFObject()
    PalmPosition = avango.osg.SFVec3()
    PalmVelocity = avango.osg.SFVec3()
    PalmNormal = avango.osg.SFVec3()
    Direction = avango.osg.SFVec3()
    IsValid = avango.SFBool()
    SphereCenter = avango.osg.SFVec3()
    SphereRadius = avango.SFFloat()
    
    
    def __init__(self):
        self.super(AVLeapHand).__init__()
        
    def init_with_leap_data(self, hand):
        self.ID.value = hand.id
        self.Pointables.value = create_av_leap_data_factory(hand.pointables, AVLeapPointable)
        self.Fingers.value = create_av_leap_data_factory(hand.pointables, AVLeapFinger)
        self.Tools.value = create_av_leap_data_factory(hand.pointables, AVLeapTool)
        self.PalmPosition.value = leap_av_vec(hand.palm_position)
        self.PalmVelocity.value = leap_av_vec(hand.palm_velocity)
        self.PalmNormal.value = leap_av_vec(hand.palm_normal)
        self.Direction.value = leap_av_vec(hand.direction)
        self.IsValid.value = hand.is_valid
        self.SphereCenter.value = leap_av_vec(hand.sphere_center)
        self.SphereRadius.value = hand.sphere_radius
        
        
    def __str__(self):
        
        buffer = ""
        buffer += "id: " + str(self.ID.value) + "\n"
        buffer += " fingers" + "\n"
        for f in self.Fingers.value:
            buffer += "  " + str(f)  + "\n"
        buffer += " pointables" + "\n"
        for p in self.Pointables.value:
            buffer += "  " + str(p)  + "\n"
        buffer += " tools" + "\n"
        for p in self.Tools.value:
            buffer += "  " + str(p)  + "\n"
        buffer += " palm_position: " +  str(self.PalmPosition.value)  + "\n"
        buffer += " palm_velocity: " +  str(self.PalmVelocity.value) + "\n"
        buffer += " palm_normal: " +  str(self.PalmNormal.value) + "\n"
        buffer += " direction: " +  str(self.Direction.value) + "\n"
        buffer += " is_valid: " +  str(self.IsValid.value) + "\n"
        buffer += " sphere_center: " +  str(self.SphereCenter.value) + "\n"
        buffer += " sphere_radius: " +  str(self.SphereRadius.value) + "\n"
        
        return buffer
            

class AVLeapFrame(avango.script.Script):
    
    ID = avango.SFInt()
    Timestamp = avango.SFLong()
    IsValid = avango.SFBool()
    
    Hands = avango.script.MFObject()
    Fingers = avango.script.MFObject()
    Pointables = avango.script.MFObject()
    Tools = avango.script.MFObject()
    Gestures = avango.script.MFObject()
    
    def __init__(self):
        self.super(AVLeapFrame).__init__()
        
#     def init_with_leap_data(self, leap_frame):
#         
#         self.ID.value = leap_frame.id+0
#         self.Timestamp.value = leap_frame.timestamp
#         self.IsValid.value = leap_frame.is_valid
#         
#         self.Pointables.value = create_av_leap_data_factory(leap_frame.pointables, AVLeapPointable)
#         self.Fingers.value = create_av_leap_data_factory(leap_frame.pointables, AVLeapFinger)
#         self.Tools.value = create_av_leap_data_factory(leap_frame.pointables, AVLeapTool)
#         self.Hands.value = create_av_leap_data_factory(leap_frame.hands, AVLeapHand)
#         self.Gestures.value = create_av_leap_data_factory(leap_frame.gestures, AVLeapGesture)
#         
    def update_with_leap_data(self, leap_frame):
        self.ID.value = leap_frame.id+0
        self.Timestamp.value = leap_frame.timestamp
        self.IsValid.value = leap_frame.is_valid
        
        self.Pointables.value = create_av_leap_data_factory(leap_frame.pointables, AVLeapPointable)
        self.Fingers.value = create_av_leap_data_factory(leap_frame.pointables, AVLeapFinger)
        self.Tools.value = create_av_leap_data_factory(leap_frame.pointables, AVLeapTool)
        self.Hands.value = create_av_leap_data_factory(leap_frame.hands, AVLeapHand)
        self.Gestures.value = create_av_leap_data_factory(leap_frame.gestures, AVLeapGesture)
        
    def __str__(self):
        buffer = ""
        buffer += "AVLeapFrame: " + str(self.ID.value) + "\n"
        buffer += "Timestamp: " + str(self.Timestamp.value) + "\n"
        buffer += "Hands: "
        for h in self.Hands.value:
            buffer += str(h)  + "\n"
             
        buffer += "Fingers: " + "\n"
        for f in self.Fingers.value:
            buffer += str(f) + "\n"
        buffer += "Pointables: " + "\n"
        for p in self.Pointables.value:
            buffer += str(p) + "\n"
        buffer += "Tools: " + "\n"
        for t in self.Tools.value:
            buffer += str(f) + "\n"
        
        return buffer
 
# def create_av_leap_hands(leap_hands):
#     av_hands = []
#     for i in range(0,len(leap_hands)):
#         av_hand = AVLeapHand()
#         av_hand.init_with_leap_hand(leap_hands[i])
#         av_hands.append(av_hand)
#     return av_hands
# 
# def create_av_leap_gestures(leap_gestures):
#     av_gestures = []
#     for i in range(0,len(leap_gestures)):
#         av_gesture = AVLeapGesture()
#         av_gesture.init_with_leap_gesture(leap_gestures[i])
#         av_gestures.append(av_gesture)
#     return av_gestures
# 
#         
#         

def create_av_leap_data_factory(pointers_raw, pointer_type):
    pointers = []
    for i in range(0,len(pointers_raw)):
        p = pointer_type()
        p.init_with_leap_data(pointers_raw[i])
        pointers.append(p)
    return pointers


def leap_av_vec(leap_vec):
    return avango.osg.Vec3(leap_vec.x,leap_vec.y,leap_vec.z)

class LeapVec3(object):
    def __init__(self, v):
        self.x = float(v.x)
        self.y = float(v.y)
        self.z = float(v.z)
        
    def __str__(self):
        return str(self.x) + " " + str(self.y) + " " + str(self.z);

        
class LeapPointable(object):
    def __init__(self, pointable):
        self.id = pointable.id
        self.tip_position = LeapVec3(pointable.tip_position)
        self.tip_velocity = LeapVec3(pointable.tip_velocity)
        self.direction = LeapVec3(pointable.direction)
        self.width = pointable.width
        self.length = pointable.length
        self.is_tool = pointable.is_tool
        self.is_finger = pointable.is_finger
        self.is_valid = pointable.is_valid
        self.touch_zone = pointable.touch_zone
        self.touch_distance = pointable.touch_distance
        self.stabilized_tip_position = LeapVec3(pointable.stabilized_tip_position)
        
    def __str__(self):
         return "id: " + str(self.id)  \
                + " tip_position: " +  str(self.tip_position) \
                + " tip_velocity: " +  str(self.tip_velocity) \
                + " direction: " +  str(self.direction) \
                + " width: " +  str(self.width) \
                + " length: " +  str(self.length) \
                + " is_tool: " +  str(self.is_tool) \
                + " is_finger: " +  str(self.is_finger) \
                + " is_valid: " +  str(self.is_valid) \
                + " touch_zone: " +  str(self.touch_zone) \
                + " touch_distance: " +  str(self.touch_distance) \
                + " stabilized_tip_position: " +  str(self.stabilized_tip_position) 
     
        
class LeapFinger(LeapPointable):
    def __init__(self, finger):
        
        super(LeapFinger, self).__init__(finger)

class LeapTool(LeapPointable):
    def __init__(self, tool):
        super(LeapTool, self).__init__(tool)

class LeapGestureType:
    TYPE_INVALID    = -1#, /**< An invalid type. */
    TYPE_SWIPE      = 1#,  /**< A straight line movement by the hand with fingers extended. */
    TYPE_CIRCLE     = 4#,  /**< A circular movement by a finger. */
    TYPE_SCREEN_TAP = 5#,  /**< A forward tapping movement by a finger. */
    TYPE_KEY_TAP    = 6#   /**< A downward tapping movement by a finger. */

class LeapGestureState:
    STATE_INVALID = -1, #< An invalid state */
    STATE_START   = 1 #< The gesture is starting. Just enough has happened to recognize it. */
    STATE_UPDATE  = 2 #< The gesture is in progress. (Note: not all gestures have updates). */
    STATE_STOP    = 3 

class LeapGesture(object):
    def __init__(self, gesture):
        #print "LeapGesture: " + str(gesture.id) + " " + str(gesture.type)
        self.id = gesture.id
        self.type = int(gesture.type)
        self.state = int(gesture.state)
        self.duration = gesture.duration
        self.duration_seconds = gesture.duration_seconds
        self.pointables = create_leap_data_factory(gesture.pointables, LeapPointable)
        self.hands = create_leap_data_factory(gesture.hands, LeapHand)
        
    def __str__(self):
        buffer = ""
        buffer += "id: " + str(self.id) + "\n"
        buffer += "type: "
        if self.type == LeapGestureType.TYPE_SWIPE:
            buffer += "TYPE_SWIPE" + "\n"
        elif self.type == LeapGestureType.TYPE_CIRCLE:
            buffer += "TYPE_CIRCLE" + "\n"
        elif self.type == LeapGestureType.TYPE_SCREEN_TAP:
            buffer += "TYPE_SCREEN_TAP" + "\n"
        elif self.type == LeapGestureType.TYPE_KEY_TAP:
            buffer += "TYPE_KEY_TAP" + "\n"
        else:
            buffer += "STATE_INVALID" + "\n"
        buffer += "state: "
        if self.state == LeapGestureState.STATE_START:
            buffer += "STATE_START" + "\n"
        elif self.state == LeapGestureState.STATE_UPDATE:
            buffer += "STATE_UPDATE" + "\n"
        elif self.state == LeapGestureState.STATE_STOP:
            buffer += "STATE_STOP" + "\n"
        else:
            buffer += "STATE_INVALID" + "\n"
            
        buffer += "duration: " + str(self.duration) + "\n"
        buffer += "duration_seconds: " + str(self.duration_seconds) + "\n"
        
        #pollutes the output
#         buffer += "hands: " + "\n"
#         for h in self.hands:
#             buffer += str(h)  + "\n"
#         
#         buffer += "pointables: " + "\n"
#         for p in self.pointables:
#             buffer += str(p)  + "\n"
            
        return buffer
    
        
def create_leap_data_factory(pointers_raw, pointer_type):
    pointers = []
    for i in range(0,len(pointers_raw)):
        pointers.append(pointer_type(pointers_raw[i]))
    return pointers


class LeapHand(object):

    def __init__(self, hand):
        self.id = hand.id
        self.pointables = create_leap_data_factory(hand.pointables, LeapPointable)
        self.fingers = create_leap_data_factory(hand.fingers, LeapFinger)
        self.tools = create_leap_data_factory(hand.tools, LeapTool)
        self.palm_position = LeapVec3(hand.palm_position)
        self.palm_velocity = LeapVec3(hand.palm_velocity)
        self.palm_normal = LeapVec3(hand.palm_normal)
        self.direction = LeapVec3(hand.direction)
        self.is_valid = hand.is_valid
        self.sphere_center = LeapVec3(hand.sphere_center)
        self.sphere_radius = hand.sphere_radius
        
    def __str__(self):
        buffer = ""
        buffer += "id: " + str(self.id) + "\n"
        buffer += " fingers" + "\n"
        for f in self.fingers:
            buffer += "  " + str(f)  + "\n"
        buffer += " pointables" + "\n"
        for p in self.pointables:
            buffer += "  " + str(p)  + "\n"
        buffer += " tools" + "\n"
        for p in self.tools:
            buffer += "  " + str(p)  + "\n"
        buffer += " palm_position: " +  str(self.palm_position)  + "\n"
        buffer += " palm_velocity: " +  str(self.palm_velocity) + "\n"
        buffer += " palm_normal: " +  str(self.palm_normal) + "\n"
        buffer += " direction: " +  str(self.direction) + "\n"
        buffer += " is_valid: " +  str(self.is_valid) + "\n"
        buffer += " sphere_center: " +  str(self.sphere_center) + "\n"
        buffer += " sphere_radius: " +  str(self.sphere_radius) + "\n"
        
        return buffer

class LeapFrame(object):
    
    def __init__(self, frame):
        self.id = frame.id
        self.timestamp = frame.timestamp
        self.pointables = create_leap_data_factory(frame.pointables, LeapPointable)
        self.fingers = create_leap_data_factory(frame.fingers, LeapFinger)
        self.tools = create_leap_data_factory(frame.tools, LeapTool)
        self.hands = create_leap_data_factory(frame.hands, LeapHand)
        self.gestures = create_leap_data_factory(frame.gestures(), LeapGesture)
        self.is_valid = frame.is_valid
        
        #print "num gestures: " + str(len(self.gestures))
#        for gesture in self.gestures:
#            print " " + str(gesture)
        
    def __str__(self):
        buffer = ""
        buffer += "LeapFrame: " + str(self.id) + "\n"
        buffer += "Hands: " + "\n"
        for h in self.hands:
            buffer += str(h)  + "\n"
            
        buffer += "Fingers: " + "\n"
        for f in self.fingers:
            buffer += str(f) + "\n"
            
        buffer += "Pointables: " + "\n"
        for p in self.pointables:
            buffer += str(p) + "\n"
            
        buffer += "Tools: " + "\n"
        for t in self.tools:
            buffer += str(t) + "\n"
        
        return buffer
    


class LeapMotionInputListener(Leap.Listener):

    def __init__(self, message_queue):
        Leap.Listener.__init__(self)
        self._message_queue = message_queue

    def on_init(self, controller):
        print "Initialized"

    def on_connect(self, controller):
        print "Connected"

    def on_disconnect(self, controller):
        print "Disconnected"

    def on_exit(self, controller):
        print "Exited"

    def on_frame(self, controller):
        
        #since the controller is removed before the script is closed (happens in the main thread
        #this try/except will catch potential exceptions
        #try:
            raw_frame = controller.frame()
            leap_frame = LeapFrame(raw_frame)
            if self._message_queue!=None:
                self._message_queue.put(leap_frame)
        #except NoneType: 
        #    pass
        
    
class AVLeapMotionFrameListener(avango.script.Script):
    
    AVLeapFrame = avango.script.SFObject()
    
    def __init__(self):
        self.super(AVLeapMotionFrameListener).__init__()
        self.always_evaluate(True)
        self.AVLeapFrame.value = avango.utils.AVLeapFrame()
        
        self._message_queue = None;
        
    def set_message_queue(self, message_queue):
        self._message_queue = message_queue
        
    def evaluate(self):
        if self._message_queue is None:
            return
        
        while not self._message_queue.empty():
            frame = self._message_queue.get_nowait()
            
            #update the fields
            self.AVLeapFrame.value.update_with_leap_data(frame)
            
            self.AVLeapFrame.touch()
            #completely override
            #av_leap_frame = avango.utils.AVLeapFrame()
            #av_leap_frame.init_with_leap_data(frame)
            #self.AVLeapFrame.value = av_leap_frame        


def create_leapmotion_listener(enable_gestures):
    message_queue = Queue.Queue()
    listener = LeapMotionInputListener(message_queue)
    keep_alive.append(listener)
    
    controller = Leap.Controller()
    
    if enable_gestures:
        controller.enable_gesture(Leap.Gesture.TYPE_CIRCLE, True);
        controller.enable_gesture(Leap.Gesture.TYPE_SWIPE, True);
        controller.enable_gesture(Leap.Gesture.TYPE_KEY_TAP, True);
        controller.enable_gesture(Leap.Gesture.TYPE_SCREEN_TAP, True);
        
    
    controller.add_listener(listener)
    keep_alive.append(controller)
    
    av_leapmotion_listener = AVLeapMotionFrameListener()
    av_leapmotion_listener.set_message_queue(message_queue)

    return av_leapmotion_listener

def shutdown_leapmotion_listener():
    self.keep_alive[1].remove_listener(self.keep_alive[0])
    
    
