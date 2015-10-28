import avango
import avango.script
import code
import threading
import sys
import signal
import queue
import os

try:
    import pyreadline as readline
except ImportError:
    import readline

import rlcompleter

print_green = '\033[1;32m'
print_reset = '\033[0m'


class GuaVE(avango.script.Script):

    Prompt = avango.SFString()
    HistoryFile = avango.SFString()

    def __init__(self):
        self.super(GuaVE).__init__()
        self.always_evaluate(True)

        self.__input_queue = queue.Queue()
        self.__input_lock = threading.Lock()
        self.__vars = {}
        self.Prompt.value = "gua> "
        self.HistoryFile.value = os.path.expanduser("~/.guahistory")

    def start(self, locals, globals, show_banner=True):

        self.__vars = globals.copy()
        self.__vars.update(locals)

        self.__shell = code.InteractiveConsole(self.__vars)

        print("")

        if show_banner:
            print("")
            print("                                             _       ")
            print("                                            | |      ")
            print("  __ _ _   _  __ _  ___ __ _ _ __ ___   ___ | | ___  ")
            print(" / _` | | | |/ _` |/ __/ _` | '_ ` _ \ / _ \| |/ _ \ ")
            print("| (_| | |_| | (_| | (_| (_| | | | | | | (_) | |  __/ ")
            print(" \__, |\__,_|\__,_|\___\__,_|_| |_| |_|\___/|_|\___| ")
            print("  __/ |                                              ")
            print(" |___/                                               ")
            print("")
            print("")
            print("")
            print(print_green +
                  "Welcome to GuaVE, the guacamole virtual environment!" +
                  print_reset)
            print("")
            print("Press Ctrl-D to exit to exit GuaVE.")
            print("----------------------------------------------------")

        self.__input_thread = threading.Thread(target=self.__read_input)
        self.__input_thread.daemon = True
        self.__input_thread.start()

        signal.signal(signal.SIGINT, self.__signal_handler)

    def list_variables(self):
        vars = self.__vars.keys()
        vars.sort()
        for v in vars:
            print(v)

    def evaluate(self):
        while (not self.__input_queue.empty()):
            # clear line
            sys.stdout.write('\r\033[2K')
            sys.stdout.flush()

            self.__shell.push(self.__input_queue.get())

            #write new prompt
            sys.stdout.write(print_green + self.Prompt.value + print_reset)
            sys.stdout.flush()

            readline.write_history_file(self.HistoryFile.value)

    def __signal_handler(self, signal, frame):
        print("Bye!")
        sys.exit(0)

    def __read_input(self):

        readline.set_completer(rlcompleter.Completer(self.__vars).complete)
        readline.parse_and_bind("tab: complete")

        if os.path.exists(self.HistoryFile.value):
            readline.read_history_file(self.HistoryFile.value)

        while (True):
            try:
                line = input('\001' + print_green + '\002' + self.Prompt.value
                             + '\001' + print_reset + '\002')
                self.__input_queue.put(line)
            except EOFError:
                print("Bye")  #, press Ctrl-C to kill guacamole...")
                os._exit(0)
            except IOError as err:
                print("I/O error: {0}".format(err))
                os._exit(1)
            except:
                print("Unexpected error:", sys.exc_info()[0])
                os._exit(1)
