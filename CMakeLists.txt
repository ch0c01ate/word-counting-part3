cmake_minimum_required(VERSION 3.14)
project(word_counting_part3)

set(CMAKE_CXX_STANDARD 14)

find_package(TBB 2020 REQUIRED tbb OPTIONAL_COMPONENTS tbbmalloc)
message(STATUS "Parallel STL uses TBB ${TBB_VERSION} (interface version: ${TBB_INTERFACE_VERSION})")

if (WIN32)
    find_library( LibArchive NAMES libarchive.dll )
elseif(APPLE)
    find_library( LibArchive NAMES libarchive.dylib )
else()
    find_library( LibArchive NAMES libarchive.so )
endif ()

find_package( ICU REQUIRED COMPONENTS uc i18n )

set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)
find_package( Boost REQUIRED COMPONENTS filesystem locale system)

add_executable(word_counting_part3 main.cpp utils.cpp utils.h)

target_link_libraries(word_counting_part3 TBB::tbb ${LibArchive} ${Boost_LIBRARIES})