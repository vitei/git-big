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
# Ragel
#-------------------------------------------------------------------------------

find_program(RAGEL ragel)

if(RAGEL)
	set(ATTRIBUTES_PARSER ${PROJECT_SOURCE_DIR}/src/attributes_parser.rl)
	set(ATTRIBUTES_PARSER_C ${PROJECT_BINARY_DIR}/attributes_parser.c)

	add_custom_command(OUTPUT ${ATTRIBUTES_PARSER_C}
	                   COMMAND ${RAGEL} -G2 -o ${ATTRIBUTES_PARSER_C} ${ATTRIBUTES_PARSER}
	                   DEPENDS ${ATTRIBUTES_PARSER})
endif()

#-------------------------------------------------------------------------------
# Compile
#-------------------------------------------------------------------------------

# Generated files
configure_file(src/config.h.in ${PROJECT_BINARY_DIR}/config.h)

# Includes
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR})

# Files
file(GLOB_RECURSE SOURCE src/**.c src/**.h)

# Parser
if(RAGEL)
	list(REMOVE_ITEM SOURCE ${PROJECT_SOURCE_DIR}/src/attributes_parser.c)
	list(APPEND SOURCE ${ATTRIBUTES_PARSER_C})
endif()

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

add_test(NAME cli-help
         COMMAND ${PROJECT_SOURCE_DIR}/tests/001-cli-help)
add_test(NAME init
         COMMAND ${PROJECT_SOURCE_DIR}/tests/002-init)
add_test(NAME init-empty
         COMMAND ${PROJECT_SOURCE_DIR}/tests/003-init-empty)
add_test(NAME clean-filter
         COMMAND ${PROJECT_SOURCE_DIR}/tests/004-clean-filter)
add_test(NAME smudge-filter
         COMMAND ${PROJECT_SOURCE_DIR}/tests/005-smudge-filter)
add_test(NAME commit-hook
         COMMAND ${PROJECT_SOURCE_DIR}/tests/006-commit-hook)
add_test(NAME commit-hook-incorrect
         COMMAND ${PROJECT_SOURCE_DIR}/tests/007-commit-hook-incorrect)
add_test(NAME big-push-hook
         COMMAND ${PROJECT_SOURCE_DIR}/tests/008-big-push-hook)
add_test(NAME big-push-hook-incorrect
         COMMAND ${PROJECT_SOURCE_DIR}/tests/009-big-push-hook-incorrect)

