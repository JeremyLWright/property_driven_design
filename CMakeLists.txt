cmake_minimum_required(VERSION 3.3)

IF(MSVC)
set(CMAKE_CXX_FLAGS "/EHsc")
ENDIF()
include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
    autocheck
	GIT_REPOSITORY git@github.com:JeremyLWright/autocheck.git
    TIMEOUT 30
    CMAKE_ARGS -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    # Disable install step
    INSTALL_COMMAND ""
	UPDATE_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir for googlemock and googletest
ExternalProject_Get_Property(autocheck source_dir)
include_directories(${source_dir}/include)
include_directories(${source_dir}/gtest/include)
include_directories(include)

ExternalProject_Add(
    googlemock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    TIMEOUT 30
    CMAKE_ARGS -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir for googlemock and googletest
ExternalProject_Get_Property(googlemock source_dir)
include_directories(${source_dir}/include)
include_directories(${source_dir}/gtest/include)
include_directories(include)

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")
include_directories(${CATCH_INCLUDE_DIR})

enable_testing(true)

add_executable(01_minmax 01_minmax/minmax.test.cpp)
add_dependencies(01_minmax googlemock autocheck)


add_executable(01_catch_minmax 01_minmax/minmax.catch.test.cpp)
add_dependencies(01_catch_minmax catch)

ExternalProject_Get_Property(googlemock binary_dir)
set(EXTRA_LIBRARIES 
    ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES} 
    ${binary_dir}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})
target_link_libraries(01_minmax ${EXTRA_LIBRARIES})
