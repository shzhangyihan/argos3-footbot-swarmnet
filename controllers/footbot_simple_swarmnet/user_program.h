START_USER_PROGRAM

typedef struct state {
    int id;
} state_t;

Channel * channel;
Publisher * publisher;
Subscriber * subscriber;
state_t my_state;

void sent_callback() {
   printf("%d sent\n", my_state.id);
   publisher->send((unsigned char *) &my_state, sizeof(my_state));
}

void recv_callback(unsigned char * msg, int size, int ttl, Meta_t * meta) {
   state_t * recv_state = (state_t *) msg;
   printf("%d recv with id %d from dist %d\n", my_state.id, recv_state->id, meta->dist);
}

void loop() {
   // publisher->send((unsigned char *) &my_state, sizeof(my_state));
}

void setup() {
    channel = swarmnet->new_channel(0, 0, true);
    publisher = channel->new_publisher(sent_callback);
    subscriber = channel->new_subscriber(200, recv_callback);
    my_state.id = swarmos.random_func();
    publisher->send((unsigned char *) &my_state, sizeof(my_state));
}

END_USER_PROGRAM