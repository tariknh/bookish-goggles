set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# "ON" = build Option 1, "OFF" = build Option 2.
option(BUILD_ASSIGNMENT_01_OPTION_1 "Build Assignment Option 1 (Standard)" OFF)

add_executable(Assignment-01
    main.cpp
 "song.cpp" "song.h" "songList.cpp" "songList.h")

# Conditionally add the correct source file
if(BUILD_ASSIGNMENT_01_OPTION_1)
    # If ON, add option1.cpp and define 'ASSIGNMENT_OPTION=1' for C++
    target_sources(Assignment-01
      PRIVATE
        option1.cpp 
        option1.h
    )
    target_compile_definitions(Assignment-01 PRIVATE "ASSIGNMENT_01_OPTION=1")
else()
    # If OFF, add option2.cpp and define 'ASSIGNMENT_OPTION=2' for C++
    target_sources(Assignment-01
      PRIVATE
        option2.cpp
        option2.h
    )
    target_compile_definitions(Assignment-01 PRIVATE "ASSIGNMENT_01_OPTION=2")
endif()
target_link_libraries(Assignment-01
    PRIVATE
    SharedLib
)


add_custom_command(TARGET Assignment-01 POST_BUILD
  # Add a custom command here if needed
  COMMAND ${CMAKE_COMMAND} -E echo "Assignment-01 post-build step"
)