include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()
if (DEFINED ENV{MLE_WORKPRINTS})
  message(STATUS "MLE_WORKPRINTS=" $ENV{MLE_WORKPRINTS})
else()
  message(STATUS "MLE_WORKPRINTS NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern/include/workprints")
  message(STATUS "Using " $ENV{MLE_WORKPRINTS})
endif()

find_path(
  MagicLantern_INCLUDE_DIR
  NAMES mle/mle.h
  PATHS /opt/MagicLantern/include
)
find_library(
  MagicLantern_LIBRARY_DEBUG
  NAMES mlertd
  PATHS /opt/MagicLantern/lib
  PATH_SUFFIXES mle mle/runtime
)
find_library(
  MagicLantern_LIBRARY_RELEASE
  NAMES mlert
  PATHS /opt/MagicLantern/lib

  PATH_SUFFIXES mle mle/runtime
)
select_library_configurations(MagicLantern)

if(MagicLantern_INCLUDE_DIR AND EXISTS "${MagicLantern_INCLUDE_DIR}/mle/mlBasic.h")
  file(STRINGS "${MagicLantern_INCLUDE_DIR}/mle/mlBasic.h" _MagicLantern_VERSION_DEFINE REGEX "[\t ]*#define[\t ]+MLE_VERSION[\t ]+\"[^\"]*\".*")
  string(REGEX REPLACE "[\t ]*#define[\t ]+MLE_VERSION[\t ]+\"([^\"]*)\".*" "\\1" MagicLantern_VERSION "${_MagicLantern_VERSION_DEFINE}")
  unset(_MagicLantern_VERSION_DEFINE)
endif()

set(MagicLantern_DEFINITIONS)
set(MagicLantern_INCLUDE_DIRS ${MagicLantern_INCLUDE_DIR})
set(MagicLantern_LIBRARIES ${MagicLantern_LIBRARY})

find_package_handle_standard_args(
  MagicLantern
  FOUND_VAR MagicLantern_FOUND
  REQUIRED_VARS MagicLantern_INCLUDE_DIR MagicLantern_LIBRARY
  VERSION_VAR MagicLantern_VERSION
  )

if(MagicLantern_FOUND AND NOT TARGET MagicLantern::MagicLantern)
  add_library(MagicLantern::MagicLantern UNKNOWN IMPORTED)
  if(MagicLantern_LIBRARY_RELEASE)
    set_property(TARGET MagicLantern::MagicLantern APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(MagicLantern::MagicLantern PROPERTIES IMPORTED_LOCATION_RELEASE "${MagicLantern_LIBRARY_RELEASE}")
  endif()
  if(MagicLantern_LIBRARY_DEBUG)
    set_property(TARGET MagicLantern::MagicLantern APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(MagicLantern::MagicLantern PROPERTIES IMPORTED_LOCATION_DEBUG "${MagicLantern_LIBRARY_DEBUG}")
  endif()
  set_target_properties(MagicLantern::MagicLantern PROPERTIES INTERFACE_COMPILE_DEFINITIONS "COIN_DLL")
  set_target_properties(MagicLantern::MagicLantern PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MagicLantern_INCLUDE_DIRS}")
endif()

mark_as_advanced(MagicLantern_DEFINITIONS)
mark_as_advanced(MagicLantern_INCLUDE_DIRS)
mark_as_advanced(MagicLantern_LIBRARIES)
