cmake_minimum_required(VERSION 3.16)

project(MealTracker VERSION 0.1 LANGUAGES CXX)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        ui/mainwindow.cpp
        ui/mainwindow.h
        ui/mainwindow.ui
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MealTracker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        domain/meal.cpp
        domain/meal.h
        repository/csvrepository.cpp
        repository/csvrepository.h
        repository/irepository.h
        controller/controller.cpp
        controller/controller.h
        filter/FilterStrategy.h
        filter/filterbytype.h filter/filterbytype.cpp
        filter/filterbycaloriesrange.h filter/filterbycaloriesrange.cpp
        filter/filterbyname.h filter/filterbyname.cpp
        filter/andfilter.h filter/andfilter.cpp
        filter/orfilter.h filter/orfilter.cpp
        command/command.h
        command/addcommand.h command/addcommand.cpp
        command/removecommand.h command/removecommand.cpp
        command/updatecommand.h command/updatecommand.cpp
        repository/jsonrepository.h repository/jsonrepository.cpp
        tests/test_main.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MealTracker APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MealTracker SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MealTracker
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MealTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MealTracker)
endif()
set_target_properties(MealTracker PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MealTracker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MealTracker)
endif()

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(MealTrackerTests
    tests/test_main.cpp
    domain/Meal.cpp
    repository/CSVRepository.cpp
    repository/jsonrepository.cpp
    controller/Controller.cpp
    filter/filterbyname.cpp
    filter/filterbytype.cpp
    filter/filterbycaloriesrange.cpp
    filter/andfilter.cpp
    filter/orfilter.cpp
    command/addcommand.cpp
    command/removecommand.cpp
    command/updatecommand.cpp
)

target_include_directories(MealTrackerTests PRIVATE
    ${CMAKE_SOURCE_DIR}/domain
    ${CMAKE_SOURCE_DIR}/repository
    ${CMAKE_SOURCE_DIR}/controller
    ${CMAKE_SOURCE_DIR}/filter
    ${CMAKE_SOURCE_DIR}/command
)

target_link_libraries(MealTrackerTests
    gtest_main
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME MealTrackerTests COMMAND MealTrackerTests)
