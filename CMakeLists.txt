include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME AudioTouchpadSkip)
set(CMAKE_CXX_STANDARD 20)

message(STATUS "Using ${CMAKE_GENERATOR}")

if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
  if (NOT MINGW_PATH)
    message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
  endif()
  set(USE_MINGW "True")
  set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
  set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.23.0)

if (DEFINED BAKKES_SDK_PATH)
  set(BAKKES_INCLUDE_DIRS "${BAKKES_SDK_PATH}/include")
  set(BAKKES_LIBS "${BAKKES_SDK_PATH}/lib")
  message(STATUS "BakkesSDK loaded")
else()
  message(STATUS "No BakkesSDK path")
endif()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

target_precompile_headers(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/pch.h)

if (USE_MINGW)
  target_include_directories(${PROJECT_NAME} PUBLIC ${MINGW_PATH}/include)
  target_link_directories(${PROJECT_NAME} PUBLIC ${MINGW_PATH}/lib)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${BAKKES_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${BAKKES_LIBS})
target_link_libraries(${PROJECT_NAME} pluginsdk)

