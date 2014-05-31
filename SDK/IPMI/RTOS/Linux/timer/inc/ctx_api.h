

#ifndef  TX_API_H
#define  TX_API_H

#define POSIX_SOURCE 1
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <mqueue.h>
#include <asm/unistd.h>

#define SIGNO_EXPIRE SIGUSR1
#define SIGNO_SETTING SIGUSR2


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

#define TIME_TICKS 4
#define BILLION 100000000L
#define MILLION 1000000L
#define OS_MUTEX_ID                     0x4D555445UL
#define OS_SEMAPHORE_ID                 0x53454D41UL


#define OS_INTERNAL_TIMER			TX_INTERNAL_TIMER
#define OS_TIMER					TX_TIMER
#define OS_TIMER_PTR				TX_TIMER_PTR

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

#define OS_TIMER_ALREADY_ACTIVATE	0x20

#define TIMER_SETTING_QUEUE "/timer_setting_queue" 
#define SETTING_QUEUE_MAX_SET 10
#define OS_TIMER_HWND timer_t
#define OS_TIMER_CREATE		0x01
#define OS_TIMER_DELETE		0x02
#define OS_TIMER_ACTIVATE	0x03
#define	OS_TIMER_CHANGE		0x04
#define OS_TIMER_DEACTIVATE	0x05

typedef  struct TX_THREAD_STRUCT
{
		pthread_t  pthread;
		pthread_mutex_t mutex;
		pthread_cond_t cond;
		pthread_attr_t attr;
		

    CHAR_PTR    tx_thread_name;         
    UINT        tx_priority;            
    
    UINT        tx_pthread_created;     
    UINT        tx_delayed_suspend;     
    UINT        tx_suspending;          
    UINT				tx_terminates;					   
    
    void        (*tx_thread_entry)(ULONG);
    ULONG       tx_entry_parameter;
    

    struct TX_THREAD_STRUCT 
                *tx_created_next,    
                *tx_created_previous;
    
} TX_THREAD;

typedef TX_THREAD *     TX_THREAD_PTR;

typedef struct TX_INTERNAL_TIMER_STRUCT
{
	ULONG			tx_remaining_msec;
	ULONG			tx_re_initialize_msec;
	VOID			(*tx_timeout_function)(ULONG);
	ULONG			tx_timeout_param;

	sigset_t		*tx_mask;	
	UINT			tx_task;
	volatile UINT			tx_status;
	UINT			tx_auto_activate;
	char			tx_dead;

}TX_INTERNAL_TIMER;

typedef struct _TX_TIMER_STRUCT
{
	OS_TIMER_HWND			tx_timer_id;		
	pthread_t               tx_thread_id;		
	CHAR_PTR 				tx_timer_name;		

	TX_INTERNAL_TIMER		tx_timer_internal;	
}_TX_TIMER;

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

typedef struct TX_QUEUE_STRUCT
{
	
	mqd_t 					tx_queue_id;				
	CHAR_PTR 				tx_queue_name;				
	UINT 					tx_queue_message_size;		
	ULONG 					tx_queue_capacity;			
	ULONG 					tx_queue_enqueued;
	ULONG 					tx_queue_available_storage;
	UINT					tx_queue_front_send_num;  	
} TX_QUEUE;

typedef TX_QUEUE *		TX_QUEUE_PTR;

typedef struct TX_EVENT_FLAGS_GROUP_STRUCT
{
	pthread_cond_t			tx_event_flags_id;			
	CHAR_PTR 				tx_event_flags_name;		

	ULONG 					tx_event_flags_current;
	CHAR					tx_event_recheck; 
	pthread_mutex_t			tx_event_recheck_mutex; 
} TX_EVENT_FLAGS_GROUP;

typedef TX_EVENT_FLAGS_GROUP *      TX_EVENT_FLAGS_GROUP_PTR;

typedef struct TX_SEMAPHORE_STRUCT
{
	ULONG tx_semaphore_id;
	CHAR_PTR tx_semaphore_name;
	ULONG tx_semaphore_count;
	
	sem_t p_semaphore;
} TX_SEMAPHORE;

typedef TX_SEMAPHORE *      TX_SEMAPHORE_PTR;

typedef struct TX_MUTEX_STRUCT
{

    ULONG       tx_mutex_id;
    CHAR_PTR    tx_mutex_name;
    ULONG       tx_mutex_ownership_count;


    pthread_mutex_t	p_mutex;

} TX_MUTEX;

typedef TX_MUTEX *      TX_MUTEX_PTR;





UINT _lx_timer_create(_TX_TIMER *timer_ptr, char *name_ptr,
					 void (*expiration_function)(ULONG), ULONG expiration_input,
					 ULONG initial_msec, ULONG reschedule_msec,
					 UINT auto_activate);



UINT _lx_timer_activate(_TX_TIMER *timer_ptr);

UINT _lx_timer_change(	_TX_TIMER *timer_ptr,
						ULONG initial_msec, ULONG reschedule_msec);

UINT _lx_timer_deactivate(_TX_TIMER *timer_ptr);

UINT _lx_timer_delete(_TX_TIMER *timer_ptr);

UINT _lx_timer_info_get(	_TX_TIMER *timer_ptr, CHAR **name,
						UINT *active, ULONG *remaining_msec,
						ULONG *reschedule_msec,
						_TX_TIMER **next_timer);





#endif


