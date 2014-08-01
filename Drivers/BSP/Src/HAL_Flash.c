/*
仅用于F207 F407

--1--
FLASH第3扇区用于存储客户参数
开始地址是：0x0800C000 长度：16 Kbytes

--2--

--3--

*/

#include "HAL_Flash.h"


//--1--
#define FLASH_PARAMETER_DAAR	0x0800C000		//开始地址 
#define FLASH_PARAMETER_LENTH	0x00004000		//空间 


#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

static FLASH_EraseInitTypeDef EraseInitStruct;


static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;
  
  	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  	{
    	sector = FLASH_SECTOR_0;  
  	}
  	else
	if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  	{
    	sector = FLASH_SECTOR_1;  
  	}
  	else
	if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  	{
    	sector = FLASH_SECTOR_2;  
  	}
  	else
	if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  	{
    	sector = FLASH_SECTOR_3;  
  	}
  	else 
	if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  	{
    	sector = FLASH_SECTOR_4;  
  	}
   	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_5))*/
  	{
    	sector = FLASH_SECTOR_5;
  	}

  	return sector;
}




////////////////////////////////////////////////////////////////////////////////
static u8 FlashMemory[16*1024];
//----FLASH读参数---------------------------------------------------------------
void HAL_FlashRead(u32 addr, u8* buff, u16 bufflen)
{
	memcpy(buff, &FlashMemory[addr], bufflen);

/*
	addr += FLASH_PARAMETER_DAAR;
	//SSM.InfomationSwitsh |= FLASH_WRITTING_READING;		//设置FALSH读写标志
	memcpy(buff, (char*)addr, bufflen);
	//SSM.InfomationSwitsh &= ~FLASH_WRITTING_READING;	//清除FALSH读写标志
*/
}
//----FLASH随及写参数-----------------------------------------------------------
bool HAL_FlashWrite(u32 addr, u8* buff, u16 bufflen)
{
	//超出FLASH空间
	if( (addr + bufflen) > FLASH_PARAMETER_LENTH )
		return FALSE;

	memcpy(&FlashMemory[addr],  buff, bufflen);

	return TRUE;
/*
	u32 FirstSector, Address;			
	int i;
	uint32_t SectorError = 0;

	//超出FLASH空间
	if( (addr + bufflen) > FLASH_PARAMETER_LENTH )
		return FALSE;

	//SSM.InfomationSwitsh |= FLASH_WRITTING_READING;		//设置FALSH读写标志

	//读取扇区数据
	memcpy(HalBusMemory, (char*)FLASH_PARAMETER_DAAR, FLASH_PARAMETER_LENTH);	
	//改写数据
	memcpy(&HalBusMemory[addr], (char*)buff, bufflen);					

	HAL_FLASH_Unlock();

	FirstSector = GetSector(FLASH_PARAMETER_DAAR);

	//Fill EraseInit structure
  	EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
  	EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
  	EraseInitStruct.Sector = FirstSector;
  	EraseInitStruct.NbSectors = 1;
  
  	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
		return FALSE;


	//FLASH扇区编程
  	Address = FLASH_PARAMETER_DAAR;
	for(i=0; i<FLASH_PARAMETER_LENTH; i+=4)
	{
    	if(HAL_FLASH_Program(TYPEPROGRAM_WORD, Address, *((u32*)&HalBusMemory[i])) == HAL_OK)
    		Address = Address + 4;
		else
			return FALSE;
    }

	HAL_FLASH_Lock();
	return TRUE;
*/
}


