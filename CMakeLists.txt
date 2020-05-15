cmake_minimum_required(VERSION 3.14)
project(word_counting)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS -pthread)

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
find_package(TBB 2020 REQUIRED tbb OPTIONAL_COMPONENTS tbbmalloc)

include_directories(${Boost_INCLUDE_DIRS} )
add_executable(word_counting main.cpp utils.h utils.cpp indexing.h indexing.cpp merge.h merge.cpp)
target_link_libraries(word_counting TBB::tbb ${LibArchive} ${Boost_LIBRARIES})