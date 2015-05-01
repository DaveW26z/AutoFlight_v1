# May be required to run CMake with  -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.4m.so -DPYTHON_INCLUDE_DIR=/usr/include/python3.4 -DPYTHON_INCLUDE_DIR2=/usr/include/python3.4


cmake_minimum_required(VERSION 2.8.9)

project(AutoFlight)

# C++11 and warnings
#set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O2 -s")
set(CMAKE_CXX_FLAGS "-Wall -std=c++11  -g -O0")
#                                      ^^^^^^ for debugging only
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Set executable output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY release_linux)
# Add cmake/modules directory to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

set(QT_USE_QTWEBKIT TRUE)

# Tell cmake where Qt is
#set(CMAKE_PREFIX_PATH "/home/lukas/workspace/libs/qt5/qtbase")
set(CMAKE_PREFIX_PATH "/opt/Qt5.4.1/5.4/gcc_64")

# Include libraries in the ../lib folder
include_directories("${PROJECT_SOURCE_DIR}/lib/include/gamepad")
include_directories("${PROJECT_SOURCE_DIR}/lib/include/")

# Include libdrone
include_directories("${PROJECT_SOURCE_DIR}/../libdrone/include")

# Find the QtWidgets library
add_definitions(-DQT_NO_KEYWORDS)
find_package(Qt5Widgets)
find_package(Qt5WebKitWidgets)
find_package(Qt5OpenGL)

# Find the Boost libraries
set(Python_ADDITIONAL_VERSIONS 3)
find_package(Boost 1.53 COMPONENTS program_options python-py34 system filesystem thread chrono date_time timer REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Find OpenCV
set(OpenCV_DIR "/opt/opencv/share/OpenCV")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Find FFMPEG and dependencies
#find_package(FFMPEG REQUIRED)
#include_directories(${FFMPEG_INCLUDE_DIR})
#find_package(ZLIB REQUIRED)
set(FFMPEG_LIBRARIES
    "/opt/ffmpeg/lib/libavcodec.so"
    "/opt/ffmpeg/lib/libavformat.so"
    "/opt/ffmpeg/lib/libavutil.so"
	"/opt/ffmpeg/lib/libswscale.so"
    "/opt/ffmpeg/lib/libswresample.so" )
include_directories("/opt/ffmpeg/include")

# Find Python
set(Python_ADDITIONAL_VERSIONS 3.4)
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

file(GLOB AutoFlight_SRC
    "src/*.h"
    "src/*.cpp"
    "src/widgets/*.h"
    "src/widgets/*.cpp"
    "src/dialogs/*.h"
    "src/dialogs/*.cpp"
    "src/input/*.h"
    "src/input/*.cpp"
    "src/autoscript/*.h"
    "src/autoscript/*.cpp"
    "src/autoscript/opencv/*.h"
    "src/autoscript/opencv/*.cpp"
    "src/autoscript/opencv/*.hpp"
    "src/tools/*.h"
    "src/tools/*.cpp"
    "src/imgprocui/*.h"
    "src/imgprocui/*.cpp"
    "src/imgproc/*.h"
    "src/imgproc/*.cpp"
    "src/imgprocui/nodeseditor/*.h"
    "src/imgprocui/nodeseditor/*.cpp"

    "lib/include/qcustomplot/*.h"
    "lib/include/qcustomplot/*.cpp"
    "lib/include/pugixml/*.hpp"
    "lib/include/pugixml/*.cpp"
    "lib/include/glm/*.hpp"
    "lib/include/glm/core/*.hpp"
)

# Add the resources
qt5_add_resources(AutoFlight_RCCS "${PROJECT_SOURCE_DIR}/src/resources.qrc")

# Create the AutoFlight executable
add_executable(AutoFlight ${AutoFlight_SRC} ${AutoFlight_RCCS})

# Update BUILD_NUMBER in afconstants.h header
add_custom_target(
  generate_build_number
  COMMAND cd ${PROJECT_SOURCE_DIR} && python3 "${PROJECT_SOURCE_DIR}/cmake/update_build_number.py"
)
add_dependencies(AutoFlight generate_build_number)

target_link_libraries(AutoFlight "${PROJECT_SOURCE_DIR}/../libdrone/build/libdrone/libdrone.so")
target_link_libraries(AutoFlight ${Boost_LIBRARIES})
target_link_libraries(AutoFlight ${OpenCV_LIBS})
target_link_libraries(AutoFlight ${FFMPEG_LIBRARIES})
target_link_libraries(AutoFlight ${ZLIB_LIBRARIES})
target_link_libraries(AutoFlight ${PYTHON_LIBRARIES})
target_link_libraries(AutoFlight GL x264)

# Link libraries in ../lib
target_link_libraries(AutoFlight "${PROJECT_SOURCE_DIR}/lib/release-linux64/libstem_gamepad.a")
target_link_libraries(AutoFlight "${PROJECT_SOURCE_DIR}/lib/release-linux64/libqscintilla2.a")

# Use the Widgets module from Qt 5
qt5_use_modules(AutoFlight Widgets PrintSupport WebKit WebKitWidgets OpenGL)