cmake_minimum_required(VERSION 3.16)
project(esp32_hack)

set(FQBN esp32:esp32:esp32)
set(PORT /dev/ttyUSB0)

include_directories(${CMAKE_SOURCE_DIR}/src/speaker)

add_custom_target(build
  COMMAND arduino-cli compile
          --fqbn ${FQBN}
          ${CMAKE_SOURCE_DIR}/src
)

add_custom_target(upload
  COMMAND arduino-cli upload
          -p ${PORT}
          --fqbn ${FQBN}
          ${CMAKE_SOURCE_DIR}/src
)

