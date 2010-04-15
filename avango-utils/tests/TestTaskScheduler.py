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
import avango.osg
import avango.utils
import unittest
import time


class TimeDeltaIncrement(avango.utils.TaskSchedulerTask):
    Delta = avango.SFInt()
    Value = avango.SFInt()
    
    def __init__(self):
        self.super(TimeDeltaIncrement).__init__()
        
        self.Delta.value = 1
        self.Value.value = 0
    
    def on_active(self):
        self.Value.value += self.Delta.value
                  
        
class TaskSchedulerTestCase(unittest.TestCase):
    def testMakeInstance(self):
        self.assert_(avango.utils.nodes.TaskScheduler())
        self.assert_(avango.utils.nodes.TaskSchedulerTask())
        
    def testTaskInvokation(self):
        time_sensor = avango.nodes.TimeSensor()
        task_scheduler = avango.utils.nodes.TaskScheduler()
        task_scheduler.TimeIn.connect_from(time_sensor.Time)
        
        task1 = TimeDeltaIncrement()
        task1.TimeDelta.value = 0.1
        
        task2 = TimeDeltaIncrement()
        task2.TimeDelta.value = 0.2
        
        task_scheduler.add_delayed_task(task1)
        task_scheduler.add_delayed_task(task2)
        
        avango.evaluate()
        
        self.assertEqual(task1.Value.value, 0)
        self.assertEqual(task2.Value.value, 0)
        self.assertEqual(task_scheduler.get_num_tasks(),2)
        time.sleep(0.05)
        avango.evaluate()
        
        self.assertEqual(task1.Value.value, 0)
        self.assertEqual(task2.Value.value, 0)
        self.assertEqual(task_scheduler.get_num_tasks(),2)
        time.sleep(0.06)
        avango.evaluate()
        
        self.assertEqual(task1.Value.value, 1)
        self.assertEqual(task2.Value.value, 0)
        self.assertEqual(task_scheduler.get_num_tasks(),1)
        time.sleep(0.1)
        avango.evaluate()
        
        self.assertEqual(task1.Value.value, 1)
        self.assertEqual(task2.Value.value, 1)
        self.assertEqual(task_scheduler.get_num_tasks(),0)
        
        
def Suite():
   suite = unittest.TestLoader().loadTestsFromTestCase(TaskSchedulerTestCase)
   return suite
