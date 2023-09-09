enable_testing()

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include")

set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_executable(runLruCacheTests Test_LruCache.cpp  main.cpp)
target_link_libraries(runLruCacheTests libgtest libgmock)
add_test(LruCacheUnitTests runLruCacheTests)


add_executable(runExceptionsTests Test_Exceptions.cpp  main.cpp)
target_link_libraries(runExceptionsTests libgtest libgmock)
add_test(ExceptionsUnitTests runExceptionsTests)