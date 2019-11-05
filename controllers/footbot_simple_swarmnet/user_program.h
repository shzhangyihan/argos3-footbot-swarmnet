START_USER_PROGRAM

int motor_state;

void loop() {
    printf("clock: %d motion: %d\n", swarmos.get_clock(), motor_control->current_status());
    if(motor_control->current_status() == Stop) {
        if(motor_state == 0) {
            motor_control->turn_right(50);
            LED_control->turn_on(0, 255, 0, 50);
            motor_state = 1;
            printf("left\n");
        }
        else if(motor_state == 1) {
            motor_control->move_forward(50);
            LED_control->turn_on(255, 255, 255, 50);
            motor_state = 2;
            printf("forward\n");
        }
        else if(motor_state == 2) {
            motor_control->turn_left(50);
            LED_control->turn_on(255, 0, 0, 50);
            motor_state = 3;
            printf("right\n");
        }
        else if(motor_state == 3) {
            motor_control->move_forward(50);
            LED_control->turn_on(255, 255, 255, 50);
            motor_state = 0;
            printf("forward\n");
        }
    }
}

void setup() {
    motor_state = 0;
}

END_USER_PROGRAM
