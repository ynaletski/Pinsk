
#define n_sec_day (86400L)
#define M_one (-1)

struct fill_s
{
float VolT;
float Amount   ;
float UnitPrice;
float Density;
float Temp;
};

struct err_s
{
int err[10];
};

struct arch_str // 24 bytes
{
unsigned  int event;
long int time;
/*
unsigned  char sec;
unsigned  char min;
unsigned  char hour;
unsigned  char day  ;
unsigned  char month;
unsigned  char year ;
*/
 float VolI;
 float MassI;
 union  {
  struct fill_s fill;
  struct err_s err;
  } u;
};
//------------------------------
struct arch_str_t // 24 bytes
{
unsigned  char sec;
unsigned  char min;
unsigned  char hour;
unsigned  char day  ;
unsigned  char month;
unsigned  char year ;
};
//------------------------------


#define evt_t_on     1
#define evt_t_off    2
#define evt_start_f  3
#define evt_end_f    4
#define evt_est_f    5
#define evt_err_f    6
#define evt_err      7
#define evt_none     8
#define evt_wrong_start  9
#define evt_clr_err  10
#define evt_no_dev  11
#define evt_ep1_f   12       // остановлено по команде pmp1
#define evt_ch_date   13
#define evt_ch_time   14
#define evt_ch_sum    15
#define evt_ch_win    16
#define evt_res       17      // reset
#define evt_exit      18      // restart
#define evt_re        19
#define evt_re_err    20
#define evt_we        21
#define evt_we_err    22
#define evt_Slot      23
#define evt_chg_param 24
#define evt_exm_f    25
#define evt_exp_f    26
#define evt_overfl   27
#define evt_wd_pmp   28
#define evt_ES_off   29
#define evt_UZA_off  30
#define evt_eq_pr    31
#define evt_stop_eq  32
#define evt_cont_f   33
#define evt_beg_eq   34
#define evt_end_gas  35
#define evt_DLT_off  36
#define evt_MVD_CL_strt       37
#define evt_MVD_CL_BS1        38
#define evt_MVD_CL_BS2        39
#define evt_MVD_CL_OK         40
#define evt_MVD_CL_TM         41

#define evt_LVL_off   50
#define evt_TRAP_off  51
#define evt_CONS_off  52

#define evt_est_fh   70

#define evt_start_fL  101

#define evt_start_hst 128

#define Par_VF   300
#define Par_MF   301
#define Par_PASS 302
#define Par_MC   303
#define Par_ID   304
#define Par_FB   305
#define Par_VB   306
#define Par_VC   307
#define Par_DN   308

void f_prepare_evt (struct arch_str *s_arch, unsigned int  evt );
void f_get_flash_p();
unsigned int f_get_next_seg(unsigned int seg);
unsigned int f_get_prev_seg(unsigned int seg);
unsigned long int get_vdate(unsigned int seg);
int f_wr_evt( unsigned int evt );
int  f_get_max_n_arch();
int  f_rd_astr(struct arch_str *a_str,int nn );
void  f_print_a_str(int i);
int f_print_str_a(struct arch_str *a_str);
struct arch_str *f_get_astr(int nn );
struct arch_str *f_get_dat_s(time_t time_00);
void f_prepare_t(struct arch_str_t *s_arch);
time_t f_get_time_t(struct arch_str_t *ar_str_ptr);

extern unsigned int flash_seg;
extern unsigned int flash_offset;

// расположение переменных в NVRAM

#define nr_year   0
#define nr_month  1
#define nr_day    2
#define nr_hour   3
#define nr_min    4
#define nr_sec    5
#define nr_v0     6
#define nr_v1     7
#define nr_v2     8
#define nr_v3     9
#define nr_m0    10
#define nr_m1    11
#define nr_m2    12
#define nr_m3    13
#define nr_win_sum 14

#define nr_dv0    15
#define nr_dv1    16
#define nr_dv2    17
#define nr_dv3    18

#define nr_dt0    19
#define nr_dt1    20
#define nr_dt2    21
#define nr_dt3    22

#define nr_sm0    23
#define nr_sm1    24
#define nr_sm2    25
#define nr_sm3    26

#define nr_flg_n  27

// NVRAM Addr = 30 - max

int f_print_d_str(time_t time_00, int mode);
int  f_get_max_d_arch();
struct arch_str *f_get_fst_stor();
struct arch_str *f_get_lst_stor();

#define tMin   1
#define tMax   2
#define tNear  3
#define tIntrp 4

extern float tMassI,tVolI;
extern struct tm  mdat0;

extern time_t ltime0,ltime;

time_t f_get_time(struct arch_str *ar_str_ptr);

extern int s_mode;
extern struct arch_str_t ahour00;
extern struct arch_str_t tim_000;
extern struct arch_str_t adate00;

void f_rd_cor();
void f_wr_cor();

