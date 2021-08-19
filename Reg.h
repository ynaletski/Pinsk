
#define YN_OUT   DriveSV[0]

#define Mask_reg_p (1)
#define Mask_reg_d (2)

#define K_F_INT (10000L)

#define k_div   (2048L)
#define n_shft  11


#define Add_lim_h (2000)
#define Add_lim  (Add_lim_h * k_div)


#define  YiN  LIM_P_c
//#define I_real  Delta_current[0]

#define Dlt_P_Dem  (0.05 * K_F_INT)

extern long int YN,XN,XN_1,XN_2;
extern float P_dem;

extern float P_dem_c;

extern long int f_Beg;
extern long int maxP_errP, maxP_errM;
extern float LIM_P,LIM_M;
extern long int K0,K1,K2,K3;
extern long int k_p,k_i,k_d;

extern float P_Dif_d;
extern long int kd_p,kd_i,kd_d;
extern long int maxPd_errP, maxPd_errM;
extern long int LIMd_P,LIMd_M;
extern long int Kd0,Kd1,Kd2,Kd3;

extern long int YdN,XdN,XdN_1,XdN_2;

extern int mode_r;

extern float Flow_on[] ;
extern float Flow_off[];
extern int flag_flow;

extern long int I_dem;
extern long int ki_p,ki_i,ki_d;
extern long int maxPi_errP, maxPi_errM;
extern long int LIMi_P,LIMi_M ;
extern long int Ki0,Ki1,Ki2,Ki3;
extern long int XiN,XiN_1,XiN_2;

extern float LIM_P_c;
extern int mode_i;
extern int flg_prn_reg;

extern long int Flow_dem;
extern long int Hop;
extern long int Hop_cur;

extern long int kf_p,kf_i,kf_d;
extern long int Kf0,Kf1,Kf2,Kf3;
extern long int YfN,XfN,XfN_1,XfN_2;

extern long int dF_perm;
extern long int  LIM_FLOW_P;
extern long int  LIM_FLOW_M;
extern long int  maxF_errP;
extern long int  maxF_errM;

extern long int  LIM_FLOW_PL;
extern long int  LIM_FLOW_ML;
extern long int  maxF_errPL;
extern long int  maxF_errML;

extern float Value_beg;
extern long int Flow_dem;
extern long int flow_D;

void f_init_reg_P();
void f_reg_P();
void f_reg_D();
void f_count_cf();
void f_reg_I();
void f_init_reg_F();
void f_reg_F();

extern long int kf_pL,kf_iL,kf_dL;
extern long int Kf0L,Kf1L,Kf2L,Kf3L;

extern float LIM_P_act; // актуальное значение номинальной частоты привода
extern float LIM_P_dec; // значение ступени снижения частоты привода
extern float LIM_P_beg; // значение частоты привода при начальном наполнении

extern float f_LIM_P_beg,f_LIM_P, f_LIM_P_dec;

