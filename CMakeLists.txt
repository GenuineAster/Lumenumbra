cmake_minimum_required(VERSION 2.8)

project(Lumenumbra)

# Set options
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type ( Debug / Release )" FORCE)
set(SFML_STATIC_LIBS FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Enable C++11
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")

if(SFML_STATIC_LIBS)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)
find_package(JsonCpp REQUIRED)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else()
    set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
    message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
	message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()

if(JSONCPP_FOUND)
    include_directories(${JSONCPP_INCLUDE_DIR})
else()
    set(JSONCPP_ROOT "" CACHE PATH "JsonCpp top-level directory")
    message("\n-> JsonCpp directory not found. Set JSONCPP_ROOT to JsonCpp's top-level path (containing \"include\" and \"lib\" directories).")
endif()

# Add source files
set(SRC_DIR "src")

include_directories("src")

message(${CMAKE_SOURCE_DIR})

set( SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Game/Game.hpp
    ${SRC_DIR}/Game/Game.cpp
    ${SRC_DIR}/Entity/Entity.hpp
    ${SRC_DIR}/Entity/Entity.cpp
    ${SRC_DIR}/LightingEngine/LightingEngine.hpp
	${SRC_DIR}/LightingEngine/LightingEngine.cpp
	${SRC_DIR}/LightingEngine/Light/Light.hpp
	${SRC_DIR}/LightingEngine/Light/Light.cpp
	${SRC_DIR}/LightingEngine/Light/Tools/Tools.hpp
)

add_executable(Lumenumbra ${SOURCES})

# Link SFML and JSONCPP
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    message("\n-- Linking SFML in Release mode.")
    target_link_libraries(Lumenumbra ${SFML_GRAPHICS_LIBRARY_RELEASE} ${SFML_WINDOW_LIBRARY_RELEASE} ${SFML_AUDIO_LIBRARY_RELEASE} ${SFML_SYSTEM_LIBRARY_RELEASE})
else()
    message("\n-- Linking SFML in Debug mode.")
    target_link_libraries(Lumenumbra ${SFML_GRAPHICS_LIBRARY_DEBUG} ${SFML_WINDOW_LIBRARY_DEBUG} ${SFML_AUDIO_LIBRARY_DEBUG} ${SFML_SYSTEM_LIBRARY_DEBUG})
endif()
    message("\n-- Linking JsonCpp.")
    target_link_libraries(Lumenumbra ${JSONCPP_LIBRARY})

install(TARGETS Lumenumbra RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/run/)