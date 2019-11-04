add_library(footbot_simple_swarmnet SHARED footbot_simple_swarmnet.h footbot_simple_swarmnet.cpp)
target_link_libraries(footbot_simple_swarmnet
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  swarmnet
  swarmos)
add_subdirectory(swarmnet)
add_subdirectory(swarmos)
