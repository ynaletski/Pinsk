
/*
#define ES_IN    msk_int[num_in1]
#define UZA_IN   msk_int[num_in2]
#define LVL_IN   msk_int[num_in3]
#define STRT_IN  msk_int[num_in4]
#define STOP_IN  msk_int[num_in5]
#define CONS_IN  msk_int[num_in6]
#define TRAP_IN  msk_int[num_in7]
#define O_SH_IN  msk_int[num_in8]
*/


#define KOL_INP (24)
#define KOL_D_INP (13)
//     1  2  3 4 5  6  7  8  9  10  11  12
// MKS 2 11 14 1 0 12 13  4 11   5   0   3 1

#define ES_IN      f_ik(1)
#define UZA_IN     f_ik(2)
#define LVL_IN     f_ik(3)
#define STRT_IN    f_ik(4)
#define STOP_IN    f_ik(5)
#define CONS_IN    f_ik(6)
#define TRAP_IN    f_ik(7)
#define ES_IN_L    f_ik(8)
#define UZA_IN_L   f_ik(9)
#define LVL_IN_L   f_ik(10)
#define STOP_IN_L  f_ik(11)
#define STRT_IN_L  f_ik(12)
#define O_SH_IN    f_ik(13)

#define INP_MSK_OK_v   (0x23b)
#define INP_MSK_OK_v_L (0x28a)

#define UZA_E    2
#define LVL_E    3
#define STOP_E   5
#define CONS_E   6
#define TRAP_E   7
#define ID_E     8
#define STOP_H   9

#define OUT1 msk_int[num_out1]
#define OUT2 msk_int[num_out2]
#define OUT3 msk_int[num_out3]
#define OUT4 msk_int[num_out4]
#define OUT5 msk_int[num_out5]
#define OUT6 msk_int[num_out6]
#define OUT7 msk_int[num_out7]
#define OUT8 msk_int[num_out8]

#define O_One_Shot  msk_int[16]

#define INP1 msk_int[num_in1]
#define INP2 msk_int[num_in2]
#define INP3 msk_int[num_in3]
#define INP4 msk_int[num_in4]
#define INP5 msk_int[num_in5]
#define INP6 msk_int[num_in6]
#define INP7 msk_int[num_in7]
#define INP8 msk_int[num_in8]

#define CLP0 msk_int[num_out7]
#define PUMP msk_int[num_out3]

#define    mski_inp1     inv_msk[1]
#define    mski_inp2     inv_msk[2]
#define    mski_inp3     inv_msk[3]
#define    mski_inp4     inv_msk[4]
#define    mski_inp5     inv_msk[5]
#define    mski_inp6     inv_msk[6]
#define    mski_inp7     inv_msk[7]
#define    mski_inp8     inv_msk[8]
#define    mski_inp9     inv_msk[9]
#define    mski_inp10    inv_msk[10]
#define    mski_inp11    inv_msk[11]
#define    mski_inp12    inv_msk[12]
#define    mski_inp13    inv_msk[13]
#define    mski_inp14    inv_msk[14]
#define    mski_inp15    inv_msk[15]
#define    mski_inp16    inv_msk[16]
#define    mski_inp17    inv_msk[17]
#define    mski_inp18    inv_msk[18]


extern unsigned int OUT_VAR;  // состояние дискретных выходов
extern unsigned int INP_VAR;  // состояние дискретных входов
extern unsigned int INP_VAR_c;  // состояние дискретных входов

extern int analog_num[];
extern float analog_scale[];
extern float analog_offset[];

#define RCV_FLG      0x100

#define Out_on(IntMsk)  OUT_VAR |= IntMsk
#define Out_off(IntMsk)  OUT_VAR &= ~IntMsk

#define  T_Mot_Stop (35000L)

extern int fl_GO;
extern int fl_EQ;     // флаг выравнивания давления через MVD
extern int flagS_ES;  // признак останова по нажатию ES
extern int flagS_UZA; // признак останова по отсутствию UZA
extern int flag_dlv_fst;
extern int flag_prn_mass;
extern int flag_rcv_fst;

extern int sw_dlv_liq;
extern int sw_dlv_gas;
extern int sw_rcv_liq;
extern int sw_rcv_gas;
extern float dose_rcv,dose_dlvr;

extern int key__1;
extern int flag_continue;
extern int flag_rcv;

void f_sens();
int f_dlv_liq(int key);
int f_dlv_gas(int key);
int f_rcv_liq(int key);
int f_rcv_gas(int key);
void f_stop_cnt_eq();
void f_stop_EQ();
void f_stop_EQn();
int f_need_EQ_dlv();
int f_need_EQ_rcv();
int f_need_LIQ_dlv();
int f_need_LIQ_rcv();

extern long int time_com_cl    ;
extern long int time_com_cl_off;
extern long int time_cnt_cl_off;
extern long int time_gas_cl_on ;
extern long int time_gas_cl_off;

extern long int time_EQ_rcv;
extern long int time_EQ_dlv;
extern long int time_LIQ_rcv;
extern long int time_LIQ_dlv;


extern int flagS_DLT;
extern unsigned int INP_inv;
//extern unsigned int INP_VARi;
extern unsigned int INP_VARi0;
extern unsigned int INP_MSK_OK;
extern int flagE_UZA;
extern unsigned int OUT_VAR_c;

void f_inp7188();
void f_one_shot();

extern int *DriveStatus;
extern int DriveSV[];
extern int DriveSA[];
extern int DriveSD[];
extern int DriveState[];
extern int DriveCurrent[];
extern int DriveError[];

extern int (*f_Drive_St_chk)();
extern int (*f_Drive_Rn_chk)();
extern void (*f_Drive_Stop)();
extern void (*f_Drive_Run)();

extern int flag_ext_t;
extern int Drive_SV_beg[];
extern int Drive_SA_beg[];
extern int Drive_SD_beg[];
extern char list_avt[][32];

extern long int Tim_Prep1;
extern long int Tim_Prep_ND;
extern float F_Dns_off;
extern float P_beg_valve;

extern int  inv_msk[] ;
int f_ik(int i);
void f_init_mki();

extern int num_in9 ,num_in10 ;
extern int num_in11,num_in12 ;
extern int num_in13,num_in14 ;
void f_var_i();

extern unsigned int INP_MSK_OK_L;

void f_out();

extern int en_bot[];

extern int result_dlv;

//09.09.2021 YN 
extern int fl_GO_t;