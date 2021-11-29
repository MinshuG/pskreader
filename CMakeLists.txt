cmake_minimum_required (VERSION 3.8)

project ("Psk")

# include_directories("${PROJECT_SOURCE_DIR}/pybind11/include")
# add_library (pyPsk "Psk.cpp" "Psk.h" "binding.cpp")

add_executable (Psk "Psk.cpp" "Psk.h" "main.cpp")
