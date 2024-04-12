# CMakeList.txt : CMake project for Airfoil, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Airfoil")

include(CMakePrintHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxBUILD_SHARED OFF)
#set(wxBUILD_USE_STATIC_RUNTIME ON)

find_package(wxWidgets REQUIRED core base gl OPTIONAL_COMPONENTS net propgrid aui adv)
include(${wxWidgets_USE_FILE})

cmake_print_variables(wxWidgets_LIBRARIES)

# OpenMP
FIND_PACKAGE(OpenMP REQUIRED)

if(OPENMP_FOUND)
    message("OpenMP found")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

cmake_print_variables(CMAKE_C_FLAGS CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_EXE_LINKER_FLAGS OpenMP_libomp_LIBRARY)

include_directories(include airfoilgui mathplot aerolib/include)

add_library(mathplot STATIC mathplot/mathplot.cpp)
add_library(aerolib STATIC aerolib/src/Util.cpp)

# Link wxWidgets and wxMathPlot libraries
target_compile_definitions(mathplot PRIVATE _UNICODE=1)
target_link_libraries(mathplot ${wxWidgets_LIBRARIES})

# Add source to this project's executable.
set(PROJECT_SOURCES
	airfoilgui/FrameGUI.h
	airfoilgui/FrameGUI.cpp
	include/AirfoilApp.h 	
	include/AirfoilFrame.h 
    include/Settings.h
    include/Fluid.h
    include/Region.h
    include/Canvas.h
	src/AirfoilApp.cpp
	src/AirfoilFrame.cpp 
	src/Settings.cpp
	src/Fluid.cpp
	src/Region.cpp
	src/Canvas.cpp
)


add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} )
file(COPY data/ DESTINATION data)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Airfoil PROPERTY CXX_STANDARD 20)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE=1)
if(WIN32)
    #target_compile_definitions(${PROJECT_NAME} PRIVATE WXUSINGDLL=1)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} ${OpenMP_libomp_LIBRARY} mathplot aerolib)
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# TODO: Add tests and install targets if needed.
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY data/ DESTINATION data)
