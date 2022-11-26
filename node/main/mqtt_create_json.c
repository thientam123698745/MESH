#include "cJSON.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mesh_app.h"
#include <string.h>
static const char *MESH_TAG = "DATA_SEND";
double power_str;

void msg_create(char *outputBuf, uint32_t maxLength, int i)
{

    cJSON *root = cJSON_CreateObject();
    char rawData[30];
    sprintf(rawData, MACSTR, MAC2STR(device_node.mac));
    cJSON_AddStringToObject(root, "event", "DATA_SEND");
    cJSON_AddStringToObject(root, "id", rawData);
    cJSON_AddNumberToObject(root, "layer", mesh_layer);
    cJSON_AddNumberToObject(root, "index", nodeId);
    cJSON_AddNumberToObject(root, "indexNode", nodeId);
    cJSON_AddNumberToObject(root, "indexChild", i);
    cJSON_AddStringToObject(root, "data", data_rx);
    cJSON_AddNumberToObject(root, "turn_onArray", turn_on_array);
    if (i > 0)
        ESP_LOGI(MESH_TAG, "parent %d sent (%s) to child %d", nodeId, data_rx, i);
    cJSON_PrintPreallocated(root, outputBuf, maxLength, 0);
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
    cJSON_AddNumberToObject(root, "layer", my_layer);
    cJSON_AddNumberToObject(root, "indexNode", nodeId);
    cJSON_AddNumberToObject(root, "indexChild", childId);
    cJSON_AddNumberToObject(root, "power", power);

    cJSON_PrintPreallocated(root, outputBuf, maxLength, 0);
    cJSON_Delete(root);
}