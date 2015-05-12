#!/usr/bin/python
import sys
import jsonloader


def start():

    loader = jsonloader.jsonloader()
    app = loader.create_application_from_json(sys.argv[1])
    app.run()


if __name__ == '__main__':
    start()
