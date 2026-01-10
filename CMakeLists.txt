cmake_minimum_required(VERSION 3.16)
project(esp32_hack NONE)

set(FQBN esp32:esp32:esp32)

# Set PORT based on operating system
if(WIN32)
  set(PORT COM8)
else()
  set(PORT /dev/ttyUSB0)
endif()

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

