#include "sys.h"
#include "bsp.h"
static void startTasks(void);
xQueueHandle xQueue;

static void vSenderTask(void *pvParameters)
{
	long lValueToSend;
	portBASE_TYPE xStatus;
	lValueToSend=(long) pvParameters;
	while(1)
	{
		xStatus=xQueueSendToBack(xQueue,&lValueToSend,0);
		if(xStatus!=pdPASS)
			printf("Could not send to the queue.\r\n");
		vTaskDelay(250/portTICK_RATE_MS);
	}
}
static void vReceiveTask(void *pvParameters)
{
	long lReceivedValue;
	portBASE_TYPE xStaus;
	const portTickType xTicksToWait=100/portTICK_RATE_MS;
	while(1)
	{
		if(uxQueueMessagesWaiting(xQueue)!=0)
			printf("Queue should have been empty!\r\n");
		xStaus=xQueueReceive(xQueue,&lReceivedValue,xTicksToWait);
		if(xStaus==pdPASS)
			printf("Received = %ld",lReceivedValue);
		else
			printf("Could not receive from the queue\r\n");
	}
}
//FreeRTOSConfig.h 中的configTICK_RATE_HZ 设为10(HZ)，则时间片长度为100ms
//或使用taskENTER_CRITICAL()与taskEXIT_CRITICAL()
int main(void)
{
	bsp_Init();
	xQueue=xQueueCreate(5,sizeof(long));
	if(xQueue!=NULL)
		{
			startTasks();
		}
	for( ;; );
}
static void startTasks(void)
{
	xTaskCreate(vSenderTask,NULL,1000,(void*)100,1,NULL);
	xTaskCreate(vSenderTask,NULL,1000,(void*)200,1,NULL);
	xTaskCreate(vReceiveTask,NULL,1000,NULL,2,NULL);
	vTaskStartScheduler();
}

