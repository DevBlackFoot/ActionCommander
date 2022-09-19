#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cppcoro" for configuration "Release"
set_property(TARGET cppcoro APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cppcoro PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/cppcoro.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS cppcoro )
list(APPEND _IMPORT_CHECK_FILES_FOR_cppcoro "${_IMPORT_PREFIX}/lib/cppcoro.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
