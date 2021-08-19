/************************************************************/
/*       7188XA/XB/XC7188EX + X608 head file                */
/*             Version : 1.00                               */
/*          [12/Sep/2002] by Jason                          */
/*	        [11/October/2005] by Liam                       */
/************************************************************/
/*  X608:   512K SRAM Battery BAckup (10 years)             */
/************************************************************/
/********************* M5M51008 ******************************
 * Funtion:   512K Byte SRAM (1k Block,one Block is 128 Byte)*
 *                                                           *
 * Date:       2002/6/20                                     *
 * ControlPin: /OE ---> /RD                                  *
 *             /S1 ---> /CS                                  *
 *              S2 ---> DIO14                                *
 *              /W ---> /WR                                  *
 *                                                           *
 *                     offset 0~127 bytes                    *
 *               0    1  2                  125 126 127 byte *
 *             ----+---+---+---------------+---+---+---+     *
 * Block 0     |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 1     |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 *                 :                               :         *
 *                 :                               :         *
 *                 :                               :         *
 *                 :                               :         *
 *                 :                               :         *
 *             ----+---+---+---------------+---+---+---+     *
 * Block 4092  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 4093  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 4094  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 4095  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 *                                                           *
 *************************************************************/
#define NoError 0
#define OffsetError -2
 
int X608_Init(void);  //If you want to read /write the SRAM of XBoard,please run initial function first.  
//Return 0
//READ/WRITE access function by ( block,offset ) mode 
//READ/WRITE access size = 1 Byte
//block = 0~1023 ,offset = 0~127 ,data = 1Byte(0~255) 
int X608_Read(int block,int offset);
// return   0       OK
//         -1       BlockError
//         -2       OffsetError
//         other    data
int X608_Write(int block,int offset,unsigned char data);
// return   0    OK
//         -1    BlockError
//         -2    OffsetError

//READ/WRITE access function by ( address ) mode
//READ/WRITE access size = len*1 Byte
//address = 0~(512*1024-1) ,len = 0~65535
int X608_ReadFn(unsigned long address,int len,unsigned char *buf);
// return   0    OK
void X608_WriteFn(unsigned long address,int len,unsigned char *data);
