find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_DME gnuradio-dme)

FIND_PATH(
    GR_DME_INCLUDE_DIRS
    NAMES gnuradio/dme/api.h
    HINTS $ENV{DME_DIR}/include
        ${PC_DME_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_DME_LIBRARIES
    NAMES gnuradio-dme
    HINTS $ENV{DME_DIR}/lib
        ${PC_DME_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-dmeTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_DME DEFAULT_MSG GR_DME_LIBRARIES GR_DME_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_DME_LIBRARIES GR_DME_INCLUDE_DIRS)
