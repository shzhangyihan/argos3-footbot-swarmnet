#pragma once
/* Include swarmnet */
#include "./swarmnet/swarmnet.h"
#include "./swarmos/swarmos.h"
#include <functional>

#define NUM_OF_CONTROL_UNITS 2

enum enumType {Stop, Move_forward, Turn_left, Turn_right, On, Off};

class Motor_control_unit: public Control_unit {
    public:
        void update_control();
        void move_forward(unsigned long time);
        void turn_left(unsigned long time);
        void turn_right(unsigned long time);
        void stop_motor();
        int current_status();
        void register_common_sys(Common_system * common_sys);
        void register_this(void * this_robot);
        Motor_control_unit();
        ~Motor_control_unit();
    
    private:
        int status;
        unsigned long time_left;
        unsigned long start_time;
        Common_system * common_sys;
        void * this_robot;
};

class LED_control_unit: public Control_unit {
    public:
        void update_control();
        void turn_on(uint8_t red, uint8_t green, uint8_t blue, int time);
        int current_status();
        void register_common_sys(Common_system * common_sys);
        void register_this(void * this_robot);
        LED_control_unit();
        ~LED_control_unit();
    
    private:
        int status;
        unsigned long time_left;
        unsigned long start_time;
        Common_system * common_sys;
        void * this_robot;
};

class My_control_factory: public Control_factory {
    public:
        void update_control();
        Control_unit * get_control_unit(int index);
        void register_this(void * this_robot);
        void register_common_sys(Common_system * common_sys);
        My_control_factory();
        ~My_control_factory();
    
    private:
        Control_unit * my_control_unit[NUM_OF_CONTROL_UNITS];
        Motor_control_unit my_motor_control;
        LED_control_unit my_LED_control;
};

class my_footbot_driver_interface {
   public:
      SwarmOS swarmos;
      Swarmnet * swarmnet;
      Motor_control_unit * motor_control;
      LED_control_unit * LED_control;
      My_control_factory my_control_factory;
      virtual void loop() = 0;
      virtual void setup() = 0;
      std::function<unsigned int ()> get_footbot_clock;
      std::function<unsigned int ()> get_footbot_rand;
      std::function<void (uint8_t, uint8_t, uint8_t)> set_footbot_LED;
      std::function<void (int, int)> set_footbot_motor;

      void register_footbot_clock(std::function<unsigned int ()> get_footbot_clock);
      void register_footbot_rand(std::function<unsigned int ()> get_footbot_rand);
      void register_footbot_set_LED(std::function<void (uint8_t, uint8_t, uint8_t)> set_footbot_LED);
      void register_footbot_set_motor(std::function<void (int, int)> set_footbot_motor);
      int next_pkt(unsigned char * pkt_array);
      void recv_pkt(unsigned char * pkt_array, size_t size, Meta_t * meta);
      unsigned int get_clock();
      unsigned int custom_rand();
      void driver_loop();
      void driver_setup();
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
