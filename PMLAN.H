
extern unsigned long ComBaudPmp1;
extern int Com_lgthPmp1;
extern int Com_parityPmp1;
extern int Com_stopPmp1;

void f_stor_pmp( char type,char *buf,int buf_n);

extern int ComPortPmp1;

int f_pump1();
int ffgets_pmp1(int ii);
int f_calc_csum_pmp1(char *buf, int nn);
int f_preparePmp1(char *msg,int *msg_length);
int f_intr_pmp1();

#define N_cst  2

#define pmln_b_lgth  2048
#define pmln_b_lgth_1 2047

#define M_RD   0
#define M_WR   0x80

#define mess_lgth 22

struct stor_buf
{
long int time;
long int time_m;
char type_n;
char messg[mess_lgth];
};

#define STX 0x02
#define ETX 0x03
#define EOT 0x04

#define ENQ  0x5
#define ACK  0x6
#define NAK  0x15


#define STX_c '>'
#define ETX_c  '<'
#define EOT_c  '#'

#define ENQ_c   '*'
#define ACK_c   '!'
#define NAK_c   '?'


#define IG  '0'    // номер группы
#define PK  '1'    // ptotocol type
                   // 1 - standard pump computer
                   // 2 - with extended protocol (MPD)
                   // 3 - HT with extended LAN protocol
                   // 4 - HT with extended LAN protocol,counts and totals data control


#define pmp1_gun_bit  (int)(0x1)
#define pmp1_dlvr_bit (int)(0x2)
#define pmp1_L1_bit   (int)(0x4)
#define pmp1_L2_bit   (int)(0x8)
#define pmp1_err_bit  (int)(0x20)

#define No_say 0
#define End_dlvr 1
#define Dlvr_rqst 2
#define Status 3

#define AMNT 0x30
#define VOLM 0x31

extern int p1_state;

extern int modf;

extern int flag_StopPmp1;
extern int flag_fst_pmp1;
extern int P_addr;
extern int flag_spec;
extern int lamp_c; // OUT3 - L1

extern int flag_pmp_init;

extern long int WD_PMP_Per;
extern int  WD_PMP_Fl;

extern float *vol_p;
extern float *amount_p;

extern struct stor_buf *bf_pml;

extern int uk_fin_pm;

extern int flag_cst_chg;
extern int cnt_ltch;
extern int flag_fst_cst;
extern float cost_v_c;

extern int  WD_PMP_flag;
extern int  WD_PMP_Fl;
extern long int WD_PMP_time;

extern int fl_pr_pmln;

extern int cost_old;
extern int n_cnt_cst;
extern float cost_vvc;

extern int n_msg_t;
extern long int WD_PMP_Char;
