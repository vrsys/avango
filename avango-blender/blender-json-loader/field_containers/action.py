import avango
import avango.gua
import avango.script


class Action(avango.script.Script):

    # Input
    Keyframes = avango.gua.MFMatrix4()
    TimeStamps = avango.MFFloat()
    Time = avango.SFFloat()

    # Output
    OutTransform = avango.gua.SFMatrix4()

    def __init__(self):
        self.super(Action).__init__()
        self.Time.value = 0.0

        #Stuff
        self.frame_count = 0
        self.duration = 0.0
        self.loop = False

    def constructor(self, json_keyframes):
        for frame in json_keyframes:

            matrix_list = frame["transform"]
            matrix = load_transform_matrix(matrix_list)
            time_stamp = frame["time"]

            self.Keyframes.value.append(matrix)
            self.TimeStamps.value.append(time_stamp)

        self.frame_count = len(self.Keyframes.value)
        self.duration = self.TimeStamps.value[self.frame_count - 1]

    def evaluate(self):
        frame = self.find_key_frame()
        if not frame == -1:
            self.OutTransform.value = self.interpolate(frame)
        else:
            self.OutTransform.value = avango.gua.make_identity_mat()

    def get_time(self):
        if self.loop:
            return self.Time.value % self.duration
        return self.Time.value

    def find_key_frame(self):
        time = self.get_time()
        for i in range(self.frame_count):
            if time < self.TimeStamps.value[i]:
                return i
            elif time > self.duration:
                return self.frame_count - 1
        return -1

    def interpolate(self, frame):
        time = self.get_time()
        if frame == 0 or time > self.duration:
            return self.Keyframes.value[frame]
        else:
            time_prev = self.TimeStamps.value[frame-1]
            time_next = self.TimeStamps.value[frame]
            delta = time_next-time_prev
            current = time - time_prev
            factor = current / delta
            return self.Keyframes.value[frame-1] * (1-factor) +\
                self.Keyframes.value[frame] * (factor)


def load_transform_matrix(matrix_list):
    transform = avango.gua.make_identity_mat()

    for element in range(len(matrix_list)):
        transform.set_element(
            int(element/4),
            element % 4,
            matrix_list[element],
            )

    return transform
