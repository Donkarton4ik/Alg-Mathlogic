cmake_minimum_required(VERSION 3.16)

project(MyProject CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги компиляции
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif()

# Qt (moc для Q_OBJECT/сигналов-слотов собирается автоматически)
set(CMAKE_AUTOMOC ON)

if(APPLE AND NOT DEFINED CMAKE_PREFIX_PATH)
    execute_process(
        COMMAND brew --prefix qt
        OUTPUT_VARIABLE QT_BREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(QT_BREW_PREFIX)
        list(APPEND CMAKE_PREFIX_PATH ${QT_BREW_PREFIX})
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Исходники
set(SOURCES
    src/main.cpp
    src/Grid/GridWidget.cpp
    src/MainWindow/MainWindow.cpp
    src/Neumann/neuman_class.cpp
)

# Исполняемый файл
add_executable(debug.out ${SOURCES})
target_include_directories(debug.out PRIVATE src src/Neumann)
target_link_libraries(debug.out PRIVATE Qt6::Widgets)

# === Выбор дебаггера ===
if(APPLE)
    set(DEBUGGER lldb)
else()
    set(DEBUGGER gdb)
endif()

# make debug → собирает и запускает отладчик
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target debug.out
    COMMAND ${DEBUGGER} ./debug.out
    DEPENDS debug.out
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# make run → запускает бинарник
add_custom_target(run
    COMMAND ./debug.out
    DEPENDS debug.out
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
