
import avango.moving
import avango.display
import avango.utils

import time
import signal
import sys
import datetime
import random

argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = False
keyboard = avango.display.make_dominant_user_device(interface="Keyboard")

def signal_handler(signal, frame):
    avango.utils.shutdown_leapmotion_listener()
    
signal.signal(signal.SIGINT, signal_handler)


class Stroke(object):
    MAX_POINTS_PER_STROKE = 100
    
    def __init__(self, id):
        self.id = id
        self.points = []
         
    def add_position(self, position):
        self.points.append(position)
        
    def check_removal(self):
        if len(self.points) > Stroke.MAX_POINTS_PER_STROKE:
            self.points = self.points[len(self.points)-Stroke.MAX_POINTS_PER_STROKE:len(self.points)]
        
        
def create_sphere(pos, rad, color):
    sphere = avango.osg.nodes.Sphere(Radius = rad, Color = color, Matrix = avango.osg.make_trans_mat(pos))
    sphere.StateSet.value = avango.osg.nodes.StateSet(RenderBin = 11, RenderingHint = 2, BlendMode = 1)
    return sphere

def create_line_node(p1, p2, width, color):
    geometry = avango.osg.nodes.Geometry(Mode = 2, ColorBinding = 1, ColorArray = [color], VertexArray = [p1,p2] )
    geode = avango.osg.nodes.Geode(Drawables=[geometry])
    geode.StateSet.value = avango.osg.nodes.StateSet(RenderBin = 11, RenderingHint = 2, BlendMode = 1, LightingMode = 0, LineWidth=avango.osg.nodes.LineWidth(Width=width))
    return geode

def create_linestrip_node(points, width, color):
    geometry = avango.osg.nodes.Geometry(Mode = 3, ColorBinding = 1, ColorArray = [color], VertexArray = points )
    geode = avango.osg.nodes.Geode(Drawables=[geometry])
    geode.StateSet.value = avango.osg.nodes.StateSet(RenderBin = 11, RenderingHint = 2, BlendMode = 1, LightingMode = 0, LineWidth=avango.osg.nodes.LineWidth(Width=width))
    return geode

def convert_leap_position(pos):
    return avango.osg.Vec3(pos.x/100, pos.y/100, pos.z/50) 


class GestureProcessor(avango.script.Script):
    
    Enable = avango.SFBool()
    GesturesIn = avango.script.MFObject()
    ColorOutCircleGesture = avango.osg.SFVec4()
    ColorOutSwipeGesture = avango.osg.SFVec4()
    
    def __init__(self):
        self.super(GestureProcessor).__init__()
        self.ColorOutCircleGesture.value = avango.osg.Vec4(1,0,0,0.7)
        self.ColorOutSwipeGesture.value = avango.osg.Vec4(0,1,0,1)
        
    def evaluate(self):
        if not self.Enable.value:
            return
        
        for gesture in self.GesturesIn.value:
            r = random.random()
            g = random.random()
            b = random.random()
            
            #filter circle gestures
            if gesture.Type.value == avango.utils.LeapGestureType.TYPE_CIRCLE \
                and gesture.State.value == avango.utils.LeapGestureState.STATE_STOP :
                    a = 0.7
                    self.ColorOutCircleGesture.value = avango.osg.Vec4(r,g,b,a)
            
            elif gesture.Type.value == avango.utils.LeapGestureType.TYPE_SWIPE \
                and gesture.State.value == avango.utils.LeapGestureState.STATE_STOP :
                    a = 1
                    self.ColorOutSwipeGesture.value = avango.osg.Vec4(r,g,b,a)
         

class LeapMotionDebugVisualizer(avango.script.Script):
    
    Root = avango.osg.SFGroup()
    AVLeapFrame = avango.script.SFObject()
    
    PalmColor = avango.osg.SFVec4()
    FingerColor = avango.osg.SFVec4()
    
    DetailVis = avango.SFBool()
    DrawingVis = avango.SFBool()
    
    def __init__(self):
        self.super(LeapMotionDebugVisualizer).__init__()
        self.Root.value = avango.osg.nodes.Group()
        self.AVLeapFrame.value = avango.utils.AVLeapFrame()
        
        self.DetailVis.value = False
        self.DrawingVis.value = False
        
        self.pointer_map = {}
        
    def evaluate(self):
        
        if self.AVLeapFrame.value is None:
            return
        
        av_leap_frame = self.AVLeapFrame.value
        
        self.Root.value.Children.value = []
        
        for hand in av_leap_frame.Hands.value:
            palm_pos = convert_leap_position(hand.PalmPosition.value)
            self.Root.value.Children.value.append(create_sphere(palm_pos, 0.2, self.PalmColor.value))

            if self.DetailVis.value: 
                palm_sphere = create_sphere(convert_leap_position(hand.SphereCenter.value), hand.SphereRadius.value / 100, avango.osg.Vec4(1,0,1,1))
                palm_sphere.StateSet.value.WireframeMode.value = 1
                self.Root.value.Children.value.append(palm_sphere)
        
            for pointable in hand.Pointables.value:
                pos = convert_leap_position(pointable.TipPositionStabilized.value)
                
                self.Root.value.Children.value.append(create_sphere(pos, 0.1, self.FingerColor.value))
                self.Root.value.Children.value.append(create_line_node(palm_pos, pos, 2, avango.osg.Vec4(1,1,0,1)))
                
                if pointable.ID.value in self.pointer_map:
                    stroke = self.pointer_map[pointable.ID.value]
                else:
                    stroke = Stroke(pointable.ID.value)
                    self.pointer_map[pointable.ID.value] = stroke
                
                if self.DrawingVis.value: 
                    stroke.add_position(pos)
                    
                    for k , v in sorted(self.pointer_map.iteritems()):
                        stroke.check_removal()
                        self.Root.value.Children.value.append(create_linestrip_node(stroke.points, 3, avango.osg.Vec4(0,1,1,0.5)))
        
        #remove old pointables
        known_ids_set = set()
        for p in self.AVLeapFrame.value.Pointables.value:
            known_ids_set.add(p.ID.value)
        diff = set( self.pointer_map.keys()) - known_ids_set
        for id in diff: 
            if id in self.pointer_map:
                del self.pointer_map[id]

av_leapmotion_listener = avango.utils.create_leapmotion_listener(enable_gestures=True)
leap_visualizer = LeapMotionDebugVisualizer()
leap_visualizer.AVLeapFrame.connect_weak_from(av_leapmotion_listener.AVLeapFrame)

#full visualization
leap_visualizer.DetailVis.connect_from(avango.utils.make_key_toggle_trigger_alternate(keyboard.KeyF, False))

#drawing
leap_visualizer.DrawingVis.connect_from(avango.utils.make_key_toggle_trigger_alternate(keyboard.KeyD, False))

gesture_processor = GestureProcessor()
gesture_processor.Enable.connect_from(avango.utils.make_key_toggle_trigger_alternate(keyboard.KeyG, False))
gesture_processor.GesturesIn.connect_from(av_leapmotion_listener.AVLeapFrame.value.Gestures)
leap_visualizer.PalmColor.connect_from(gesture_processor.ColorOutCircleGesture)
leap_visualizer.FingerColor.connect_from(gesture_processor.ColorOutSwipeGesture)

#HACK Otherwise the LeapMotion thread will get no CPU time.  
class Breaker(avango.script.Script):
    
    def __init__(self):
        self.super(Breaker).__init__()
        self.always_evaluate(True)
    
    def evaluate(self):
        time.sleep(0.0016)
        
b = Breaker()

# set up scene graph
view.Root.value = avango.osg.nodes.MatrixTransform(Matrix=avango.osg.make_trans_mat(0,0,-16))
view.Root.value.Children.value.append(leap_visualizer.Root.value)

# run evaluation and render loop
avango.display.run()