

// исходные данные для построения таблицы коррекции

//int n_dat_et[MAX_ADC_COR]={0,0};
//float adc_et[MAX_ADC_COR][MAX_COR_D]={ 32124 ,  9123, 17342  };
//float val_et[MAX_ADC_COR][MAX_COR_D]={  6.54 , 2.743, 4.234  };


/*

void f_norm_AV(int num)
{ // нормализует упорядоченные данные - приводит к виду, когда на одном
  // интервале коррекции не более одного эталонного значения
  // если в исходных данных значений больше - выполняется усреднение
  //    Данные таблицы коррекции ADC:
  //
  //  float adc_et[num][MAX_COR_D];
  //  float val_et[num][MAX_COR_D];
  //  int n_dat_et[num];
  //
  // нормализованные данные записываются в массивы
  //  adc_et_tmp[]
  //  val_et_tmp[]
  //  исходные данные модифицируются
  //
  // return 1 - OK
  //        0 - Error

 float *adc;
 float *val;
 int  n_dat;
 long int ii,jj,kk;
 int ll;
 float adcS,valS,nnS;

  n_dat=n_dat_et[num];
  if(n_dat==2) return;

  adc=&adc_et[num][0];
  val=&val_et[num][0];

  ll=0;
  if(adc[0]=-MaxADC)
  {
   adc_et_tmp[ii]=adc[0];
   val_et_tmp[ii]=val[0];
   ll=1;
  }
  jj=0;
  for(ii=-MaxADC,kk=Step_COR-MaxADC;ii<MaxADC;ii+=Step_COR)
  {
      adcS=0.;valS=0;nnS=0;
      for( ;jj<n_dat;jj++)
      {
       if( (ii< adc[jj])&& (adc[jj] <= kk))
        {
         adcS+=adc[jj];
         valS+=val[jj];
         nnS+=1;
         n_used+=1;
        }
      }
      if(nnS==0) continue;
      if(nnS==1)
      {
       adc_et_tmp[ll]=adcS;
       val_et_tmp[ll]=valS;
       ll++;
      }
      else
      {
       adc_et_tmp[ll]=adcS/(float)nnS;
       val_et_tmp[ll]=valS/(float)nnS;
       ll++;
      }
  }
  if(ll==1)
  {
    adc_et_tmp[0]=adc[0];
    val_et_tmp[0]=val[0];
    adc_et_tmp[1]=adc[n_dat-1];
    val_et_tmp[1]=val[n_dat-1];
    ll=2;
  }
  _fmemcpy( &adc_et[num][0],&adc_et_tmp[0], sizeof(adc_et_tmp));
  _fmemcpy( &val_et[num][0],&val_et_tmp[0], sizeof(val_et_tmp));
  n_dat_et[num]=ll;
}

//-------------------------------------------

// результат расчета

float Cor_ADC[MAX_ADC_COR][Cor_ADC_Lgth];
float TT_PER_1_ADC[MAX_ADC_COR];

float adc_et_tmp[MAX_COR_D+2];
float val_et_tmp[MAX_COR_D+2];


void f_init_COR(int num)
{

 float *t_cor;
 float *adc;
 float *val;
 float ADC_cur;
 int n_dat;
 int ii,jj,kk;

  n_dat=n_dat_et[num]+2;
  t_cor=&Cor_ADC[num][0];

  adc=&adc_et_tmp[0];
  val=&val_et_tmp[0];

 // по исходным данным для построения таблицы коррекции
 // заполняет таблицу коррекции Cor_ADC[num][0]

 if(f_prepare_AV(num))
 {  // упорядоченные данные в
    //  adc[]
    //  val[]
    //  n_dat - количество

    t_cor[0]=Step_COR;
    t_cor[1]=val[0];

    ADC_cur=-MaxADC;

    jj=2;// индекс в t_cor[]
    ii=0;kk=1;
    for(ADC_cur=Step_COR-MaxADC;ADC_cur<MaxADC;ADC_cur+=Step_COR,jj++)
    {
       for(;kk<n_dat;kk+=1,ii+=1)
         if(adc[kk] > ADC_cur) break;
       t_cor[jj] = val[ii] + (ADC_cur-adc[ii]) * (val[kk]-val[ii])/( adc[kk]-adc[ii]);
    }
    t_cor[jj] = val[n_dat-1];
//  printf("\n\rjj=%d, val=%g ,val1=%g",jj,val[n_dat-1],val[n_dat]);
 }
 else
 {
  t_cor[0]=MaxADC;
  t_cor[1]=-MaxADC;
  t_cor[2]= 0;
  t_cor[3]= MaxADC;
 }

m_fin:
 TT_PER_1_ADC[num]=1./t_cor[0];

}
//===========================
int ADt_tmp[MAX_COR_D];

int f_prepare_AV(int num)
{ // проверяет и упорядочивает в порядке возрастания входные
  //    данные таблицы коррекции ADC:
  //
  //  float adc_et[num][MAX_COR_D];
  //  float val_et[num][MAX_COR_D];
  //  int n_dat_et[num];
  //
  // упорядоченные данные записываются в массивы
  //  adc_et_tmp[1]
  //  val_et_tmp[1]
  //  данные дополняются элементами  со значениями adc=-MaxADC и adc=MaxADC;
  //  исходные данные модифицируются (упорядочиваются)
  //
  // return 1 - OK
  //        0 - Error

 float *adc;
 float *val;
 int  n_dat;
 int ii,jj,i0,i1;
 float adc_min;
 int index_min;

  n_dat=n_dat_et[num];

  if((n_dat <= 0) || (n_dat> MAX_COR_D )) goto m_err;

  _fmemset( &ADt_tmp[0],0x0,sizeof(ADt_tmp)); // очистка массива флагов

  adc=&adc_et[num][0];
  val=&val_et[num][0];
  adc_et_tmp[0]=BIG_P;
  for(ii=1;ii<=n_dat;)
  {
      adc_min=BIG_P;
      index_min=-1;
      for(jj=0;jj<n_dat;jj++)
      {
       if(ADt_tmp[jj]) continue;
       if(adc[jj]> MaxADC) continue;
       if(adc[jj]< (-MaxADC)) continue;

       if(adc_min > adc[jj])
        {
         adc_min=adc[jj];
         index_min=jj;
        }
      }
      if(index_min < 0)
      {
       n_dat=ii-1;
       goto m_end;
      }
      if(adc_et_tmp[ii-1] != adc[index_min])
      {
       adc_et_tmp[ii]=adc[index_min];
       val_et_tmp[ii]=val[index_min];
       ii++;
      }
      ADt_tmp[index_min]=1;
  }
m_end:

  if((adc_et_tmp[1] <(-MaxADC)) || (adc_et_tmp[n_dat] > MaxADC)) goto m_err;
  if(n_dat<2) goto m_err;
  if(n_dat>MAX_COR_D) goto m_err;
  _fmemcpy( &adc[0],&adc_et_tmp[1], sizeof(float)  *((long int)n_dat) );
  _fmemcpy( &val[0],&val_et_tmp[1], sizeof(float)  *((long int)n_dat) );
  n_dat_et[num]=n_dat;

  i0=0;i1=1;
  adc_et_tmp[0]=-MaxADC;
  val_et_tmp[0]= val[i0] + ((-MaxADC)-adc[i0]) * (val[i1]-val[i0])/( adc[i1]-adc[i0]);

  i0=n_dat-2;i1=n_dat-1;
  adc_et_tmp[n_dat+1]=MaxADC;
  val_et_tmp[n_dat+1]= val[i0] + (MaxADC-adc[i0]) * (val[i1]-val[i0])/( adc[i1]-adc[i0]);

  return 1;

m_err:
    return 0;
}
//-------------------------------------------

void f_init_et()
{
int i,j;
  for(j=0;j<MAX_ADC_COR;j++)
    for(i=0;i<MAX_COR_D;i++)
     adc_et[j][i]=MaxVADC;
}
//-------------------------------------------
==============*/
