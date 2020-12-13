#include "request.h"

int main()
{
    int ret;
    struct request addCountry = {0};
    struct request seeReviews = {0};

    addCountry.message = "ADD ./data/secrets.txt 3";
    addCountry.message_length = 25;

    seeReviews.message = "SEE 2";
    seeReviews.message_length = 6;

    ret = create_request(&addCountry, 1024);
    if (ret != 0)
    {
        puts("Err add_country");
        return -1;
    }
    close_request(&addCountry);
    sleep(2);
    ret = create_request(&seeReviews, 1024);
    if (ret != 0)
    {
        puts("Err add_country");
        return -1;
    }
    read_response(&seeReviews);
    printf("%s\n", seeReviews.response);
    return 0;
}