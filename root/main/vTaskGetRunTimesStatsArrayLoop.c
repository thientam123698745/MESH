#include <stdlib.h>
#include <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
/* FreeRTOS includes. */
#include "freertos/FreeRTOS.h""
#include "mesh_app.h"

PRIVILEGED_DATA static volatile UBaseType_t uxCurrentNumberOfTasks = ( UBaseType_t ) 0U;
 static char * prvWriteNameToBuffer( char * pcBuffer,
                                        const char * pcTaskName ) PRIVILEGED_FUNCTION;

UBaseType_t uxTaskGetNumberOfTasks( void )
{
    return uxCurrentNumberOfTasks;
}

char * prvWriteNameToBufferT( char * pcBuffer, const char * pcTaskName )
{
        size_t x;

        /* Start by copying the entire string. */
        strcpy( pcBuffer, pcTaskName );

        /* Pad the end of the string with spaces to ensure columns line up when
         * printed out. */
        for( x = strlen( pcBuffer ); x < ( size_t ) ( configMAX_TASK_NAME_LEN - 1 ); x++ )
        {
            pcBuffer[ x ] = ' ';
        }

        /* Terminate. */
        pcBuffer[ x ] = ( char ) 0x00;

        /* Return the new end of string. */
        return &( pcBuffer[ x ] );
}
void vTaskGetRunTimeStatsT( char *pcWriteBuffer )
 {
 TaskStatus_t *pxTaskStatusArray;
 volatile UBaseType_t uxArraySize, x;
 uint32_t ulTotalRunTime, ulStatsAsPercentage;

*pcWriteBuffer = 0x00;

     // Take a snapshot of the number of tasks in case it changes while this
     // function is executing.
     uxArraySize = uxTaskGetNumberOfTasks();

     // Allocate a TaskStatus_t structure for each task.  An array could be
     // allocated statically at compile time.
     pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

     if( pxTaskStatusArray != NULL )
     {
         // Generate raw status information about each task.
         uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

         // For percentage calculations.
         ulTotalRunTime /= 100UL;

         // Avoid divide by zero errors.
         if( ulTotalRunTime > 0 )
         {
             // For each populated position in the pxTaskStatusArray array,
             // format the raw data as human readable ASCII data
             for( x = 0; x < uxArraySize; x++ )
             {
                 // What percentage of the total run time has the task used?
                 // This will always be rounded down to the nearest integer.
                 // ulTotalRunTimeDiv100 has already been divided by 100.
                 ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

                 if( ulStatsAsPercentage > 0UL )
                 {
                    //  sprintf( pcWriteBuffer, "%s\t\t%lu\t\t%lu%%\r\n", pxTaskStatusArray[ x ].pcTaskName, pxTaskStatusArray[ x ].ulRunTimeCounter, ulStatsAsPercentage );
                 }
                 else
                 {
                     // If the percentage is zero here then the task has
                     // consumed less than 1% of the total run time.
                    //  sprintf( pcWriteBuffer, "%s\t\t%lu\t\t<1%%\r\n", pxTaskStatusArray[ x ].pcTaskName, pxTaskStatusArray[ x ].ulRunTimeCounter );
                 }

                 pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
             }
         }

         // The array is no longer needed, free the memory it consumes.
         vPortFree( pxTaskStatusArray );
     }
 }