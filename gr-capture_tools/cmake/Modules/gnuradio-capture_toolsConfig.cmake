find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_CAPTURE_TOOLS gnuradio-capture_tools)

FIND_PATH(
    GR_CAPTURE_TOOLS_INCLUDE_DIRS
    NAMES gnuradio/capture_tools/api.h
    HINTS $ENV{CAPTURE_TOOLS_DIR}/include
        ${PC_CAPTURE_TOOLS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_CAPTURE_TOOLS_LIBRARIES
    NAMES gnuradio-capture_tools
    HINTS $ENV{CAPTURE_TOOLS_DIR}/lib
        ${PC_CAPTURE_TOOLS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-capture_toolsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_CAPTURE_TOOLS DEFAULT_MSG GR_CAPTURE_TOOLS_LIBRARIES GR_CAPTURE_TOOLS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_CAPTURE_TOOLS_LIBRARIES GR_CAPTURE_TOOLS_INCLUDE_DIRS)
