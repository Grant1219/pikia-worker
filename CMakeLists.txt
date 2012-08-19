cmake_minimum_required (VERSION 2.8)
project (WORKER)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${WORKER_SOURCE_DIR}/cmake/Modules/")

set (Boost_USE_MULTITHREADED ON)

find_package (Boost COMPONENTS filesystem program_options thread signals system REQUIRED)
find_package (Lua51 REQUIRED)
find_package (PostgreSQL REQUIRED)
find_package (PQXX REQUIRED)
find_package (Luabind REQUIRED)

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${LUA_INCLUDE_DIR})
include_directories (${PostgreSQL_INCLUDE_DIRS})
include_directories (${PQXX_INCLUDE_DIR})
include_directories (${LUABIND_INCLUDE_DIR})

include_directories ("${WORKER_SOURCE_DIR}/include")
include_directories ("${WORKER_SOURCE_DIR}/vendor/include")

link_directories ("${WORKER_SOURCE_DIR}/vendor/lib")

add_definitions (-std=c++0x)

set (SRC
    "${WORKER_SOURCE_DIR}/src/configuration.cpp"
    "${WORKER_SOURCE_DIR}/src/connection_bundle.cpp"
    "${WORKER_SOURCE_DIR}/src/job_buffer.cpp"
    "${WORKER_SOURCE_DIR}/src/job_context.cpp"
    "${WORKER_SOURCE_DIR}/src/job_dispatcher.cpp"
    "${WORKER_SOURCE_DIR}/src/script_manager.cpp"
    "${WORKER_SOURCE_DIR}/src/worker.cpp"
    "${WORKER_SOURCE_DIR}/src/main.cpp"
    )

add_executable (worker ${SRC})

target_link_libraries (worker ${Boost_LIBRARIES})
target_link_libraries (worker ${LUA_LIBRARIES})
target_link_libraries (worker ${PostgreSQL_LIBRARIES})
target_link_libraries (worker ${PQXX_LIBRARIES})
target_link_libraries (worker ${LUABIND_LIBRARIES})

target_link_libraries (worker beanstalk hiredis)
