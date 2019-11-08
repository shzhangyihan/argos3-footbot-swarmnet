#pragma once
#include "footbot_sn_driver.h"

/*
 * motor control unit
 */

void Motor_control_unit::update_control() {
    if(this->common_sys->get_clock == NULL) return;
    if(this->status == Stop || this->time_left == 0) return;
    unsigned long cur_time = this->common_sys->get_clock();
    if(cur_time - this->start_time > this->time_left) {
        status = Stop;
        time_left = 0;
        ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_motor(0, 0);
    }
}

void Motor_control_unit::move_forward(unsigned long time) {
    if(this->common_sys->get_clock == NULL) return;
    this->start_time = (this->common_sys)->get_clock();
    this->status = Move_forward;
    this->time_left = time;
    ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_motor(2, 2);
}

void Motor_control_unit::turn_left(unsigned long time) {
    if(this->common_sys->get_clock == NULL) return;
    this->start_time = this->common_sys->get_clock();
    this->status = Turn_left;
    this->time_left = time;
    ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_motor(1, 2);
}

void Motor_control_unit::turn_right(unsigned long time) {
    if(this->common_sys->get_clock == NULL) return;
    this->start_time = this->common_sys->get_clock();
    this->status = Turn_right;
    this->time_left = time;
    ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_motor(2, 1);
}

void Motor_control_unit::stop_motor() {
    if(this->common_sys->get_clock == NULL) return;
    this->start_time = this->common_sys->get_clock();
    this->status = Stop;
    this->time_left = 0;
    ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_motor(0, 0);
}

int Motor_control_unit::current_status() {
    return this->status;
}

void Motor_control_unit::register_common_sys(Common_system * common_sys) {
    this->common_sys = common_sys;
}

void Motor_control_unit::register_this(void * this_robot) {
    this->this_robot = this_robot;
}

Motor_control_unit::Motor_control_unit() {
    time_left = 0;
    status = Stop;
}

Motor_control_unit::~Motor_control_unit() { }


/*
 * LED control unit
 */

void LED_control_unit::update_control() {
    if(this->common_sys->get_clock == NULL) return;
    if(this->status == Off || this->time_left == 0) return;
    unsigned long cur_time = this->common_sys->get_clock();
    if(cur_time - this->start_time > this->time_left) {
        status = Off;
        time_left = 0;
        ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_LED(0, 0, 0);
    }
}

void LED_control_unit::turn_on(uint8_t red, uint8_t green, uint8_t blue, int time) {
    if(this->common_sys->get_clock == NULL) return;
    this->start_time = (this->common_sys)->get_clock();
    this->status = On;
    this->time_left = time;
    ((my_footbot_driver_interface *)(this->this_robot))->set_footbot_LED(red, green, blue);;
}

int LED_control_unit::current_status() {
    return this->status;
}

void LED_control_unit::register_common_sys(Common_system * common_sys) {
    this->common_sys = common_sys;
}

void LED_control_unit::register_this(void * this_robot) {
    this->this_robot = this_robot;
}

LED_control_unit::LED_control_unit() {
    time_left = 0;
    status = Off;
}

LED_control_unit::~LED_control_unit() { }


/*
 * control factory
 */

void My_control_factory::update_control() {
    for(int i = 0; i < NUM_OF_CONTROL_UNITS; i++) {
        if(my_control_unit[i] != NULL) {
            my_control_unit[i]->update_control();
        }
    }
}

Control_unit * My_control_factory::get_control_unit(int index) {
    return my_control_unit[index];
}

void My_control_factory::register_this(void * this_robot) {
    ((Motor_control_unit *)my_control_unit[0])->register_this(this_robot);
    ((LED_control_unit *)my_control_unit[1])->register_this(this_robot);
}

void My_control_factory::register_common_sys(Common_system * common_sys) {
    this->common_sys = common_sys;
    for(int i = 0; i < NUM_OF_CONTROL_UNITS; i++) {
        my_control_unit[i]->register_common_sys(this->common_sys);
    }
}

My_control_factory::My_control_factory() {
    my_control_unit[0] = &my_motor_control;
    my_control_unit[1] = &my_LED_control;
}

My_control_factory::~My_control_factory() { }
    
/*
 * actual footbot swarmnet driver interface
 */

void my_footbot_driver_interface::register_footbot_clock(std::function<uint16_t ()> get_footbot_clock) {
    this->get_footbot_clock = get_footbot_clock;
}

void my_footbot_driver_interface::register_footbot_rand(std::function<unsigned int ()> get_footbot_rand) {
    this->get_footbot_rand = get_footbot_rand;
}

void my_footbot_driver_interface::register_footbot_set_LED(std::function<void (uint8_t, uint8_t, uint8_t)> set_footbot_LED) {
    this->set_footbot_LED = set_footbot_LED;
}

void my_footbot_driver_interface::register_footbot_set_motor(std::function<void (int, int)> set_footbot_motor) {
    this->set_footbot_motor = set_footbot_motor;
}

int my_footbot_driver_interface::next_pkt(unsigned char * pkt_array) {
    int ret = swarmnet->next_pkt(pkt_array);
    return ret;
}

void my_footbot_driver_interface::recv_pkt(unsigned char * pkt_array, size_t size, Meta_t * meta) {
    swarmnet->receive(pkt_array, size, meta);
}

uint16_t my_footbot_driver_interface::get_clock() {
    return get_footbot_clock();
}

unsigned int my_footbot_driver_interface::custom_rand() {
    return get_footbot_rand();
}

void my_footbot_driver_interface::driver_loop() {
    swarmos.execute_loop();
}

void my_footbot_driver_interface::driver_setup() {
    swarmos.set_common_sys_get_clock(std::bind(&my_footbot_driver_interface::get_clock, this));
    swarmos.set_common_sys_random_func(std::bind(&my_footbot_driver_interface::custom_rand, this));
    swarmos.register_user_loop(std::bind(&my_footbot_driver_interface::loop, this));
    my_control_factory.register_this(this);
    swarmos.register_control_factory(&my_control_factory);
    motor_control = (Motor_control_unit *) my_control_factory.get_control_unit(0);
    LED_control = (LED_control_unit *) my_control_factory.get_control_unit(1);
    swarmnet = swarmos.get_swarmnet();

    setup();
}