# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\pokedex-project_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\pokedex-project_autogen.dir\\ParseCache.txt"
  "pokedex-project_autogen"
  )
endif()
