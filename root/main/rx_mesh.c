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
static const char *MESH_TAG = "ROOT_RX";
uint8_t rx_buf[1024] = {
    0,
};
void esp_mesh_p2p_rx_main(void *arg)
{
    int recv_count = 0;
    mesh_addr_t from;
    int send_count = 0;
    mesh_data_t data;
    int flag = 0;
    data.data = rx_buf;
    // data.size = sizeof(rx_buf);
    // data.size = 100;
    while (true)
    {
        data.size = sizeof(rx_buf);
        esp_err_t err = esp_mesh_recv(&from, &data, portMAX_DELAY, &flag, NULL, 0);
        // printf("esp_mesh_p2p_rx_main\n");
        if (err != ESP_OK)
        {
            ESP_LOGE(MESH_TAG, "err:0x%x, size:%d", err, data.size);
            continue;
        }
        // ESP_LOG_BUFFER_CHAR(MESH_TAG, (char *)data.data, 100);
        on_msg_parse((char *)data.data, data.size);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void rx_mesh_init(void)
{
    xTaskCreatePinnedToCore(esp_mesh_p2p_rx_main, "MPRX", 8192, NULL, 7, NULL, 0);
}