/* Mesh IP Internal Networking Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "mesh_app.h"
#include "mqtt_client.h"

static const char *TAG = "mesh_mqtt";
static esp_mqtt_client_handle_t s_client = NULL;
char data_rx[300];
uint8_t start_loop;
xTaskHandle array_handle;
xTaskHandle buffer_handle;
static bool state;
static bool state_buffer;
static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe(s_client, "tree/root", 0);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        // ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
    {
        // ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        // printf("TOPIC=%.*s\n", event->topic_len, event->topic);
        // printf("DATA=%.*s\n", event->data_len, event->data);
        ESP_LOGI(TAG, "ROOT RECIEVE (%.*s) FROM MQTT", event->data_len, event->data);
        memset(data_rx, 0, sizeof(data_rx));
        strncpy(data_rx, event->data, event->data_len);
        
        
    }
    break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_app_publish(char *topic, char *publish_string)
{
    if (s_client)
    {
        esp_mqtt_client_publish(s_client, topic, publish_string, 0, 1, 0);
        // ESP_LOGI(TAG, "sent publish returned msg_id=%d", msg_id);
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://broker.hivemq.com:1883",
    };

    s_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_event_handler, s_client);
    esp_mqtt_client_start(s_client);
}
