set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# "ON" = build Option 1, "OFF" = build Option 2.
option(BUILD_ASSIGNMENT_04_OPTION_1 "Build Assignment Option 1 (Standard)" OFF)

add_executable(Assignment-04
    main.cpp
 "Graph.h" "Graph.cpp")

if(BUILD_ASSIGNMENT_04_OPTION_1)
    target_sources(Assignment-04
      PRIVATE
        option1.cpp
        option1.h
    )
    target_compile_definitions(Assignment-04 PRIVATE "ASSIGNMENT_04_OPTION=1")
else()
    target_sources(Assignment-04
      PRIVATE
        option2.cpp
        option2.h
    )
    target_compile_definitions(Assignment-04 PRIVATE "ASSIGNMENT_04_OPTION=2")
endif()

target_link_libraries(Assignment-04
    PRIVATE
    SharedLib
)


add_custom_command(TARGET Assignment-04 POST_BUILD
  # Add a custom command here if needed
  COMMAND ${CMAKE_COMMAND} -E echo "Assignment-04 post-build step"
)
