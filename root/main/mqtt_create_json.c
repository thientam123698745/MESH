#include "cJSON.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mesh_app.h"
#include <string.h>
static const char *MESH_TAG = "DATA_SEND";
void msg_create(char *outputBuf, uint32_t maxLength, uint8_t *parent, int i)
{
    cJSON *root = cJSON_CreateObject();
    char rawData[30];
    sprintf(rawData, MACSTR, MAC2STR(device_node.mac));
    cJSON_AddStringToObject(root, "event", "DATA_SEND");
    cJSON_AddStringToObject(root, "id", rawData);
    cJSON_AddNumberToObject(root, "layer", mesh_layer);
    cJSON_AddNumberToObject(root, "index", i);
    cJSON_AddNumberToObject(root, "turn_onArray", start_loop);
    if (strlen(data_rx) != 0)
    {
        // ESP_LOGI(MESH_TAG, "data=%s", data_rx)
        cJSON_AddStringToObject(root, "data", data_rx);
    }
    else
        cJSON_AddStringToObject(root, "data", "");

    cJSON_PrintPreallocated(root, outputBuf, maxLength, 0);
    ESP_LOGI(MESH_TAG, "root sent (%s) to parent %d\n", data_rx, i);
    // else if (i >= 3)
    //     ESP_LOGI(MESH_TAG, "root gui toi child %d:data=(%s)\n", i, data_rx);
    cJSON_Delete(root);
}
void msg_create_power(char *outputBuf, uint32_t maxLength, double power)
{
    cJSON *root = cJSON_CreateObject();
    char rawData[30];
    sprintf(rawData, MACSTR, MAC2STR(device_node.mac));
    cJSON_AddStringToObject(root, "event", "POWER_MEASUE");
    cJSON_AddNumberToObject(root, "ID", NumID);
    cJSON_AddStringToObject(root, "MAC", rawData);
    cJSON_AddNumberToObject(root, "layer", mesh_layer);
    cJSON_AddNumberToObject(root, "indexNode", 0);
    cJSON_AddNumberToObject(root, "power", power);

    cJSON_PrintPreallocated(root, outputBuf, maxLength, 0);
    cJSON_Delete(root);
}