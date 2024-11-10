cmake_minimum_required(VERSION 3.16)

project(JsonTree VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


find_package(yaml-cpp REQUIRED)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/yaml-cpp")
set(YAML_CPP_DIR "/usr/local/opt/yaml-cpp")
# above may be different for you, m3 mac here w/ brew
# cmake -DYAML_CPP_DIR=$(brew --prefix yaml-cpp) ..

find_package(nlohmann_json REQUIRED)
# find_package(GTest REQUIRED)



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        # jsontree.h
        # jsontree.cpp
        # yamljsonutils.h
        mainwindow.ui
        yamljsonutils.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

# target_sources(JsonTree
#   PRIVATE
#     # testYamlJsonConverter.cpp
# )
qt_add_executable(JsonTree
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        data.json
        jsontree.h
        jsontree.cpp
        yamljsonutils.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET JsonTree APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(JsonTree SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(JsonTree
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(JsonTree
    PUBLIC
        yaml-cpp::yaml-cpp
        nlohmann_json::nlohmann_json
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.JsonTree)
endif()
set_target_properties(JsonTree PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS JsonTree
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(JsonTree)
endif()



# # Add test executable
# add_executable(YamlJsonConverterTests testYamlJsonConverter.cpp)
# # Link GTest and other dependencies for tests
# target_link_libraries(YamlJsonConverterTests
#     PRIVATE
#         GTest::gtest
#         GTest::gtest_main
#         yaml-cpp::yaml-cpp
#         nlohmann_json::nlohmann_json
#         Qt${QT_VERSION_MAJOR}::Widgets
# )

# # Enable testing
# enable_testing()
# add_test(NAME YamlJsonTests COMMAND YamlJsonConverterTests)
