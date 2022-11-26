#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mesh_app.h"

#include "esp_log.h"
static const char *TAG = "loop_buffer";

#define COL 5000
#define ROW 5000
#define K 500
xTaskHandle array_handle;
xTaskHandle buffer_handle;
typedef struct
{
    int third_array[ROW][COL];
} holder_t;

void array_loop(void *p)
{
    for (int h = 0; h < 10; h++)
    {
        ESP_LOGI(TAG, "Start loop");
        holder_t holder;
        int first_array[ROW][COL], second_array[ROW][COL];
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                first_array[i][j] = i;
                second_array[i][j] = i;
                holder.third_array[i][j] = i;
            }
        for (unsigned long i = 0; i < COL; i++)
        {

            for (unsigned long j = 0; j < ROW; j++)
            {
                for (unsigned long k = 0; k < K; k++)
                {
                    holder.third_array[j][i] = holder.third_array[j][i] + first_array[k][i] + second_array[k][i];
                }
            }
        }
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}
void buffer(void *p)
{
    char buffer[3];
    strcpy(buffer, data_rx);
    vTaskDelete(NULL);
}
void array_loop_init(void)
{
    xTaskCreatePinnedToCore(array_loop, "array_loop", 8192, NULL, 1, &array_handle, 0);
    xTaskCreatePinnedToCore(buffer, "buffer", 8192, NULL, 1, &buffer_handle, 0);
    vTaskSuspend(array_handle);
    vTaskSuspend(buffer_handle);
}