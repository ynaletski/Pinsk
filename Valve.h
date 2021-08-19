
#define HOLD 1
#define UP   2
#define DN   4

extern void (*ff_valve)();
void f_init_valve();
void f_two_valve();

extern int valve_mode;
extern float P_min_valve;
extern float P_min_valveL;

extern long int Per_up_min_v;
extern long int Per_up_max_v;
extern long int Per_dn_min_v;
extern long int Per_dn_max_v;
extern long int Tim_hld     ;
extern long int Per_UP_dl   ;
extern long int Per_DN_dl   ;

extern long int Per_up_min_vL;
extern long int Per_up_max_vL;
extern long int Per_dn_min_vL;
extern long int Per_dn_max_vL;
extern long int Tim_hldL     ;
extern long int Per_UP_dlL   ;
extern long int Per_DN_dlL   ;

extern float Vol_beg;
extern float Flow_beg;
extern float dFP_beg   ;

extern float Flow_nom;

extern float Mass_min;
extern float Flow_min;
extern float Mass_min1;
extern float Flow_min1;
extern float Mass_min2;
extern float Flow_min2;

extern float dFP_min   ;
extern float dFP_min1  ;
extern float dFP_min2 ;
extern float dFP_nom   ;

extern float Flow_nomL;

extern float Mass_minL;
extern float Flow_minL;
extern float Mass_min1L;
extern float Flow_min1L;
extern float Mass_min2L;
extern float Flow_min2L;

extern float dFP_minL   ;
extern float dFP_min1L  ;
extern float dFP_min2L ;
extern float dFP_nomL   ;

void f_two_valveL();
void f_en_two_valvL();


extern int valve_modeL;

extern int Flag_Low;


