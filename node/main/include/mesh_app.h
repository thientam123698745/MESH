#pragma once
#include "esp_mesh_internal.h"
#include "esp_mesh.h"
#include "xtensa/core-macros.h"
#define num_ap_connected 10
#define Router_ssid "Tam"
#define Router_pass "2525252525"
#define NumID 3
#define iDelay 100
mesh_addr_t mesh_parent_addr;
extern int mesh_layer;
extern esp_netif_t *netif_sta;
extern int nodeId;
extern int childId;
extern int my_layer;

typedef struct
{
    uint8_t mac[6];
    mesh_addr_t route_table[50];
    int route_table_size;
} mesh_route_t;
typedef struct
{
    char id[24];
    int layer;
    int index;
} id_device_t;
extern id_device_t id_device[7];
extern unsigned long ccount;
unsigned long start_ccount;
unsigned long end_ccount;
// xthal_get_ccount XTHAL_GET_CCOUNT
// extern char data_send_power[1024];
extern int turn_on_array;
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
extern xTaskHandle array_handle;
extern xTaskHandle buffer_handle;
extern double power_str;
extern mesh_route_t device_node;
extern char data_rx[300];
void wifi_init(void);
void init_scan(void);
void mesh_scan_done_handler(int num);
void tx_mesh_init(void);
void rx_mesh_init(void);
void msg_create(char *outputBuf, uint32_t maxLength, int i);
void on_msg_parse(char *data, uint32_t len);
void mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void mesure_power_init(void);
void msg_create_power(char *outputBuf, uint32_t maxLength, double power);
void array_loop_init(void);
void on_msg_parse_control(char *data, int index);
void array_loop(void *p);
void buffer(void *p);