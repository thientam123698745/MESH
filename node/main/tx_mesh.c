#include <string.h>
#include <stdio.h>

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_mesh_internal.h"
#include "mesh_light.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "mesh_app.h"
#include "esp_system.h"

static const char *MESH_TAG = "NODE_TX";
uint8_t tx_buf[1024] = {
    0,
};
int turn_on_array;
// char data_send_power[1024];
void esp_mesh_p2p_tx_main(void *arg)
{

    esp_err_t err;
    // int send_count = 0;
    char data_control_power[1024];
    char data_send_power[1024];

    // char demo[1024] = {0};
    // const mesh_opt_t opt = {
    //     .type = MESH_OPT_SEND_GROUP,
    // };
    vTaskDelay(1 * 1000 / portTICK_RATE_MS);
    mesh_data_t data;
    msg_create(data_send_power, sizeof(data_send_power), 0);
    data.size = sizeof(data_send_power);
    data.data = (uint8_t *)data_send_power;
    err = esp_mesh_send(NULL, &data, MESH_DATA_TODS, NULL, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(MESH_TAG, "send err:0x%x, size:%d", err, data.size);
    }
    memset(data_send_power, 0, sizeof(data_send_power));
    vTaskDelay(3 * 100 / portTICK_RATE_MS);

    while (true)
    {
        data.tos = MESH_TOS_P2P;
        data.proto = MESH_PROTO_BIN;
        data.size = sizeof(data_control_power);
        data.data = (uint8_t *)data_control_power;

        // ESP_LOGW(MESH_TAG, "heap:%d", esp_get_free_heap_size());
        for (int i = 1; i < device_node.route_table_size; i++)
        {
            // msg_create(demo, 200, i);
            // sprintf(demo, "node gui xuong child %d", i);
            // if (data_control_power == NULL)
            //     continue;
            msg_create(data_control_power, 1024, i);
            err = esp_mesh_send(&device_node.route_table[i], &data, MESH_DATA_P2P, NULL, 0);

            // printf("%s\n", (char *)data.data);
            if (err != ESP_OK)
            {
                ESP_LOGE(MESH_TAG, "send err:0x%x, size:%d", err, data.size);
                continue;
            }
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void tx_mesh_init(void)
{
    xTaskCreatePinnedToCore(esp_mesh_p2p_tx_main, "MPTX", 8192, NULL, 3, NULL, 0);
}