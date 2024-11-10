# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/JsonTree_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/JsonTree_autogen.dir/ParseCache.txt"
  "JsonTree_autogen"
  )
endif()
