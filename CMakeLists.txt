cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(SimpleHTTPServer VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(SimpleHTTPServer)
endif()

if(WIN32)
    set(BOOST_ROOT          "C:/boost/boost_1_62_0/boost_mingw53")
    set(Boost_USE_STATIC_LIBS        ON)
    set(Boost_USE_MULTITHREADED ON)
endif()

find_package(Boost COMPONENTS system program_options REQUIRED)
find_package(Threads)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} connection.h connection_manager.h
    header.h mime_types.h reply.h request.h request_handler.h request_parser.h server.h
    join_threads.h thread_pool.h threadsafe_queue.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )
#set(CMAKE_CXX_FLAGS "-std=c++11")//for CMake 2.8
set_target_properties (${PROJECT_NAME} PROPERTIES
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

if(WIN32)
        target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()

target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS SimpleHTTPServer RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT tima_mamedxp@mail.ru)

include(CPack)
