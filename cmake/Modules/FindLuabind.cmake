# Locate Luabind library 
# This module defines 
#  LUABIND_FOUND, if false, do not try to link to Luabind 
#  LUABIND_LIBRARIES 
#  LUABIND_INCLUDE_DIR, where to find luabind.hpp 
# 
# Note that the expected include convention is 
#  #include <luabind/luabind.hpp> 
# and not 
#  #include <luabind.hpp> 

if (NOT LUABIND_INCLUDE_DIR OR NOT LUABIND_LIBRARIES)

    find_path (LUABIND_INCLUDE_DIR luabind.hpp
        HINTS
        $ENV{LUABIND_DIR}
        PATH_SUFFIXES luabind include/luabind include
        PATHS
        /usr/local
        /usr
        /opt/local
        /opt
        )

    find_library (LUABIND_LIBRARY
        NAMES luabind
        HINTS
        $ENV{LUABIND_DIR}
        PATH_SUFFIXES lib64 lib
        PATHS
        /usr/local
        /usr
        /opt/local
        /opt
        )

    find_library (LUABIND_LIBRARY_DBG
        NAMES luabindd
        HINTS
        $ENV{LUABIND_DIR}
        PATH_SUFFIXES lib64 lib
        PATHS
        /usr/local
        /usr
        /opt/local
        /opt
        )

    if (LUABIND_LIBRARY)
        set (LUABIND_LIBRARIES ${LUABIND_LIBRARY} CACHE STRING "Luabind Libraries")
    endif (LUABIND_LIBRARY)

    include (FindPackageHandleStandardArgs)
    # handle the QUIETLY and REQUIRED arguments and set LUABIND_FOUND to TRUE if
    # all listed variables are TRUE
    find_package_handle_standard_args (Luabind DEFAULT_MSG LUABIND_LIBRARIES LUABIND_INCLUDE_DIR)

    if (NOT LUABIND_FIND_QUIETLY)

        if (LUABIND_LIBRARY_DBG)
            message (STATUS "Luabind debug library availible: ${LUABIND_LIBRARY_DBG}")
        endif ()

    endif ()

    mark_as_advanced (LUABIND_INCLUDE_DIR LUABIND_LIBRARIES LUABIND_LIBRARY LUABIND_LIBRARY_DBG)

endif (NOT LUABIND_INCLUDE_DIR OR NOT LUABIND_LIBRARIES)
