cmake_minimum_required(VERSION 3.4)
project("py_lib")
set(CMAKE_CXX_STANDARD 17)

set(SOURCE "${PROJECT_SOURCE_DIR}/src")
set(DEPENDENCIES "${PROJECT_SOURCE_DIR}/dep")

add_subdirectory(${DEPENDENCIES}/pybind11)

pybind11_add_module (
	py_lib
	${SOURCE}/encryption.cpp
	${SOURCE}/check.cpp
	${SOURCE}/hash.cpp
	${SOURCE}/math.cpp
	${SOURCE}/io.cpp
	${SOURCE}/bind.cpp
)

target_include_directories (
	py_lib
	PRIVATE ${SOURCE}
)