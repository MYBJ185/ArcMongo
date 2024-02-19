#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mongo::mongoc_static" for configuration "RelWithDebInfo"
set_property(TARGET mongo::mongoc_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mongo::mongoc_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/mongoc-static-1.0.lib"
  )

list(APPEND _cmake_import_check_targets mongo::mongoc_static )
list(APPEND _cmake_import_check_files_for_mongo::mongoc_static "${_IMPORT_PREFIX}/lib/mongoc-static-1.0.lib" )

# Import target "mongo::mongoc_shared" for configuration "RelWithDebInfo"
set_property(TARGET mongo::mongoc_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mongo::mongoc_shared PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/mongoc-1.0.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/mongoc-1.0.dll"
  )

list(APPEND _cmake_import_check_targets mongo::mongoc_shared )
list(APPEND _cmake_import_check_files_for_mongo::mongoc_shared "${_IMPORT_PREFIX}/lib/mongoc-1.0.lib" "${_IMPORT_PREFIX}/bin/mongoc-1.0.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
