
#define MAX_ADC_COR  2  // количество корретируемых каналов ADC
#define MAX_COR_D 16    // количество исходных точек коррекции
#define MaxADC 65536L
#define MaxVADC 100000L
#define NA_scale (65535.)

#define Step_COR 1024L
#define Cor_ADC_Lgth  130
//#define Cor_ADC_Lgth  (( (MaxADC+MaxADC) /Step_COR)+2)


extern int n_dat_et[MAX_ADC_COR];
extern float adc_et[MAX_ADC_COR][MAX_COR_D];
extern float val_et[MAX_ADC_COR][MAX_COR_D];

// результат расчета

extern float Cor_ADC[MAX_ADC_COR][Cor_ADC_Lgth];
extern float TT_PER_1_ADC[MAX_ADC_COR];

void f_init_COR(int num);
int f_prepare_AV(int num);
void f_init_et();

