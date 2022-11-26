/* Mesh Manual Networking Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
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
esp_netif_t *netif_sta;
const uint8_t MESH_ID[6] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77};
static const char *MESH_TAG = "WIFI_APP";

const char *ssid_wifi = Router_ssid;
const char *pass_wifi = Router_pass;
void ip_event_handler(void *arg, esp_event_base_t event_base,
                      int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(MESH_TAG, "<IP_EVENT_STA_GOT_IP>IP:" IPSTR, IP2STR(&event->ip_info.ip));
}
void wifi_init(void)
{
    ESP_ERROR_CHECK(mesh_light_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    /* tcpip initialization */
    ESP_ERROR_CHECK(esp_netif_init());
    /* event initialization */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /* crete network interfaces for mesh (only station instance saved for further manipulation, soft AP instance ignored */
    ESP_ERROR_CHECK(esp_netif_create_default_wifi_mesh_netifs(&netif_sta, NULL));
    /* wifi initialization */
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_start());
    // ESP_ERROR_CHECK(esp_mesh_set_topology(MESH_TOPO_TREE));
    /* mesh initialization */
    ESP_ERROR_CHECK(esp_mesh_init());
    ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &mesh_event_handler, NULL));
    /* mesh enable IE crypto */
    mesh_cfg_t cfg = MESH_INIT_CONFIG_DEFAULT();
#if CONFIG_MESH_IE_CRYPTO_FUNCS
    /* modify IE crypto key */
    ESP_ERROR_CHECK(esp_mesh_set_ie_crypto_funcs(&g_wifi_default_mesh_crypto_funcs));
    ESP_ERROR_CHECK(esp_mesh_set_ie_crypto_key(CONFIG_MESH_IE_CRYPTO_KEY, strlen(CONFIG_MESH_IE_CRYPTO_KEY)));
#else
    /* disable IE crypto */
    ESP_LOGI(MESH_TAG, "<Config>disable IE crypto");
    ESP_ERROR_CHECK(esp_mesh_set_ie_crypto_funcs(NULL));
#endif
    /* mesh ID */
    memcpy((uint8_t *)&cfg.mesh_id, MESH_ID, 6);
    /* router */
    cfg.channel = CONFIG_MESH_CHANNEL;
    cfg.router.ssid_len = strlen(ssid_wifi);
    memcpy((uint8_t *)&cfg.router.ssid, ssid_wifi, cfg.router.ssid_len);
    memcpy((uint8_t *)&cfg.router.password, pass_wifi,
           strlen(pass_wifi));
    /* mesh softAP */
    ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(CONFIG_MESH_AP_AUTHMODE));
    cfg.mesh_ap.max_connection = num_ap_connected;
    cfg.mesh_ap.nonmesh_max_connection = CONFIG_MESH_NON_MESH_AP_CONNECTIONS;
    memcpy((uint8_t *)&cfg.mesh_ap.password, CONFIG_MESH_AP_PASSWD,
           strlen(CONFIG_MESH_AP_PASSWD));
    ESP_ERROR_CHECK(esp_mesh_set_config(&cfg));
    /* mesh start */
    ESP_ERROR_CHECK(esp_mesh_start());
    ESP_LOGI(MESH_TAG, "mesh starts successfully, heap:%d\n", esp_get_free_heap_size());
}
