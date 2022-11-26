#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mesh_app.h"
static const char *MESH_TAG = "DATA_RECEIVE";
void on_msg_parse(char *data, uint32_t len)
{

    cJSON *root = cJSON_Parse(data);
    cJSON *event = cJSON_GetObjectItem(root, "event");
    cJSON *id = cJSON_GetObjectItem(root, "ID");
    cJSON *layer = cJSON_GetObjectItem(root, "layer");
    cJSON *index = cJSON_GetObjectItem(root, "indexNode");
    cJSON *indexchild = cJSON_GetObjectItem(root, "indexChild");
    if (cJSON_IsString(event) && strcmp(event->valuestring, "POWER_MEASUE") == 0)
    {
        // if (cJSON_IsNumber(layer) && layer->valueint == 2 && cJSON_IsString(id) && index->valueint > 0)
        // {
        //     char topic_s[100];
        //     if (index->valueint != 0)
        //     {
        //         // ESP_LOGI(MESH_TAG, "root recieve (%s) from parent %d\n", data_rx, index->valueint);
        //         sprintf(topic_s, "tree/parent_%d/powermesh", index->valueint);
        //         mqtt_app_publish(topic_s, data);
        //     }
        // }
        // if (cJSON_IsNumber(layer) && layer->valueint == 3)
        // {
        //     // printf("%s\n", data);

        //     char topic_s[100];
        //     if (index->valueint != 0)
        //     {
        //         // printf("%d %d\n", index->valueint, device_node.layer[index->valueint]);

        //         sprintf(topic_s, "tree/parent_%d/child_%d/powermesh", index->valueint, indexchild->valueint);
        //         mqtt_app_publish(topic_s, data);
        //     }
        // }
                char topic_s[100];
                sprintf(topic_s, "tree/ID:%d/powermesh", id->valueint);
                mqtt_app_publish(topic_s, data);
    }
    if (cJSON_IsNumber(layer))
    {
        for (int i = 1; i < device_node.route_table_size; i++)
        {
            char mac[24] = {0};
            sprintf(mac, MACSTR, MAC2STR(device_node.route_table[i].addr));
            if (cJSON_IsString(id) && !strcmp(id->valuestring, mac))
                {
                    device_node.layer[i] = layer->valueint;
                    printf("Device node %d layer: %d",i,layer->valueint);
                }
            vTaskDelay(1 * 100 / portTICK_RATE_MS);
        }
    }
    cJSON_Delete(root);
}