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
static const char *MESH_TAG = "ROOT_TX";
uint8_t tx_buf[1024] = {
    0,
};
void esp_mesh_p2p_tx_main(void *arg)
{
    esp_err_t err;
    int send_count = 0;
    mesh_data_t data;
    char demo[1024] = "phuong den roi";

    data.proto = MESH_PROTO_BIN;
    data.tos = MESH_TOS_P2P;
    data.data = (uint8_t *)demo;
    data.size = sizeof(demo);

    while (true)
    {
        // ESP_LOG_BUFFER_HEX(MESH_TAG, data.data, data.size);
        for (int i = 1; i < device_node.route_table_size; i++)
        {
            // ESP_LOGI(MESH_TAG, "ID mac: " MACSTR, MAC2STR(device_node.route_table[i].addr));
            // printf("root gui xuong node " MACSTR "\n", MAC2STR(device_node.route_table[i].addr));
            if (device_node.layer[i] <= 2)
            {
                msg_create(demo, 1024, device_node.route_table[i].addr, i);
                // printf("%s\n", (char *)data.data);
                esp_err_t err = esp_mesh_send(&device_node.route_table[i], &data, MESH_DATA_P2P, NULL, 0);
                if (err != ESP_OK)
                {
                    ESP_LOGE(MESH_TAG, "send err:0x%x, size:%d", err, data.size);
                    continue;
                }
                // esp_mesh_send(NULL, &data, MESH_DATA_NONBLOCK, NULL, 0);
                vTaskDelay(1000 / portTICK_RATE_MS);
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void tx_mesh_init(void)
{
    xTaskCreatePinnedToCore(esp_mesh_p2p_tx_main, "MPTX", 8192, NULL, 5, NULL, 0);
}