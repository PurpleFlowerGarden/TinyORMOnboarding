cmake_minimum_required(VERSION 3.24)

cmake_policy(SET CMP0091 NEW)

project(TinyORMOnboarding CXX)

option(MSVC_RUNTIME_DYNAMIC "Use Dynamic MSVCRT" ON)
if(MSVC_RUNTIME_DYNAMIC)
  set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
  set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

add_test(NAME QSqlQueryTest COMMAND QSqlQueryTest -o -,txt -o "${CMAKE_CURRENT_BINARY_DIR}/QSqlQueryTest.xml,junitxml" COMMAND_EXPAND_LISTS)
set_property(TEST QSqlQueryTest PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/bin")
set_property(TEST QSqlQueryTest APPEND PROPERTY ENVIRONMENT_MODIFICATION "QT_PLUGIN_PATH=set:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/Qt6/plugins")

add_test(NAME TinyOrmQueryTest COMMAND TinyOrmQueryTest -o -,txt -o "${CMAKE_CURRENT_BINARY_DIR}/TinyOrmQueryTest.xml,junitxml" COMMAND_EXPAND_LISTS)
set_property(TEST TinyOrmQueryTest PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/bin")
set_property(TEST TinyOrmQueryTest APPEND PROPERTY ENVIRONMENT_MODIFICATION "QT_PLUGIN_PATH=set:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/Qt6/plugins")
