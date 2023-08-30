# CMakeList.txt : CMake project for hybridTrace, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("hybridTrace")

## get submodules from github
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
## Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

##           CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/assimp/CMakeLists.txt")
    message(FATAL_ERROR "The assimp submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


file(
    GLOB imgui_src 
    "external/imgui/*.cpp" 
    "external/imgui/backends/imgui_impl_glfw.cpp"
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)

file(GLOB src
    "src/core/*.cpp"
    "src/core/gizmos/*.cpp"
    "src/math/*.cpp"
    "src/app/*.cpp"
)

#Set asset paths
set(ASSET_DIR 
    " \"${CMAKE_CURRENT_BINARY_DIR}/../../../assets/\" "
)
set(SHADER_DIR " \"${CMAKE_CURRENT_BINARY_DIR}/../../../assets/shaders/\" ")
set(MODEL_DIR " \"${CMAKE_CURRENT_BINARY_DIR}/../../../assets/models/\" ")
set(IMAGE_DIR " \"${CMAKE_CURRENT_BINARY_DIR}/../../../assets/images/\" ")


message("Using Asset directory: ${ASSET_DIR}")
message("Using Shader directory: ${SHADER_DIR}")
message("Using Model directory: ${MODEL_DIR}")
message("Using Model directory: ${IMAGE_DIR}")



configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/src/core/settings.h.in"
    "${CMAKE_CURRENT_LIST_DIR}/src/core/settings.h"
)

# Add source to this project's executable.
add_executable (
    ${PROJECT_NAME}
    "src/main.cpp" 
    external/glad/src/glad.c
    ${imgui_src}
    ${src}
)

add_subdirectory(external/glfw)

#Disable warnings as errors in assimp
OPTION ( ASSIMP_WARNINGS_AS_ERRORS
  "Do not treat assimp warnings as errors."
  OFF
)

#TODO: Build shared libs for debugging?, but not release
OPTION( BUILD_SHARED_LIBS
  "Do not build package with shared libraries."
  ON
)
OPTION( ASSIMP_INSTALL
  "Disable this if you want to use assimp as a submodule."
  OFF
)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/assimp)

target_include_directories(
	${PROJECT_NAME} 
	PUBLIC external/glfw/include
    PUBLIC external/glad/include
    PUBLIC external/imgui
    PUBLIC external/imgui/backends
    PUBLIC external/assimp/include
    PUBLIC src
    PUBLIC src/core
    PUBLIC src/math
    PUBLIC src/app
)

target_link_directories(
	${PROJECT_NAME} 
	PUBLIC external/glfw/src
)

target_link_libraries(
	${PROJECT_NAME}
	glfw
    assimp
)

set_property(TARGET hybridTrace PROPERTY CXX_STANDARD 17)


# TODO: Add tests and install targets if needed.
