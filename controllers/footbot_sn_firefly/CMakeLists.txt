add_library(footbot_sn_firefly SHARED footbot_sn_firefly.h footbot_sn_firefly.cpp footbot_sn_driver.cpp)
target_link_libraries(footbot_sn_firefly
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  swarmnet
  swarmos)
