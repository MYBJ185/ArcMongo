include(CMakeFindDependencyMacro)
find_dependency(bson-1.0 1.26.0)

# If we need to import a TLS package for our imported targets, do that now:
set(MONGOC_TLS_BACKEND [[SecureChannel]])
set(_tls_package [[NO]])
if(_tls_package)
  # We bring our own FindLibreSSL, since most systems do not have one yet. The system's version
  # will be preferred, if possible.
  set(_prev_path "${CMAKE_MODULE_PATH}")
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/3rdParty")
  find_dependency("${_tls_package}")
  set(CMAKE_MODULE_PATH "${_prev_path}")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/mongoc-targets.cmake")

unset(_required)
unset(_quiet)
if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
  set(_required REQUIRED)
endif()
if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
  set(_quiet QUIET)
endif()

set(_mongoc_built_with_bundled_utf8proc "ON")
if(NOT _mongoc_built_with_bundled_utf8proc AND NOT TARGET PkgConfig::PC_UTF8PROC)
  # libmongoc was compiled against an external utf8proc and links against a
  # FindPkgConfig-generated IMPORTED target. Find that package and generate that
  # imported target here:
  find_dependency(PkgConfig)
  pkg_check_modules(PC_UTF8PROC ${_required} ${_quiet} libutf8proc IMPORTED_TARGET GLOBAL)
endif()

# Find dependencies for SASL
set(_sasl_backend [[SSPI]])
if(_sasl_backend STREQUAL "Cyrus")
  # We need libsasl2. The find-module should be installed within this package.
  # temporarily place it on the module search path:
  set(_prev_path "${CMAKE_MODULE_PATH}")
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/3rdParty")
  find_dependency(SASL2 2.0)
  set(CMAKE_MODULE_PATH "${_prev_path}")
endif()
