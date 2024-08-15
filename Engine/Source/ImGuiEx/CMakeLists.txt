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
set(LIBRARY_NAME "ImGuiEx")

file(GLOB_RECURSE SOURCES
  "ImGuiEx.cpp"
  "imgui/imgui.cpp"
  "imgui/imgui_draw.cpp"
  "imgui/imgui_demo.cpp"
  "imgui/imgui_tables.cpp"
  "imgui/imgui_widgets.cpp"
  "imgui/backends/imgui_impl_vulkan.cpp"
  "imgui/backends/imgui_impl_glfw.cpp"
)

add_library(${LIBRARY_NAME} STATIC ${SOURCES})

target_link_libraries(${LIBRARY_NAME} PRIVATE
  "Runtime"
)

target_include_directories(${LIBRARY_NAME} SYSTEM PUBLIC
  "${TURBINE_ROOT_DIRECTORY}/Engine/Source"
  "${TURBINE_ROOT_DIRECTORY}/Engine/Source/ImGuiEx"
  "${TURBINE_ROOT_DIRECTORY}/Engine/Source/ImGuiEx/imgui"
)