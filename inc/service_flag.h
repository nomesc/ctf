#include "request.h"

struct service_flag
{
    bool color;
    char *country_name;
}

void
service_flag_cb(struct request *req);

int handle_service_flag(struct request *req);
