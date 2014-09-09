#avango + guacamole + python
set AVANGO_DIR=%cd%\..\..
set Path=%Path%;%AVANGO_DIR%\lib\avango
set PYTHONPATH=%PYTHONPATH%;%AVANGO_DIR%\lib\Release\avango;%PYTHONPATH%;%AVANGO_DIR%\lib\Release\avango\daemon

start "python3 ../examples_common/daemon.py"

start "python3 main.py"
