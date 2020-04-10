#include "imu_task.h"

#include "cmsis_os.h"

#include "ahrs.h"
#include "bsp_spi.h"
#include "spi.h"
#include "bmi088driver.h"
#include "ist8310driver.h"
#include "bsp_bmi088.h"

#include "bsp_spi.h"
#include "bsp_offset.h"

#include "bsp_init.h"
extern SPI_HandleTypeDef hspi1;
extern void BMI088_Drive_Init(void);
extern void BMI088_Read_update(void);
extern void gyro_offset_init(void);
extern void bim088_gyro_offset_update(void);

osThreadId imuTaskHandle;
void imu_task_creat(void)
{
  osThreadDef(imuTask, imu_task, osPriorityRealtime, 0, 512);
  imuTaskHandle = osThreadCreate(osThread(imuTask), NULL);
}
void imu_task(void const *pvParameters)
{
	TickType_t peroid = osKernelSysTick();
	/*����������ʱʱ��(��λms)*/
	osDelay(IMU_TASK_INIT_TIME1);
	/*��ʼ��BMI088*/
   	while(BMI088_Init()){osDelay(100);}
	/*��ʼ��ist8310*/
//    while(ist8310_init()){osDelay(100);}
	BMI088_Drive_Init();
	cali_param_init();//��ȡУ׼ֵ����FLASH��ȡ�����û�У�����У׼������У׼ֵ����FLASH
	//��ȡ��ǰ�������������
	// imu_Task_handle = xTaskGetHandle(pcTaskGetName(NULL));
	osDelay(IMU_TASK_INIT_TIME2);
	while(1)
	{
		BMI088_Read_update();//��ȡBMI088����
		calibrate_imu();//��̬У׼�����ݴ�ŵ�Ƭ��FLASH
		osDelayUntil(&peroid,IMU_TASK_PERIOD);//������ʱ
	}
}


