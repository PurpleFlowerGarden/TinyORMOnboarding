cmake_minimum_required(VERSION 3.24)

cmake_policy(SET CMP0091 NEW)

project(TinyORMOnboarding CXX)

option(MSVC_RUNTIME_DYNAMIC "Use Dynamic MSVCRT" ON)
if(MSVC_RUNTIME_DYNAMIC)
  set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
  set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(Qt6 REQUIRED COMPONENTS Concurrent Core Sql Test)
qt_standard_project_setup()

add_executable(QSqlQueryTest)
target_sources(QSqlQueryTest PRIVATE QSqlQueryTest.cpp)
target_link_libraries(QSqlQueryTest PRIVATE Qt::Core Qt::Sql Qt::Test)
set_target_properties(QSqlQueryTest PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY})

find_package(TinyOrm CONFIG REQUIRED)
add_executable(TinyOrmQueryTest)
target_sources(TinyOrmQueryTest PRIVATE TinyOrmQueryTest.cpp)
target_link_libraries(TinyOrmQueryTest PRIVATE Qt::Core Qt::Sql Qt::Test TinyOrm::TinyOrm)
set_target_properties(TinyOrmQueryTest PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY})

enable_testing()
add_test(NAME QSqlQueryTest COMMAND QSqlQueryTest -o "${CMAKE_CURRENT_BINARY_DIR}/QSqlQueryTest.xml,junitxml" COMMAND_EXPAND_LISTS)
add_test(NAME TinyOrmQueryTest COMMAND TinyOrmQueryTest -o "${CMAKE_CURRENT_BINARY_DIR}/TinyOrmQueryTest.xml,junitxml" COMMAND_EXPAND_LISTS)
