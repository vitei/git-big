project(git-big)
cmake_minimum_required(VERSION 2.6)

#-------------------------------------------------------------------------------
# Settings
#-------------------------------------------------------------------------------

set(GIT_BIG_VERSION_MAJOR 0)
set(GIT_BIG_VERSION_MINOR 0)
set(GIT_BIG_VERSION_BUILD 1)

set(GIT_BIG_VERSION ${GIT_BIG_VERSION_MAJOR}.${GIT_BIG_VERSION_MINOR}.${GIT_BIG_VERSION_BUILD})

#-------------------------------------------------------------------------------
# Compile
#-------------------------------------------------------------------------------

# Generated files
configure_file(src/config.h.in ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR})

# Files
file(GLOB_RECURSE SOURCE src/**.c src/**.h)

# Main executable
add_executable(git-big ${SOURCE})

# Libraries
target_link_libraries(git-big git2)

# Platform-specfic libraries
if(APPLE)

elseif(LINUX)
	target_link_libraries(git-big crypto)
elseif(WIN32)

endif()

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------

# Install options
install(TARGETS git-big DESTINATION bin)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------

enable_testing()

add_test(integration_tests ${PROJECT_SOURCE_DIR}/tests/ctf/ctf .)

