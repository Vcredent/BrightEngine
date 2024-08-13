# ------------------------------------------------------------------------ #
# CMakeLists.txt                                                           #
# ------------------------------------------------------------------------ #
#                        This file is part of:                             #
#                           TURBINE ENGINE                                 #
# ------------------------------------------------------------------------ #
#                                                                          #
# Copyright (C) 2022 Vcredent All rights reserved.                         #
#                                                                          #
# Licensed under the Apache License, Version 2.0 (the "License");          #
# you may not use this file except in compliance with the License.         #
#                                                                          #
# You may obtain a copy of the License at                                  #
#     http://www.apache.org/licenses/LICENSE-2.0                           #
#                                                                          #
# Unless required by applicable law or agreed to in writing, software      #
# distributed under the License is distributed on an "AS IS" BASIS,        #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied  #
# See the License for the specific language governing permissions and      #
# limitations under the License.                                           #
#                                                                          #
# ------------------------------------------------------------------------ #
set(LIBRARY_NAME "Runtime")

file(GLOB_RECURSE SOURCES
  "Drivers/*.cpp"
  "Win32/*.cpp"
  "Window/*.cpp"
  "${TURBINE_THIRDPARTY_DIRECTORY}/vma/*.cpp"
  "${TURBINE_THIRDPARTY_DIRECTORY}/volk/*.cpp"
)

include_directories(SYSTEM ${TURBINE_THIRDPARTY_DIRECTORY})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(LINK_DIRECTORIES
  "${TURBINE_THIRDPARTY_DIRECTORY}/GLFW/lib-mingw-w64"
  "${TURBINE_THIRDPARTY_DIRECTORY}/vulkan"
)

set(LINK_LIBRARIES
  "glfw3"
  "vulkan-1"
)

add_library(${LIBRARY_NAME} STATIC ${SOURCES})

target_link_directories(${LIBRARY_NAME} PUBLIC
  ${LINK_DIRECTORIES}
)

target_link_libraries(${LIBRARY_NAME} PUBLIC
  ${LINK_LIBRARIES}
)

target_include_directories(${LIBRARY_NAME} SYSTEM PUBLIC
  "${TURBINE_ROOT_DIRECTORY}/Engine/Source"
  "${TURBINE_ROOT_DIRECTORY}/Engine/Source/Runtime"
  "${TURBINE_THIRDPARTY_DIRECTORY}"
)