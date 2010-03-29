import avango
import avango.script

class TaskScheduler(avango.script.Script):
    Time = avango.SFDouble()

    def __init__(self):
        self.tasks = []
        self.Time.value = 0

    def cleanup(self):
        self.disconnect_and_clear_all_fields()
        self.tasks = []

    def evaluate(self):
        time = self.Time.value
        tasks = self.tasks
        self.tasks = []
        for task in tasks:
            if time > task[0]:
                task[1]()
            else:
                self.tasks.append(task)
        if len(self.tasks) == 0:
            self.Time.disconnect()

    def delay_call(self, time, callback):
        if len(self.tasks) == 0:
            self.Time.connect_from(time_sensor.Time)
        self.tasks.append((self.Time.value + time, callback))