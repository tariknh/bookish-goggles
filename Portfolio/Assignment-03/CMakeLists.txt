set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 4.0)
project(Assignment-03)

# "ON" = build Option 1, "OFF" = build Option 2.
option(BUILD_ASSIGNMENT_03_OPTION_1 "Build Assignment Option 1 (Standard)" OFF)

add_executable(Assignment-03
    main.cpp
   "TExpressionTree.cpp")

# Conditionally add the correct source file
if(BUILD_ASSIGNMENT_03_OPTION_1)
    # If ON, add option1.cpp and define 'ASSIGNMENT_OPTION=1' for C++
    target_sources(Assignment-03
      PRIVATE
        option1.cpp 
        option1.h
    )
    target_compile_definitions(Assignment-03 PRIVATE "ASSIGNMENT_03_OPTION=1")
else()
    # If OFF, add option2.cpp and define 'ASSIGNMENT_OPTION=2' for C++
    target_sources(Assignment-03
      PRIVATE
        option2.cpp
        option2.h
    )
    target_compile_definitions(Assignment-03 PRIVATE "ASSIGNMENT_03_OPTION=2")
endif()
target_link_libraries(Assignment-03
    PRIVATE
   # SharedLib
)


add_custom_command(TARGET Assignment-03 POST_BUILD
  # Add a custom command here if needed
  COMMAND ${CMAKE_COMMAND} -E echo "Assignment-03 post-build step"
)
