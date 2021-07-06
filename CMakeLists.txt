
cmake_minimum_required(VERSION 3.16)

project( hello_wayland )

set(SRC_FILES 
	src/main.cpp
	src/wayland_manager.cpp
	src/gl_manager.cpp
	)

set(INC_DIR
	include/
	include/common/
	)

set(WL_LIB
	wayland-client
	wayland-server
	wayland-egl )

set(GL_LIB
	EGL
	GLESv2 )

add_definitions(-DDEBUG)

add_executable(${CMAKE_PROJECT_NAME}
				${SRC_FILES} )

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
							${INC_DIR} )

target_link_libraries(${CMAKE_PROJECT_NAME}
						${WL_LIB}
						${GL_LIB})

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
