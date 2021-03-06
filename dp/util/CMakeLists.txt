project(DPUtil)

find_package(Boost COMPONENTS filesystem system REQUIRED )
find_package(DevIL)

if(NOT IL_FOUND)
  message("DevIL not found, disabling support for image file io in DPUtil.")
endif()

include( CopyFile )

include_directories( ${Boost_INCLUDE_DIRS}
                     ${NVPMAPI_INCLUDE_DIR} 
                   )

add_definitions(
  "-DDP_UTIL_EXPORTS"
  ${GLEW_DEFINITIONS}
)

if (IL_FOUND)
  include_directories(${IL_INCLUDE_DIR})
  add_definitions(-DHAVE_IL)
endif()

set(DPUTIL_INCLUDES
  Array.h
  Backtrace.h
  BitArray.h
  BitMask.h
  Config.h
  DynamicLibrary.h
  File.h
  FileFinder.h
  FrameProfiler.h
  HashGenerator.h
  HashGeneratorMurMur.h
  HashGeneratorMD5.h
  Image.h
  Locale.h
  Memory.h
  NVPerfMon.h
  Observer.h
  PlugIn.h
  PlugInCallback.h
  Reflection.h
  Semantic.h
  SharedPtr.h
  Singleton.h
  StridedIterator.h
  Timer.h
  WeakPtr.h
)

add_definitions("-DDP_UTIL_USE_BOOST")

source_group(headers FILES ${DPUTIL_INCLUDES})

set(DPUTIL_SOURCES
  src/Backtrace.cpp
  src/BitArray.cpp
  src/DynamicLibrary.cpp
  src/File.cpp
  src/FileFinder.cpp
  src/FileMapping.cpp
  src/FrameProfiler.cpp
  src/HashGeneratorMurMur.cpp
  src/HashGeneratorMD5.cpp
  src/Image.cpp
  src/Locale.cpp
  src/Memory.cpp
  src/NVPerfMon.cpp
  src/Observer.cpp
  src/PlugIn.cpp
  src/Reflection.cpp
  src/Timer.cpp
)

source_group(sources FILES ${DPUTIL_SOURCES})


add_library(DPUtil SHARED
  ${DPUTIL_INCLUDES}
  ${DPUTIL_SOURCES}
)

CopyDevIL( DPUtil "${DP_BINARY_PATH}" )
CopyGLEW( DPUtil "${DP_BINARY_PATH}" )

target_link_libraries( DPUtil
  ${Boost_LIBRARIES}
)

if (IL_FOUND)
  target_link_libraries( DPUtil ${IL_LIBRARIES})
endif()

if (MSVC)
  target_link_libraries( DPUtil
    DP
    DbgHelp
  )
endif()

if (WIN32 AND CMAKE_COMPILER_IS_GNUCC)
  # GCC requires the declspec(export) keyword behind the class keyword to export the vtable ( class declspec(export) MyKlass {} ). 
  # Export everything for now and adopt sources later.
  set_target_properties( DPUtil PROPERTIES LINK_FLAGS "-Wl,--export-all" )
endif()
