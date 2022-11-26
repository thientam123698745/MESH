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
#include "mesh_app.h"
static const char *MESH_TAG = "ROOT_SCAN_DEVICE";
mesh_route_t device_node;
void scan_device_route(void *p)
{
    while (true)
    {
        // ESP_LOGI(MESH_TAG, "Dang scan device");
        esp_mesh_get_routing_table((mesh_addr_t *)&device_node.route_table,
                                   50 * num_ap_connected, &device_node.route_table_size);
        ESP_LOGI(MESH_TAG, "scan_device_route:: Number ap connected: %d Rount table size: %d", num_ap_connected, device_node.route_table_size);

        vTaskDelay(5 * 1000 / portTICK_RATE_MS);
    }
}
void init_scan(void)
{
    esp_read_mac(device_node.mac, ESP_MAC_WIFI_STA);
    xTaskCreatePinnedToCore(scan_device_route, "scan_device_route", 3072, NULL, 5, NULL, 0);
}