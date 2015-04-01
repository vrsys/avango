import avango
import avango.script
from enum import Enum

class State(Enum):
    blending = 0
    play = 1

class AnimationControl(avango.script.Script):

    def __init__(self):

        self.super(AnimationControl).__init__()

        self._animation_node = None
        self._timer = avango.nodes.TimeSensor()

        self._state = State.play

        self._last_animation = None
        self._current_animation = None

        self._last_blending_start = 0.0
        self._last_looping = False
        self._current_blending_start = 0.0
        self._current_looping = False
        self._first_play = False
        
        self._blending_factor = 1.0
        self._blending_duration = 0.5
        self._blending_end = 0.0

    def my_constructor(self, animation_node):
        self._timer.ReferenceTime.value = self._timer.RealTime.value
        self._animation_node = animation_node
        # set some anim because evaluate is called before anim can be set from outside
        self.switch_to("idle")
        self.switch_to("idle")

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
        return self._first_play

    def switch_to(self, animation_name, loop_mode = True):
        self._last_animation = self._current_animation
        self._current_animation = animation_name

        self._last_blending_start = self._current_blending_start
        self._current_blending_start = self._timer.Time.value
        self._blending_end = self._current_blending_start

        self._last_looping = self._current_looping
        self._current_looping = loop_mode

        self._animation_node.Animation1.value = self._animation_node.Animation2.value
        self._animation_node.Animation2.value = animation_name

        # reset time
        self._timer.ReferenceTime.value = self._current_blending_start + self._timer.ReferenceTime.value
        self._current_blending_start = 0

        self._animation_node.BlendFactor.value = self._blending_factor = 1.0
        
        self._state = State.play

    def blend_to(self, animation_name, blending_duration = 0.5, loop_mode = True):

        self._last_animation = self._current_animation
        self._current_animation = animation_name

        self._last_blending_start = self._current_blending_start
        self._current_blending_start = self._timer.Time.value
        self._blending_end = self._current_blending_start + blending_duration

        self._blending_duration = blending_duration

        self._last_looping = self._current_looping
        self._current_looping = loop_mode
        self._first_play = True

        self._animation_node.Animation1.value = self._animation_node.Animation2.value
        self._animation_node.Animation2.value = animation_name

        self._animation_node.BlendFactor.value = self._blending_factor = (self._timer.Time.value - self._current_blending_start) / self._blending_duration
        
        self._state = State.blending

        self.evaluate()

    def evaluate(self):

        # always update time of current anim
        if not self._current_looping and self._timer.Time.value > self._current_blending_start + self._animation_node.get_duration(self._current_animation):
            self._animation_node.Time2.value = self._animation_node.get_duration(self._current_animation) * 0.999999
            self._first_play = False
        else:
            self._animation_node.Time2.value = (self._timer.Time.value - self._current_blending_start)

        if self._state == State.blending:
            # update time for old animation only while blending
            if not self._last_looping and self._timer.Time.value > self._last_blending_start + self._animation_node.get_duration(self._last_animation):
                self._animation_node.Time1.value = self._animation_node.get_duration(self._last_animation) * 0.999999
            else:
                self._animation_node.Time1.value = (self._timer.Time.value - self._last_blending_start)

            self._blending_factor = 1.0 - (self._blending_end - self._timer.Time.value) / self._blending_duration
            # if blending is finished, switch to normal play
            if self._blending_factor >= 1.0:

                self._state = State.play
                self._blending_factor = 1.0
                # reset time
                self._timer.ReferenceTime.value = self._current_blending_start + self._timer.ReferenceTime.value
                self._current_blending_start = 0

            self._animation_node.BlendFactor.value = self._blending_factor