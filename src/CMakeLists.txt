add_subdirectory(ActorGraph) # Uncomment this line for final submission
add_subdirectory(Map)

add_executable (pathfinder.exe pathfinder.cpp)
target_link_libraries(pathfinder.exe PRIVATE actorgraph)

add_executable (linkpredictor.exe linkpredictor.cpp)
target_link_libraries(linkpredictor.exe PRIVATE actorgraph)

