

int ffgets_com_rtu( int ii);
int f_chg_toRTU(char *buf);
int f_check_crc_RTU(char *msg,int msg_length);
int f_prepareRTU( char *msg,int *msg_length);
int f_prepareRTU1( char *msg,int *msg_length);
unsigned int  crc16(unsigned char *buffer, int buffer_length);

extern unsigned char table_crc_hi[];
extern unsigned char table_crc_lo[];
extern long int byte_timout_rtu[5];


