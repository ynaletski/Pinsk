
void f_MVD_cor()
{
  // запускается после окончания налива
  // через время t_mvd_cor мс состояние процесса записывается в журнал
int i;
long int ltmp,i1;
float ftmp;


  if( f_timer(tm_mvd_p,(t_mvd_cor-1000) ) == 0 )
  {
   fl_VI=0;
   fl_MI=0;
   cl_MVD_sw=0;
   t_prep_MVD=TimeStamp;
   return;
  }
  if(cl_MVD_sw == 10) goto m00;

  if((MVD_fn[GR] != 0) && (MVD_fn[GR] != F_FAULT))
  {
      if( f_timer(t_prep_MVD,(long int)2000L ) == 0 ) return;
      f_icp_errS(No_F_dat_err);

//  printf("=P="); //WWWW
      return;
  }
  switch (cl_MVD_sw)
   {
    case 0:

       if( fabs(s_MVD[GR].FlowM) > F_Dns_off_DLV)
             f_icp_errS(DNS_not_off);

       // // Stop totalizers
        MVD_fn[GR]=MVD_WR_C;
        s_MVD[GR].r_addr=2; // Stop totalizers
        s_MVD[GR].i=0;

        cl_MVD_sw=1;
        t_prep_MVD=TimeStamp;
//        printf("\n\r0");
        return;

    case 1:
       // установка значения среза объемного расхода для режима ожидания
        MVD_fn[GR]=MVD_WR_F;
        s_MVD[GR].r_addr=197; // Cuttoff for volume flow
        s_MVD[GR].f=dens_cutoff1;

        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=2;
        return;
    case 2:

       // установка значения среза массового расхода для режима ожидания
        MVD_fn[GR]=MVD_WR_F;
        s_MVD[GR].r_addr=195; // Cuttoff for mass flow
        s_MVD[GR].f=dens_cutoff;

        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=3;
        return;

    case 3:
//        printf("\n\r2");
        cl_MVD_sw=10;
        fl_VI=0;
        fl_MI=0;
        return;

    default:
        return;
   }

m00:
  if( f_timer(tm_mvd_p,t_mvd_cor )==0 )
  {
   fl_VI=0;
   fl_MI=0;
   return;
  }

  if((fl_VI==0) || (fl_MI==0))
  {
//    if( f_timer(tm_mvd_p,(t_mvd_cor*3))==0 )
    if( f_timer(tm_mvd_p,(MVD[GR].pool_time*24))==0 )
       return;
    else
     {
//     f_icp_error(&MVD[GR],RD_ERR );
       f_icp_errS(No_F_dat_err);

//  printf("=Q="); //WWWW
       goto m1;
     }
  }

m1:
//printf("\r\n f_MVD_cor() ");
  if(flag_fill_ok)
  {

     ftmp=-s_MVD[GR].VolT-vol_f-rnd_add;

     if(flag_StopPmp1)
       f_wr_evt(evt_ep1_f);
     else if(ftmp > ex_p)
      {
//     f_icp_errS(exp_err);
       f_wr_evt(evt_exp_f);
       f_wr_deb();
       goto m11;
      }
     else if(ftmp < (-ex_m) )
      {
//     f_icp_errS(exm_err);
       f_wr_evt(evt_exm_f);
       f_wr_deb();
       goto m11;
      }
     else
       f_wr_evt(evt_end_f);

     if((ftmp > ex_p_d) || (ftmp < (-ex_p_d) ))
      {
       f_wr_deb();
      }
     else if(flag_prok != 0)
       f_wr_deb();

  }
  else
  {
     if(FL_err)
     {
        f_wr_evt(evt_err_f);
        f_wr_deb();
//        flag_prok=0;
     }
     else if(flag_flch)
        f_wr_evt(evt_overfl);
     else if(flag_gun)
        f_wr_evt(evt_gun);
     else if(WD_PMP_Fl)
        f_wr_evt(evt_wd_pmp);
     else if(flag_StopPmp1)
        f_wr_evt(evt_ep1_f);
     else
        f_wr_evt(evt_est_f);

 m11:
       VolT_id=-s_MVD[GR].VolT;
  /*
       switch(ModeFill)
       {
       case F_Vol:
            VolT_id=-s_MVD[GR].VolT;
            break;
       case F_Vol_Dens:
            VolT_id=s_MVD[GR].VolTd;
            break;
       case F_Mass:
            VolT_id=-s_MVD[GR].MassT;
            break;
       default:
            VolT_id=s_MVD[GR].VolTd;
            break;
       }
  */

    if(flag_prok != 0)
       f_wr_deb();

  }

     VolT_id=((long int)(VolT_id * 100.+0.5));
     VolT_id *= 0.01;

     rnd_add_m = round_summ_m;

     if(rnd_add_m > rnd_lim_m) rnd_add_m = rnd_lim_m;
     else if(rnd_add_m < -rnd_lim_m) rnd_add_m = -rnd_lim_m;

     MassT_id=-s_MVD[GR].MassT+rnd_add_m;
     MassT_id=((long int)(MassT_id * 1000.+0.5));
     MassT_id *= 0.001;
     round_summ_m  -= MassT_id+s_MVD[GR].MassT;

 //    VolT_id будет послано в отчете
 // Если счетчик колонки > отчета в PC , round_summ < 0

       switch(ModeFill)
       {
       case F_Vol:
//          round_summ+=VolT_id-s_MVD[GR].VolT;
            round_summ+=VolT_id+s_MVD[GR].VolT;
            break;
       case F_Vol_Dens:
            round_summ+=VolT_id-s_MVD[GR].VolTd;
            break;
       case F_Mass:
//          round_summ+=VolT_id-s_MVD[GR].MassT;
            round_summ+=VolT_id+s_MVD[GR].MassT;
            break;
       default:
            round_summ+=VolT_id-s_MVD[GR].VolTd;
            break;
       }

     summ_v_id=VolT_id*cost_v;

     summ_v_id=f_round(summ_v_id);

     VolT_id1=VolT_id;
     summ_v_id1=summ_v_id;

     Modef &= ~pmp1_dlvr_bit;
     p1_state |= End_dlvr;

     flag_end_dlvr=1;

     t_end_dlvr=TimeStamp;

     flag_nal=0;
     offs_cor=(vol2 - vol_f - rnd_add);
//   if(flow_mom > 0) dsc_tmp=offs_cor/flow_mom;
//   else
         dsc_tmp=0;

 // вычисление rnd_add для следующего отпуска
   rnd_add =round_summ;

   if(rnd_add > rnd_lim) rnd_add = rnd_lim;
   else if(rnd_add < -rnd_lim) rnd_add = -rnd_lim;

// корректирует параметры наполнения
  if(fct_offs==0)
     {
m_end:
          ff_serv_N=NULL;
          return;
     }
     if(flag_fill_ok != 1) goto m_end;
     if(FL_err)   goto m_end;
     if(flag_flch+flag_gun+WD_PMP_Fl+flag_StopPmp1) goto m_end;
     if(flow_mom < flow_min_c) goto m_end;

     if(fct_offs != 0)
       CL_val[1] += fct_offs*offs_cor;
//   if(fct_delay != 0)
//     CL_T2 += fct_delay*dsc_tmp;
     ff_serv_N=NULL;
     f_wr_cor();
}
