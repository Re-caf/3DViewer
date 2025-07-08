# Устанавливаем минимальную версию CMake
cmake_minimum_required(VERSION 3.14)

# Включаем поддержку тестирования
enable_testing()

# Устанавливаем Google Test через FetchContent (если он не установлен)
include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0  # Указываем версию (если нужно)
)

# Загружаем и делаем доступным Google Test
FetchContent_MakeAvailable(gtest)

# Создаем исполняемые файлы для тестов
add_executable(s21_transform_tests s21_transform_tests.cpp)
add_executable(testParser testParser.cpp)

# Связываем тесты с библиотеками
target_link_libraries(s21_transform_tests PRIVATE model gtest gtest_main)
target_link_libraries(testParser PRIVATE model gtest gtest_main)

# Добавляем тесты в CMake
include(GoogleTest)
gtest_discover_tests(s21_transform_tests)
gtest_discover_tests(testParser)