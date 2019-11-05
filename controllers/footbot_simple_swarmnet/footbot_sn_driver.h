#pragma once
/* Include swarmnet */
#include "./swarmnet/swarmnet.h"
#include "./swarmos/swarmos.h"
#include <functional>

class my_footbot_driver_interface {
   public:
      SwarmOS swarmos;
      Swarmnet * swarmnet;
      virtual void loop() = 0;
      virtual void setup() = 0;
      std::function<unsigned int ()> get_footbot_clock;
      std::function<unsigned int ()> get_footbot_rand;

      void register_footbot_clock(std::function<unsigned int ()> get_footbot_clock) {
         this->get_footbot_clock = get_footbot_clock;
      }

      void register_footbot_rand(std::function<unsigned int ()> get_footbot_rand) {
         this->get_footbot_rand = get_footbot_rand;
      }

      int next_pkt(unsigned char * pkt_array) {
         int ret = swarmnet->next_pkt(pkt_array);
         return ret;
      }

      void recv_pkt(unsigned char * pkt_array, size_t size, Meta_t * meta) {
         swarmnet->receive(pkt_array, size, meta);
      }

      unsigned int get_clock() {
         return get_footbot_clock();
      }

      unsigned int custom_rand() {
         return get_footbot_rand();
      }

      void driver_loop() {
         swarmos.execute_loop();
      }

      void driver_setup() {
         swarmos.set_common_sys_get_clock(std::bind(&my_footbot_driver_interface::get_clock, this));
         swarmos.set_common_sys_random_func(std::bind(&my_footbot_driver_interface::custom_rand, this));
         swarmos.register_user_loop(std::bind(&my_footbot_driver_interface::loop, this));
         swarmnet = swarmos.get_swarmnet();
         setup();
      }
};

#define START_USER_PROGRAM class my_footboot: public my_footbot_driver_interface {
#define END_USER_PROGRAM   };
#define CLASS my_footboot
#define new_publisher(func) new_publisher(std::bind(&CLASS::func, this))
#define new_subscriber(dist, func) new_subscriber(dist, std::bind(&CLASS::func, this, \
                                                  std::placeholders::_1, \
                                                  std::placeholders::_2, \
                                                  std::placeholders::_3,\
                                                  std::placeholders::_4))

#include "user_program.h"
