taskkill /F /IM python.exe /T

set AVANGO_DIR=C:\Data\Repositories\avango
set Path=%Path%;%AVANGO_DIR%\lib\Release
set PYTHONPATH=%PYTHONPATH%;%AVANGO_DIR%\lib\Python3.5;%PYTHONPATH%;%AVANGO_DIR%\lib\python3.5\avango\daemon;%AVANGO_DIR%\examples

start cmd /K main.py