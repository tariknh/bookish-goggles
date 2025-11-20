set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# "ON" = build Option 1, "OFF" = build Option 2.
option(BUILD_ASSIGNMENT_02_OPTION_1 "Build Assignment Option 1 (Standard)" ON)

add_executable(Assignment-02
    main.cpp
    DirectoryTypes.h
    DirectoryTypes.cpp
)

# Conditionally add the correct source file
if(BUILD_ASSIGNMENT_02_OPTION_1)
    # If ON, add option1.cpp and define 'ASSIGNMENT_OPTION=1' for C++
    target_sources(Assignment-02
      PRIVATE
        option1.cpp 
        option1.h
    )
    target_compile_definitions(Assignment-02 PRIVATE "ASSIGNMENT_02_OPTION=1")
else()
    # If OFF, add option2.cpp and define 'ASSIGNMENT_OPTION=2' for C++
    target_sources(Assignment-02
      PRIVATE
        option2.cpp
        option2.h
    )
    target_compile_definitions(Assignment-02 PRIVATE "ASSIGNMENT_02_OPTION=2")
endif()
target_link_libraries(Assignment-02
    PRIVATE
    SharedLib
)


add_custom_command(TARGET Assignment-02 POST_BUILD
  # Add a custom command here if needed
  COMMAND ${CMAKE_COMMAND} -E echo "Assignment-02 post-build step"
)
