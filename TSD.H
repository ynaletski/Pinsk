
#define max_TSD 2
#define ToutAnsTSD 100

#define TSD_EM    16
#define TSD_WR_P   8
#define TSD_WR_S   1

#define TSD_max_out_err 10
#define TSD_max_inp_err 10

void f_TSD(int ii);
void f_TSD_rd(int ii);
void f_TSD_flt(int ii);
void f_clr_TSD(int ii);
void f_rev_TSD(int ii);
void f_chk_new_TSD(int ii);
void f_print_TSD(int ii);
void  f_prep_TSD(int dot,char *buf,float f_value);
void f_init_TSD();

int ffgets_com_GLB( int ii);
void f_GLB_rd(int ii);
void f_GLB_flt(int ii);
void f_TSD_GLB(int ii);
void f_init_TSD_GLB(int ii);
void f_prep_StrG(int ii,int mm);
extern int TSD_GLB_fl[];
extern int flag_G_em;
extern long int G_em_per;

extern struct s_icp_dev TSD[];
extern struct COM_rqst TSD_Rqst[];

extern int TSD_fn[];     // регистр функций драйвера
extern int TSD_fl[];

extern int sw_TSD_1;

extern int flag_TSD_fst;
extern struct s_icp_dev TSD[];
extern struct COM_rqst TSD_Rqst[];

extern int n_dot[max_TSD][3];

extern float TSD_val_c[max_TSD][3];
extern float *TSD_value[max_TSD][3];

extern long int TSD_period;  // ms

extern int TSD_vn[max_TSD][3];
extern float *list_vn[];

extern struct dis_set  ds_vnn[];

extern char TsdBuf[max_TSD][3][10];

