cmake_minimum_required(VERSION 3.26)

project(
  SimpleBeheading
  VERSION 1.0.4
  LANGUAGES CXX
)
  
set(PROJECT_LICENSE "")
set(PROJECT_COPYRIGHT "Copyright (c) 2024")
set(PROJECT_FRIENDLY_NAME ${PROJECT_NAME})
set(PROJECT_LONG_NAME "Simple Beheading binaries")
set(PROJECT_DESCRIPTION ${PROJECT_LONG_NAME})
set(PROJECT_AUTHOR "Seb263")

# ##############################################################################
# # Source build guard
# ##############################################################################

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ##############################################################################
# # Add CommonLibNG plugin
# ##############################################################################

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CLIBNGPlugin)
add_simple_commonlibsse_ng_plugin(AUTHOR "${PROJECT_AUTHOR}" LICENSE "${PROJECT_COPYRIGHT}")

# ##############################################################################
# # Build options
# ##############################################################################

message("Options:")
option(ZIP_TO_DIST "Zip the mod to 7z file in dist." ON)
message("\tZip to dist: ${ZIP_TO_DIST}")

# ##############################################################################
# # Find dependencies
# ##############################################################################

find_package(magic_enum CONFIG REQUIRED)
find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/detail/SimpleIni.h")

# ##############################################################################
# # Post target setup
# ##############################################################################

target_include_directories(${PROJECT_NAME} PRIVATE ${CLIB_UTIL_INCLUDE_DIRS})

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	magic_enum::magic_enum
)
