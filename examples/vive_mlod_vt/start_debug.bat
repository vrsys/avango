@echo off

set AVANGO_DIR=C:\Data\Repositories\project_hmd_eyes\avango
set Path=%Path%;%AVANGO_DIR%\lib\Release;
set PYTHONPATH=%PYTHONPATH%;%AVANGO_DIR%\lib\python3.5;%AVANGO_DIR%\lib\python3.5\avango\daemon;%AVANGO_DIR%\examples

start "avango" cmd /K python_d -m pdb -c continue main.py