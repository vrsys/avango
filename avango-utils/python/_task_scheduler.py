# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import avango
import avango.script

class TaskSchedulerTask(avango.script.Script):
    TimeDelta = avango.SFDouble()
    
    def __init__(self):
        self.super(TaskSchedulerTask).__init__()
        
        self.TimeDelta.value = 1.0
        
    def on_active(self):
        pass
    
class TaskScheduler(avango.script.Script):
    TimeIn = avango.SFDouble()

    def __init__(self):
        self.super(TaskScheduler).__init__()
        
        self.__tasks = []
        self.TimeIn.value = 0

    def evaluate(self):
        time = self.TimeIn.value
        tasks = self.__tasks
        
        n = len(self.__tasks)
        while n != 0:
            n = n - 1
            task_exec_time = self.__tasks[n][0]
            
            if time > task_exec_time:
                task = self.__tasks[n][1]
                task.on_active()
                del self.__tasks[n]

    def get_num_tasks(self):
        return len(self.__tasks)

    def add_delayed_task(self, task):
        self.__tasks.append([self.TimeIn.value + task.TimeDelta.value, task])
        