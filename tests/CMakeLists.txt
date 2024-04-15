add_executable(tests 
    src/DashboardTests.cpp
)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

target_link_libraries(tests Catch2::Catch2WithMain)

target_link_libraries(tests hazelsrc)

# vim:ft=cmake
