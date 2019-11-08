add_library(footbot_sn_firefly_new SHARED footbot_sn_firefly_new.h footbot_sn_firefly_new.cpp footbot_sn_driver.cpp)
target_link_libraries(footbot_sn_firefly_new
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  swarmnet
  swarmos)
