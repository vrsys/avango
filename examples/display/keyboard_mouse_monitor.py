from avango import display
import avango
import avango.script
import sys

def field_dump(script):
    buffer = ""
    for i in xrange(script._get_num_fields()):
        field = script._get_field(i)
        field_name = field._get_name()
        field_type = field._get_type()
        buffer += "Field: <"+ str(field_name) + "> of type: <" + str(field_type) + "> = <" + str(field.value) + ">\n"
    return buffer  

class MouseListener(avango.script.Script):
    
    MousePosNorm = avango.osg.SFVec2()
    MousePos = avango.osg.SFVec2()
    MouseButtonLeft = avango.SFBool()
    MouseButtonRight = avango.SFBool()
    
    def __init__(self):
        self.super(MouseListener).__init__()
        self.Name.value = "MouseListener"

    def evaluate(self):
        buffer = field_dump(self)
        print buffer
        
class KeyboardListener(avango.script.Script):
    
    KeyUp = avango.SFBool()
    KeyDown = avango.SFBool()
    KeyLeft = avango.SFBool()
    KeyRight = avango.SFBool()
    
    def __init__(self):
        self.super(KeyboardListener).__init__()
        self.Name.value = "KeyboardListener"
        
    def evaluate(self):
        buffer = field_dump(self)
        print buffer
        
argv = display.init(sys.argv)
view = display.make_view()
##create a keyboard device
keyboard = display.make_dominant_user_device(interface="Keyboard")
##create a mouse device
mouse = display.make_dominant_user_device(interface="Mouse")

if not keyboard or not mouse:
    print "The display <" + display.get_display_type() + "> does not support the device Keyboard and/or Mouse"

mouse_listener = MouseListener()
mouse_listener.MousePosNorm.connect_from(mouse.MousePosNorm)
mouse_listener.MousePos.connect_from(mouse.MousePos)
mouse_listener.MouseButtonLeft.connect_from(mouse.MouseButtonLeft)
mouse_listener.MouseButtonRight.connect_from(mouse.MouseButtonRight)

keyboard_listener = KeyboardListener()
keyboard_listener.KeyUp.connect_from(keyboard.KeyUp)
keyboard_listener.KeyDown.connect_from(keyboard.KeyDown)
keyboard_listener.KeyLeft.connect_from(keyboard.KeyLeft)
keyboard_listener.KeyRight.connect_from(keyboard.KeyRight)

sphere = avango.osg.nodes.Sphere(Radius=0.1, Matrix=avango.osg.make_trans_mat(0, 1.7, -1.2))
view.Root.value = sphere

display.run()
