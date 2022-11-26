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
static const char *MESH_TAG = "NODE_RX";
uint8_t rx_buf[1024] = {
    0,
};
void esp_mesh_p2p_rx_main(void *arg)
{
    // int recv_count = 0;
    mesh_addr_t from;
    // int send_count = 0;
    mesh_data_t data;
    int flag = 0;
    data.data = rx_buf;
    const mesh_opt_t opt = {
        .type = MESH_OPT_SEND_GROUP,
    };
    // data.size = sizeof(rx_buf);
    data.size = 1024;

    while (true)
    {
        esp_err_t err = esp_mesh_recv(&from, &data, portMAX_DELAY, &flag, NULL, 0);
        if (err != ESP_OK)
        {
            ESP_LOGE(MESH_TAG, "err:0x%x, size:%d", err, data.size);
            continue;
        }

        on_msg_parse((char *)data.data, data.size);
        ESP_LOGW(MESH_TAG, "layer %d\n", my_layer);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void rx_mesh_init(void)
{
    xTaskCreatePinnedToCore(esp_mesh_p2p_rx_main, "MPRX", 8192, NULL, 4, NULL, 0);
}