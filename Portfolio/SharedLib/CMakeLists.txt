# --- Step 1: Create the Library ---

# Define a library target named "SharedLib".
# We use STATIC because we are using cpp and header files.
add_library(SharedLib STATIC)

# --- Step 2: Add Header Files to the Library ---

# This command explicitly lists the header files that belong to the library.
# This helps Visual Studio display them nicely in the Solution Explorer.
target_sources(SharedLib
    PUBLIC
    # You can add more functionalty to SharedLib.h just by adding more definitions in SharedLib.h.
    SharedLib.h
    LinkedList.hpp
    # Or add other shared files here
    PRIVATE
    ReadNames.cpp
    ReadGraph.cpp
    ReadSongs.cpp
    FileReaderUtils.cpp
    BinarySearchTable.hpp

)

# --- Step 3: Make Headers "Findable" ---

# This is the most important command here.
# It tells any other project that links to "SharedLib" to add this
# directory (CMAKE_CURRENT_SOURCE_DIR) to its list of include paths.
# This is what allows you to write #include "list.hpp" in your main.cpp.
# Note: CMAKE_CURRENT_SOURCE_DIR is a built-in variable that points to the directory
target_include_directories(SharedLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

