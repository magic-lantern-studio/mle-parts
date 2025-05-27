include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

find_path(
  SoGtk_INCLUDE_DIR
  NAMES Inventor/Gtk/SoGtk.h
)
find_library(
  SoGtk_LIBRARY_DEBUG
  NAMES SoGtkd SoGtk1d
)
find_library(
  SoGtk_LIBRARY_RELEASE
  NAMES SoGtk SoGtk1
)
select_library_configurations(SoGtk)

if(SoGtk_INCLUDE_DIR AND EXISTS "${SoGtk_INCLUDE_DIR}/Inventor/Gtk/SoGtkBasic.h")
  file(STRINGS "${SoGtk_INCLUDE_DIR}/Inventor/Gtk/SoGtkBasic.h" _SoGtk_VERSION_DEFINE REGEX "[\t ]*#define[\t ]+SOGTK_VERSION[\t ]+\"[^\"]*\".*")
  string(REGEX REPLACE "[\t ]*#define[\t ]+SOGTK_VERSION[\t ]+\"([^\"]*)\".*" "\\1" SoGtk_VERSION "${_SoGtk_VERSION_DEFINE}")
  file(STRINGS "${SoGtk_INCLUDE_DIR}/Inventor/Gtk/SoGtkBasic.h" _SoGtk_GUI_TOOLKIT_VERSION_DEFINE REGEX "[\t ]*#define[\t ]+GUI_TOOLKIT_VERSION[\t ]+\"[0-9][0-9]*\".*")
  string(REGEX REPLACE "[\t ]*#define[\t ]+GUI_TOOLKIT_VERSION[\t ]+\"([0-9])[0-9]*\".*" "\\1" SoGtk_GUI_TOOLKIT_VERSION_MAJOR "${_SoGtk_GUI_TOOLKIT_VERSION_DEFINE}")
  if(NOT SoGtk_GUI_TOOLKIT_VERSION_MAJOR STREQUAL "")
    set(SoGtk_HAVE_GTK${SoGtk_GUI_TOOLKIT_VERSION_MAJOR} ON)
  endif()
  unset(_SoGtk_VERSION_DEFINE)
	unset(_SoGtk_GUI_TOOLKIT_VERSION_DEFINE)
endif()

set(SoGtk_DEFINITIONS -DSOGTK_DLL)
set(SoGtk_INCLUDE_DIRS ${SoGtk_INCLUDE_DIR})
set(SoGtk_LIBRARIES ${SoGtk_LIBRARY})

find_package_handle_standard_args(
  SoGtk
  FOUND_VAR SoGtk_FOUND
  REQUIRED_VARS SoGtk_INCLUDE_DIR SoGtk_LIBRARY
  VERSION_VAR SoGtk_VERSION
)

#message(STATUS "Checking for SoGtk::SoGtk target.")
if(SoGtk_FOUND AND NOT TARGET SoGtk::SoGtk)
#  message(STATUS "Setting SoGtk::SoGtk properties.")
  add_library(SoGtk::SoGtk UNKNOWN IMPORTED)
  if(SoGtk_LIBRARY_RELEASE)
    set_property(TARGET SoGtk::SoGtk APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(SoGtk::SoGtk PROPERTIES IMPORTED_LOCATION_RELEASE "${SoGtk_LIBRARY_RELEASE}")
  endif()
  if(SoGtk_LIBRARY_DEBUG)
    set_property(TARGET SoGtk::SoGtk APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(SoGtk::SoGtk PROPERTIES IMPORTED_LOCATION_DEBUG "${SoGtk_LIBRARY_DEBUG}")
  endif()
  set_target_properties(SoGtk::SoGtk PROPERTIES INTERFACE_COMPILE_DEFINITIONS "SOGTK_DLL")
  set_target_properties(SoGtk::SoGtk PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SoGtk_INCLUDE_DIRS}")
endif()

mark_as_advanced(SoGtk_DEFINITIONS)
mark_as_advanced(SoGtk_INCLUDE_DIRS)
mark_as_advanced(SoGtk_LIBRARIES)
