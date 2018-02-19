set AVANGO_DIR=%cd%\..\..
set Path=%Path%;%AVANGO_DIR%\lib\Release
set PYTHONPATH=%PYTHONPATH%;%AVANGO_DIR%\lib\Python3.5;%PYTHONPATH%;%AVANGO_DIR%\lib\python3.5\avango\daemon;%AVANGO_DIR%\examples

start "python ../examples_common/daemon.py"

start "python main.py"
