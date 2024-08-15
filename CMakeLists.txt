# ------------------------------------------------------------------------ #
# CMakeLists.txt                                                           #
# ------------------------------------------------------------------------ #
#                        This file is part of:                             #
#                           TURBINE ENGINE                                 #
# ------------------------------------------------------------------------ #
#                                                                          #
# Copyright (C) 2022 TheSampson All rights reserved.                       #
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
cmake_minimum_required(VERSION 3.26)
project(TurbineEngine)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options("/source-charset:utf-8")
  add_compile_options("/execution-charset:utf-8")
endif()

set(TURBINE_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(TURBINE_THIRDPARTY_DIRECTORY "${TURBINE_ROOT_DIRECTORY}/Engine/Source/ThirdParty")

include_directories(SYSTEM "${TURBINE_ROOT_DIRECTORY}/Engine/Source/Include")

add_subdirectory("Engine/Source/Runtime")
add_subdirectory("Engine/Source/ImGuiEx")
add_subdirectory("Engine/Source/Sandbox")