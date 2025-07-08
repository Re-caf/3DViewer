cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
cmake_policy(SET CMP0100 NEW)
# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# # Указываем флаги для покрытия
option(ENABLE_TEST_COVERAGE "Enable test coverage reporting" ON)

# Добавляем путь к папке с исходниками модели
add_subdirectory(model)
add_subdirectory(gui)
add_dependencies(gui model)

# Добавляем путь к папке с тестами
enable_testing()
add_subdirectory(tests)