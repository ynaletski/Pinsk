
//extern const unsigned long far *TimeTicks;

void far *_MK_FP_(unsigned s,unsigned off);

#define n_bufcom 200  // длина буфера команды

#define EKD_KBD   1
#define EKD_PG    2
#define EKD_ST    3

#define Ekd_max_inp_err    10
#define Ekd_max_out_err    10

#define KEY_SIZE_e  32
#define KEY_SIZ1_e (KEY_SIZE-1)

extern int  ekd_inp_err;
extern int  ekd_out_err;

//#define size_max 29


void f_clr_scr_Ekd_p(int page);
void f_clr_scr_c_Ekd_p(int page);
void f_start_fst( int nn);
void  f_prn_beginEkd();
void SetEkdPage(int page);
void  f_chg_ekd(int nny);
int f_empty_Ekd(int ii);
int ffgets_com_Ekd( int ii);
void ServiceCOM( int ii);
int ffgets_host( int port);
void f_Flt_ans(int ii);
void f_empty(int ii);
int EkdKbhit(void);
int EkdGetch(void);
void SetDisplayPage(int page);
void EkdShowCursor(void);
void EkdCursorOff(void);
void EkdGotoxy(int x,int y);
void SetDisplayPage(int page);
void SendToEkd();
void f_Ekd_KBD_ans(int ii);
void f_Ekd_SetAns(int ii);
void f_Ekd_Flt(int ii);

void EkdMoveToXY(int x,int y);
void EkdPuts(char *str);
int  EkdPrintf(char *format , ...);
void EkdPutch(char ch);
void f_clr_scr_Ekd();
void f_clr_scr_c_Ekd();
void f_rev_scr_Ekd();
int f_get_nb( long int baud);

void f_Ekd(void);
int f_menu_Ekd();
void  f_prn_begin();
void f_prn_CRC_error();
void f_prn_error();

int f_timer(unsigned long int time_stamp,unsigned long int delay);

int fstinE();
extern void (*ff_serv)();

extern char cb_COM[][n_bufcom];
extern int n_bcom[];

extern int pool_Ekd;
extern int fn_Ekd;

extern int ekd_key_in,ekd_key_out;

extern char Ekd_str[];
extern int ekd_page;

extern int  (*ff_gets_com[5])(int ii);
extern void (*ff_answ_com[5])(int ii);
extern void (*ff_answ_flt[5])(int ii);

extern int fl_ekd_new;

extern int x_ekd,y_ekd;
extern int ekdx,ekdy;

extern int flag_ekd;
extern int flag_ekd1;

extern int sw_fst;
extern char fst_str[];
extern int  fst_n;
extern int fl_mmi_new1;
extern int mmi_sw;
extern int flag_echo_host;

extern int ttt1,ttt2,ttt3;
extern unsigned long ttt1_l,ttt2_l,ttt3_l;

#define printf  printCom4
#define getch() getch_host()
#define kbhit() IsCom(4)

//#define putch   ToCom4
#define putch  (*ff_putch[ComPortHost])

#define puts   puts_host

// записывает текущее значение таймера в пределах 1 мс в 100 ns единицах
// значение от 0 до 9999
#define tim1()     asm{mov dx,0ff60H;in ax, dx;mov ttt1,ax}
#define tim2()     asm{mov dx,0ff60H;in ax, dx;mov ttt2,ax}
#define tim3()     asm{mov dx,0ff60H;in ax, dx;mov ttt3,ax}
#define tim4()     asm{mov dx,0ff60H;in ax, dx;mov ttt4,ax}
#define TimeStamp  *TimeTicks

extern int PC_mode;
extern unsigned long ToutAns[5];
extern unsigned long int time_snd[5];

extern int last_out[5];

extern unsigned long ComBaud[];
extern int Com_lgth[];
extern int Com_parity[];
extern int Com_stop[];
extern long int baudrate_val[];
extern struct s_icp_dev MMI;
extern struct COM_rqst MMI_Rqst;
extern float var[];

extern int page_MMI;
extern int page_MMI_c;
extern char sw_ver[20];

//--------------------------------

void PrintMmiOp(struct dis_set_MMI *ds_s,int flag_prn);
void ScanfMmiOp(struct dis_set_MMI *ds_s);

extern struct dis_set_MMI  ds_list1[];
extern struct dis_set_MMI  ds_list2[];
extern struct dis_set_MMI  ds_list3[];

extern int  flag_prn;
//extern const char *list1_dsr[];

extern int sw_mmi;
extern int max_list1;
extern int max_list2;
extern int max_list3;

extern int CRC_err;
extern unsigned long int time_rcv[];

extern int ComProt[5];

int f_cycle();
void f_time_dd();
void f_print_dd();
void f_one_sec();
void f_mmi_err();
extern long int scom_tim[5];
extern long int Tm_snd[];

extern unsigned long int one_sec_time;
extern int flag_Serv;

extern char *list1_dsr[];
extern char *list2_dsr[];

extern int FlagWinSum;

int init_win_sum(int cor);
extern float chg_nn_par,chg_old,chg_new;

extern int un_mmi1;
extern int un_mmi2;
extern int un_mmi3;
extern char *list_un_mmi[];
extern float vBIG_P;

//20.08.2021 YN
#if defined(UNDERPRESSURE)
extern float ftmp_naMMI[3];
#else
extern float ftmp_naMMI[2];
#endif

void f_before_MMI();
void f_after_MMI();
extern int ZeroPage;
extern int EmptPage;

extern int year,month,day,hour,min,sec;
extern float MassStamp;
extern float VolStamp;
extern struct s_icp_dev Ekd;
extern struct COM_rqst Ekd_Rqst;
extern struct COM_rqst Ekd_Rqst_c;
extern struct COM_rqst Ekd_Rqst_c1;
extern int sw_ekd;
extern int num_page;
extern long int zero_time;
extern long int fl_zeroing;
extern long int Password;
extern long int PassW;
extern int Ekd_fl;

extern int sw_fst_ekd;
extern int Curs_x,Curs_y,Curs_stt;

