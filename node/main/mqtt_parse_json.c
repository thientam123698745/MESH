#include <string.h>
#include "cJSON.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mesh_app.h"
uint8_t start_loop;

static const char *MESH_TAG = "DATA_RECEIVE";
int nodeId;
int childId;
static int state;
static bool start_array;
void on_msg_parse(char *data, uint32_t len)
{
    char rawData[30];
    sprintf(rawData, MACSTR, MAC2STR(device_node.mac));
    // printf("%s\n", data);
    cJSON *root = cJSON_Parse(data);
    cJSON *id = cJSON_GetObjectItem(root, "id");
    cJSON *event = cJSON_GetObjectItem(root, "event");
    cJSON *layer = cJSON_GetObjectItem(root, "layer");
    cJSON *index = cJSON_GetObjectItem(root, "index");
    if (cJSON_IsString(event) && strcmp(event->valuestring, "DATA_SEND") == 0)
    {
        cJSON *turn_onArray = cJSON_GetObjectItem(root, "turn_onArray");
        if (cJSON_IsNumber(turn_onArray))
        {
                // printf("%d\n", turn_onArray->valueint);
            
            if (turn_onArray->valueint == 1)
            {
                start_loop = 1;
                // vTaskResume(array_handle);
                // turn_on_array = 1;
                // if (start_array == false)
                //     {
                //         start_array = true;
                //         xTaskCreatePinnedToCore(array_loop, "array_loop", 8192, NULL, 1, &array_handle, 0);
                //     }
            }
            if (turn_onArray->valueint == 2)
            {
                start_loop = 2;
                // xTaskCreatePinnedToCore(buffer, "buffer", 8192, NULL, 1, &buffer_handle, 0);
                // vTaskResume(buffer_handle);
                // turn_on_array = 2;
            }
        }
        if (cJSON_IsNumber(index))
        {
            if (my_layer == 2 && cJSON_IsNumber(layer) && layer->valueint == 1)
            {
                // printf("%d\n", nodeId);
                nodeId = index->valueint;
                cJSON *data = cJSON_GetObjectItem(root, "data");
                if (cJSON_IsString(data))
                {
                    strcpy(data_rx, data->valuestring);
                    ESP_LOGW(MESH_TAG, "parent %d recieved (%s) from root", nodeId, data_rx);
                }
                // ESP_LOGW(MESH_TAG, "%s", data->valuestring);
            }
            else if (my_layer == 3 && cJSON_IsNumber(layer) && layer->valueint == 2)
            {
                nodeId = index->valueint;
                cJSON *data = cJSON_GetObjectItem(root, "data");
                // cJSON *indexNode = cJSON_GetObjectItem(root, "indexNode");
                cJSON *indexchild = cJSON_GetObjectItem(root, "indexChild");
                if (cJSON_IsNumber(indexchild) && cJSON_IsString(data))
                {
                    strcpy(data_rx, data->valuestring);
                    childId = indexchild->valueint;
                }
                ESP_LOGI(MESH_TAG, "child %d recieved (%s) from parent %d", indexchild->valueint, data->valuestring, index->valueint);
            }
        }
        if (cJSON_IsNumber(layer) && layer->valueint >= 3)
        {
            cJSON *data_s = cJSON_GetObjectItem(root, "data");
            cJSON *indexNode = cJSON_GetObjectItem(root, "indexNode");
            cJSON *indexchild = cJSON_GetObjectItem(root, "indexChild");
            if (cJSON_IsNumber(indexNode) && cJSON_IsNumber(indexchild) && cJSON_IsString(data_s))
            {
                // strcpy(data_rx, data_s->valuestring);
                ESP_LOGI(MESH_TAG, "parent %d recieved (%s) from child %d", indexNode->valueint, data_s->valuestring, indexchild->valueint);
            }
        }
    }
    if (cJSON_IsNumber(layer) && layer->valueint == 3 && cJSON_IsString(event) && strcmp(event->valuestring, "POWER_MEASUE") == 0)
    {
        mesh_data_t data_m;
        data_m.size = 1024;
        data_m.data = (uint8_t *)data;
        // printf("%s\n", data_mesh.data);
        esp_err_t err = esp_mesh_send(NULL, &data_m, 0, NULL, 0);
        if (err != ESP_OK)
        {
            ESP_LOGE(MESH_TAG, "send err:0x%x, size:%d", err, data_m.size);
        }

        vTaskDelay(1 * 100 / portTICK_RATE_MS);
        // strcpy(data_send_power, data);
    }
    cJSON_Delete(root);
}
void on_msg_parse_control(char *data, int index)
{
    // cJSON *root = cJSON_Parse(data);
    // cJSON_AddNumberToObject(root, "indexChild", index);
    // cJSON_AddNumberToObject(root, "indexNode", nodeId);
    // cJSON_PrintPreallocated(root, data, 200, 0);
    // cJSON_Delete(root);
    // printf("%s\n", data);
}