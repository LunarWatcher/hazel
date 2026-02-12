add_executable(tests 
    src/Main.cpp
)

Include(FetchContent)

set(CATCH_CONFIG_DEFAULT_REPORTER "explicit" CACHE STRING "" FORCE)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.12.0
)


FetchContent_Declare(stc
    GIT_REPOSITORY https://github.com/LunarWatcher/stc
)

FetchContent_MakeAvailable(Catch2 stc)

find_package(cpr REQUIRED)

target_link_libraries(tests Catch2::Catch2 stc::stc stc::testutil cpr::cpr)

target_link_libraries(tests hazelsrc)

# vim:ft=cmake
