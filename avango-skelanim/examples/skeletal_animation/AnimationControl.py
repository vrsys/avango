import avango
import avango.script

class AnimationControl(avango.script.Script):

    _animation_node = None
    _timer = avango.nodes.TimeSensor()
    _state = 1 # 1 = play; 2 = blending

    _last_animation = None
    _current_animation = None

    _last_blending_start = 0.0
    _last_looping = False
    _current_blending_start = 0.0
    _current_looping = False
    _is_looping = False
    
    _blending_factor = 1.0
    _blending_duration = 0.5
    _blending_end = 0.0


    def __init__(self):

        self.super(AnimationControl).__init__()

    def my_constructor(self, animation_node):

        self._animation_node = animation_node

        self.play("idle")
        self.play("idle")

        self.always_evaluate(True)

        self._animation_node.Time1.value = 0.5
        self._animation_node.Time2.value = 0.5

    def get_current_animation(self):
        return self._current_animation

    def get_last_animation(self):
        return self._last_animation

    def get_blending_factor(self):
        return self._blending_factor

    def is_looping(self):
        return self._is_looping

    def play(self, animation_name, loop_mode = True):
        self._last_animation = self._current_animation
        self._current_animation = animation_name

        self._last_blending_start = self._current_blending_start
        self._current_blending_start = self._timer.Time.value
        self._blending_end = self._current_blending_start

        self._last_looping = self._current_looping
        self._current_looping = loop_mode

        self._animation_node.Animation1.value = self._animation_node.Animation2.value
        self._animation_node.Animation2.value = animation_name

        self._animation_node.BlendFactor.value = self._blending_factor = 1.0
        self._state = 1

    def blend_to(self, animation_name, blending_duration = 0.5, loop_mode = True):

        self._last_animation = self._current_animation
        self._current_animation = animation_name

        self._last_blending_start = self._current_blending_start
        self._current_blending_start = self._timer.Time.value
        self._blending_end = self._current_blending_start + blending_duration

        self._blending_duration = blending_duration

        self._last_looping = self._current_looping
        self._current_looping = loop_mode
        self._is_looping = True

        self._animation_node.Animation1.value = self._animation_node.Animation2.value
        self._animation_node.Animation2.value = animation_name

        self._animation_node.BlendFactor.value = self._blending_factor = (self._timer.Time.value - self._current_blending_start) / self._blending_duration
        
        self._state = 2


    def evaluate(self):
        
        if not self._last_looping and self._timer.Time.value > self._last_blending_start + self._animation_node.get_duration(self._last_animation):
            self._animation_node.Time1.value = self._animation_node.get_duration(self._last_animation) * 0.999999
        else:
            self._animation_node.Time1.value = self._timer.Time.value - self._last_blending_start

        if not self._current_looping and self._timer.Time.value > self._current_blending_start + self._animation_node.get_duration(self._current_animation):
            self._animation_node.Time2.value = self._animation_node.get_duration(self._current_animation) * 0.999999
            self._is_looping = False
        else:
            self._animation_node.Time2.value = self._timer.Time.value - self._current_blending_start

        if self._state == 2:

            self._blending_factor = 1.0 - (self._blending_end - self._timer.Time.value) / self._blending_duration
            if self._blending_factor >= 1.0:

                self._state = 1
                self._blending_factor = 1.0

            self._animation_node.BlendFactor.value = self._blending_factor


