#include "request.h"

int main()
{
    int ret;
    struct request add_country = {0};
    struct request see_info1 = {0};
    struct request see_info2 = {0};

    add_country.message = "ADD test1 0";
    add_country.message_length = strlen(add_country.message);

    see_info1.message = "POP romania";
    see_info1.message_length = strlen(see_info1.message);

    see_info2.message = "POP test1";
    see_info2.message_length = strlen(see_info2.message);

    ret = create_request(&see_info1, 1024);
    if (ret != 0)
    {
        puts("Err see_info1");
        return -1;
    }
    ret = read_response(&see_info1);
    printf("%d\t%s\n", ret, see_info1.response);

    ret = create_request(&add_country, 1024);
    if (ret != 0)
    {
        puts("Err add_country");
        return -1;
    }

    ret = create_request(&see_info2, 1024);
    if (ret != 0)
    {
        puts("Err see_info2");
        return -1;
    }
    ret = read_response(&see_info2);
    printf("%d\t%s\n", ret, see_info2.response);

    close_request(&see_info1);
    close_request(&add_country);
    close_request(&see_info2);

    return 0;
}