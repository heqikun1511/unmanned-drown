#include "SI24R1.h"
#include "spi.h"

uint8_t  TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};
uint8_t si24r1_rx_buff[5]={0};

uint8_t  SI24R1_check(void)       //用于测试spi是否正常
{
	//需要先读取一次，spi正常才可以写入
	HAL_Delay(200);
	SI24R1_Read_Buf(TX_ADDR,si24r1_rx_buff,TX_ADR_WIDTH);
	SI24R1_Write_Buf(SI24R1_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
	SI24R1_Read_Buf(TX_ADDR,si24r1_rx_buff,TX_ADR_WIDTH);

	for(uint8_t i=0;i<TX_ADR_WIDTH;i++)
	{
		if(si24r1_rx_buff[i]!=TX_ADDRESS[i])
			return 1;

		else
			return 0;
	}


}

static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t rx_data=0;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 1, 1000);
	return rx_data;
}


/********************************************************
 * Function: SI24R1 GPIO Initialization
 * Input: None
 * Return: None
 *********************************************************/

void SI24R1_Init(void)
{
	HAL_Delay(200); 
	while(SI24R1_check()==1)
	{
		HAL_Delay(100);



		}

SI24R1_RX_Mode();//默认进入接收模式






}
/********************************************************
 * Function: Write register (single byte)
 * Input: reg - register address (WRITE_REG | reg)
 *        value - register value
 * Return: status register value
 *********************************************************/
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

	CS_LOW;               
	status = SPI_RW(reg);				
	SPI_RW(value);
	CS_HIGH;
	
	return(status);
}


/********************************************************
 * Function: Write register (multiple bytes)
 * Input: reg - register address (WRITE_REG | reg)
 *        pBuf - pointer to data buffer
 *        bytes - number of bytes to write
 * Return: status register value
 *********************************************************/
uint8_t SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status,byte_ctr;

  CS_LOW;                                  			
  status = SPI_RW(reg);                          
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
    SPI_RW(*pBuf++);
  CS_HIGH;                                      	

  return(status);       
}							  					   


/********************************************************
 * Function: Read register (single byte)
 * Input: reg - register address (READ_REG | reg)
 * Return: register value
 *********************************************************/
uint8_t SI24R1_Read_Reg(uint8_t reg)
{
	
 	uint8_t value;

	CS_LOW;    
	SPI_RW(reg);			
	value = SPI_RW(0);
	CS_HIGH;              

	return(value);
}


/********************************************************
 * Function: Read register (multiple bytes)
 * Input: reg - register address (READ_REG | reg)
 *        pBuf - pointer to receive buffer
 *        bytes - number of bytes to read
 * Return: status register value
 *********************************************************/
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status,byte_ctr;

  CS_LOW;
  status = SPI_RW(reg);
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   // Read data, MSB first
  CS_HIGH;

  return(status);
}


/********************************************************
 * Function: SI24R1 RX Mode Initialization
 * Input: None
 * Return: None
 *********************************************************/
void SI24R1_RX_Mode(void)
{
	CE_LOW;
	SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// RX pipe0 uses same address as TX
	SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);               						// Enable auto-ACK on pipe0
	SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);           						// Enable RX pipe0
	SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, 40);                 						// Select RF channel 40
	SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// Set payload width for pipe0
	SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x0f);            						// Data rate 2Mbps, TX power 7dBm
	SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0f);              						// CRC enable, 16-bit CRC, power up, RX mode
	SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);  										// Clear all interrupt flags
	CE_HIGH;                                            									// CE high, start listening
}						


/********************************************************
 * Function: SI24R1 TX Mode Initialization
 * Input: None
 * Return: None
 *********************************************************/
void SI24R1_TX_Mode(void)
{
	CE_LOW;
	SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // Write TX address
	SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // RX pipe0 same as TX address for auto-ACK

	SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);       										// Enable auto-ACK on pipe0
	SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);   										// Enable RX pipe0
	SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0a);  										// Auto-retransmit: delay 250us+86us, 10 retries
	SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, 40);         										// Select RF channel 40
	SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x0f);    										// Data rate 2Mbps, TX power 7dBm
	SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0e);      										// CRC enable, 16-bit CRC, power up, TX mode
	CE_HIGH;
}


/********************************************************
 * Function: Receive a data packet 将接受到的保存在FIFO中
 * Input: rxbuf - pointer to receive buffer
 * Return: 0 - data received
 *         1 - no data received
 *********************************************************/
uint8_t SI24R1_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;
	state = SI24R1_Read_Reg(STATUS);  			                 // Read status register
	SI24R1_Write_Reg(SI24R1_WRITE_REG+STATUS,state);               // Clear RX_DS interrupt flag

	if(state & RX_DR)								                           // Data received
	{
		SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     // Read payload
		SI24R1_Write_Reg(SI24R1_WRITE_REG + FLUSH_RX,0xff);	//清空队列				              // Flush RX FIFO
		return 0; 
	}   
	return 1;                                                   // No data received
}


/********************************************************
 * Function: Transmit a data packet
 * Input: txbuf - pointer to data to send
 * Return: 0x10 - max retries exceeded / transmit failed
 *         0x20 - transmit success
 *         0xFF - unknown failure
 *********************************************************/
uint8_t SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state;
	CE_LOW;																			//待机模式才可以配置						  // CE low, disable SI24R1
    SI24R1_Write_Buf(SI24R1_WRITE_REG + WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	    // Write data to TX FIFO, 32 bytes
 	CE_HIGH;																										  // CE high, start transmission
	
	//这里是轮讯，不要中断
	while((state&TX_DS)==0&& (state&MAX_RT)==0){
		state=SI24R1_Read_Reg(STATUS);																							// Transmit success or max retries exceeded
	}																				  // Wait for TX interrupt
	state = SI24R1_Read_Reg(STATUS);  											  // Read status register
	SI24R1_Write_Reg(SI24R1_WRITE_REG+STATUS, state); 								// Clear TX_DS & MAX_RT interrupt flags
	if(state&MAX_RT)																			    // Max retries exceeded
	{
		SI24R1_Write_Reg(SI24R1_WRITE_REG + FLUSH_TX,0xff);			//最大充值次数，手动除							    // Flush TX FIFO
		return MAX_RT; 
	}
	if(state&TX_DS)																			      // Transmit success
	{
		return TX_DS;
	}
	return 0XFF;																						  // Transmit failed
}

