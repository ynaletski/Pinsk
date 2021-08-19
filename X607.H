/************************************************************/
/*       7188XA/XB/XC7188EX + X607 head file                */
/*             Version : 1.00                               */
/*          [20/June/2002] by David                         */
/*          [11/October/2005] by Liam                       */
/************************************************************/
/*  X607:   128K SRAM Battery BAckup (10 year)              */
/************************************************************/
/********************* M5M51008 ******************************
 * Funtion:   128K Byte SRAM (1k Block,one Block is 128 Byte)*
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
 * Block 1021  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 1022  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 1023  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 * Block 1024  |   |   |   |               |   |   |   |     *
 *             |---+---+---+ ------------- +---+---+---+     *
 *                                                           *
 *************************************************************/
#define NoError 0
#define OffsetError -2
 
int X607_Init(void);   //If you want to read /write the SRAM of XBoard,please run initial function first.
//Return 0
//READ/WRITE access function by [block,offset] mode 
//READ/WRITE access size = 1 Byte
//block = 0~1023 ,offset = 0~127 ,data = 1Byte(0~255) 
int X607_Read(int block,int offset);
// return   0       OK
//         -1       BlockError
//         -2       OffsetError
//         other    data
int X607_Write(int block,int offset,unsigned char data);
// return   0    OK
//         -1    BlockError
//         -2    OffsetError

//READ/WRITE access function by [absolute address] mode
//READ/WRITE access size = len*1 Byte
//address = 0~(128*1024-1) ,len = 0~32768
int X607_ReadFn(unsigned long address,int len,unsigned char *data);
// return   0    OK
void X607_WriteFn(unsigned long address,int len,unsigned char *data);
