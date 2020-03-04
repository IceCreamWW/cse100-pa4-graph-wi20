add_executable (test_ActorGraph.exe test_ActorGraph.cpp)
target_link_libraries(test_ActorGraph.exe PRIVATE gtest_main actorgraph)
add_test(test_ActorGraph test_ActorGraph.exe)

add_executable (test_Map.exe test_Map.cpp)
target_link_libraries(test_Map.exe PRIVATE gtest_main actorgraph)
add_test(test_Map test_Map.exe)
