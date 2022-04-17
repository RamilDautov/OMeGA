cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

project(OMeGA VERSION 0.2.0)

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

include(arch)
include(clang-format)
include(CompilerWarnings)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

set(THIRD_PARTY_DIR_NAME "third_party")
set(${PROJECT_NAME_UPPER}_LIB_PATH ${CMAKE_CURRENT_LIST_DIR}/${THIRD_PARTY_DIR_NAME})
set(SOURCE_DIR		${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RESOURCES_DIR	${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "app")

option(${PROJECT_NAME_UPPER}_INSTALL_DEPENDENCIES "Install required libraries in package" ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer	REQUIRED)

find_package(Threads	REQUIRED)
find_package(fmt	REQUIRED)
find_package(spdlog	REQUIRED)

if(${PROJECT_NAME_UPPER}_INSTALL_DEPENDENCIES)
	if(NOT ${PROJECT_NAME_UPPER}_LIB_PATH)
		message(WARNING "Variable '${PROJECT_NAME_UPPER}_LIB_PATH' does not specified. '${PROJECT_NAME_UPPER}_INSTALL_DEPENDENCIES' disabled")
	else()
		file(GLOB LIBRARY_LIST "${${PROJECT_NAME_UPPER}_LIB_PATH}/*")
		foreach(INDEX ${LIBRARY_LIST})
			file(GLOB OUT "${INDEX}/lib/${TARGET_ARCH}/*${CMAKE_SHARED_LIBRARY_SUFFIX}*")
			list(APPEND DLLS ${OUT})
		endforeach()
	endif()

	install(FILES ${DLLS} DESTINATION bin COMPONENT app)
endif()

# logger

set(TARGET_NAME ${PROJECT_NAME}-logger)
add_library(${TARGET_NAME} INTERFACE)
target_compile_definitions(${TARGET_NAME} INTERFACE SPDLOG_FMT_EXTERNAL)
target_link_libraries(${TARGET_NAME} INTERFACE fmt::fmt spdlog::spdlog)

add_library(Omega::logger ALIAS ${TARGET_NAME})

#common

add_executable(${PROJECT_NAME} 
								${SOURCE_DIR}/main.cpp
								${SOURCE_DIR}/scale.cpp
								${SOURCE_DIR}/speaker.cpp
								${SOURCE_DIR}/midi.cpp
								${SOURCE_DIR}/utility/logger.cpp
								${SOURCE_DIR}/algorithm/genetic.cpp)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		SDL_MAIN_HANDLED
)
include_directories(${SDL2_INCLUDE_DIR} ${FMT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC  ${INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} fmt::fmt spdlog::spdlog SDL2::mixer)

#configure_file(${SOURCE_DIR}/version.h.in	 ${CMAKE_CURRENT_BINARY_DIR}/version.h)
configure_file(${INCLUDE_DIR}/version.h.in	 ${CMAKE_CURRENT_BINARY_DIR}/version.h)

# library
add_library(${PROJECT_NAME}_lib  STATIC
								${SOURCE_DIR}/scale.cpp
								${SOURCE_DIR}/speaker.cpp
								${SOURCE_DIR}/midi.cpp
								#${SOURCE_DIR}/utility/logger.cpp
								${SOURCE_DIR}/algorithm/genetic.cpp
)
target_compile_definitions(${PROJECT_NAME}_lib
	PRIVATE
		SDL_MAIN_HANDLED
)
include_directories(${SDL2_INCLUDE_DIR} ${FMT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}_lib PUBLIC  ${INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME}_lib PRIVATE Omega::logger SDL2::Core SDL2::mixer)

install(TARGETS ${PROJECT_NAME}
	RUNTIME
		DESTINATION bin COMPONENT app
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
)

include(CPackConfig)

set(CPACK_GENERATOR ZIP)
set(CPACK_COMPONENTS_ALL app)

add_cpack_target(${PROJECT_NAME} ${PROJECT_VERSION})