# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Desafioll-infoll_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Desafioll-infoll_autogen.dir\\ParseCache.txt"
  "Desafioll-infoll_autogen"
  )
endif()
