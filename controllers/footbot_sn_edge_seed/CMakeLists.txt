add_library(footbot_sn_edge_seed SHARED footbot_sn_edge_seed.h footbot_sn_edge_seed.cpp footbot_sn_driver.cpp)
target_link_libraries(footbot_sn_edge_seed
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  swarmnet
  swarmos)
