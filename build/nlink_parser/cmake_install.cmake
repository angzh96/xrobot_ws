# Install script for directory: /home/kinetic/xrobot_ws/src/nlink_parser

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/kinetic/xrobot_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/nlink_parser/msg" TYPE FILE FILES
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackAnchorframe0.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNode0.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNode1.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNode2.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNodeframe0.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNodeframe1.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNodeframe2.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackNodeframe3.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackTag.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackTagframe0.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/TofsenseCascade.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/TofsenseFrame0.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackAoaNode0.msg"
    "/home/kinetic/xrobot_ws/src/nlink_parser/msg/LinktrackAoaNodeframe0.msg"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/nlink_parser/cmake" TYPE FILE FILES "/home/kinetic/xrobot_ws/build/nlink_parser/catkin_generated/installspace/nlink_parser-msg-paths.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/kinetic/xrobot_ws/devel/include/nlink_parser")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/kinetic/xrobot_ws/devel/share/roseus/ros/nlink_parser")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/kinetic/xrobot_ws/devel/share/common-lisp/ros/nlink_parser")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/kinetic/xrobot_ws/devel/share/gennodejs/ros/nlink_parser")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  execute_process(COMMAND "/usr/bin/python" -m compileall "/home/kinetic/xrobot_ws/devel/lib/python2.7/dist-packages/nlink_parser")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/kinetic/xrobot_ws/devel/lib/python2.7/dist-packages/nlink_parser")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/kinetic/xrobot_ws/build/nlink_parser/catkin_generated/installspace/nlink_parser.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/nlink_parser/cmake" TYPE FILE FILES "/home/kinetic/xrobot_ws/build/nlink_parser/catkin_generated/installspace/nlink_parser-msg-extras.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/nlink_parser/cmake" TYPE FILE FILES
    "/home/kinetic/xrobot_ws/build/nlink_parser/catkin_generated/installspace/nlink_parserConfig.cmake"
    "/home/kinetic/xrobot_ws/build/nlink_parser/catkin_generated/installspace/nlink_parserConfig-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/nlink_parser" TYPE FILE FILES "/home/kinetic/xrobot_ws/src/nlink_parser/package.xml")
endif()

