
#define RD_STATUS 0x10      // ввод состояний входов/выходов
#define W_OUT     0x20      // вывод значений выходов

#define C_LATCH   0x30      // очистка защелки
#define C_WDOG    0x40      // очистка флага сброса по WD
#define S_WDOG    0x50      // установка WD с парам. I7060_wd_E[],I7060_wd_VV[]

#define FAULT_COD  0xf0

#define I7060_max_out_err 10
#define I7060_max_inp_err 10

void f_7060(int ii);
void f_7060_rd(int ii);
void f_7060_Flt(int ii);
void f_init_7060(int ii);

extern struct s_icp_dev I7060[];
extern struct COM_rqst I7060_Rqst[];

extern int i7060_inpl[];
extern int i7060_inph[];
extern int i7060_wdf[];

extern int i7060_fn[];
extern int I7060_wd_E[];
extern int I7060_wd_VV[];

extern int I7060_fl[];

extern int i7060_out[];
extern int i7060_inp[];
extern int i7060_outc[]; // текущее состояние выходов в 7060

extern int i7060_outc_r[];
extern int OUT_VAR_c_r;

