#pragma once
/* Include swarmnet */
#include "./swarmnet/swarmnet.h"
#include <functional>

class my_footbot_driver_interface {
   public:
      Swarmnet * swarmnet;
      virtual void loop() = 0;
      virtual void setup() = 0;

      int next_pkt(unsigned char * pkt_array) {
         int ret = swarmnet->next_pkt(pkt_array);
         return ret;
      }
};

class my_footboot: public my_footbot_driver_interface {

typedef struct state {
    int id;
} state_t;

Channel * channel;
Publisher * publisher;
Subscriber * subscriber;
state_t my_state;

void sent_callback() {
   printf("%d sent\n", my_state.id);
}

void recv_callback(unsigned char * msg, int size, int ttl, Meta_t * meta) {
}

void loop() {
   // publisher->send((unsigned char *) &my_state, sizeof(my_state));
   printf("in loop\n");
}

void setup() {
    printf("setup\n");
    swarmnet = new Swarmnet();
    channel = swarmnet->new_channel(0, 0, true);
    publisher = channel->new_publisher(std::bind(&my_footboot::sent_callback, this));
    subscriber = channel->new_subscriber(200, std::bind(&my_footboot::recv_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
   //  publisher->send((unsigned char *) &my_state, sizeof(my_state));
}
};