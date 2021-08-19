
#define RD_STATUS 0x10      // ввод состояний входов/выходов
#define W_OUT     0x20      // вывод значений выходов

#define C_LATCH   0x30      // очистка защелки
#define C_WDOG    0x40      // очистка флага сброса по WD
#define S_WDOG    0x50      // установка WD с парам. I7041_wd_E[],I7041_wd_VV[]

#define FAULT_COD  0xf0

#define I7041_max_out_err 10
#define I7041_max_inp_err 10

void f_7041(int ii);
void f_7041_rd(int ii);
void f_7041_Flt(int ii);

extern struct s_icp_dev I7041[];
extern struct COM_rqst I7041_Rqst[];

extern int i7041_inpl[];
extern int i7041_inph[];
extern int i7041_wdf[];

extern int i7041_fn[];
extern int I7041_wd_E[];
extern int I7041_wd_VV[];

extern int I7041_fl[];

extern int i7041_inp[];

