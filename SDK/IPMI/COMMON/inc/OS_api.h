
#ifndef _OS_API_H_
#define _OS_API_H_

#ifdef	OS_THREAD_X





#ifndef  TX_API_H
#define  TX_API_H



#ifdef   __cplusplus

extern   "C" {

#endif



#ifndef TX_PORT
#define TX_PORT



#define OS_MINIMUM_STACK         100         
#define OS_INT_DISABLE           0x80        
#define OS_INT_ENABLE            0x00        



#ifndef OS_TIMER_THREAD_STACK_SIZE
#define OS_TIMER_THREAD_STACK_SIZE  1024    
#endif
#ifndef OS_TIMER_THREAD_PRIORITY    
#define OS_TIMER_THREAD_PRIORITY    0       
#endif




#define TX_INLINE_INITIALIZATION



typedef void *                      VOID_PTR;
typedef char                        CHAR;
typedef char *                      CHAR_PTR;
typedef unsigned char               UCHAR;
typedef unsigned char *             UCHAR_PTR;
typedef int                         INT;
typedef int *                       INT_PTR;
typedef unsigned int                UINT;
typedef unsigned int *              UINT_PTR;
typedef long                        LONG;
typedef long *                      LONG_PTR;
typedef unsigned long               ULONG;
typedef unsigned long *             ULONG_PTR;




#define REG_1                       register
#define REG_2                       register
#define REG_3                       register
#define REG_4                       register
#define REG_5                       register




#define TX_THREAD_PORT_EXTENSION    




unsigned int  _tx_thread_interrupt_control(unsigned int);

#define OS_INTERRUPT_SAVE_AREA      unsigned int interrupt_save;

#define OS_DISABLE                  interrupt_save =  _tx_thread_interrupt_control(OS_INT_DISABLE);
#define OS_RESTORE                  _tx_thread_interrupt_control(interrupt_save);



#ifdef  OS_THREAD_INIT
CHAR                            _tx_version_id[] = 
                                    "Copyright (c) 1996-2004 Express Logic Inc. * ThreadX H8/300H-Hitachi Version G4.0b.4.0b *";
#else
extern  CHAR                    _tx_version_id[];
#endif


#endif



#define OS_INTERNAL_TIMER		TX_INTERNAL_TIMER
#define OS_TIMER				TX_TIMER
#define OS_THREAD				TX_THREAD
#define OS_SEMAPHORE			TX_SEMAPHORE
#define OS_QUEUE				TX_QUEUE
#define OS_EVENT_FLAGS_GROUP	TX_EVENT_FLAGS_GROUP
#define OS_BLOCK_POOL			TX_BLOCK_POOL
#define OS_BYTE_POOL			TX_BYTE_POOL
#define OS_MUTEX				TX_MUTEX


#define OS_TIMER_PTR				TX_TIMER_PTR
#define OS_THREAD_PTR				TX_THREAD_PTR
#define OS_SEMAPHORE_PTR			TX_SEMAPHORE_PTR
#define OS_QUEUE_PTR				TX_QUEUE_PTR
#define OS_EVENT_FLAGS_GROUP_PTR	TX_EVENT_FLAGS_GROUP_PTR
#define OS_BLOCK_POOL_PTR			TX_BLOCK_POOL_PTR
#define OS_BYTE_POOL_PTR			TX_BYTE_POOL_PTR
#define OS_MUTEX_PTR				TX_MUTEX_PTR

#define OS_NO_INHERIT               TX_NO_INHERIT


#define OS_NO_WAIT          0
#define OS_WAIT_FOREVER     0xFFFFFFFFUL
#define OS_AND              2
#define OS_AND_CLEAR        3
#define OS_OR               0
#define OS_OR_CLEAR         1
#define OS_1_ULONG          1
#define OS_2_ULONG          2
#define OS_4_ULONG          4
#define OS_8_ULONG          8
#define OS_16_ULONG         16
#define OS_32_ULONG         32
#define OS_NO_TIME_SLICE    0
#define OS_MAX_PRIORITIES   32
#define OS_AUTO_START       1
#define OS_DONT_START       0
#define OS_AUTO_ACTIVATE    1
#define OS_NO_ACTIVATE      0
#define OS_TRUE             1
#define OS_FALSE            0
#define OS_NULL             0
#define OS_LOOP_FOREVER     1
#define OS_INHERIT          1
#define OS_NO_INHERIT       0



#define OS_READY            0
#define OS_COMPLETED        1
#define OS_TERMINATED       2
#define OS_SUSPENDED        3   
#define OS_SLEEP            4
#define OS_QUEUE_SUSP       5
#define OS_SEMAPHORE_SUSP   6
#define OS_EVENT_FLAG       7
#define OS_BLOCK_MEMORY     8
#define OS_BYTE_MEMORY      9
#define OS_IO_DRIVER        10
#define OS_FILE             11
#define OS_TCP_IP           12
#define OS_MUTEX_SUSP       13



#define OS_SUCCESS          0x00
#define OS_DELETED          0x01
#define OS_NO_MEMORY        0x10
#define OS_POOL_ERROR       0x02
#define OS_PTR_ERROR        0x03
#define OS_WAIT_ERROR       0x04
#define OS_SIZE_ERROR       0x05
#define OS_GROUP_ERROR      0x06
#define OS_NO_EVENTS        0x07
#define OS_OPTION_ERROR     0x08
#define OS_QUEUE_ERROR      0x09
#define OS_QUEUE_EMPTY      0x0A
#define OS_QUEUE_FULL       0x0B
#define OS_SEMAPHORE_ERROR  0x0C
#define OS_NO_INSTANCE      0x0D
#define OS_THREAD_ERROR     0x0E
#define OS_PRIORITY_ERROR   0x0F
#define OS_START_ERROR      0x10
#define OS_DELETE_ERROR     0x11
#define OS_RESUME_ERROR     0x12
#define OS_CALLER_ERROR     0x13
#define OS_SUSPEND_ERROR    0x14
#define OS_TIMER_ERROR      0x15
#define OS_TICK_ERROR       0x16
#define OS_ACTIVATE_ERROR   0x17
#define OS_THRESH_ERROR     0x18
#define OS_SUSPEND_LIFTED   0x19
#define OS_WAIT_ABORTED     0x1A
#define OS_WAIT_ABORT_ERROR 0x1B
#define OS_MUTEX_ERROR      0x1C
#define OS_NOT_AVAILABLE    0x1D
#define OS_NOT_OWNED        0x1E
#define OS_INHERIT_ERROR    0x1F







typedef  struct TX_INTERNAL_TIMER_STRUCT
{

    ULONG       tx_remaining_ticks;
    ULONG       tx_re_initialize_ticks;

    void        (*tx_timeout_function)(ULONG);
    ULONG       tx_timeout_param;



    struct TX_INTERNAL_TIMER_STRUCT
                *tx_active_next,
                *tx_active_previous;

    struct TX_INTERNAL_TIMER_STRUCT
                **tx_list_head;
} TX_INTERNAL_TIMER;



typedef  struct TX_TIMER_STRUCT
{

    ULONG       tx_timer_id;

    CHAR_PTR    tx_timer_name;




    TX_INTERNAL_TIMER   tx_timer_internal;

    struct TX_TIMER_STRUCT  
                *tx_timer_created_next,
                *tx_timer_created_previous;
} TX_TIMER;

typedef TX_TIMER *      TX_TIMER_PTR;




typedef  struct TX_THREAD_STRUCT
{

    ULONG       tx_thread_id;           
    ULONG       tx_run_count;           
    VOID_PTR    tx_stack_ptr;           
    VOID_PTR    tx_stack_start;         
    VOID_PTR    tx_stack_end;           
    ULONG       tx_stack_size;          
    ULONG       tx_time_slice;          
    ULONG       tx_new_time_slice;      

    struct TX_THREAD_STRUCT 
                *tx_ready_next,      
                *tx_ready_previous;


    TX_THREAD_PORT_EXTENSION
  
         

    CHAR_PTR    tx_thread_name;         
    UINT        tx_priority;            
    UINT        tx_state;               
    UINT        tx_delayed_suspend;     
    UINT        tx_suspending;          
    UINT        tx_preempt_threshold;   
    ULONG       tx_priority_bit;        

    void        (*tx_thread_entry)(ULONG);
    ULONG       tx_entry_parameter;


    TX_INTERNAL_TIMER
                tx_thread_timer;


    void        (*tx_suspend_cleanup)(struct TX_THREAD_STRUCT *);
    VOID_PTR    tx_suspend_control_block;
    struct TX_THREAD_STRUCT
                *tx_suspended_next,
                *tx_suspended_previous;
    ULONG       tx_suspend_info;
    VOID_PTR    tx_additional_suspend_info;
    UINT        tx_suspend_option;
    UINT        tx_suspend_status;


    struct TX_THREAD_STRUCT 
                *tx_created_next,    
                *tx_created_previous;

#ifndef TX_NO_FILEX
    VOID_PTR    tx_filex_ptr;
#endif

} TX_THREAD;

typedef TX_THREAD *     TX_THREAD_PTR;



typedef struct TX_MUTEX_STRUCT
{

    ULONG       tx_mutex_id;

    CHAR_PTR    tx_mutex_name;

    ULONG       tx_mutex_ownership_count;


    TX_THREAD   *tx_mutex_owner;


    UINT        tx_mutex_inherit;


    UINT        tx_mutex_original_priority;
    UINT        tx_mutex_original_threshold;


    struct TX_THREAD_STRUCT  *tx_mutex_suspension_list;
    ULONG                    tx_mutex_suspended_count;

    struct TX_MUTEX_STRUCT 
                *tx_mutex_created_next,    
                *tx_mutex_created_previous;

} TX_MUTEX;



typedef struct TX_SEMAPHORE_STRUCT
{

    ULONG       tx_semaphore_id;

    CHAR_PTR    tx_semaphore_name;


    ULONG       tx_semaphore_count;


    struct TX_THREAD_STRUCT  *tx_semaphore_suspension_list;
    ULONG                    tx_semaphore_suspended_count;

    struct TX_SEMAPHORE_STRUCT 
                *tx_semaphore_created_next,    
                *tx_semaphore_created_previous;

} TX_SEMAPHORE;

typedef TX_SEMAPHORE *      TX_SEMAPHORE_PTR;



typedef struct TX_QUEUE_STRUCT
{

    ULONG       tx_queue_id;

    CHAR_PTR    tx_queue_name;

    UINT        tx_queue_message_size;

    ULONG       tx_queue_capacity;


    ULONG       tx_queue_enqueued;
    ULONG       tx_queue_available_storage;

	UINT					tx_queue_front_send_num;
	TX_MUTEX				tx_queue_mutex;



    ULONG_PTR   tx_queue_start;
    ULONG_PTR   tx_queue_end;


    ULONG_PTR   tx_queue_read;
    ULONG_PTR   tx_queue_write;


    struct TX_THREAD_STRUCT  *tx_queue_suspension_list;
    ULONG                    tx_queue_suspended_count;

    struct TX_QUEUE_STRUCT 
                *tx_queue_created_next,    
                *tx_queue_created_previous;

} TX_QUEUE;

typedef TX_QUEUE *      TX_QUEUE_PTR;



typedef struct TX_EVENT_FLAGS_GROUP_STRUCT
{

    ULONG       tx_event_flags_id;

    CHAR_PTR    tx_event_flags_name;


    ULONG       tx_event_flags_current;



    UINT        tx_event_flags_reset_search;

	CHAR					tx_event_recheck;
	CHAR					tx_event_check_done;
	TX_MUTEX				tx_event_mutex;


    struct TX_THREAD_STRUCT  *tx_event_flags_suspension_list;
    ULONG                    tx_event_flags_suspended_count;

    struct TX_EVENT_FLAGS_GROUP_STRUCT 
                *tx_event_flags_created_next,    
                *tx_event_flags_created_previous;

} TX_EVENT_FLAGS_GROUP;

typedef TX_EVENT_FLAGS_GROUP *      TX_EVENT_FLAGS_GROUP_PTR;



typedef struct TX_BLOCK_POOL_STRUCT
{

    ULONG       tx_block_pool_id;

    CHAR_PTR    tx_block_pool_name;

    ULONG       tx_block_pool_available;

    ULONG       tx_block_pool_total;

    CHAR_PTR    tx_block_pool_available_list;

    CHAR_PTR    tx_block_pool_start;

    ULONG       tx_block_pool_size;

    ULONG       tx_block_pool_block_size;


    struct TX_THREAD_STRUCT  *tx_block_pool_suspension_list;
    ULONG                    tx_block_pool_suspended_count;

    struct TX_BLOCK_POOL_STRUCT 
                *tx_block_pool_created_next,    
                *tx_block_pool_created_previous;

} TX_BLOCK_POOL;

typedef TX_BLOCK_POOL *      TX_BLOCK_POOL_PTR;



typedef struct TX_BYTE_POOL_STRUCT
{

    ULONG       tx_byte_pool_id;

    CHAR_PTR    tx_byte_pool_name;

    ULONG       tx_byte_pool_available;

    ULONG       tx_byte_pool_fragments;

    CHAR_PTR    tx_byte_pool_list;


    CHAR_PTR    tx_byte_pool_search;

    CHAR_PTR    tx_byte_pool_start;

    ULONG       tx_byte_pool_size;


    struct TX_THREAD_STRUCT  *tx_byte_pool_owner;


    struct TX_THREAD_STRUCT  *tx_byte_pool_suspension_list;
    ULONG                    tx_byte_pool_suspended_count;

    struct TX_BYTE_POOL_STRUCT 
                *tx_byte_pool_created_next,    
                *tx_byte_pool_created_previous;

} TX_BYTE_POOL;

typedef TX_BYTE_POOL *      TX_BYTE_POOL_PTR;




typedef TX_MUTEX *      TX_MUTEX_PTR;




#ifndef  TX_SOURCE_CODE




#ifdef TX_DISABLE_ERROR_CHECKING


#define os_kernel_enter             _tx_initialize_kernel_enter

#define os_byte_allocate            _tx_byte_allocate
#define os_byte_pool_create         _tx_byte_pool_create
#define os_byte_pool_delete         _tx_byte_pool_delete
#define os_byte_pool_info_get       _tx_byte_pool_info_get
#define os_byte_pool_prioritize     _tx_byte_pool_prioritize
#define os_byte_release             _tx_byte_release

#define os_block_allocate           _tx_block_allocate
#define os_block_pool_create        _tx_block_pool_create
#define os_block_pool_delete        _tx_block_pool_delete
#define os_block_pool_info_get      _tx_block_pool_info_get
#define os_block_pool_prioritize    _tx_block_pool_prioritize
#define os_block_release            _tx_block_release

#define os_event_flags_create       _tx_event_flags_create
#define os_event_flags_delete       _tx_event_flags_delete
#define os_event_flags_get          _tx_event_flags_get
#define os_event_flags_info_get     _tx_event_flags_info_get
#define os_event_flags_set          _tx_event_flags_set

#define os_interrupt_control        _tx_thread_interrupt_control

#define os_queue_create             _tx_queue_create
#define os_queue_delete             _tx_queue_delete
#define os_queue_flush              _tx_queue_flush
#define os_queue_info_get           _tx_queue_info_get
#define os_queue_receive            _tx_queue_receive
#define os_queue_send               _tx_queue_send
#define os_queue_front_send         _tx_queue_front_send
#define os_queue_prioritize         _tx_queue_prioritize

#define os_semaphore_create         _tx_semaphore_create
#define os_semaphore_delete         _tx_semaphore_delete
#define os_semaphore_get            _tx_semaphore_get
#define os_semaphore_info_get       _tx_semaphore_info_get
#define os_semaphore_prioritize     _tx_semaphore_prioritize
#define os_semaphore_put            _tx_semaphore_put

#define os_mutex_create             _tx_mutex_create
#define os_mutex_delete             _tx_mutex_delete
#define os_mutex_get                _tx_mutex_get
#define os_mutex_info_get           _tx_mutex_info_get
#define os_mutex_prioritize         _tx_mutex_prioritize
#define os_mutex_put                _tx_mutex_put

#define os_thread_create            _tx_thread_create
#define os_thread_delete            _tx_thread_delete
#define os_thread_identify          _tx_thread_identify
#define os_thread_info_get          _tx_thread_info_get
#define os_thread_preemption_change _tx_thread_preemption_change
#define os_thread_priority_change   _tx_thread_priority_change
#define os_thread_relinquish        _tx_thread_relinquish
#define os_thread_resume            _tx_thread_resume_api
#define os_thread_sleep             _tx_thread_sleep
#define os_thread_suspend           _tx_thread_suspend_api
#define os_thread_terminate         _tx_thread_terminate
#define os_thread_time_slice_change _tx_thread_time_slice_change
#define os_thread_wait_abort        _tx_thread_wait_abort

#define os_time_get                 _tx_time_get
#define os_time_set                 _tx_time_set
#define os_timer_activate           _tx_timer_activate_api
#define os_timer_change             _tx_timer_change
#define os_timer_create             _tx_timer_create
#define os_timer_deactivate         _tx_timer_deactivate_api
#define os_timer_delete             _tx_timer_delete
#define os_timer_info_get           _tx_timer_info_get

#else


#define os_kernel_enter             _tx_initialize_kernel_enter




#define os_byte_allocate            _txe_byte_allocate
#define os_byte_pool_create         _txe_byte_pool_create
#define os_byte_pool_delete         _txe_byte_pool_delete
#define os_byte_pool_info_get       _txe_byte_pool_info_get
#define os_byte_pool_prioritize     _txe_byte_pool_prioritize
#define os_byte_release             _txe_byte_release

#define os_block_allocate           _txe_block_allocate
#define os_block_pool_create        _txe_block_pool_create
#define os_block_pool_delete        _txe_block_pool_delete
#define os_block_pool_info_get      _txe_block_pool_info_get
#define os_block_pool_prioritize    _txe_block_pool_prioritize
#define os_block_release            _txe_block_release

#define os_event_flags_create       _txe_event_flags_create
#define os_event_flags_delete       _txe_event_flags_delete
#define os_event_flags_get          _txe_event_flags_get
#define os_event_flags_info_get     _txe_event_flags_info_get
#define os_event_flags_set          _txe_event_flags_set

#define os_interrupt_control        _tx_thread_interrupt_control

#define os_queue_create             _txe_queue_create
#define os_queue_delete             _txe_queue_delete
#define os_queue_flush              _txe_queue_flush
#define os_queue_info_get           _txe_queue_info_get
#define os_queue_receive            _txe_queue_receive
#define os_queue_send               _txe_queue_send
#define os_queue_front_send         _txe_queue_front_send
#define os_queue_prioritize         _txe_queue_prioritize

#define os_semaphore_create         _txe_semaphore_create
#define os_semaphore_delete         _txe_semaphore_delete
#define os_semaphore_get            _txe_semaphore_get
#define os_semaphore_info_get       _txe_semaphore_info_get
#define os_semaphore_prioritize     _txe_semaphore_prioritize
#define os_semaphore_put            _txe_semaphore_put

#define os_mutex_create             _txe_mutex_create
#define os_mutex_delete             _txe_mutex_delete
#define os_mutex_get                _txe_mutex_get
#define os_mutex_info_get           _txe_mutex_info_get
#define os_mutex_prioritize         _txe_mutex_prioritize
#define os_mutex_put                _txe_mutex_put

#define os_thread_create            _txe_thread_create
#define os_thread_delete            _txe_thread_delete
#define os_thread_identify          _tx_thread_identify
#define os_thread_info_get          _txe_thread_info_get
#define os_thread_preemption_change _txe_thread_preemption_change
#define os_thread_priority_change   _txe_thread_priority_change
#define os_thread_relinquish        _txe_thread_relinquish
#define os_thread_resume            _txe_thread_resume_api
#define os_thread_sleep             _tx_thread_sleep
#define os_thread_suspend           _txe_thread_suspend_api
#define os_thread_terminate         _txe_thread_terminate
#define os_thread_time_slice_change _txe_thread_time_slice_change
#define os_thread_wait_abort        _txe_thread_wait_abort

#define os_time_get                 _tx_time_get
#define os_time_set                 _tx_time_set
#define os_timer_activate           _txe_timer_activate_api
#define os_timer_change             _txe_timer_change
#define os_timer_create             _txe_timer_create
#define os_timer_deactivate         _txe_timer_deactivate_api
#define os_timer_delete             _txe_timer_delete
#define os_timer_info_get           _txe_timer_info_get

#endif


void        os_application_define(void *);


void        os_kernel_enter(void);

UINT        os_byte_allocate(TX_BYTE_POOL *pool_ptr, void **memory_ptr, ULONG memory_size,
                    ULONG wait_option);
UINT        os_byte_pool_create(TX_BYTE_POOL *pool_ptr, CHAR *name_ptr, void *pool_start,
                    ULONG pool_size);
UINT        os_byte_pool_delete(TX_BYTE_POOL *pool_ptr);
UINT        os_byte_pool_info_get(TX_BYTE_POOL *pool_ptr, CHAR **name, ULONG *available_bytes, 
                    ULONG *fragments, TX_THREAD **first_suspended, 
                    ULONG *suspended_count, TX_BYTE_POOL **next_pool);
UINT        os_byte_pool_prioritize(TX_BYTE_POOL *pool_ptr);

UINT        os_byte_release(void *memory_ptr);

UINT        os_block_allocate(TX_BLOCK_POOL *pool_ptr, void **block_ptr, ULONG wait_option);
UINT        os_block_pool_create(TX_BLOCK_POOL *pool_ptr, CHAR *name_ptr, ULONG block_size,
                    void *pool_start, ULONG pool_size);
UINT        os_block_pool_delete(TX_BLOCK_POOL *pool_ptr);
UINT        os_block_pool_info_get(TX_BLOCK_POOL *pool_ptr, CHAR **name, ULONG *available_blocks, 
                    ULONG *total_blocks, TX_THREAD **first_suspended, 
                    ULONG *suspended_count, TX_BLOCK_POOL **next_pool);
UINT        os_block_pool_prioritize(TX_BLOCK_POOL *pool_ptr);
UINT        os_block_release(void *block_ptr);

UINT        os_event_flags_create(TX_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr);
UINT        os_event_flags_delete(TX_EVENT_FLAGS_GROUP *group_ptr);
UINT        os_event_flags_get(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags,
                    UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option);
UINT        os_event_flags_info_get(TX_EVENT_FLAGS_GROUP *group_ptr, CHAR **name, ULONG *current_flags, 
                    TX_THREAD **first_suspended, ULONG *suspended_count, 
                    TX_EVENT_FLAGS_GROUP **next_group);
UINT        os_event_flags_set(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set, 
                    UINT set_option);

UINT        os_interrupt_control(UINT new_posture);

UINT        os_queue_create(TX_QUEUE *queue_ptr, CHAR *name_ptr, UINT message_size, 
                        void *queue_start, ULONG queue_size);
UINT        os_queue_delete(TX_QUEUE *queue_ptr);
UINT        os_queue_flush(TX_QUEUE *queue_ptr);
UINT        os_queue_info_get(TX_QUEUE *queue_ptr, CHAR **name, ULONG *enqueued, ULONG *available_storage,
                    TX_THREAD **first_suspended, ULONG *suspended_count, TX_QUEUE **next_queue);
UINT        os_queue_receive(TX_QUEUE *queue_ptr, void *destination_ptr, ULONG wait_option);
UINT        os_queue_send(TX_QUEUE *queue_ptr, void *source_ptr, ULONG wait_option);
UINT        os_queue_front_send(TX_QUEUE *queue_ptr, void *source_ptr, ULONG wait_option);
UINT        os_queue_prioritize(TX_QUEUE *queue_ptr);


UINT        os_semaphore_create(TX_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count);
UINT        os_semaphore_delete(TX_SEMAPHORE *semaphore_ptr);
UINT        os_semaphore_get(TX_SEMAPHORE *semaphore_ptr, ULONG wait_option);
UINT        os_semaphore_info_get(TX_SEMAPHORE *semaphore_ptr, CHAR **name, ULONG *current_value, 
                    TX_THREAD **first_suspended, ULONG *suspended_count, 
                    TX_SEMAPHORE **next_semaphore);
UINT        os_semaphore_prioritize(TX_SEMAPHORE *semaphore_ptr);
UINT        os_semaphore_put(TX_SEMAPHORE *semaphore_ptr);

UINT        os_mutex_create(TX_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit);
UINT        os_mutex_delete(TX_MUTEX *mutex_ptr);
UINT        os_mutex_get(TX_MUTEX *mutex_ptr, ULONG wait_option);
UINT        os_mutex_info_get(TX_MUTEX *mutex_ptr, CHAR **name, ULONG *count, TX_THREAD **owner,
                    TX_THREAD **first_suspended, ULONG *suspended_count, 
                    TX_MUTEX **next_mutex);
UINT        os_mutex_prioritize(TX_MUTEX *mutex_ptr);
UINT        os_mutex_put(TX_MUTEX *mutex_ptr);

UINT        os_thread_create(TX_THREAD *thread_ptr, CHAR *name_ptr, 
                void (*entry_function)(ULONG), ULONG entry_input,
                void *stack_start, ULONG stack_size, 
                UINT priority, UINT preempt_threshold, 
                ULONG time_slice, UINT auto_start);
UINT        os_thread_delete(TX_THREAD *thread_ptr);
TX_THREAD  *tx_thread_identify(void);
UINT        os_thread_info_get(TX_THREAD *thread_ptr, CHAR **name, UINT *state, ULONG *run_count, 
                UINT *priority, UINT *preemption_threshold, ULONG *time_slice, 
                TX_THREAD **next_thread, TX_THREAD **next_suspended_thread);
UINT        os_thread_preemption_change(TX_THREAD *thread_ptr, UINT new_threshold,
                        UINT *old_threshold);
UINT        os_thread_priority_change(TX_THREAD *thread_ptr, UINT new_priority,
                        UINT *old_priority);
void        os_thread_relinquish(void);
UINT        os_thread_resume(TX_THREAD *thread_ptr);
UINT        os_thread_sleep(ULONG timer_ticks);
UINT        os_thread_suspend(TX_THREAD *thread_ptr);
UINT        os_thread_terminate(TX_THREAD *thread_ptr);
UINT        os_thread_time_slice_change(TX_THREAD *thread_ptr, ULONG new_time_slice, ULONG *old_time_slice);
UINT        os_thread_wait_abort(TX_THREAD *thread_ptr);

ULONG       os_time_get(void);
void        os_time_set(ULONG new_time);

UINT        os_timer_activate(TX_TIMER *timer_ptr);
UINT        os_timer_change(TX_TIMER *timer_ptr, ULONG initial_ticks, ULONG reschedule_ticks);
UINT        os_timer_create(TX_TIMER *timer_ptr, CHAR *name_ptr, 
                UINT expiration_function_id, ULONG expiration_input, ULONG initial_ticks,
                ULONG reschedule_ticks, UINT auto_activate);
UINT        os_timer_deactivate(TX_TIMER *timer_ptr);
UINT        os_timer_delete(TX_TIMER *timer_ptr);
UINT        os_timer_info_get(TX_TIMER *timer_ptr, CHAR **name, UINT *active, ULONG *remaining_ticks, 
                ULONG *reschedule_ticks, TX_TIMER **next_timer);


#endif



#ifdef   __cplusplus
        }
#endif

#endif

#else 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <mqueue.h>

#include <sys/syscall.h>
#include <sys/sem.h>
#include <sys/param.h>

typedef void                        VOID;
typedef void *                      VOID_PTR;
typedef char                        CHAR;
typedef char *                      CHAR_PTR;
typedef unsigned char               UCHAR;
typedef unsigned char *             UCHAR_PTR;
typedef int                         INT;
typedef int *                       INT_PTR;
typedef unsigned int                UINT;
typedef unsigned int *              UINT_PTR;
typedef long                        LONG;
typedef long *                      LONG_PTR;
typedef unsigned long               ULONG;
typedef unsigned long *             ULONG_PTR;



#define OS_INTERRUPT_SAVE_AREA      ;
#define OS_DISABLE                  ;
#define OS_RESTORE                  ;
#define OS_ISR_DISABLE                  ;
#define OS_ISR_RESTORE                  ;

#define TIME_TICKS 1000/HZ
#define BILLION 100000000L
#define MILLION 1000000L
#define OS_MUTEX_ID                     0x4D555445UL
#define OS_SEMAPHORE_ID                 0x53454D41UL

#define OS_TIMER_ALREADY_ACTIVATE	0x20

#define TIMER_SETTING_QUEUE "/timer_setting_queue" 
#define SETTING_QUEUE_MAX_SET 10
#define OS_TIMER_HWND timer_t
#define OS_TIMER_CREATE		0x01
#define OS_TIMER_DELETE		0x02
#define OS_TIMER_ACTIVATE	0x03
#define	OS_TIMER_CHANGE		0x04
#define OS_TIMER_DEACTIVATE	0x05


typedef struct TX_MUTEX_STRUCT
{
    ULONG       tx_mutex_id;
    CHAR_PTR    tx_mutex_name;

	int sem_id;
	int shm_id;

} TX_MUTEX;

typedef struct TX_QUEUE_STRUCT
{
	
	mqd_t					tx_queue_id;				
	CHAR 					tx_queue_name[30];			
	UINT 					tx_queue_message_size;		
	ULONG 					tx_queue_capacity;			
	ULONG 					tx_queue_enqueued;
	ULONG 					tx_queue_available_storage;
	UINT					tx_queue_front_send_num;
	TX_MUTEX				tx_queue_mutex;
} TX_QUEUE;


typedef TX_QUEUE *		TX_QUEUE_PTR;

typedef struct TX_EVENT_FLAGS_GROUP_STRUCT
{
	CHAR 					tx_event_flags_name[30];	

	ULONG 					tx_event_flags_current;
	CHAR					tx_event_recheck;
	CHAR					tx_event_check_done;
	TX_MUTEX				tx_event_mutex;
} TX_EVENT_FLAGS_GROUP;

typedef TX_EVENT_FLAGS_GROUP *      TX_EVENT_FLAGS_GROUP_PTR;



union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

typedef struct TX_SEMAPHORE_STRUCT
{
	ULONG tx_semaphore_id;
	CHAR_PTR tx_semaphore_name;
	
	int sem_id;
	int shm_id;
} TX_SEMAPHORE;



typedef struct SHM_MUTEX_STRUCT
{
	unsigned int own_pid;
	unsigned long ownership_count;
} SHM_MUTEX;

ULONG ctx_hash(UCHAR *name_ptr);

typedef struct TX_INTERNAL_TIMER_STRUCT
{
	ULONG			tx_remaining_msec;
	ULONG			tx_re_initialize_msec;
	VOID			(*tx_timeout_function)(ULONG);
	ULONG			tx_timeout_param;
} TX_INTERNAL_TIMER;

typedef struct _TX_TIMER_STRUCT
{
	OS_TIMER_HWND			tx_timer_id;		
	pthread_t               tx_thread_id;		
	CHAR_PTR 				tx_timer_name;		

	TX_INTERNAL_TIMER		tx_timer_internal;	
} _TX_TIMER;

typedef struct TX_TIMER_STRUCT
{
	OS_TIMER_HWND			tx_timer_hwnd;		
	pthread_t				tx_thread_id;		
	UINT					expiration_function;
	ULONG					expiration_input;
	ULONG					initial_msec;
	ULONG					reschedule_msec;
	UINT					auto_activate;
	ULONG					set_flag;
} TX_TIMER;

typedef TX_TIMER *      TX_TIMER_PTR;

#define OS_INTERNAL_TIMER			TX_INTERNAL_TIMER
#define OS_TIMER					TX_TIMER
#define OS_THREAD					char
#define OS_SEMAPHORE				TX_SEMAPHORE
#define OS_QUEUE					TX_QUEUE 
#define OS_EVENT_FLAGS_GROUP		TX_EVENT_FLAGS_GROUP
#define OS_BLOCK_POOL				char
#define OS_BYTE_POOL				char
#define OS_MUTEX					TX_MUTEX
#define OS_TIMER_PTR				TX_TIMER_PTR
#define OS_THREAD_PTR				char*
#define OS_SEMAPHORE_PTR			char*
#define OS_QUEUE_PTR				char*
#define OS_EVENT_FLAGS_GROUP_PTR	char*
#define OS_BLOCK_POOL_PTR			char*
#define OS_BYTE_POOL_PTR			char*
#define OS_MUTEX_PTR				char*


#define OS_NO_WAIT          0
#define OS_WAIT_FOREVER     0xFFFFFFFFUL
#define OS_AND              2
#define OS_AND_CLEAR        3
#define OS_OR               0
#define OS_OR_CLEAR         1
#define OS_1_ULONG          1
#define OS_2_ULONG          2
#define OS_4_ULONG          4
#define OS_8_ULONG          8
#define OS_16_ULONG         16
#define OS_32_ULONG         32
#define OS_NO_TIME_SLICE    0
#define OS_MAX_PRIORITIES   32
#define OS_AUTO_START       1
#define OS_DONT_START       0
#define OS_AUTO_ACTIVATE    1
#define OS_NO_ACTIVATE      0
#define OS_TRUE             1
#define OS_FALSE            0
#define OS_NULL             0
#define OS_LOOP_FOREVER     1
#define OS_INHERIT          1
#define OS_NO_INHERIT       0



#define OS_READY            0
#define OS_COMPLETED        1
#define OS_TERMINATED       2
#define OS_SUSPENDED        3   
#define OS_SLEEP            4
#define OS_QUEUE_SUSP       5
#define OS_SEMAPHORE_SUSP   6
#define OS_EVENT_FLAG       7
#define OS_BLOCK_MEMORY     8
#define OS_BYTE_MEMORY      9
#define OS_IO_DRIVER        10
#define OS_FILE             11
#define OS_TCP_IP           12
#define OS_MUTEX_SUSP       13



#define OS_SUCCESS          0x00
#define OS_DELETED          0x01
#define OS_NO_MEMORY        0x10
#define OS_POOL_ERROR       0x02
#define OS_PTR_ERROR        0x03
#define OS_WAIT_ERROR       0x04
#define OS_SIZE_ERROR       0x05
#define OS_GROUP_ERROR      0x06
#define OS_NO_EVENTS        0x07
#define OS_OPTION_ERROR     0x08
#define OS_QUEUE_ERROR      0x09
#define OS_QUEUE_EMPTY      0x0A
#define OS_QUEUE_FULL       0x0B
#define OS_SEMAPHORE_ERROR  0x0C
#define OS_NO_INSTANCE      0x0D
#define OS_THREAD_ERROR     0x0E
#define OS_PRIORITY_ERROR   0x0F
#define OS_START_ERROR      0x10
#define OS_DELETE_ERROR     0x11
#define OS_RESUME_ERROR     0x12
#define OS_CALLER_ERROR     0x13
#define OS_SUSPEND_ERROR    0x14
#define OS_TIMER_ERROR      0x15
#define OS_TICK_ERROR       0x16
#define OS_ACTIVATE_ERROR   0x17
#define OS_THRESH_ERROR     0x18
#define OS_SUSPEND_LIFTED   0x19
#define OS_WAIT_ABORTED     0x1A
#define OS_WAIT_ABORT_ERROR 0x1B
#define OS_MUTEX_ERROR      0x1C
#define OS_NOT_AVAILABLE    0x1D
#define OS_NOT_OWNED        0x1E
#define OS_INHERIT_ERROR    0x1F

#define OS_EVENT_RETRY_DELAY 100000 

#include <timer_api.h>
#include <syscall.h>
#include <lx_api.h>


#define os_queue_create             lx_queue_create
#define os_queue_delete             lx_queue_delete
#define os_queue_flush              lx_queue_flush
#define os_queue_send               lx_queue_send
#define os_queue_receive            lx_queue_receive
#define os_queue_info_get           lx_queue_info_get
#define os_queue_front_send         lx_queue_front_send


#define os_event_flags_create		lx_event_flags_create
#define os_event_flags_delete		lx_event_flags_delete
#define os_event_flags_set			lx_event_flags_set
#define os_event_flags_get			lx_event_flags_get
#define os_event_flags_info_get		lx_event_flags_info_get

#define os_semaphore_create         lx_semaphore_create  
#define os_semaphore_delete         lx_semaphore_delete  
#define os_semaphore_get            lx_semaphore_get     
#define os_semaphore_info_get       lx_semaphore_info_get
#define os_semaphore_put            lx_semaphore_put     

#define os_mutex_create             lx_mutex_create  
#define os_mutex_delete             lx_mutex_delete  
#define os_mutex_get                lx_mutex_get     
#define os_mutex_info_get           lx_mutex_info_get
#define os_mutex_put                lx_mutex_put     

#define os_timer_deactivate         lx_timer_deactivate
#define os_timer_activate           lx_timer_activate
#define os_timer_create             lx_timer_create
#define os_timer_change             lx_timer_change
#define os_timer_delete             lx_timer_delete


#define os_tx_thread_sleep          lx_thread_sleep
#define os_thread_relinquish        lx_thread_relinquish

#define os_thread_resume            lx_thread_resume

#define os_time_get                 lx_time_get

#define os_thread_sleep             lx_thread_sleep

#define os_thread_create            lx_thread_create
#define os_thread_suspend           lx_thread_suspend
#endif

#endif  


 
