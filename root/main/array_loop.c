#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mesh_app.h"
#include "esp_log.h"
static const char *TAG = "loop_buffer";

#define COL 100
#define ROW 100
#define K 500
bool startloop = 0;
xTaskHandle array_handle;
xTaskHandle buffer_handle;
typedef struct
{
    int third_array[ROW][COL];
} holder_t;
static bool state_thread;
void array_loop(void *p)
{
    int count = 0;
    while(true)
    {   
         
        // printf("array_loop\n");
        // printf("%s\n", data_rx);
        if (strncmp("startloop", data_rx, sizeof(data_rx)) == 0)
        {
            {
                start_loop = 1;
                // if (state)
                // {
                //     // ESP_LOGI(TAG, "START LOOP");
                //     vTaskResume(array_handle);
                // }
                // else
                // if (state == false)
                // {
                //     state = true;
                //     xTaskCreatePinnedToCore(array_loop, "array_loop", 8192, NULL, 1, &array_handle, 0);
                // }

                while (true)
                    {
                        if (strstr(data_rx, "start_buffer") != NULL)
                        {
                            // printf("Start buffer\n");
                            start_loop = 2;
                            break;
                        }
                        // ESP_LOGI(TAG, "Start loop2");
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
                        vTaskDelay(200 / portTICK_RATE_MS);
                    }
            }
        }
            
        if (strstr(data_rx, "start_buffer") != NULL)
            {
                // printf("strar buffer\n");
                start_loop = 2;
                char buffer[5];
                strcpy(buffer, data_rx);
                vTaskDelete(NULL);
                vTaskDelay(500 / portTICK_RATE_MS);
            }
         
        {
            // printf("array loop delay 5s\n");
            vTaskDelay(500 / portTICK_RATE_MS);
        }
    }
    printf("Array loop delete\n");   
    vTaskDelete(NULL);
}
void buffer(void *p)
{
    char buffer[5];
    strcpy(buffer, data_rx);
    vTaskDelete(NULL);
}

void array_loop_init(void)
{
    xTaskCreatePinnedToCore(array_loop, "array_loop", 16192, NULL, 1, &array_handle, 1);
    // xTaskCreate(buffer, "buffer", 8192, NULL, 1, &buffer_handle);
    // vTaskSuspend(array_handle);
    // vTaskSuspend(buffer_handle);
}