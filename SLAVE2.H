
#define Cmd2_reg    (7)


extern void (*ff_Slv2)();

extern int flag_Slv2;

extern int ComPortSlv2;
extern int AddrSlv2;
extern int State_SLV2;

extern long int Com_Baud_Slave2;
extern unsigned long ComBaudSlv2 ;
extern int Com_lgthSlv2  ;
extern int Com_paritySlv2;
extern int Com_stopSlv2;

extern int  WD_PMP2_flag;
extern int  WD_PMP2_Fl;

extern long int WD_PMP2_time;
extern long int WD_PMP2_t1;
extern long int WD_PMP2_Per; // величина таймаута по приему сообщения

extern void *list_Slv2_int[];

extern int n_sl2_int;
extern int n_sl2_int1;

extern float *list_Slv2_flt[];

void f_Slave2RTU();
int ffgets_Slv2RTU(int ComPort);
void  f_intr_Slv2RTU();

extern int n_sl2_lng ;
extern int n_sl2_lng1;

extern int n_sl2_flt ;
extern int n_sl2_flt1;
extern int n_sl2_flt0;

extern int Slv2_int[];
extern float Slv2_float[];


#define Addr2LowInt 0
#define Addr2HighInt (AddrLowInt+n_sl2_int-1)

#define Addr2LowFloat  1000
#define Addr2HighFloat (Addr2LowFloat + n_sl2_flt0 + n_sl2_flt0 + n_sl2_lng + n_sl2_lng -  2 )

#define Addr2LowLong (Addr2LowFloat + n_sl2_flt0 + n_sl2_flt0 )


#define Addr2Slot1  2000
#define Addr2Slot2  3000
#define Addr2Slot3  4000
#define Addr2Slot4  5000

extern int    CMD_IN   ;
extern int    i_LVL    ;
extern int    ii_LVL[] ;
extern float  LIn_val  ;

#define Short_OK (40)


extern float Vol_kd  ;
extern float Mass_kd ;
extern float Press1  ;
extern float Press2  ;
extern float Temp1   ;
extern float Temp2   ;

extern int CMD_OUT   ;
extern int Reg_State ;
extern int Reg_err   ;

extern int   FlagNewDT ;
extern int   Tank1F    ;
extern int   Tank2F    ;
extern float New_Dens1 ;
extern float New_Temp1 ;
extern float New_Dens2 ;
extern float New_Temp2 ;
extern float New_Dens1c;
extern float New_Temp1c;
extern float New_Dens2c;
extern float New_Temp2c;


int f_set_slv( struct eee_dat *LIn_D, int Slv_nmbO );

extern float MassTotal,VolTotal,DenTotal,TempTotal;
extern float MassGRK, VolGRK;
void f_cnt_totals();

extern int Flag_rd_cmd;

