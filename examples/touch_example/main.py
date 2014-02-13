#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
# import avango.gua
from examples_common.GuaVE import GuaVE
from examples_common.device import TouchDevice

def start():

  touchy = TouchDevice()

  while True:
    avango.evaluate()


if __name__ == '__main__':
  start()

