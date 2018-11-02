avango
======
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/vrsys/avango?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

AVANGO is a free software framework designed for interactive, distributed applications. It supports a large range of displays from standard desktop applications to large-scale immersive Virtual Reality installations.

Please cite our work when using our software in your own research or publication.

https://www.uni-weimar.de/de/medien/professuren/vr/software/

Installation
------------

You need a NVIDIA GPU that supports OpenGL 4.2 and CUDA.

### Linux
The following guide serves as a step-by-step tutorial on how to build the dependencies and avango itself on Linux. It was tested on a fresh installation of Ubuntu 16.04, and the latest commit at the point of writing was `0264611`.

As a prerequisite, our rendering engine guacamole must be installed. Please follow the instructions on the [GitHub repository](https://github.com/vrsys/guacamole) for further instructions. The steps to install _avango_ afterwards are as follows:
  * `apt-get install libpython3.5-dev`
  * clone https://github.com/vrsys/avango (this repository)
  * adapt find script `cmake/modules/find_schism.cmake` such that `SCHISM_INCLUDE_SEARCH_DIRS` and `SCHISM_LIBRARY_SEARCH_DIRS` contain the paths created during the installation of schism in the guacamole installation tutorial
  * configure with CMake and set paths to dependencies as mentioned in the guacamole tutorial if not found automatically; in particular, set `GUACAMOLE_INCLUDE_DIRS` and `GUACAMOLE_LIBRARY_DIRS` to `install/include` and `install/lib` in the guacamole directory
  * when CMake can't find `libboost_python3` although it was installed, change `Boost_PYTHON3_LIBRARY_DEBUG` and `Boost_PYTHON3_LIBRARY_RELEASE` to `/usr/lib/x86_64-linux-gnu/libboost_python-py35.so`
  * CMake flags that were set to ON during writing of this tutorial: `AVANGO_DISTRIBUTION_SUPPORT`, `AVANGO_LOD_SUPPORT`, `AVANGO_PHYSICS_SUPPORT`, `AVANGO_RUNTIME_PROGRAM_COMPILATION`, `AVANGO_SKELANIM_SUPPORT`, `AVANGO_TV_3_SUPPORT`, `AVANGO_VIDEO3D_SUPPORT`
  * `make`

If everything worked well, you can start by running an example in the `examples/` directory, e.g. the `simple_example`.
