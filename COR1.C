
float cl_val_c=-100000;
float cl_t_c=-100000;
int num_clp_sv=1; // 1...3 номер клапана сохраняемого в NVRAM

#define CL_valP  (*lst_val[num_clp_sv])
#define CL_TP    (*lst_tim[num_clp_sv])

float *lst_val[4]={
  &CL_val[1],
  &CL_val[1],
  &CL_val[0],
  &CL_val0
};

float *lst_tim[4]={
  &CL_T2,
  &CL_T2,
  &CL_T1,
  &CL_T0,
};

void f_wr_cor()
{
float ftmp;
union  { float f; char c[4]; } o;

    ftmp=CL_valP+CL_TP;

    if( (cl_val_c != CL_valP) || (cl_t_c != CL_TP ) )
    {
       o.f= CL_valP ;
       cl_val_c=o.f;
       WriteNVRAM((int) nr_dv0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dv1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dv2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dv3   ,(int)o.c[3] );

       o.f= CL_TP;
       cl_t_c=o.f;
       WriteNVRAM((int) nr_dt0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dt1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dt2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dt3   ,(int)o.c[3] );

       o.f= ftmp;
       WriteNVRAM((int) nr_sm0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_sm1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_sm2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_sm3   ,(int)o.c[3] );
    }
}
/*-----------------------------------------*/
void f_rd_cor()
{
union  { float f; char c[4]; } o;
float ftmp,ftmp1,ftmp2,ftmp3;

 o.c[0]= ReadNVRAM( nr_dv0 );
 o.c[1]= ReadNVRAM( nr_dv1 );
 o.c[2]= ReadNVRAM( nr_dv2 );
 o.c[3]= ReadNVRAM( nr_dv3 );
 ftmp = o.f;
 o.c[0]= ReadNVRAM( nr_dt0 );
 o.c[1]= ReadNVRAM( nr_dt1 );
 o.c[2]= ReadNVRAM( nr_dt2 );
 o.c[3]= ReadNVRAM( nr_dt3 );
 ftmp1 = o.f;
 ftmp3=ftmp+ftmp1;
 o.c[0]= ReadNVRAM( nr_sm0 );
 o.c[1]= ReadNVRAM( nr_sm1 );
 o.c[2]= ReadNVRAM( nr_sm2 );
 o.c[3]= ReadNVRAM( nr_sm3 );
 ftmp2 = o.f;
 if(ftmp2== ftmp3)
 {
   CL_valP=ftmp;
   CL_TP=ftmp1;
 }
}
/*-----------------------------------------*/
