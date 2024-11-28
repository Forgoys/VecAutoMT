//By PengZhang(NUDT,zhangpeng13a@nudt.edu.cn) 
// 2020.07.27
//
// 
#ifndef HTHREAD_HOST_H_
#define HTHREAD_HOST_H_

#include "hthread_host_macro.h"

#ifdef __cplusplus
extern "C"
{
#endif
  int hthread_dev_open(int cluster_id);
  int hthread_dev_owner(int cluster_id);
  int hthread_dev_close(int cluster_id);
  int hthread_dat_load(int cluster_id, const char *file_path);
  int hthread_dat_unload(int cluster_id);
  const char * hthread_dat_query(int cluster_id);
  
  int hthread_group_create(int cluster_id, int num,
  	const char *func_name, int scalar_args,
  	int ptr_args, uint64_t *arg_array);
  int hthread_group_create(int cluster_id, int num);
  int hthread_group_masked_create(int cluster_id, 
  	unsigned int pmask, const char *func_name, 
  	int scalar_args, int ptr_args, uint64_t *arg_array);
  int hthread_group_exec(int thread_id, const char *func_name, int scalar_args, int ptr_args, uint64_t *arg_array);
  int hthread_group_get_status(int thread_id);
  int hthread_group_wait(int thread_id);
  int hthread_group_destroy(int thread_id);
  int hthread_get_avail_threads(int cluster_id);
  
  void *hthread_malloc(int cluster_id, unsigned long bytes, int mode);
  void hthread_free(void *ptr);
  
  int hthread_barrier_create(int cluster_id);
  void hthread_barrier_destroy(int b_id);
  int hthread_rwlock_create(int cluster_id);
  void hthread_rwlock_destroy(int lock_id);

  //intr_id: 16-59
  //User must register intr handler with intr_handler_register APIs on DSP.
  void hthread_intr_send(int g_id, int t_id, unsigned long intr_id);
  unsigned long hthread_handler_register(int thread_id, void (*func)(int id, unsigned long));


//  void *hthread_gsm_malloc(int cluster_id, unsigned long bytes);
//  void hthread_gsm_free(int cluster_id, void *ptr);

//TODO. Support group to group, async/sync send/recv. Treat CPU as a group.
//  int hthread_send(int tag, int num, void *ptr, unsigned long bytes);
//  int hthread_recv(int tag, void *ptr, unsigned long bytes);
//  int hthread_send_async(int tag, int num, void *ptr, unsigned long bytes);
//  int hthread_recv_async(int tag, void *ptr, unsigned long bytes);
//  void hthread_msg_wait(int tag);
//  int hthread_msg_test(int tag);	//Return 0,means the msg ch is Idle, 1 means Busy. 
#ifdef __cplusplus
}
#endif
#endif


