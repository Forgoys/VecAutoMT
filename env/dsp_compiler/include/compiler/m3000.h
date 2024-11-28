///*vectorization function interface for M3000*/
#ifndef C6XC_H
#define C6XC_H
#define  vector                  __vector
#define  lvector                 __lvector
#define  dvector                 __dvector
#define  lvector_float16         __hfloat
/***********fan 2018-10-23  总函数 vec_x	  begin **********/
#define  vec_ld                    __builtin_vec_ld
#define  vec_st                    __builtin_vec_st
#define  vec_add                   __builtin_vec_add_3000
#define  vec_sub                   __builtin_vec_sub_3000
#define  vec_subc                  __builtin_vec_subc
#define  vec_muli                  __builtin_vec_mul_3000
#define  vec_mula                  __builtin_vec_mula_3000
#define  vec_mulb                  __builtin_vec_mulb_3000
#define  vec_eq                    __builtin_vec_eq
#define  vec_lt                    __builtin_vec_lt
#define  vec_gt                    __builtin_vec_fcmpg
#define  vec_sat                   __builtin_vec_sat
#define  vec_neg                   __builtin_vec_neg
#define  vec_abs                   __builtin_vec_abs
#define  vec_max                   __builtin_vec_max
#define  vec_min                   __builtin_vec_min
#define  vec_and                   __builtin_vec_and
#define  vec_or                    __builtin_vec_or
#define  vec_xor                   __builtin_vec_xor
#define  vec_not                   __builtin_vec_not      
#define  vec_lzd                   __builtin_vec_lzd
#define  vec_shfll                 __builtin_vec_shfll
#define  vec_shflr                 __builtin_vec_shflr
#define  vec_shfar                 __builtin_vec_shfar
#define  vec_bclr                  __builtin_vec_bclr
#define  vec_bset                  __builtin_vec_bset
#define  vec_bex                   __builtin_vec_bex
#define  vec_btst                  __builtin_vec_btst_3000
#define  vec_bext                  __builtin_vec_bext
#define  vec_fman                  __builtin_vec_fman_3000
#define  vec_flog                  __builtin_vec_flog_3000
#define  vec_frsq                  __builtin_vec_frsq_3000
#define  vec_frcp                  __builtin_vec_frcp_3000
#define  vec_fti                   __builtin_vec_fdsint_3000
#define  vec_ftru                  __builtin_vec_fdstru_3000
#define  vec_itf                   __builtin_vec_itf_3000
#define  vec_fstdl                 __builtin_vec_fstdl_3000
#define  vec_fstdh                 __builtin_vec_fstdh_3000
#define  vec_fdts                  __builtin_vec_fdts_3000
#define  vec_mov                   __builtin_vec_mov
#define  vec_movi                  __builtin_vec_movi
#define  vec_svbcast               __builtin_vec_svbcast
#define  vec_fdot32                __builtin_yhft_vfdot32sp2
#define  vec_fcreal32              __builtin_yhft_vfcreal32sp2
#define  vec_fcimag32              __builtin_yhft_vfcimag32sp2                         
#define  vec_mvcgc                 __builtin_yhft_vmvcgc
#define  vec_bale2                 __builtin_yhft_bale2
#define  vec_bale2h                __builtin_yhft_bale2h
#define  vec_bale2lh               __builtin_yhft_bale2lh
#define  vec_bale2hl               __builtin_yhft_bale2hl 
#define  vec_bale4h                __builtin_yhft_bale4h
#define  vec_bale4l                __builtin_yhft_bale4l
#define  vec_sbale2                __builtin_vec_sbale2_3000
#define  vec_sbale4                __builtin_vec_sbale4_3000
#define  vec_itl2                  __builtin_yhft_itl2
#define  vec_itl4                  __builtin_yhft_itl4                                    
#define  vec_ubale4h               __builtin_yhft_ubale4h
#define  vec_ubale4l               __builtin_yhft_ubale4l

#define  vec_fexts32l             __builtin_yhft_vfexts32l
#define  vec_fexts32h             __builtin_yhft_vfexts32h
#define  vec_fexth16ll            __builtin_yhft_vfexth16ll
#define  vec_fexth16lh            __builtin_yhft_vfexth16lh
#define  vec_fexth16hl            __builtin_yhft_vfexth16hl
#define  vec_fexth16hh            __builtin_yhft_vfexth16hh

/******************************fan2018-10-23  total   end ************************/
/***********************指令集各具体指令 vec_vxx   (和总函数区别在于具体指令前含v,总函数不含v) begin *************************/
//load
#define  vec_ldhsi                 __builtin_altivec_lvx_v16si
#define  vec_ldhsf                 __builtin_altivec_lvx_v16sf
#define  vec_ldhusi                __builtin_altivec_lvx_uv16si
#define  vec_ldwdi                 __builtin_altivec_lvx_v16di
#define  vec_ldwdf                 __builtin_altivec_lvx_v16df
#define  vec_ldwudi                __builtin_altivec_lvx_uv16di
#define  vec_lddwdi                __builtin_altivec_lvdx
#define  vec_lddwudi               __builtin_altivec_lvx_uv32di
#define  vec_lddwdf                __builtin_altivec_lvx_v32df
#define  vec_lddw2df               __builtin_altivec_lvdxv16ti
#define  vec_vldwsi                __builtin_altivec_lvx_v32si
#define  vec_vldwsf                __builtin_altivec_lvx_v32sf
#define  vec_vldwdfm2              __builtin_altivec_lvx_v32dfm2

//store                          
#define  vec_sthsi                 __builtin_altivec_stvx_v16si
#define  vec_sthsf                 __builtin_altivec_stvx_v16sf
#define  vec_sthusi                __builtin_altivec_stvx_uv16si
#define  vec_stwdi                 __builtin_altivec_stvx_v16di
#define  vec_stwdf                 __builtin_altivec_stvx_v16df
#define  vec_stwudi                __builtin_altivec_stvx_uv16di
#define  vec_stdwdi                __builtin_altivec_stvdx
#define  vec_stdwudi               __builtin_altivec_stvdx_uv32di
#define  vec_stdwdf                __builtin_altivec_stvx_v32df
#define  vec_stdw2df               __builtin_altivec_stvdxv16ti
#define  vec_vstwsi                __builtin_altivec_stvx_v32si
#define  vec_vstwusi               __builtin_altivec_stvx_uv32si
#define  vec_vstwsf                __builtin_altivec_stvx_v32sf
#define  vec_stdw                  __builtin_altivec_stvdx  
#define  vec_vstw32dfm16           __builtin_altivec_stvx_v32dfm16  
    
#define  vec_vsubc                 __builtin_yhft_vsubc
#define  vec_vsubuc32              __builtin_yhft_vsubc32                                 
//FCMP                           
#define  vec_vfcmped               __builtin_yhft_vfcmped
#define  vec_vfcmpes32             __builtin_yhft_vfcmpes32
#define  vec_vfcmpgd               __builtin_yhft_vfcmpgd
#define  vec_vfcmpgs32             __builtin_yhft_vfcmpgs32
#define  vec_vfcmpld               __builtin_yhft_vfcmpld
#define  vec_vfcmpls32             __builtin_yhft_vfcmpls32
//FID转换
#define  vec_vfdint                __builtin_yhft_vfdint_3
#define  vec_vfsint32              __builtin_yhft_vfsint32_3
#define  vec_vfdtru                __builtin_yhft_vfdtru_3
#define  vec_vfdtru32              __builtin_yhft_vfstru32_3
#define  vec_vfintd                __builtin_yhft_vfintd_3
#define  vec_vfintdu               __builtin_yhft_vfintdu_3
#define  vec_vfints32              __builtin_yhft_vfints32_3
#define  vec_vfintsu32             __builtin_yhft_vfintsu32_3
#define  vec_vfdpsp32              __builtin_yhft_vfdpsp32_3
#define  vec_vfspdp32t             __builtin_yhft_vfspdp32t_3
#define  vec_vfsphdp32t            __builtin_yhft_vfsphdp32t_3
//浮点特殊运算                   
#define  vec_vfmand                __builtin_yhft_vfmand_3
#define  vec_vfmans32              __builtin_yhft_vfmans32_3
#define  vec_vflogd                __builtin_yhft_vflogd_3
#define  vec_vflogs32              __builtin_yhft_vflogs32_3
#define  vec_vfrsqd                __builtin_yhft_vfrsqd_3
#define  vec_vfrsqs32              __builtin_yhft_vfrsqs32_3
#define  vec_vfrcpd                __builtin_yhft_vfrcpd_3
#define  vec_vfrcps32              __builtin_yhft_vfrcps32_3                                
//饱和操作                       
#define  vec_vsat                  __builtin_yhft_vsat
#define  vec_vsat32                __builtin_yhft_vsat32
//取反                           
#define  vec_vneg                  __builtin_yhft_vneg
#define  vec_vneg32                __builtin_yhft_vneg32
//取绝对值                       
#define  vec_vabs                  __builtin_yhft_vabs
#define  vec_vabs32                __builtin_yhft_vabs32
#define  vec_vfabsd                __builtin_yhft_vfabsd
#define  vec_vfabss32              __builtin_yhft_vfabss32

//取最大值                       
#define  vec_vmax                  __builtin_yhft_vmax
#define  vec_vmaxu                 __builtin_yhft_vmaxu
#define  vec_vmax32                __builtin_yhft_vmax32
#define  vec_vmaxu32               __builtin_yhft_vmaxu32
#define  vec_vmax32sfvs0           __builtin_yhft_vmax32sfvs0
#define  vec_vmaxdfvs0             __builtin_yhft_vmaxdfvs0
//取最小值                       
#define  vec_vmin                  __builtin_yhft_vmin
#define  vec_vminu                 __builtin_yhft_vminu
#define  vec_vmin32                __builtin_yhft_vmin32
#define  vec_vminu32               __builtin_yhft_vminu32
//比较
#define  vec_veq                   __builtin_yhft_veqs                           
#define  vec_vequ                  __builtin_yhft_vequ
#define  vec_veqim                 __builtin_yhft_veqim
#define  vec_veq32                 __builtin_yhft_veq32s
#define  vec_veq32u                __builtin_yhft_veq32u
#define  vec_veq32im               __builtin_yhft_veq32im
#define  vec_vlt                   __builtin_yhft_vlts
#define  vec_vltim                 __builtin_yhft_vltim
#define  vec_vltu                  __builtin_yhft_vltuu
#define  vec_vltuim                __builtin_yhft_vltuim
#define  vec_vlt32                 __builtin_yhft_vlt32s
#define  vec_vlt32im               __builtin_yhft_vlt32im
#define  vec_vltu32                __builtin_yhft_vltu32u
#define  vec_vltu32im              __builtin_yhft_vltu32im
//逻辑操作                       
#define  vec_vand                  __builtin_yhft_vand
#define  vec_vandim                __builtin_yhft_vandim
#define  vec_vor                   __builtin_yhft_vor
#define  vec_vorim                 __builtin_yhft_vorim
#define  vec_vxor                  __builtin_yhft_vxor
#define  vec_vxorim                __builtin_yhft_vxorim
#define  vec_vnot                  __builtin_yhft_vnot
#define  vec_vnotim                __builtin_yhft_vnotim
//定点ALU特殊操作                
#define  vec_vlzd                  __builtin_yhft_vlzd
#define  vec_vlzd32                __builtin_yhft_vlzd32
//逻辑左移                       
#define  vec_vshfll                __builtin_yhft_vshfll_r
#define  vec_vshfllim              __builtin_yhft_vshfll_im
#define  vec_vshfll32              __builtin_yhft_vshfll32_r
#define  vec_vshfll32im            __builtin_yhft_vshfll32_im
//逻辑右移
#define  vec_vshflr                __builtin_yhft_vshflr_r
#define  vec_vshflrim              __builtin_yhft_vshflr_im
#define  vec_vshflr32              __builtin_yhft_vshflr32_r
#define  vec_vshflr32im            __builtin_yhft_vshflr32_im
//算术右移                       
#define  vec_vshfar                __builtin_yhft_vshfar_r
#define  vec_vshfarim              __builtin_yhft_vshfar_im
#define  vec_vshfar32              __builtin_yhft_vshfar32_r
#define  vec_vshfar32im            __builtin_yhft_vshfar32_im
//位清零                         
#define  vec_vbclr                 __builtin_yhft_vbclr
#define  vec_vbclr32               __builtin_yhft_vbclr32
//位置1                          
#define  vec_vbset                 __builtin_yhft_vbset
#define  vec_vbset32               __builtin_yhft_vbset32
//位置换                         
#define  vec_vbex                  __builtin_yhft_vbex
#define  vec_vbex32                __builtin_yhft_vbex32
//位测试                         
#define  vec_vbtst                 __builtin_yhft_vbtst
#define  vec_vbtst32               __builtin_yhft_vbtst32
//位扩展                         
#define  vec_vbext32               __builtin_yhft_vbext32
#define  vec_vbext                 __builtin_yhft_vbext
#define  vec_vbext32u              __builtin_yhft_vbext32u
#define  vec_vbextu                __builtin_yhft_vbextu
//打包
#define  vec_vbale2sf2_sf          __builtin_yhft_vbale2sf2_sf
#define  vec_vbale2hsf2_sf          __builtin_yhft_vbale2hsf2_sf                           
#define  vec_vbale2                __builtin_yhft_vbale2
#define  vec_vbale2h               __builtin_yhft_vbale2h
#define  vec_vbale2lh              __builtin_yhft_vbale2lh
#define  vec_vbale2hl              __builtin_yhft_vbale2hl
#define  vec_vbale4h               __builtin_yhft_vbale4h
#define  vec_vbale4l               __builtin_yhft_vbale4l
#define  vec_vsbale2               __builtin_yhft_vsbale2
#define  vec_vsbale4               __builtin_yhft_vsbale4
#define  vec_vbale2sf32            __builtin_yhft_vbale2lf
#define  vec_bale2hdf               __builtin_yhft_bale2h64df
#define  vec_vbale2si32              __builtin_yhft_vbale2lint
#define  vec_bale2_sf              __builtin_yhft_bale2sftosf2
#define  vec_bale2_si              __builtin_yhft_bale2sitosi2
#define  vec_bale2_sfsf2           __builtin_yhft_bale2sfsf2
//解包   
#define  vec_vubale4h			         __builtin_yhft_ubale4h64
#define  vec_vubale4l			         __builtin_yhft_ubale4l64
//域内交换                       
#define  vec_vitl2                 __builtin_yhft_itl264
#define  vec_vitl4				         __builtin_yhft_itl464
//浮点乘法                       
#define  vec_vfmuls32              __builtin_yhft_vfmuls32
#define  vec_vfmuld                __builtin_yhft_vfmuld
//浮点乘加                       
#define  vec_vfmulad               __builtin_yhft_vfmulad
#define  vec_vfmulas32             __builtin_yhft_vfmulas32
//浮点乘减                       
#define  vec_vfmulbd               __builtin_yhft_vfmulbd
#define  vec_vfmulbs32             __builtin_yhft_vfmulbs32
//浮点乘后加                     
#define  vec_vfdot32               __builtin_yhft_vfdot32     
#define  vec_vfcreal32             __builtin_yhft_vfcreal32
#define  vec_vfcreal32sp2          __builtin_yhft_vfcreal32sp2
#define  vec_vfcimag32             __builtin_yhft_vfcimag32
#define  vec_vfdot32sp2            __builtin_yhft_vfdot32sp2     
#define  vec_vfcimag32sp2          __builtin_yhft_vfcimag32sp2
//转移指令                       
#define  vec_vmovudi                  __builtin_yhft_vmov
#define  vec_vmovdf                __builtin_yhft_vmovdf
#define  vec_vmovdi                __builtin_yhft_vmovdi
#define  vec_vmov32sf                __builtin_yhft_vmov32
#define  vec_vmov32si                __builtin_yhft_vmov32si
#define  vec_vmovu32si             __builtin_yhft_vmovu32si
#define  vec_vmovidi               __builtin_yhft_vmovi
#define  vec_vmoviudi              __builtin_yhft_vmoviudi
#define  vec_vmovi32si             __builtin_yhft_vmovi32si
#define  vec_vmoviu32si            __builtin_yhft_vmoviu32si
#define  vec_vmovdif32             __builtin_yhft_vmovdif32
#define  vec_vmovif32              __builtin_yhft_vmovi32sf
#define  vec_vmovidf               __builtin_yhft_vmovidf
#define  vec_vmovidfc              __builtin_yhft_vmovidfc
#define  vec_vmovidfcn             __builtin_yhft_vmovidfcn
#define  vec_vmovi32sf               __builtin_yhft_vmovi32sf
#define  vec_vmovi24               __builtin_yhft_vmovi24
#define  vec_vmvcgc                __builtin_yhft_vmvcgc

#define  vec_vmovhi         __builtin_yhft_vmovhi
#define  vec_vmovuhi         __builtin_yhft_vmovuhi
#define  vec_vmovhf         __builtin_yhft_vmovhf
#define  vec_vmovihi        __builtin_yhft_vmovihi
#define  vec_vmoviuhi       __builtin_yhft_vmoviuhi
#define  vec_vmovihf        __builtin_yhft_vmovihf


//广播指令                       
#define  vec_vsvbcast_di           __builtin_yhft_vsvbcast_di
#define  vec_vsvbcast_udi           __builtin_yhft_vsvbcast_udi
#define  vec_vsvbcast_df           __builtin_yhft_vsvbcast_df
#define  vec_vsvbcast_sf2          __builtin_yhft_vsvbcast_sf
#define  vec_vsvbcast_si2           __builtin_yhft_vsvbcast_si
#define  vec_vsvbcast_usi2          __builtin_yhft_vsvbcast_usi
#define  vec_vsvbcast_dfcn         __builtin_yhft_vsvbcast_dfcn
#define  vec_vsvbcast_sdf2         __builtin_yhft_vsvbcast_sf2  

#define  vec_vsvbcast_hi          __builtin_yhft_vsvbcast_hi
#define  vec_vsvbcast_uhi          __builtin_yhft_vsvbcast_uhi
#define  vec_vsvbcast_hf          __builtin_yhft_vsvbcast_hf
                              
#define  vec_vextw                 __builtin_yhft_vext32
#define  vec_vsextw                __builtin_yhft_vsext32                               
#define  vec_vbale2sf32            __builtin_yhft_vbale2lf
#define  vec_vbale2hsi32            __builtin_yhft_vbale2hlint
#define  vec_vbale2hf              __builtin_yhft_vbale2hf
#define  vec_vbale2si32              __builtin_yhft_vbale2lint
#define  vec_bale2_sf              __builtin_yhft_bale2sftosf2
/*move the scalar data to the VLR*/
#define  mov_to_vlr                   __builtin_mov_to_vlr
#define  mov_to_vlr_v16di    __builtin_mov_to_vlr_v16di
#define  movcgcvlr       __builtin_yhft_mov_to_vlrv
/***********************指令集各指令 vec_vxx   (和总函数区别在于具体指令前含v,总函数不含v) end*************************/
/******************************S===>V***********************************************/
/*added by jack to exchange the data betweent the scalar space and vector space*/
/*move the scalar data to the vector space*/
#define  mov_to_svr0             __builtin_mov_to_svr0 
#define  mov_to_svr1             __builtin_mov_to_svr1 
#define  mov_to_svr2             __builtin_mov_to_svr2 
#define  mov_to_svr3             __builtin_mov_to_svr3 
#define  mov_to_svr4             __builtin_mov_to_svr4 
#define  mov_to_svr5             __builtin_mov_to_svr5 
#define  mov_to_svr6             __builtin_mov_to_svr6 
#define  mov_to_svr7             __builtin_mov_to_svr7 
#define  mov_to_svr8             __builtin_mov_to_svr8 
#define  mov_to_svr9             __builtin_mov_to_svr9 
#define  mov_to_svr10            __builtin_mov_to_svr10 
#define  mov_to_svr11            __builtin_mov_to_svr11 
#define  mov_to_svr12            __builtin_mov_to_svr12 
#define  mov_to_svr13            __builtin_mov_to_svr13 
#define  mov_to_svr14            __builtin_mov_to_svr14 
#define  mov_to_svr15            __builtin_mov_to_svr15 
#define  mov_from_svr_v16di      __builtin_mov_from_svr_v16di 
#define  mov_from_svr_v16df      __builtin_mov_from_svr_v16df 
#define  mov_from_svr_v32si      __builtin_mov_from_svr_v32si 
#define  mov_from_svr_v32sf      __builtin_mov_from_svr_v32sf 

/*added by hw to exchange the data betweent the scalar space and vector space double*/
/*move the scalar data to the vector space*/
#define  mov_to_svr0_df             __builtin_mov_to_svr0_df 
#define  mov_to_svr1_df             __builtin_mov_to_svr1_df 
#define  mov_to_svr2_df             __builtin_mov_to_svr2_df 
#define  mov_to_svr3_df             __builtin_mov_to_svr3_df 
#define  mov_to_svr4_df             __builtin_mov_to_svr4_df 
#define  mov_to_svr5_df             __builtin_mov_to_svr5_df 
#define  mov_to_svr6_df             __builtin_mov_to_svr6_df 
#define  mov_to_svr7_df             __builtin_mov_to_svr7_df 
#define  mov_to_svr8_df             __builtin_mov_to_svr8_df 
#define  mov_to_svr9_df             __builtin_mov_to_svr9_df 
#define  mov_to_svr10_df            __builtin_mov_to_svr10_df 
#define  mov_to_svr11_df            __builtin_mov_to_svr11_df 
#define  mov_to_svr12_df            __builtin_mov_to_svr12_df 
#define  mov_to_svr13_df            __builtin_mov_to_svr13_df 
#define  mov_to_svr14_df            __builtin_mov_to_svr14_df 
#define  mov_to_svr15_df            __builtin_mov_to_svr15_df 

/******************************V===>S***********************************************/
/*move the vector data to the scalar space*/
#define  mov_from_svr0           __builtin_mov_from_svr0 
#define  mov_from_svr1           __builtin_mov_from_svr1 
#define  mov_from_svr2           __builtin_mov_from_svr2 
#define  mov_from_svr3           __builtin_mov_from_svr3 
#define  mov_from_svr4           __builtin_mov_from_svr4 
#define  mov_from_svr5           __builtin_mov_from_svr5 
#define  mov_from_svr6           __builtin_mov_from_svr6 
#define  mov_from_svr7           __builtin_mov_from_svr7 
#define  mov_from_svr8           __builtin_mov_from_svr8 
#define  mov_from_svr9           __builtin_mov_from_svr9 
#define  mov_from_svr10          __builtin_mov_from_svr10 
#define  mov_from_svr11          __builtin_mov_from_svr11 
#define  mov_from_svr12          __builtin_mov_from_svr12 
#define  mov_from_svr13          __builtin_mov_from_svr13 
#define  mov_from_svr14          __builtin_mov_from_svr14 
#define  mov_from_svr15          __builtin_mov_from_svr15 
#define  mov_to_svr_v16di        __builtin_mov_to_svr_v16di 
#define  mov_to_svr_v16df        __builtin_mov_to_svr_v16df 
#define  mov_to_svr_v32si        __builtin_mov_to_svr_v32si 
#define  mov_to_svr_v32sf        __builtin_mov_to_svr_v32sf 


/*add hw move the vector data to the scalar space double 20190902*/
#define  mov_from_svr0_df           __builtin_mov_from_svr0_df 
#define  mov_from_svr1_df           __builtin_mov_from_svr1_df 
#define  mov_from_svr2_df           __builtin_mov_from_svr2_df 
#define  mov_from_svr3_df           __builtin_mov_from_svr3_df 
#define  mov_from_svr4_df           __builtin_mov_from_svr4_df 
#define  mov_from_svr5_df           __builtin_mov_from_svr5_df 
#define  mov_from_svr6_df           __builtin_mov_from_svr6_df 
#define  mov_from_svr7_df           __builtin_mov_from_svr7_df 
#define  mov_from_svr8_df           __builtin_mov_from_svr8_df 
#define  mov_from_svr9_df           __builtin_mov_from_svr9_df 
#define  mov_from_svr10_df          __builtin_mov_from_svr10_df 
#define  mov_from_svr11_df          __builtin_mov_from_svr11_df 
#define  mov_from_svr12_df          __builtin_mov_from_svr12_df 
#define  mov_from_svr13_df          __builtin_mov_from_svr13_df 
#define  mov_from_svr14_df          __builtin_mov_from_svr14_df 
#define  mov_from_svr15_df          __builtin_mov_from_svr15_df 
/******************************半精度浮点指令***********************************************/
//hw M3000 20180829
#define  vec_vfhint16              __builtin_yhft_vfhint16  
#define  vec_vfcmplh16             __builtin_yhft_vfcmplh16 
#define  vec_vfcmpgh16             __builtin_yhft_vfcmpgh16 
#define  vec_vfcmpeh16             __builtin_yhft_vfcmpeh16 
#define  vec_vfinth16              __builtin_yhft_vfinth16  
#define  vec_vfinthu16             __builtin_yhft_vfinthu16 
#define  vec_vfsphp16              __builtin_yhft_vfsphp16  
#define  vec_vfhpsp16l             __builtin_yhft_vfhpsp16l 
#define  vec_vfhpsp16h             __builtin_yhft_vfhpsp16h 
#define  vec_vfmanh16              __builtin_yhft_vfmanh16  
#define  vec_vflogh16              __builtin_yhft_vflogh16  
#define  vec_vfabsh16              __builtin_yhft_vfabsh16  
#define  vec_vfexts32l             __builtin_yhft_vfexts32l 
#define  vec_vfexts32h             __builtin_yhft_vfexts32h 
#define  vec_vfexth16ll            __builtin_yhft_vfexth16ll
#define  vec_vfexth16lh            __builtin_yhft_vfexth16lh
#define  vec_vfexth16hl            __builtin_yhft_vfexth16hl
#define  vec_vfexth16hh            __builtin_yhft_vfexth16hh  
#define  vec_vfmax16               __builtin_yhft_vfmax16   
#define  vec_vfhtru16              __builtin_yhft_vfhtru16  
#define  vec_vfsubh16              __builtin_yhft_vfsubh16  
#define  vec_vfaddh16              __builtin_yhft_vfaddh16  
#define  vec_vfmulah16             __builtin_yhft_vfmulah16 
#define  vec_vfmulbh16             __builtin_yhft_vfmulbh16 
#define  vec_vfmulh16              __builtin_yhft_vfmulh16
                                             
#define  vec_lv64hf                __builtin_altivec_lvx_v64hf
#define  vec_stv64hf               __builtin_altivec_stvx_v64hf
#define  vec_luv64hi               __builtin_altivec_lvx_uv64hi
#define  vec_lv64hi                __builtin_altivec_lvx_v64hi
#define  vec_stv64hi               __builtin_altivec_stvx_v64hi
#define  vec_stuv64hi              __builtin_altivec_stvx_uv64hi
#define  vec_negdf                 __builtin_yhft_vneg_df
#define  vec_neg32sf               __builtin_yhft_vneg32sf
/******************************增加2000B+m3000***********************************************/
//hw 20190108
#define  vec_vldwusi               __builtin_altivec_lvx_uv32si
#define  vec_addim_3               __builtin_yhft_vaddim_3   
#define  vec_add_3                 __builtin_yhft_vadd_3     
#define  vec_adduim_3              __builtin_yhft_vadduim_3  
#define  vec_addu_3                __builtin_yhft_vaddu_3    
#define  vec_add32im_3             __builtin_yhft_vadd32im_3 
#define  vec_add32_3               __builtin_yhft_vadd32_3   
#define  vec_addu32im_3            __builtin_yhft_vaddu32im_3
#define  vec_addu32_3              __builtin_yhft_vaddu32_3  
                                       
#define  vec_subim_3               __builtin_yhft_vsubim_3   
#define  vec_sub_3                 __builtin_yhft_vsub_3     
#define  vec_subuim_3              __builtin_yhft_vsubuim_3  
#define  vec_subu_3                __builtin_yhft_vsubu_3    
#define  vec_sub32im_3             __builtin_yhft_vsub32im_3 
#define  vec_sub32_3               __builtin_yhft_vsub32_3   
#define  vec_subu32im_3            __builtin_yhft_vsubu32im_3
#define  vec_subu32_3              __builtin_yhft_vsubu32_3  
            
/******************************m3000condition***********************************************/
#define vec_vfhint16c           __builtin_yhft_vfhint16c  
#define vec_vfcmplh16c          __builtin_yhft_vfcmplh16c 
#define vec_vfcmpgh16c          __builtin_yhft_vfcmpgh16c 
#define vec_vfcmpeh16c          __builtin_yhft_vfcmpeh16c 
#define vec_vfinth16c           __builtin_yhft_vfinth16c  
#define vec_vfinthu16c          __builtin_yhft_vfinthu16c 
#define vec_vfsphp16c           __builtin_yhft_vfsphp16c  
#define vec_vfhpsp16lc          __builtin_yhft_vfhpsp16lc 
#define vec_vfhpsp16hc          __builtin_yhft_vfhpsp16hc 
#define vec_vfmanh16c           __builtin_yhft_vfmanh16c  
#define vec_vflogh16c           __builtin_yhft_vflogh16c  
#define vec_vfabsh16c           __builtin_yhft_vfabsh16c  
#define vec_vfexts32lc          __builtin_yhft_vfexts32lc 
#define vec_vfexts32hc          __builtin_yhft_vfexts32hc 
#define vec_vfexth16llc         __builtin_yhft_vfexth16llc
#define vec_vfexth16lhc         __builtin_yhft_vfexth16lhc
#define vec_vfexth16hlc         __builtin_yhft_vfexth16hlc
#define vec_vfexth16hhc         __builtin_yhft_vfexth16hhc
#define vec_vfiniacclc          __builtin_yhft_vfiniacclc 
#define vec_vfiniacchc          __builtin_yhft_vfiniacchc   
#define vec_vfmax16c            __builtin_yhft_vfmax16c   
#define vec_vfhtru16c           __builtin_yhft_vfhtru16c   
#define vec_vfmulah16c          __builtin_yhft_vfmulah16c 
#define vec_vfmulbh16c          __builtin_yhft_vfmulbh16c 
#define vec_vfmulh16c           __builtin_yhft_vfmulh16c

#define  vec_lv64hfc               __builtin_altivec_lvx_v64hfc
#define  vec_vfmuldc               __builtin_yhft_vfmuldc
#define  vec_vfaddh16c             __builtin_yhft_vfaddh16c
#define  vec_vfsubh16c             __builtin_yhft_vfsubh16c
#define  vec_vfmuls32c             __builtin_yhft_vfmuls32c
#define  vec_vfmuladc              __builtin_yhft_vfmuladc
#define  vec_vfmuladcc             __builtin_yhft_vfmuladcc
#define  vec_vfmulas32c            __builtin_yhft_vfmulas32c
#define  vec_vfmulbdc              __builtin_yhft_vfmulbdc
#define  vec_vfmulbs32c            __builtin_yhft_vfmulbs32c
#define  vec_vfdot32c              __builtin_yhft_vfdot32c
#define  vec_vfcreal32c            __builtin_yhft_vfcreal32c
#define  vec_vfcimag32c            __builtin_yhft_vfcimag32c
#define  vec_vmovc                 __builtin_yhft_vmovc
#define  vec_vmovic                __builtin_yhft_vmovic
#define  vec_vmovcu         __builtin_yhft_vmovcu      
#define  vec_vmovicu         __builtin_yhft_vmovicu     
#define  vec_vmovcdf         __builtin_yhft_vmovcdf     
#define  vec_vmovicdf         __builtin_yhft_vmovicdf    
#define  vec_vmovc32si         __builtin_yhft_vmovc32si   
#define  vec_vmovic32si         __builtin_yhft_vmovic32si  
#define  vec_vmovc32usi         __builtin_yhft_vmovc32usi  
#define  vec_vmovic32usi         __builtin_yhft_vmovic32usi 
#define  vec_vmovc32sf         __builtin_yhft_vmovc32sf   
#define  vec_vmovic32sf         __builtin_yhft_vmovic32sf   

#define  vec_vmovhic         __builtin_yhft_vmovhic
#define  vec_vmovuhic         __builtin_yhft_vmovuhic
#define  vec_vmovhfc         __builtin_yhft_vmovhfc
#define  vec_vmovihic        __builtin_yhft_vmovihic
#define  vec_vmoviuhic       __builtin_yhft_vmoviuhic
#define  vec_vmovihfc        __builtin_yhft_vmovihfc

#define  vec_vsvbcast_hic          __builtin_yhft_vsvbcast_hic
#define  vec_vsvbcast_uhic          __builtin_yhft_vsvbcast_uhic
#define  vec_vsvbcast_hfc          __builtin_yhft_vsvbcast_hfc
                              
#define  vec_vsvbcast_dic          __builtin_yhft_vsvbcast_dic
#define  vec_vsvbcast_udic          __builtin_yhft_vsvbcast_udic
#define  vec_vsvbcast_dfc          __builtin_yhft_vsvbcast_dfc
#define  vec_vsvbcast_sfc          __builtin_yhft_vsvbcast_sfc
#define  vec_vsvbcast_sic          __builtin_yhft_vsvbcast_sic
#define  vec_vsvbcast_usic          __builtin_yhft_vsvbcast_usic
                                
#define  vec_vfcmpedc              __builtin_yhft_vfcmpedc
#define  vec_vfcmpes32c            __builtin_yhft_vfcmpes32c
#define  vec_vfcmpgdc              __builtin_yhft_vfcmpgdc
#define  vec_vfcmpgs32c            __builtin_yhft_vfcmpgs32c
#define  vec_vfcmpldc              __builtin_yhft_vfcmpldc
#define  vec_vfcmpls32c            __builtin_yhft_vfcmpls32c
#define  vec_vfdintc               __builtin_yhft_vfdintc_3
#define  vec_vfsint32c             __builtin_yhft_vfsint32c_3
#define  vec_vfdtruc               __builtin_yhft_vfdtruc_3
#define  vec_vfstru32c             __builtin_yhft_vfstru32c_3
#define  vec_vfintdc               __builtin_yhft_vfintdc_3
#define  vec_vfintduc              __builtin_yhft_vfintduc_3
#define  vec_vfints32c             __builtin_yhft_vfints32c_3
#define  vec_vfintsu32c            __builtin_yhft_vfintsu32c_3
#define  vec_vfdpsp32c             __builtin_yhft_vfdpsp32c_3
#define  vec_vfspdp32tc            __builtin_yhft_vfspdp32tc_3
#define  vec_vfsphdp32tc           __builtin_yhft_vfsphdp32tc_3
#define  vec_vfmandc               __builtin_yhft_vfmandc_3
#define  vec_vfmans32c             __builtin_yhft_vfmans32c_3
#define  vec_vflogdc               __builtin_yhft_vflogdc_3
#define  vec_vflogs32c             __builtin_yhft_vflogs32c_3
#define  vec_vfabsdc               __builtin_yhft_vfabsdc
#define  vec_vfabss32c             __builtin_yhft_vfabss32c
#define  vec_vfrsqdc               __builtin_yhft_vfrsqdc_3
#define  vec_vfrsqs32c             __builtin_yhft_vfrsqs32c_3
#define  vec_vfrcpdc               __builtin_yhft_vfrcpdc_3
#define  vec_vfrcps32c             __builtin_yhft_vfrcps32c_3
                               
#define  vec_vaddc                 __builtin_yhft_vaddc
#define  vec_vaddimc               __builtin_yhft_vaddimc
#define  vec_vadduc                __builtin_yhft_vadduc
#define  vec_vadduimc              __builtin_yhft_vadduimc
                               
#define  vec_vadd32c               __builtin_yhft_vadd32c
#define  vec_vadd32imc             __builtin_yhft_vadd32imc
#define  vec_vaddu32c              __builtin_yhft_vaddu32c
#define  vec_vaddu32imc            __builtin_yhft_vaddu32imc
                               
#define  vec_vsubdiimc             __builtin_yhft_vsubdiimc
#define  vec_vsubdic               __builtin_yhft_vsubdic
#define  vec_vsubuimc              __builtin_yhft_vsubuimc
#define  vec_vsubuc                __builtin_yhft_vsubuc
#define  vec_vsubcc                __builtin_yhft_vsubcc
#define  vec_vsub32imc             __builtin_yhft_vsub32imc
#define  vec_vsub32c               __builtin_yhft_vsub32c
#define  vec_vsub32uimc            __builtin_yhft_vsub32uimc
#define  vec_vsub32uc              __builtin_yhft_vsub32uc
#define  vec_vsubc32c              __builtin_yhft_vsubc32c
#define  vec_vsatc                 __builtin_yhft_vsatc
#define  vec_vsat32c               __builtin_yhft_vsat32c
#define  vec_vnegc                 __builtin_yhft_vnegc
#define  vec_vneg32c               __builtin_yhft_vneg32c
#define  vec_vabsc                 __builtin_yhft_vabsc
#define  vec_vabs32c               __builtin_yhft_vabs32c
#define  vec_vmaxc                 __builtin_yhft_vmaxc
#define  vec_vmax32c               __builtin_yhft_vmax32c
#define  vec_vmaxuc                __builtin_yhft_vmaxuc
#define  vec_vmaxu32c              __builtin_yhft_vmaxu32c
#define  vec_vminc                 __builtin_yhft_vminc
#define  vec_vmin32c               __builtin_yhft_vmin32c
#define  vec_vminuc                __builtin_yhft_vminuc
#define  vec_vminu32c              __builtin_yhft_vminu32c
                               
#define  vec_veqc                  __builtin_yhft_veqc
#define  vec_veqimc                __builtin_yhft_veqimc
#define  vec_vequc                 __builtin_yhft_vequc
#define  vec_veq32c                __builtin_yhft_veq32c
#define  vec_veq32uc                __builtin_yhft_veq32uc
#define  vec_veq32imc              __builtin_yhft_veq32imc
#define  vec_vltc                  __builtin_yhft_vltc
#define  vec_vltimc                __builtin_yhft_vltimc
#define  vec_vltuc                 __builtin_yhft_vltuc
#define  vec_vltuimc               __builtin_yhft_vltuimc    
#define  vec_vlt32c                __builtin_yhft_vlt32c
#define  vec_vlt32imc              __builtin_yhft_vlt32imc
#define  vec_vltu32c               __builtin_yhft_vltu32c
#define  vec_vltu32imc             __builtin_yhft_vltu32imc
#define  vec_vmovudic              __builtin_yhft_vmovudic
#define  vec_vmoviudic              __builtin_yhft_vmoviudic
#define  vec_vmovi24c              __builtin_yhft_vmovi24c

#define  vec_vandc                 __builtin_yhft_vandc
#define  vec_vandimc               __builtin_yhft_vandimc
#define  vec_vorc                  __builtin_yhft_vorc
#define  vec_vorimc                __builtin_yhft_vorimc
#define  vec_vxorc                 __builtin_yhft_vxorc
#define  vec_vxorimc               __builtin_yhft_vxorimc
#define  vec_vnotc                 __builtin_yhft_vnotc
#define  vec_vnotimc               __builtin_yhft_vnotimc
#define  vec_vlzdc                 __builtin_yhft_vlzdc
#define  vec_vlzd32c               __builtin_yhft_vlzd32c

#define  vec_vshfll32imc           __builtin_yhft_vshfll32imc      
#define  vec_vshfll32c             __builtin_yhft_vshfll32c         
#define  vec_vshfllimc             __builtin_yhft_vshfllimc         
#define  vec_vshfllc               __builtin_yhft_vshfllc           
#define  vec_vshflr32imc           __builtin_yhft_vshflr32imc       
#define  vec_vshflr32c             __builtin_yhft_vshflr32c         
#define  vec_vshflrimc             __builtin_yhft_vshflrimc         
#define  vec_vshflrc               __builtin_yhft_vshflrc           
#define  vec_vshfar32imc           __builtin_yhft_vshfar32imc       
#define  vec_vshfar32c             __builtin_yhft_vshfar32c         
#define  vec_vshfarc               __builtin_yhft_vshfarc           
#define  vec_vshfarimc             __builtin_yhft_vshfarimc         
#define  vec_vbclr32c              __builtin_yhft_vbclr32c          
#define  vec_vbclrc                __builtin_yhft_vbclrc            
#define  vec_vbset32c              __builtin_yhft_vbset32c          
#define  vec_vbsetc                __builtin_yhft_vbsetc            
#define  vec_vbex32c               __builtin_yhft_vbex32c           
#define  vec_vbexc                 __builtin_yhft_vbexc             
#define  vec_vbtst32c              __builtin_yhft_vbtst32c          
#define  vec_vbtstc                __builtin_yhft_vbtstc            
#define  vec_vbext32c              __builtin_yhft_vbext32c          
#define  vec_vbextc                __builtin_yhft_vbextc            
#define  vec_vbext32uc             __builtin_yhft_vbext32uc         
#define  vec_vbextuc               __builtin_yhft_vbextuc           
#define  vec_vbale2c               __builtin_yhft_vbale2c           
      
#define  vec_vbale2hc              __builtin_yhft_vbale2hc          
#define  vec_vbale2lhc             __builtin_yhft_vbale2lhc         
#define  vec_vbale2hlc             __builtin_yhft_vbale2hlc         
#define  vec_vbale4lc              __builtin_yhft_vbale4lc          
#define  vec_vbale4hc              __builtin_yhft_vbale4hc          
#define  vec_vsbale2c              __builtin_yhft_vsbale2c          
#define  vec_vsbale4c              __builtin_yhft_vsbale4c          
#define  vec_vubale4hc             __builtin_yhft_vubale4hc         
#define  vec_vubale4lc             __builtin_yhft_vubale4lc         
#define  vec_vitl2c                __builtin_yhft_vitl2c            
#define  vec_vitl4c                __builtin_yhft_vitl4c    

//#define  vec_ldhsic                __builtin_yhft_lvxv16sic
#define  vec_ldhvsic               __builtin_altivec_lvx_v16sic
//#define  vec_ldhsic2               __builtin_altivec_lvx_v16sic2
//#define  vec_sthsic                __builtin_yhft_stvxv16sic
#define  vec_sth_sic               __builtin_altivec_stvx_v16sic 
//#define  vec_sth_si1c              __builtin_altivec_stvx_v16si1c     
#define  vec_lv16dic               __builtin_altivec_lvc_v16dic
#define  vec_stv16dic              __builtin_altivec_stvc_v16dic
 /*********************m3000 simd********************/    
#define  sfmuls32           __builtin_yhft_sfmuls32
#define  sfmulas32          __builtin_yhft_sfmulas32
#define  sadd32             __builtin_yhft_sadd32
#define  sfmulbs32          __builtin_yhft_sfmulbs32

#define  slv2si             __builtin_altivec_lvx_v2si
#define  slv2sf             __builtin_altivec_lvx_v2sf
#define  stv2si             __builtin_altivec_stvx_v2si
#define  stv2sf             __builtin_altivec_stvx_v2sf
#define  st32sf             __builtin_altivec_stvx_v32sfs
#define  lv32sf             __builtin_altivec_lvx_v32sfs
#define  sluv2si            __builtin_altivec_lvx_uv2si
#define  stuv2si            __builtin_altivec_stvx_uv2si
#define  sfdot32            __builtin_yhft_sfdot32
#define  sfcreal32          __builtin_yhft_sfcreal32
#define  sfcimag32          __builtin_yhft_sfcimag32
#define  sadd32im           __builtin_yhft_sadd32im

#define  saddu32            __builtin_yhft_saddu32
#define  saddu32im          __builtin_yhft_saddu32im

#define  sub32              __builtin_yhft_sub32
#define  sub32im            __builtin_yhft_sub32im
#define  subu32             __builtin_yhft_subu32
#define  subu32im           __builtin_yhft_subu32im

#define  smulis32           __builtin_yhft_smulis32
#define  smulis32im         __builtin_yhft_smulis32im
#define  smuliu32           __builtin_yhft_smuliu32
#define  smuliu32im         __builtin_yhft_smuliu32im
#define  smulisu32          __builtin_yhft_smulisu32
#define  smulisu32im        __builtin_yhft_smulisu32im

#define  sfcmpes32          __builtin_yhft_sfcmpes32
#define  sfcmpgs32          __builtin_yhft_sfcmpgs32
#define  sfcmpls32          __builtin_yhft_sfcmpls32

#define  smulaus32t         __builtin_yhft_smulaus32t
#define  smulaus32tim       __builtin_yhft_smulaus32tim
#define  smulasu32t         __builtin_yhft_smulasu32t
#define  smulasu32tim       __builtin_yhft_smulasu32tim
#define  smulau32t          __builtin_yhft_smulau32t
#define  smulau32tim        __builtin_yhft_smulau32tim
#define  smulas32t          __builtin_yhft_smulas32t
#define  smulas32tim        __builtin_yhft_smulas32tim

#define  smulbus32t         __builtin_yhft_smulbus32t
#define  smulbus32tim       __builtin_yhft_smulbus32tim
#define  smulbsu32t         __builtin_yhft_smulbsu32t
#define  smulbsu32tim       __builtin_yhft_smulbsu32tim
#define  smulbu32t          __builtin_yhft_smulbu32t
#define  smulbu32tim        __builtin_yhft_smulbu32tim
#define  smulbs32t          __builtin_yhft_smulbs32t
#define  smulbs32tim        __builtin_yhft_smulbs32tim


#define  sfsint32	         __builtin_yhft_sfsint32         
#define  sfstru32	         __builtin_yhft_sfstru32         
#define  sfints32	         __builtin_yhft_sfints32         
#define  sfintsu32 	       __builtin_yhft_sfintsu32  
#define  sfdpsp32           __builtin_yhft_sfdpsp32     
#define  sfspdp32t	         __builtin_yhft_sfspdp32t       
#define  sfsphdp32t	       __builtin_yhft_sfsphdp32t 

#define  sfmans32           __builtin_yhft_sfmans32
#define  sflogs32           __builtin_yhft_sflogs32
#define  sfabss32           __builtin_yhft_sfabss32
#define  sfrsqs32           __builtin_yhft_sfrsqs32
#define  sfrcps32           __builtin_yhft_sfrcps32

#define  sfadds32           __builtin_yhft_sfadds32
#define  sfsubs32           __builtin_yhft_sfsubs32
#define  subc32             __builtin_yhft_subc32

#define  ssat32             __builtin_yhft_ssat32                                 
#define  sneg32             __builtin_yhft_sneg32                                 
#define  sabs32             __builtin_yhft_sabs32                                 
#define  smax32             __builtin_yhft_smax32                                 
#define  smaxu32            __builtin_yhft_smaxu32                                
#define  smin32             __builtin_yhft_smin32                                 
#define  sminu32            __builtin_yhft_sminu32                                
#define  seq32              __builtin_yhft_seq32                                  
#define  seq32im            __builtin_yhft_seq32im                                
#define  slt32              __builtin_yhft_slt32                                  
#define  slt32im            __builtin_yhft_slt32im                                
#define  sltu32             __builtin_yhft_sltu32                                 
#define  sltu32im           __builtin_yhft_sltu32im                               
#define  slzd32             __builtin_yhft_slzd32  

#define  sshfll32           __builtin_yhft_sshfll32   
#define  sshfll32im         __builtin_yhft_sshfll32im 
#define  sshflr32           __builtin_yhft_sshflr32   
#define  sshflr32im         __builtin_yhft_sshflr32im 
#define  sshfar32           __builtin_yhft_sshfar32   
#define  sshfar32im         __builtin_yhft_sshfar32im 
#define  sbclr32            __builtin_yhft_sbclr32    
#define  sbset32            __builtin_yhft_sbset32    
#define  sbex32             __builtin_yhft_sbex32     
#define  sbtst32            __builtin_yhft_sbtst32    
#define  sbext32            __builtin_yhft_sbext32    
#define  sbext32u           __builtin_yhft_sbext32u   
#define  smovi2si            __builtin_yhft_smovi2si
/******************************半精度浮点指令***********************************************/
//hw M3000 20180416
#define  sfhint16           __builtin_yhft_sfhint16  
#define  sfcmplh16          __builtin_yhft_sfcmplh16 
#define  sfcmpgh16          __builtin_yhft_sfcmpgh16 
#define  sfcmpeh16          __builtin_yhft_sfcmpeh16 
#define  sfinth16           __builtin_yhft_sfinth16  
#define  sfhtru16           __builtin_yhft_sfhtru16
#define  sfinth16           __builtin_yhft_sfinth16
#define  sfinthu16          __builtin_yhft_sfinthu16 
#define  sfsphp16           __builtin_yhft_sfsphp16  
#define  sfhpsp16l          __builtin_yhft_sfhpsp16l 
#define  sfhpsp16h          __builtin_yhft_sfhpsp16h 
#define  sfmanh16           __builtin_yhft_sfmanh16  
#define  sflogh16           __builtin_yhft_sflogh16  
#define  sfabsh16           __builtin_yhft_sfabsh16  
#define  sfexts32l          __builtin_yhft_sfexts32l 
#define  sfexts32lsf          __builtin_yhft_sfexts32lfloat 
#define  sfexts32h          __builtin_yhft_sfexts32h 
#define  sfexth16ll         __builtin_yhft_sfexth16ll
#define  sfexth16lh         __builtin_yhft_sfexth16lh
#define  sfexth16hl         __builtin_yhft_sfexth16hl
#define  sfexth16hh         __builtin_yhft_sfexth16hh
#define  sfiniaccl          __builtin_yhft_sfiniaccl 
#define  sfiniacch          __builtin_yhft_sfiniacch 
#define  sfvmacc            __builtin_yhft_sfvmacc   
#define  sfvmaccw           __builtin_yhft_sfmaccw   
#define  sfmax16            __builtin_yhft_sfmax16   
#define  sfhtru16           __builtin_yhft_sfhtru16  
#define  sfsubh16           __builtin_yhft_sfsubh16  
#define  sfaddh16           __builtin_yhft_sfaddh16  
#define  sfmulah16          __builtin_yhft_sfmulah16 
#define  sfmulbh16          __builtin_yhft_sfmulbh16 
#define  sfmulh16           __builtin_yhft_sfmulh16
                                                                           
#define  lv4hf              __builtin_altivec_lvx_v4hf
#define  stv4hf             __builtin_altivec_stvx_v4hf
#define  luv4hi             __builtin_altivec_lvx_uv4hi
#define  lv4hi              __builtin_altivec_lvx_v4hi
#define  stv4hi             __builtin_altivec_stvx_v4hi
#define  stuv4hi            __builtin_altivec_stvx_uv4hi

/********************m3000 vec***************************/


#endif // C6XC_H# 
