import avango
import avango.script
from enum import Enum

class State(Enum):
    blending = 0
    play = 1

class AnimationConfig():
    
    def __init__(self, name, loop = True, speed = 1.0, duration = 0.0):
        self.name = name
        self.speed = speed
        self.duration = duration
        self.loop = loop


class AnimationControl(avango.script.Script):

    def __init__(self):

        self.super(AnimationControl).__init__()

        self._animation_node = None
        self._timer = avango.nodes.TimeSensor()

        self._state = State.play

        self._last_animation = None
        self._current_animation = None

        self._last_blending_start = 0.0
        self._current_blending_start = 0.0
        self._first_play = False
        
        self._blending_factor = 1.0
        self._blending_duration = 0.5
        self._blending_end = 0.0

    def my_constructor(self, animation_node):
        self._timer.ReferenceTime.value = self._timer.RealTime.value
        self._animation_node = animation_node
        # set some anim because evaluate is called before anim can be set from outside
        self.switch_to(AnimationConfig("idle"))
        self.switch_to(AnimationConfig("idle"))

        self.always_evaluate(True)

        self._animation_node.Time1.value = 0.5
        self._animation_node.Time2.value = 0.5

    def get_current_animation(self):
        return self._current_animation.name

    def get_last_animation(self):
        return self._last_animation.name

    def get_blending_factor(self):
        return self._blending_factor

    def is_looping(self):
        return self._first_play

    def switch_to(self, anim_config):
        self._last_animation = self._current_animation
        self._current_animation = anim_config

        self._last_blending_start = self._current_blending_start
        self._current_blending_start = self._timer.Time.value
        self._blending_end = self._current_blending_start

        self._animation_node.Animation1.value = self._animation_node.Animation2.value
        self._animation_node.Animation2.value = anim_config.name

        # reset time
        self._timer.ReferenceTime.value = self._current_blending_start + self._timer.ReferenceTime.value
        self._current_blending_start = 0

        self._animation_node.BlendFactor.value = self._blending_factor = 1.0
        
        self._state = State.play

    def blend_to(self, anim_config, blending_duration = 0.5):

        self._last_animation = self._current_animation
        self._current_animation = anim_config

        self._last_blending_start = self._current_blending_start
        self._current_blending_start = self._timer.Time.value
        self._blending_end = self._current_blending_start + blending_duration

        self._blending_duration = blending_duration

        self._first_play = True

        self._animation_node.Animation1.value = self._animation_node.Animation2.value
        self._animation_node.Animation2.value = anim_config.name

        self._animation_node.BlendFactor.value = self._blending_factor = (self._timer.Time.value - self._current_blending_start) / self._blending_duration
        
        self._state = State.blending

        self.evaluate()

    def evaluate(self):

        # always update time of current anim
        if not self._current_animation.loop and self._timer.Time.value > self._current_blending_start + self._animation_node.get_duration(self._current_animation.name) / self._current_animation.speed:
            self._first_play = False
        else:
            self._animation_node.Time2.value = ((self._timer.Time.value - self._current_blending_start) / (self._animation_node.get_duration(self._current_animation.name) / self._current_animation.speed)) % 1

        if self._state == State.blending:
            # update time for old animation only while blending
            if self._last_animation.loop or self._timer.Time.value < self._last_blending_start + self._animation_node.get_duration(self._last_animation.name) / self._last_animation.speed:
                self._animation_node.Time1.value = ((self._timer.Time.value - self._last_blending_start)/ (self._animation_node.get_duration(self._last_animation.name) / self._last_animation.speed)) % 1

            self._blending_factor = 1.0 - (self._blending_end - self._timer.Time.value) / self._blending_duration
            # if blending is finished, switch to normal play
            if self._blending_factor >= 1.0:

                self._state = State.play
                self._blending_factor = 1.0
                # reset time
                self._timer.ReferenceTime.value = self._current_blending_start + self._timer.ReferenceTime.value
                self._current_blending_start = 0

            self._animation_node.BlendFactor.value = self._blending_factor