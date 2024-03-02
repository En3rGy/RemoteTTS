cmake_minimum_required(VERSION 3.14)
project(RemoteTTS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets TextToSpeech Multimedia Test REQUIRED)

# Set the application icon for Windows
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/myappico.ico")
    set_source_files_properties(${APP_ICON_RESOURCE_WINDOWS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

# Add the executable
set(PROJECT_SOURCES
    main.cpp
    remotetts.cpp
    udpmanager.cpp
    systemstatus.cpp
    configform.cpp
    tcpmanager.cpp
    remotetts.h
    udpmanager.h
    systemstatus.h
    configform.h
    tcpmanager.h
    remotetts.ui
    configform.ui
)

if(NOT ${QT_VERSION_MAJOR} LESS 6)
    qt_add_executable(RemoteTTS
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${APP_ICON_RESOURCE_WINDOWS} # For Windows icon
    )
    # Define target properties for Android with Qt 6 as:
    # set_property(TARGET RemoteTTS APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #              ${CMAKE_CURRENT_SOURCE_DIR}/src/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RemoteTTS SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(RemoteTTS
            ${PROJECT_SOURCES}
            ${APP_ICON_RESOURCE_WINDOWS} # For Windows icon
        )
    endif()
endif()

target_link_libraries(RemoteTTS PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::TextToSpeech Qt6::Multimedia)
set_property(TARGET RemoteTTS PROPERTY AUTOMOC ON)


set_target_properties(RemoteTTS PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Set up definitions and compiler options
target_compile_definitions(RemoteTTS
    PRIVATE QT_DEPRECATED_WARNINGS
)

if(WIN32)
    target_compile_definitions(RemoteTTS PRIVATE win32)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(RemoteTTS PRIVATE _DEBUG)
    set_target_properties(RemoteTTS PROPERTIES OUTPUT_NAME "RemoteTTSD")
else()
    set_target_properties(RemoteTTS PROPERTIES OUTPUT_NAME "RemoteTTS")
endif()

# Set the destination folder for binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_finalize_executable(RemoteTTS)
endif()
