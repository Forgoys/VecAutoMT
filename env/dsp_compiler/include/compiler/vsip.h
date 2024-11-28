#include "m3000.h"
void vmemcpy(void *dst,  void *src,   int n);
void *vmalloc(int size);
void vfree(void *packet);
void DMA_transfer(void *src, void *dst,  int n);

void vsip_vadd_f_v (
     vector float* ,
     vector float* ,
     vector float* ,
     int n );  
     
void vsip_vadd_d_v (
     lvector double* src1,
     lvector double* src2,
     lvector double* dst,
     int  n) ;
     
 void vsip_vadd_i_v (
     vector signed int* src1,
     vector signed int* src2,
     vector signed int* dst,
     int  n);   
     
void vsip_vadd_vi_v (
     vector unsigned int* src1,
     vector unsigned int* src2,
     vector unsigned int* dst,
     int  n); 
     
void vsip_svadd_d_v (
       double  src1,
     __lvector double* src2,
     __lvector double* dst,
       int  n ); 
       
void vsip_svadd_f_v (
       float  src1,
       vector float* src2,
       vector float* dst,
       int  n ); 


void vsip_cvadd_f_v (
   vector float *src1,
   vector float *src2,
   vector float *dst,
   unsigned long int n);
   
void vsip_cvadd_d_v (
   lvector double *src1,
   lvector double *src2,
   lvector double *dst,
   unsigned long int n);        
            
void vsip_vcopy_i_f_v (
       vector signed int* src1,
       vector float* dst,
       int  n );
       
void vsip_vcopy_i_d_v (
       vector signed int* src1,
       lvector double* dst,
       int  n );
       

       
void vsip_vcopy_f_i_v (
       vector float* src1,
       vector signed int* dst,
       unsigned long int  n ); 
       
void vsip_vcopy_d_i_v (
       lvector double* src1,
       vector signed int* dst,
       int  n );      

void vsip_csvadd_f_v (
     float src1_r,
     float src1_i,
     vector float *src2,
     vector float *dst,
     unsigned long int n) ;
   
 void vsip_csvsub_f_v (
    float src1_r,
     float src1_i,
    vector float *src2,
    vector float *dst,
    unsigned long int n) ;    

void vsip_cvdiv_f_v (
   lvector float *src1,
   lvector float *src2,
   lvector float *dst,
   unsigned long int n);
   
void vsip_cvdiv_d_v (
   lvector double *src1_r,
   lvector double *src1_i,
   lvector double *src2_r,
   lvector double *src2_i,
   lvector double *dst_r,
   lvector double *dst_i,
   unsigned long int n);
   
void (vsip_cvma_f_v)(
  lvector float *src1,
  lvector float *src2,
  lvector float *src3,
  lvector float *dst,
  unsigned long int n);
  
double  vsip_cvmeansqval_d_v(
lvector double *src1,
int n);

float vsip_cvmeansqval_f_v (
lvector float  *src,
unsigned long int n ); 

double  vsip_cvmeanval_d_v(
lvector double *src1,
double *retval_r,
double *retval_i,
int n);

float  vsip_cvmeanval_f_v(
lvector float *src1,
float *retval_r,
float *retval_i,
int n);
 
double  vsip_cvsumval_d_v(
lvector double *src1,
double *retval_r,
double *retval_i,
int n);

float  vsip_cvsumval_f_v(
lvector float *src1,
float *retval_r,
float *retval_i,
int n);

void (vsip_cvmsa_f_v)(
  lvector float *src1,
  lvector float *src2,
  float src3_r,
  float src3_i,
  lvector float *dst,
  unsigned long int n );

void (vsip_cvsma_f_v)(
  lvector float *src1,
  float src2_r,
  float src2_i,
  lvector float *src3,
  lvector float *dst,
  unsigned long int n ); 

void (vsip_cvsam_f_v)(
  lvector float *src1,
  float src2_r,
  float src2_i,
  lvector float *src3,
  lvector float *dst,
  unsigned long int n );
  
void (vsip_cvsbm_f_v)(
  lvector float *src1,
  lvector float *src2,
  lvector float *src3,
  lvector float *dst,
  unsigned long int n );

void (vsip_cvam_f_v)(
  lvector float *src1,
  lvector float *src2,
  lvector float *src3,
  lvector float *dst,
  unsigned long int n );
       
void vsip_cvconj_f_v(
  lvector float* src,
  lvector float* dst,
  unsigned long int n );
  
void vsip_cvconj_d_v(
  lvector double* src,
  lvector double* dst,
  unsigned long int n ) ; 
  
void vsip_cvkron_d_v(
  double alpha_r,
  double alpha_i,
  lvector double *src1_r,
  lvector double *src1_i,
  double *src2_r,
  double *src2_i, 
  lvector double* dst_r,
  lvector double* dst_i,
  unsigned long int m,    
  unsigned long int n ); 
  
void vsip_cvkron_f_v(
  float alpha_r,
  float alpha_i,
  lvector float *src1,
  float *src2,
  lvector float* dst,
  unsigned long int m,    
  unsigned long int n );
 
  
void vsip_cvjmul_f_v(
  lvector float* src1,
  lvector float* src2,
  lvector float* dst,
  unsigned long int n ) ;
  
void vsip_cvmsb_f_v (
  lvector float *src1,
  lvector float *src2,
  lvector float *src3,
  lvector float *dst,
  unsigned long int n );  
  

void vsip_vkron_f_v(
  float alpha,
  lvector float *src1,
  float *src2,
  lvector float* dst,
  unsigned long int m,    
  unsigned long int n ) ;
  
void vsip_vkron_d_v(
  double alpha,
  lvector double *src1,
  double *src2,
  lvector double* dst,
  unsigned long int m,    
  unsigned long int n ); 

void vsip_vleq_d_v (
  lvector double *src1,
  lvector double *src2,
  lvector signed long int *dst,
  unsigned long int  n );
  
void vsip_vleq_f_v (
  lvector float *src1,
  lvector float *src2,
  lvector signed int *dst,
  unsigned long int  n );  

void vsip_vleq_i_v (
  lvector signed int *src1,
  lvector signed int *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlge_d_v (
  lvector double  *src1,
  lvector double  *src2,
  lvector signed long int *dst,
  unsigned long int  n );
  
void vsip_vlge_f_v (
  lvector float  *src1,
  lvector float  *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlge_i_v (
  lvector signed int *src1,
  lvector signed int *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlgt_d_v (
  lvector double  *src1,
  lvector double  *src2,
  lvector signed long int *dst,
  unsigned long int  n );
  
void vsip_vlgt_f_v (
  lvector float  *src1,
  lvector float  *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlgt_i_v (
  lvector signed int *src1,
  lvector signed int *src2,
  lvector signed int *dst,
  unsigned long int  n );  


void vsip_vlle_d_v (
  lvector double  *src1,
  lvector double  *src2,
  lvector signed long int *dst,
  unsigned long int  n );

void vsip_vlle_f_v (
  lvector float  *src1,
  lvector float  *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlle_i_v (
  lvector signed int *src1,
  lvector signed int *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vllt_d_v (
  lvector double  *src1,
  lvector double  *src2,
  lvector signed long int *dst,
  unsigned long int  n );
  
void vsip_vllt_f_v (
  lvector float  *src1,
  lvector float  *src2,
  lvector signed  int *dst,
  unsigned long int  n );
  
void vsip_vllt_i_v (
  lvector signed int *src1,
  lvector signed int *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlne_d_v (
  lvector double *src1,
  lvector double *src2,
  lvector signed long int *dst,
  unsigned long int  n );
  
void vsip_vlne_f_v (
  lvector float *src1,
  lvector float *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void vsip_vlne_i_v (
  lvector signed int *src1,
  lvector signed int *src2,
  lvector signed int *dst,
  unsigned long int  n );
  
void (vsip_csvdiv_f_v)(
  float src1_r,
  float src1_i,
  lvector float *src2,
  lvector float *dst,
  unsigned long int n);

void (vsip_crvdiv_f_v)(
  lvector float *src1,
  vector float *src2,
  lvector float *dst,
  unsigned long int n);

void (vsip_rscvdiv_f_v)(
          float  src1,
  lvector float *src2,
  lvector float *dst,
  unsigned long int n);
  
void vsip_vcmaxmgsq_f_v (
  lvector float  *src1,
  lvector float  *src2,
  vector float  *dst,
  unsigned long int n );
  
void vsip_vcminmgsq_f_v (
  lvector float  *src1,
  lvector float  *src2,
  vector float  *dst,
  unsigned long int n ); 


void vsip_vcmagsq_f_v (
     lvector float  *src,
     vector float  *dst,
     unsigned long int n );


void (vsip_vand_i_v)(
		lvector unsigned long int *src1,
    lvector unsigned long int *src2,
		lvector unsigned long int *dst,
		unsigned long int n);


void (vsip_vnot_i_v)(
		lvector unsigned long int *src,
		lvector unsigned long int *dst,
		unsigned long int n) ;

void (vsip_vor_i_v)(
		lvector unsigned long int *src1,
    lvector unsigned long int *src2,
		lvector unsigned long int *dst,
		unsigned long int n) ;
		
void vsip_vouter_f_v(
  float alpha,
  float *src1,   
  lvector float* src2,
  lvector float* dst,
  unsigned long int n,    
  unsigned long int m ); 
  
void vsip_vouter_d_v(
  double alpha,
  double *src1,   
  lvector double* src2,
  lvector double* dst,
  unsigned long int n,    
  unsigned long int m ); 
		
double  vsip_vsumval_d_v(
lvector double *src1,
int n);

float  vsip_vsumval_f_v(
lvector float *src1,
int n);

signed int  vsip_vsumval_i_v(
lvector signed int *src1,
int n);

		
double  vsip_vsumsqval_d_v(
lvector double *src1,
int n);

float  vsip_vsumsqval_f_v(
lvector float *src1,
int n);


void (vsip_vxor_i_v)(
		lvector unsigned long int *src1,
    lvector unsigned long int *src2,
		lvector unsigned long int *dst,
		unsigned long int n) ;

void (vsip_vmaxmg_d_v)(
  lvector double  *src1,
  lvector double  *src2,
  lvector double  *dst,
  unsigned long int n);

void (vsip_vmaxmg_f_v)(
  lvector float  *src1,
  lvector float  *src2,
  lvector float  *dst,
   unsigned long int n);
double  vsip_vmeanval_d_v(
lvector double *src1,
int n);

float  vsip_vmeanval_f_v(
lvector float *src1,
int n);
   
   
double  vsip_vmeansqval_d_v(
lvector double *src1,
int n);

float  vsip_vmeansqval_f_v(
lvector float *src1,
int n);   

void (vsip_vminmg_f_v)(
  lvector float  *src1,
  lvector float  *src2,
  lvector float  *dst,
   unsigned long int n);

void (vsip_vminmg_d_v)(
  lvector double  *src1,
  lvector double  *src2,
  lvector double  *dst,
  unsigned long int n);

float (vsip_vdot_f_v)(
		lvector float *src1,
    lvector float *src2,
		unsigned long int n);

double (vsip_vdot_d_v)(
		lvector double *src1,
    lvector double *src2,            
		unsigned long int n);

void (vsip_vexpoavg_f_v)(
  float a,
  lvector float*src1,
  lvector float*src2,
  lvector float*dst,
  unsigned long int n);

void (vsip_cvexpoavg_d_v)(
  double  a,
  lvector double *src1,
  lvector double *src2,
  lvector double *dst,
  unsigned long int n);

void (vsip_cvexpoavg_f_v)(
  float a,
  lvector float *src1,
  lvector float *src2,
  lvector float *dst,
  unsigned long int n);

void (vsip_cvfill_f_v)(
	float srcr,
        float srci,
	lvector float *dst,
        unsigned long int n);

void (vsip_rcvdiv_f_v)(
  vector float *src1,
  lvector float *src2,
  lvector float *dst,
  unsigned long int n);

void (vsip_vexpoavg_d_v)(
  double  a,
  lvector double *src1,
  lvector double *src2,
  lvector double *dst,
  unsigned long int n);

void (vsip_vfill_d_v)(
	double src,
	lvector double *dst,
    unsigned long int n);
    
void vsip_vhypot_d_v(
lvector double *src1,
lvector double *src2,
lvector double *dst,
int n);

void vsip_vhypot_f_v(
lvector float *src1,
lvector float *src2,
lvector float *dst,
int n);



void (vsip_vfill_f_v)(
	float src,
	lvector float *dst,
    unsigned long int n);

void (vsip_vfill_i_v)(
		int src1,
		lvector signed int *dst,
		unsigned long int n) ;

float  (vsip_cvdot_f_v)(
		lvector float *src1,
    lvector float *src2, 
    float   *cvdot_r,
    float   *cvdot_i,            
		unsigned long int n) ;



float  (vsip_cvjdot_f_v)(
		lvector float *src1,
    lvector float *src2,
    float   *cvjdot_v_r,
    float   *cvjdot_v_i,              
		unsigned long int n) ;

void vsip_cvmul_d_v(
lvector double *src1_r,
lvector double *src1_i,
lvector double *src2_r,
lvector double *src2_i,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_cvmul_f_v(
lvector float *src1,
lvector float *src2,
lvector float *dst,
int n);


 void vsip_csvmul_d_v( 
double src1_r,
double src1_i,
lvector double *src2_r,
lvector double *src2_i,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_csvmul_f_v(
float src1_r,
float src1_i,
lvector float *src2,
lvector float *dst,
int n);

void vsip_rcvmul_d_v( 
lvector double *src1,
lvector double *src2_r,
lvector double *src2_i,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_crvdiv_d_v( 
lvector double *src1_r, 
lvector double *src1_i,
lvector double *src2,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_csvdiv_d_v(
double src1_r,
double src1_i,
lvector double *src2_r,
lvector double *src2_i,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_rcvdiv_d_v( 
lvector double *src1, 
lvector double *src2_r,
lvector double *src2_i,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_rscvdiv_d_v( 
        double src1, 
lvector double *src2_r,
lvector double *src2_i,
lvector double *dst_r,
lvector double *dst_i,
int n);

void vsip_rcvmul_f_v(
vector float *src1,
lvector float *src2,
lvector float *dst,
int n);
 
 
double  (vsip_cvdot_d_v)(
		lvector double *src1_r,
    lvector double *src1_i, 
    lvector double *src2_r,
    lvector double *src2_i, 
    double   *cvdot_r,
    double   *cvdot_i,            
		unsigned long int n) ;
