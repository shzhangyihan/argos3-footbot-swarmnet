START_USER_PROGRAM

Channel *channel_1;
Publisher *publisher_1;
Subscriber *subscriber_1; 
unsigned char token[5];
int rand_id;

void sent() {
    token[1] = token[1] + 1;
    publisher_1->send(token, strlen((char*)token));
}

void call_back(unsigned char * msg, int size, int hop, Meta_t * meta) {
    printf("%d: Recv dist = %d, size = %d, counter = %d", rand_id, meta->dist, size, (int)msg[1]);
    printf(", from = %d, delay = %d\r\n", (int)msg[0], meta->msg_delay);
} 
void loop() {
}

void setup() {
    rand_id = rand() % 255;
    strncpy((char*)token, " Hello world!", 5);
    token[0] = rand_id;
    token[1] = 0;
    channel_1 = swarmnet->new_channel(1, 0, true);
    publisher_1 = channel_1->new_publisher(sent);
    subscriber_1 = channel_1->new_subscriber(100, call_back);
    publisher_1->send(token, strlen((char*)token));
}

END_USER_PROGRAM