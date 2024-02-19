#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mongo::mongocxx_shared" for configuration "RelWithDebInfo"
set_property(TARGET mongo::mongocxx_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mongo::mongocxx_shared PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/mongocxx.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "mongo::mongoc_shared"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/mongocxx.dll"
  )

list(APPEND _cmake_import_check_targets mongo::mongocxx_shared )
list(APPEND _cmake_import_check_files_for_mongo::mongocxx_shared "${_IMPORT_PREFIX}/lib/mongocxx.lib" "${_IMPORT_PREFIX}/bin/mongocxx.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
