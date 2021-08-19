#ifdef __TURBOC__
  #if (__TURBOC__ < 0x0300)
/*    #define inp    inportb */
    #define inpw   inport
/*    #define outp   outportb */
    #define outpw  outport
  #endif
#endif

#define    IN_BUF_SIZE      1024
//#define    IN_BUF_SIZE      4096

#define    NoError          0
#define    InitPinIsOpen    0
#define    InitPinIsNotopen 1
#define    QueueIsEmpty     0
#define    QueueIsNotEmpty  1
#define    PortError       -1
#define    DataError       -2
#define    ParityError     -3
#define    StopError       -4
#define    TimeOut         -5
#define    QueueEmpty      -6
#define    QueueOverflow   -7
#define    PosError        -8
#define    AddrError       -9
#define    BlockError      -10
#define    WriteError      -11
#define    SegmentError    -12
#define    BaudRateError   -13
#define    CheckSumError   -14
#define    ChannelError    -15
#define    BaudrateError   -16
#define    TriggerLevelError   -17
#define    TimeIsUp        1
#define    MemError        -18


extern char hex_to_ascii[16];

#ifdef __cplusplus
extern "C" {
#endif

void InitLib(void);
int  InstallCom(int port, unsigned long baud, int data, int parity, int stop);
int  InstallCom1(unsigned long baud, int data, int parity, int stop);
int  InstallCom2(unsigned long baud, int data, int parity, int stop);
int  InstallCom3(unsigned long baud, int data, int parity);
int  InstallCom4(unsigned long baud, int data, int parity);
int  RestoreCom(int port);
int  RestoreCom1(void);
int  RestoreCom2(void);
int  RestoreCom3(void);
int  RestoreCom4(void);
int  IsCom(int port);
int  IsCom1(void);
int  IsCom2(void);
int  IsCom3(void);
int  IsCom4(void);
int  ReadCom(int port);
int  ReadCom1(void);
int  ReadCom2(void);
int  ReadCom3(void);
int  ReadCom4(void);
int  ToCom(int port, int data);
int  ToCom1(int data);
int  ToCom2(int data);
int  ToCom3(int data);
int  ToCom4(int data);
int ToComBufn(int port,char *buf,int no);
int ToCom1Bufn(char *buf,int no);
int ToCom2Bufn(char *buf,int no);
int ToCom3Bufn(char *buf,int no);
int ToCom4Bufn(char *buf,int no);
int  ToComStr(int port, char *str);
int  ToCom1Str(char *str);
int  ToCom2Str(char *str);
int  ToCom3Str(char *str);
int  ToCom4Str(char *str);
int  ClearCom(int port);
int  ClearCom1(void);
int  ClearCom2(void);
int  ClearCom3(void);
int  ClearCom4(void);
int  WaitTransmitOver(int port);
int  WaitTransmitOver1(void);
int  WaitTransmitOver2(void);
int  WaitTransmitOver3(void);
int  WaitTransmitOver4(void);
int  Set485DirToTransmit(int port);
int  Set485DirToReceive(int port);

int  ReadInitPin(void);
void LedOff(void);
void LedOn(void);
void Init5DigitLed(void);
int  Show5DigitLed(int position, int value);
int  Show5DigitLedSeg(int pos, unsigned char data);
void DelayTimeMs(unsigned int time);

int  ReadNVRAM(int addr);
int  WriteNVRAM(int addr, int data);

int  WriteEEP(int block, int addr, int data);
int  ReadEEP(int block, int addr);
void EnableEEP(void);
void ProtectEEP(void);

void EnableWDT(void);
void RefreshWDT(void);
void DisableWDT(void);

int FlashReadId(void);
int FlashWrite(unsigned int seg, unsigned int offset, char data);
int FlashErase(unsigned int seg);
int FlashRead(unsigned int seg, unsigned int offset);

void Delay(unsigned ms);
void Delay_1(unsigned ms);

int Is7188(void);
int getch4(void);
int kbhit4(void);
int ungetch4(int key);
void putch4(int data);

#define SEND_CMD(port,cmd,timeout,checksum)  SendCmdTo7000(port,cmd,checksum)
#define RECEIVE_CMD ReceiveResponseFrom7000
int ascii_to_hex(char ascii);
int SendCmdTo7000(int iPort, unsigned char *cCmd, int iChksum);
int ReceiveResponseFrom7000(int iPort, unsigned char *cCmd, long lTimeout, int iChksum);

#ifdef __TURBOC__
 void far *_MK_FP(unsigned segment,unsigned offset);
#else
 #if defined(_MSC_VER_) && !defined (__cplusplus)
  void far *_MK_FP(unsigned segment,unsigned offset);
 #endif
#endif

int IsResetByWatchDogTimer(void);
int IsResetByPowerOff(void); /* for bios date 12/12/98 or later */

int Show5DigitLedWithDot(int pos, int data);
void Set5DigitLedTestMode(int mode);
void Set5DigitLedIntensity(int mode);
void Disable5DigitLed(void);
void Enable5DigitLed(void);
unsigned GetLibVersion(void);

/* 01/07/1999  Add Timer function */
int TimerOpen(void);
int TimerClose(void);
void TimerResetValue(void);
unsigned long TimerReadValue(void);
void DelayMs(unsigned t);
int StopWatchReset(int channel);
int StopWatchStart(int channel);
int StopWatchStop(int channel);
int StopWatchPause(int channel);
int StopWatchContinue(int channel);
int StopWatchReadValue(int channel,unsigned long *value);
int CountDownTimerStart(int channel,unsigned long count);
int CountDownTimerReadValue(int channel,unsigned long *value);
void InstallUserTimer(void (*fun)(void));
void InstallUserTimer1C(void (*fun)(void));
int IsCom3OutBufEmpty(void);
int IsCom4OutBufEmpty(void);
int DataSizeInCom(int port);
int DataSizeInCom1(void);
int DataSizeInCom2(void);
int DataSizeInCom3(void);
int DataSizeInCom4(void);
/* New for Ver 1.13 */
/* 07-23-1999 add 4 function for COM1 /DTR,/RTS output pins */
void SetDtrActive(void);
void SetDtrInactive(void);
void SetRtsActive(void);
void SetRtsInactive(void);

/* [New for Ver 1.14] */
/**** 07/27/1999 add 5 function for COM1/2/3/4 formated output ****/
int printCom(int port,char *fmt,...);
int printCom1(char *fmt,...);
int printCom2(char *fmt,...);
int printCom3(char *fmt,...);
int printCom4(char *fmt,...);
/* 08/06/1999 add 2 function for COM1 CTS,DSR input pins */
int GetCtsStatus(void);
int GetDsrStatus(void);

/* 08/25/1999 add 5 function for COM1-4 */
int IsTxBufEmpty(int port);
int IsTxBufEmpty1(void);
int IsTxBufEmpty2(void);
int IsTxBufEmpty3(void);
int IsTxBufEmpty4(void);

/* 10/11/1999 add 2 functions for COM2 */
void SetCom2AutoDir(void);
void ResetCom2AutoDir(void);

/* 10/12/1999 add 2 functions for COM1 */
void SetCom1AutoDir(void);
void ResetCom1AutoDir(void);

/* 10/19/1999 add 5 function for COM1/2/3/4 change baudrate */
int SetBaudrate(int port,unsigned long baud);
int SetBaudrate1(unsigned long baud);
int SetBaudrate2(unsigned long baud);
int SetBaudrate3(unsigned long baud);
int SetBaudrate4(unsigned long baud);

/* 10/20/1999 */
/* (1) add 10 function for COM1/2/3/4 send/detect BREAK signal */
int SendBreak(int port,unsigned timems);
void SendBreak1(unsigned timems);
void SendBreak2(unsigned timems);
void SendBreak3(unsigned timems);
void SendBreak4(unsigned timems);
int IsDetectBreak(int port);
int IsDetectBreak1(void);
int IsDetectBreak2(void);
int IsDetectBreak3(void);
int IsDetectBreak4(void);

/* (2) Add 3 functions for clear COM1/2/3/4 output buffer */
int ClearTxBuffer(int port);
void ClearTxBuffer1(void);
void ClearTxBuffer2(void);
void ClearTxBuffer3(void);
void ClearTxBuffer4(void);

/* (3) Add 3 function for set COM1/2 FIFO trigger level, can be 1,4,8,14 */
int SetFifoTriggerLevel(int port,int level);
int SetCom1FifoTriggerLevel(int level);
int SetCom2FifoTriggerLevel(int level);

/* start Ver 1.16 */
/* 09/21/2000 add 5 new EEPROM functions */
void EE_WriteEnable(void);
void EE_WriteProtect(void);
unsigned char EE_RandomRead(int Block,int Addr);
unsigned char EE_ReadNext(int Block);
void EE_MultiRead(int StartBlock,int StartAddr,int no,char *databuf);
void EE_RandomWrite(int Block,int Addr,int Data);
void EE_MultiWrite(int Block,int Addr,int no,char *Data);


/* 10/09/2000 add 5 new functions for COM2*/
void SetCom2InputBufferSize(int size);
void SetCom2OutputBufferSize(int size);

#ifdef __cplusplus
 }
#endif

