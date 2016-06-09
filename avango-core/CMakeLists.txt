###############################################################################
# write Config.h
###############################################################################

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/Config.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/Config.h )

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_CORE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/*.cpp
  src/avango/actions/*.cpp
  src/avango/fields/*.cpp
  src/avango/interface/*.cpp
  src/avango/logging/*.cpp
  src/avango/nodes/*.cpp
  src/avango/streams/*.cpp
  src/avango/support/*.cpp
  src/avango/types/*.cpp
  include/avango/*.h
  include/avango/logging/*.h
)

###############################################################################
# optional sources for distribution support
###############################################################################
if (${AVANGO_DISTRIBUTION_SUPPORT})
  file(GLOB AVANGO_CORE_DISTRIBUTION_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        src/avango/network/Helper.h
        src/avango/network/NetNodeClient.h
        src/avango/network/NetNodeServer.h
        src/avango/network/UpcallSerializer.h
        src/avango/network/Helper.cpp
        src/avango/network/Msg.cpp
        src/avango/network/NetID.cpp
        src/avango/network/NetInfo.cpp
        src/avango/network/NetLock.cpp
        src/avango/network/NetMap.cpp
        src/avango/network/NetNodeClient.cpp
        src/avango/network/NetNode.cpp
        src/avango/network/NetNodeServer.cpp
  )
endif (${AVANGO_DISTRIBUTION_SUPPORT})

link_directories(${LIB_PATHS})

add_library( avango SHARED
    	     ${AVANGO_CORE_SRC}
             ${AVANGO_CORE_DISTRIBUTION_SRC}
)

include_directories( ${INCLUDE_PATHS} 
                     include
                   )

###############################################################################
# set preprocessor configuration
###############################################################################

if (MSVC)
  set(AVANGO_BUILD_FLAGS "${AVANGO_BUILD_FLAGS} -D AV_LIBRARY")

  set_target_properties( avango PROPERTIES COMPILE_FLAGS ${AVANGO_BUILD_FLAGS})
  set_target_properties( avango PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")
endif (MSVC)

target_link_libraries( avango
  ${_AVANGO_CORE_LIBRARIES}
  debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE} 
  debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE} 
)

if (AVANGO_DISTRIBUTION_SUPPORT)
  target_link_libraries( avango optimized ${ZMQ_LIBRARY} debug ${ZMQ_LIBRARY_DEBUG})
  if (MSVC)
    target_link_libraries( avango Ws2_32.lib)
  endif(MSVC)
endif (AVANGO_DISTRIBUTION_SUPPORT)


###############################################################################
# targets for unittesting
###############################################################################
if (AVANGO_UNITTESTS)
  add_subdirectory(src/avango/nodes/tests)
  add_subdirectory(src/avango/interface/tests)
  add_subdirectory(src/avango/types/tests)
  add_subdirectory(src/avango/fields/tests)
  add_subdirectory(src/avango/network/tests)
  add_subdirectory(src/avango/logging/tests)
endif (AVANGO_UNITTESTS)

###############################################################################
# rename libaries in windows
###############################################################################
if (MSVC)
  post_build_install_target(avango ${AVANGO_INSTALL_DIRECTORY}/avango/avango.dll)
elseif (UNIX)
  post_build_install_target(avango ${AVANGO_INSTALL_DIRECTORY}/avango/libavango.so)
endif (MSVC)
