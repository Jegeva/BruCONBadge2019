#include "settings.h"
#include "settings.priv.h"


const char * get_wifi_netname()
{
    return WIFI_SSID;

}

const char * get_wifi_pass(    )
{
    return WIFI_PASSWORD;

}

char * get_client_cert();
