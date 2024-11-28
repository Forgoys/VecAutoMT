#ifndef HTHREAD_HOST_H_
#define HTHREAD_HOST_H_


#define HT_STAT_IDLE 1
#define HT_STAT_BUSY 0
#define HT_STAT_FAIL 2

#define HT_SUCCESS 0
#define HT_FAIL_UNKNOWN -1
#define HT_OUT_OF_RANGE -2
#define HT_NOT_INITIALIZED -3
#define HT_DEV_OPEN_FAILED -4
#define HT_PROGRAM_UNLOADED -5
#define HT_TIMEOUT -6
#define HT_LAST_KERNEL_EXCP -7

#define HT_MEM_RO 	1
#define HT_MEM_WO	2
#define HT_MEM_RW 	3
#define HT_MEM_CACHE 	4


interface
  function hthread_dev_open(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_dev_open
    integer(c_int),value :: cluster_id
  end function hthread_dev_open

  function hthread_dev_owner(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_dev_owner
    integer(c_int),value :: cluster_id
  end function hthread_dev_owner

  function hthread_dev_close(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_dev_close
    integer(c_int),value :: cluster_id
  end function hthread_dev_close
  
  function hthread_dat_load(cluster_id, file_path) BIND(C)
    use iso_c_binding, only: c_int,c_char
    integer(c_int) :: hthread_dat_load
    integer(c_int),value :: cluster_id
    character(c_char) :: file_path
  end function hthread_dat_load

  function hthread_dat_unload(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_dat_unload
    integer(c_int),value :: cluster_id
  end function hthread_dat_unload

  function hthread_dat_query(cluster_id,) BIND(C)
    use iso_c_binding, only: c_int,
    type(c_ptr) :: hthread_dat_query
    integer(c_int),value :: cluster_id
  end function hthread_dat_query

  function hthread_group_masked_create(cluster_id, pmask, func_name, sargs, pargs, args_array) BIND(C)
    use iso_c_binding, only: c_int,c_char,c_int64_t
    integer(c_int) :: hthread_group_masked_create
    integer(c_int),value :: cluster_id
    integer(c_int),value :: pmask
    character(c_char),intent(in) :: func_name
    integer(c_int),value :: sargs
    integer(c_int),value :: pargs
    integer(c_int64_t),intent(in) :: args_array
  end function hthread_group_masked_create

  function hthread_group_exec(thread_id, func_name, sargs, pargs, args_array) BIND(C)
    use iso_c_binding, only: c_int,c_char,c_int64_t
    integer(c_int) :: hthread_group_exec
    integer(c_int),value :: thread_id
    character(c_char),intent(in) :: func_name
    integer(c_int),value :: sargs
    integer(c_int),value :: pargs
    integer(c_int64_t),intent(in) :: args_array
  end function hthread_group_exec

  function hthread_group_get_status(thread_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_group_get_status
    integer(c_int),value :: thread_id
  end function hthread_group_get_status

  function hthread_group_wait(thread_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_group_wait
    integer(c_int),value :: thread_id
  end function hthread_group_wait

  function hthread_group_destroy(thread_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_group_destroy
    integer(c_int),value :: thread_id
  end function hthread_group_destroy

  function hthread_get_avail_threads(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_get_avail_threads
    integer(c_int),value :: cluster_id
  end function hthread_get_avail_threads

  function hthread_barrier_create(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_barrier_create
    integer(c_int),value :: cluster_id
  end function hthread_barrier_create

  function hthread_rwlock_create(cluster_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_rwlock_create
    integer(c_int),value :: cluster_id
  end function hthread_rwlock_create

  function hthread_malloc(cluster_id, bytes, mode) BIND(C)
    use iso_c_binding, only: c_int,c_long,c_ptr
    type(c_ptr) :: hthread_malloc
    integer(c_int),value :: cluster_id
    integer(c_long),value :: bytes
    integer(c_int),value :: mode
  end function hthread_malloc

  subroutine hthread_free(ptr) BIND(C)
    use iso_c_binding, only: c_ptr
    type(c_ptr),value :: ptr
  end subroutine hthread_free

  subroutine hthread_barrier_destroy(b_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int),value :: b_id
  end subroutine hthread_barrier_destroy

  subroutine hthread_rwlock_destroy(lock_id) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int),value :: lock_id
  end subroutine hthread_rwlock_destroy

end interface


interface hthread_group_create
  function hthread_group_create2(cluster_id, num) BIND(C)
    use iso_c_binding, only: c_int
    integer(c_int) :: hthread_group_create2
    integer(c_int),value :: cluster_id
    integer(c_int),value :: num 
  end function hthread_group_create2
  function hthread_group_create6(cluster_id, num, func_name, sargs, pargs, args_array) BIND(C)
    use iso_c_binding, only: c_int,c_char,c_int64_t
    integer(c_int) :: hthread_group_create6
    integer(c_int),value :: cluster_id
    integer(c_int),value :: num 
    character(c_char),intent(in) :: func_name
    integer(c_int),value :: sargs
    integer(c_int),value :: pargs
    integer(c_int64_t),intent(in) :: args_array
  end function hthread_group_create6

end interface hthread_group_create

#endif
