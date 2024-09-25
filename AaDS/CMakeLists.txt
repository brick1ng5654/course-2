cmake_minimum_required(VERSION 3.10)

project(MyProject)

# Укажите версию C++ стандарта
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")

# Добавьте исходные файлы
add_executable(lab02
    lab_02/main.cpp
    # lab_02/Array.cpp
    # lab_02/List.cpp
    # lab_02/Barray.cpp
    lab_02/Mword.cpp
)