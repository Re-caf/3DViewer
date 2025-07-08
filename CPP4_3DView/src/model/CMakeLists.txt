# Устанавливаем минимальную версию CMake
cmake_minimum_required(VERSION 3.14)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_compile_options(
#    -Werror

   -Wall
   -Wextra
   -Wpedantic

   -Wcast-align
   -Wcast-qual 
   -Wconversion
   -Wctor-dtor-privacy
   -Wduplicated-branches
   -Wduplicated-cond
   -Wextra-semi
   -Wfloat-equal
   -Wlogical-op
   -Wnon-virtual-dtor
   -Wold-style-cast
   -Woverloaded-virtual
   -Wredundant-decls
   -Wsign-conversion
   -Wsign-promo
)

# Указываем, что проект использует стандарт C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаем статическую библиотеку из исходных файлов
add_library(model STATIC
    s21_WireframeModel.cpp
    objParser.cpp
    s21_Facade.cpp
)

if(ENABLE_TEST_COVERAGE)
    target_compile_options(model PRIVATE --coverage)
    target_link_libraries(model PRIVATE --coverage)
endif()

# Указываем папки, где CMake будет искать заголовочные файлы
target_include_directories(model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # Это папка с текущими исходниками
)