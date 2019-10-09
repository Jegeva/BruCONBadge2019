#include "wifi.h"
#include "menu.h"

#include "freertos/event_groups.h"

#include "esp_event_loop.h"

ip_addr_t ip_Addr;
ip4_addr_t ip;
ip4_addr_t gw;
ip4_addr_t msk;


volatile bool bConnected = false;
volatile bool bneedWifi = true;
volatile bool bDNSFound = false;


bool wifiIsConnected()
{
    return bConnected;

}

bool isdnsfound()
{
    return bDNSFound;

}


esp_err_t wifi_event_cb(void *ctx, system_event_t *event)
{
    system_event_sta_disconnected_t * disco;

    switch( event->event_id){
    case SYSTEM_EVENT_STA_START:
        printf("sta start\n");
         esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        printf("got ip\n");
        ip = event->event_info.got_ip.ip_info.ip;
        gw = event->event_info.got_ip.ip_info.gw;
        msk = event->event_info.got_ip.ip_info.netmask;
        bConnected = true;
        //     xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        printf("connected!\n");

        break;


    case SYSTEM_EVENT_STA_STOP:
        printf("sta stop\n");
        bConnected = false;
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
        disco = (system_event_sta_disconnected_t *)&event->event_info;


        printf("sta disco need:%d id:%d info:%d\n",bneedWifi,(int)event->event_id,disco->reason);
        bConnected = false;

        break;

    case SYSTEM_EVENT_STA_LOST_IP:
             /* This is a workaround as ESP32 WiFi libs don't currently
                auto-reassociate. */
        printf("sta lost ip\n");
        bConnected = false;
        //xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;

    default:
        printf("unmanaged wifi event : %d\n",event->event_id);

        break;
    }

    return ESP_OK;
}

void BruconWifiStop()
{
    if(!bneedWifi)
        if(ESP_OK == esp_wifi_stop()){
            bConnected = false;
        }
    uint32_t battpin = 9999;
    xQueueSendFromISR(gpio_evt_queue, &battpin, NULL);
}


void needWifi(bool need)
{
   if(bConnected)
     return;
    bneedWifi=need;
    while(!bConnected);

}

char wifi_inited =0;
void wificonnect()
{
    while(1){
        printf("WIFI RQ ST n:%d c:%d\n",bneedWifi,bConnected);
        if(bConnected){	  
            bneedWifi = false;
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
        if(bneedWifi){
            last_click = xTaskGetTickCount();
            if(wifi_inited == 0){
                esp_wifi_disconnect();
                esp_wifi_stop();
                esp_wifi_deinit();

                wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
                ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
                ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
                ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
                wifi_config_t sta_config;
                strcpy( (char*)sta_config.sta.ssid, get_wifi_netname() );
                strcpy( (char*)sta_config.sta.password, get_wifi_pass() );
                printf("connecting to %s \n",sta_config.sta.ssid);
                ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
                wifi_inited=1;
            }

            esp_wifi_disconnect();
            esp_wifi_stop();

            ESP_ERROR_CHECK( esp_wifi_start() );
            esp_wifi_set_max_tx_power(127);
            ESP_ERROR_CHECK( esp_wifi_connect() );
            while( !bConnected )
                vTaskDelay(2);
            ;
            printf("Got IP: %s\n", inet_ntoa( ip ) );
            printf("Net mask: %s\n", inet_ntoa( msk ) );
            printf("Gateway: %s\n", inet_ntoa( gw ) );
            IP_ADDR4( &ip_Addr, 0,0,0,0 );
            if(bConnected)
                bneedWifi = false;
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);

    }

}
