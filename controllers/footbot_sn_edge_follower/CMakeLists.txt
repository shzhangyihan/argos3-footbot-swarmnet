add_library(footbot_sn_edge_follower SHARED footbot_sn_edge_follower.h footbot_sn_edge_follower.cpp footbot_sn_driver.cpp)
target_link_libraries(footbot_sn_edge_follower
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  swarmnet
  swarmos)
