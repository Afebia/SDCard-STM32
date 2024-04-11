/*
 * SDCARD_STM32.h
 *
 *  Created on: Apr 3, 2024
 *      Author: berat
 */

#ifndef INC_SDCARD_STM32_H_
#define INC_SDCARD_STM32_H_

#include "fatfs.h"
//#include "fatfs_sd.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

  FATFS FatFs;
  FIL Fil;
  FRESULT FR_Status;
  FATFS *FS_Ptr;
  UINT WWC; // Read/Write Word Counter
  DWORD FreeClusters;
  uint32_t TotalSize, FreeSpace;
  char RW_Buffer[200];

  uint8_t i =0;

bool SD_Card_Init(void);
void Create_File(void);
void Write_Data(float heading, float pitch, float roll);

bool SD_Card_Init(void){
	FR_Status = f_mount(&FatFs, "", 1);

	    if (FR_Status != FR_OK)
	    {
	      printf("Hata, SD Card Algilanamadi, Hata Kodu: (%i)\r\n", FR_Status);
	      return false;
	    }

	    printf("SD Card Algilandi! \r\n\n");
	    //------------------[ Get & Print The SD Card Size & Free Space ]--------------------

	    f_getfree("", &FreeClusters, &FS_Ptr);
	    TotalSize = (uint32_t)(((FS_Ptr->n_fatent - 2) * FS_Ptr->csize * 0.5)/1024);
	    FreeSpace = (uint32_t)((FreeClusters * FS_Ptr->csize * 0.5)/1024);

	    printf("SD Card Toplam Alan: %lu MB\r\n", TotalSize);
	    printf("SD Card Bos Alan: %lu MB\r\n\n", FreeSpace);

	    return true;
}

void Create_File(void){
	FR_Status = f_open(&Fil, "VoltaRocket.txt",FA_OPEN_ALWAYS | FA_WRITE);

	if(i==0){
		if(FR_Status != FR_OK)
	    {
			printf("Dosya Olusturulurken Bir Hata Olustu!, Hata Kodu: (%i)\r\n", FR_Status);
	    }
	    printf("Dosya Olusturuldu.\r\n\n");

	    i=1;
	}
}

void Write_Data(float heading, float pitch, float roll){

	sprintf(RW_Buffer, "Heading Angle: %.2f\tPitch Angle: %.2f\tRoll Angle: %.2f\n",heading, pitch, roll);
	f_lseek(&Fil, f_size(&Fil));
	f_puts(RW_Buffer, &Fil);

	HAL_Delay(100);
	f_close(&Fil);

}

#endif /* INC_SDCARD_STM32_H_ */
