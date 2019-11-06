add_library(footbot_sn_hop_seed_2 SHARED footbot_sn_hop_seed_2.h footbot_sn_hop_seed_2.cpp footbot_sn_driver.cpp)
target_link_libraries(footbot_sn_hop_seed_2
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  swarmnet
  swarmos)
