cmake_minimum_required(VERSION 3.11)
project(GUI LANGUAGES CXX)

# Включаем автоматическую генерацию moc, rcc, uic
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем модули Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets)

# Указываем исходные файлы
set(SRC_FILES
    mainwindow.cpp
    main.cpp
    openglimpl.cpp
    s21_ProgramSettings.cpp
    gifimage/qgifimage.cpp
)

set(UI_FILES
    mainwindow.ui
)

set(HEADER_FILES
    mainwindow.h
    openglimpl.h
    s21_ProgramSettings.hh
    gifimage/qgifimage.h
)

# Создание исполняемого файла
add_executable(gui
    ${SRC_FILES}
    ${UI_FILES}
)

# Подключение заголовков Qt
target_include_directories(gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линковка с библиотеками Qt6
target_link_libraries(gui PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# Линковка с OpenGL
if (APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(gui PRIVATE ${OpenGL_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(gui PRIVATE OpenGL::GL)
endif()

# Линковка с внешней моделью
target_link_libraries(gui PRIVATE model)

# 🔗 Линковка с giflib
find_path(GIF_INCLUDE_DIR gif_lib.h)
find_library(GIF_LIBRARY gif)

if (GIF_INCLUDE_DIR AND GIF_LIBRARY)
    target_include_directories(gui PRIVATE ${GIF_INCLUDE_DIR})
    target_link_libraries(gui PRIVATE ${GIF_LIBRARY})
else()
    message(FATAL_ERROR "giflib not found. Please install libgif-dev")
endif()
