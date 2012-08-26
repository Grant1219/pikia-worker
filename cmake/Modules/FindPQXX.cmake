# Find PostgreSQL C++ library and header file
# Sets
#   PQXX_FOUND to 0 or 1 depending on result
#   PQXX_INCLUDE_DIR to the directory containing the pqxx headers
#   PQXX_LIBRARIES to the pqxx client library

if (NOT PQXX_INCLUDE_DIR OR NOT PQXX_LIBRARIES)

    file (TO_CMAKE_PATH "$ENV{PQXX_DIR}" _PQXX_DIR)

    find_library (PQXX_LIBRARY
        NAMES libpqxx pqxx
        PATHS
        ${_PQXX_DIR}/lib
        ${_PQXX_DIR}
        ${CMAKE_INSTALL_PREFIX}/lib
        ${CMAKE_INSTALL_PREFIX}/bin
        /usr/local/pgsql/lib
        /usr/local/lib
        /usr/lib
        DOC "Location of libpqxx library"
        NO_DEFAULT_PATH
        )

    find_path (PQXX_INCLUDE_DIR
        NAMES pqxx/pqxx
        PATHS
        ${_PQXX_DIR}/include
        ${_PQXX_DIR}
        ${CMAKE_INSTALL_PREFIX}/include
        /usr/local/pgsql/include
        /usr/local/include
        /usr/include
        DOC "Path to pqxx/pqxx header file. Do not include the 'pqxx' directory in this value."
        NO_DEFAULT_PATH
        )

    if (PQXX_LIBRARY)
        set (PQXX_LIBRARIES ${PQXX_LIBRARY} CACHE STRING "Libraries for PostgreSQL C++ interface")
    endif (PQXX_LIBRARY)

    INCLUDE (FindPackageHandleStandardArgs)
    # handle the QUIETLY and REQUIRED arguments and set LUABIND_FOUND to TRUE if
    # all listed variables are TRUE
    find_package_handle_standard_args (PQXX DEFAULT_MSG PQXX_LIBRARIES PQXX_INCLUDE_DIR)

    mark_as_advanced (PQXX_INCLUDE_DIR PQXX_LIBRARIES PQXX_LIBRARY)

endif (NOT PQXX_INCLUDE_DIR OR NOT PQXX_LIBRARIES)
