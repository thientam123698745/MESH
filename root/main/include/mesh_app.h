#pragma once
#include "esp_mesh_internal.h"
#include "esp_mesh.h"
#include "xtensa/core-macros.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define num_ap_connected 10
#define Router_ssid "Tam"
#define Router_pass "2525252525"
#define NumID 0
#define iDelay 1000
mesh_addr_t mesh_parent_addr;
extern int mesh_layer;
extern esp_netif_t *netif_sta;
extern xTaskHandle array_handle;
extern xTaskHandle buffer_handle;

extern uint8_t start_loop;
typedef struct
{
    uint8_t mac[6];
    mesh_addr_t route_table[50];
    int route_table_size;
    int layer[50];
} mesh_route_t;
typedef struct
{
    mesh_addr_t route_table;
    int layer;
    int index;
} id_device_t;
extern id_device_t id_device[2];
extern unsigned long ccount;
unsigned long start_ccount;
unsigned long end_ccount;
// xthal_get_ccount XTHAL_GET_CCOUNT
#define BENCHMARK_START()                                 \
    do                                                    \
    {                                                     \
        start_ccount = (unsigned long)XTHAL_GET_CCOUNT(); \
    } while (0)
#define BENCHMARK_END(ccount)                  \
    do                                         \
    {                                          \
        end_ccount = XTHAL_GET_CCOUNT();       \
        *ccount = (end_ccount - start_ccount); \
    } while (0)
extern mesh_route_t device_node;
extern char data_rx[300];
void wifi_init(void);
void init_scan(void);
void mesh_scan_done_handler(int num);
void tx_mesh_init(void);
void rx_mesh_init(void);
void msg_create(char *outputBuf, uint32_t maxLength, uint8_t *parent, int i);
void on_msg_parse(char *data, uint32_t len);
void mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void mesure_power_init(void);
void msg_create_power(char *outputBuf, uint32_t maxLength, double power);
void mqtt_app_publish(char *topic, char *publish_string);
void array_loop_init(void);
void array_loop(void *p);
void buffer(void *p);
// void vTaskGetRunTimeStatsT( char * pcWriteBuffer );
// char * prvWriteNameToBufferT( char * pcBuffer, const char * pcTaskName );