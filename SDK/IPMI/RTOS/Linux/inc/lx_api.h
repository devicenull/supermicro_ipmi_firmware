#ifndef _LX_API_H_
#define _LX_API_H_


#ifdef LX_API_C

UINT lx_queue_create(OS_QUEUE *queue_ptr, CHAR *name_ptr,
						UINT message_size, VOID *queue_start, ULONG queue_size);
UINT lx_queue_delete(OS_QUEUE *queue_ptr);
UINT lx_queue_flush(OS_QUEUE *queue_ptr);
UINT lx_queue_send(OS_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
UINT lx_queue_receive(OS_QUEUE *queue_ptr, VOID *destination_ptr, ULONG wait_option);
UINT lx_queue_info_get( OS_QUEUE *queue_ptr, CHAR **name,
						ULONG *enqueued, ULONG *available_storage,
						VOID **first_suspended, ULONG *suspended_count,
						OS_QUEUE **next_queue);
UINT lx_queue_front_send(OS_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);

UINT lx_event_flags_create(OS_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr);
UINT lx_event_flags_delete(OS_EVENT_FLAGS_GROUP *group_ptr);
UINT lx_event_flags_set(OS_EVENT_FLAGS_GROUP *group_ptr,ULONG flags_to_set,UINT set_option);
UINT lx_event_flags_get(OS_EVENT_FLAGS_GROUP *group_ptr,ULONG requested_flags, UINT get_option,
						ULONG *actual_flags_ptr, ULONG wait_option);
UINT lx_event_flags_info_get(	OS_EVENT_FLAGS_GROUP *group_ptr,
								CHAR **name, ULONG *current_flags,
								OS_THREAD **first_suspended,
								ULONG *suspended_count,
								OS_EVENT_FLAGS_GROUP **next_group);	


UINT lx_semaphore_create(	OS_SEMAPHORE *semaphore_ptr,
							CHAR *name_ptr, ULONG initial_count);						
UINT lx_semaphore_delete(OS_SEMAPHORE *semaphore_ptr);

UINT lx_semaphore_get(	OS_SEMAPHORE *semaphore_ptr,
						ULONG wait_option);						
UINT lx_semaphore_info_get(	OS_SEMAPHORE *semaphore_ptr, CHAR **name,
							ULONG *current_value,
							OS_THREAD **first_suspended,
							ULONG *suspended_count,
							OS_SEMAPHORE **next_semaphore);							
UINT lx_semaphore_put(OS_SEMAPHORE *semaphore_ptr);

UINT        lx_mutex_create(OS_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit);
UINT        lx_mutex_delete(OS_MUTEX *mutex_ptr);
UINT        lx_mutex_get(OS_MUTEX *mutex_ptr, ULONG wait_option);
UINT        lx_mutex_info_get(	OS_MUTEX *mutex_ptr, CHAR **name, ULONG *count, OS_THREAD **owner,
                    			OS_THREAD **first_suspended, ULONG *suspended_count, 
                    			OS_MUTEX **next_mutex);                    		
UINT        lx_mutex_put(OS_MUTEX *mutex_ptr);

void lx_tx_thread_sleep(int time);
void lx_tx_thread_relinquish();
UINT lx_timer_deactivate(OS_TIMER *timer_ptr);
UINT lx_thread_resume(OS_THREAD *thread_ptr);
UINT lx_timer_activate(OS_TIMER *timer_ptr);
ULONG lx_time_get(void);
UINT lx_timer_create(OS_TIMER *timer_ptr, char *name_ptr, 
                UINT expiration_function_id, ULONG expiration_input, ULONG initial_ticks,
                ULONG reschedule_ticks, UINT auto_activate);
UINT lx_timer_delete(OS_TIMER *timer_ptr);
UINT lx_timer_change(OS_TIMER *timer_ptr, ULONG initial_ticks, ULONG reschedule_ticks);
UINT lx_thread_sleep(ULONG timer_ticks);
void lx_thread_relinquish(void);
UINT lx_thread_create(OS_THREAD *thread_ptr, CHAR *name_ptr, 
                void (*entry_function)(ULONG), ULONG entry_input,
                void *stack_start, ULONG stack_size, 
                UINT priority, UINT preempt_threshold, 
                ULONG time_slice, UINT auto_start);        
UINT lx_thread_suspend(OS_THREAD *thread_ptr);
ULONG lx_time_get(void);

#else
extern UINT lx_queue_create(OS_QUEUE *queue_ptr, CHAR *name_ptr,
						UINT message_size, VOID *queue_start, ULONG queue_size);
extern UINT lx_queue_delete(OS_QUEUE *queue_ptr);
extern UINT lx_queue_flush(OS_QUEUE *queue_ptr);
extern UINT lx_queue_send(OS_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
extern UINT lx_queue_receive(OS_QUEUE *queue_ptr, VOID *destination_ptr, ULONG wait_option);
extern UINT lx_queue_info_get( OS_QUEUE *queue_ptr, CHAR **name,
						ULONG *enqueued, ULONG *available_storage,
						VOID **first_suspended, ULONG *suspended_count,
						OS_QUEUE **next_queue);
extern UINT lx_queue_front_send(OS_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);


extern UINT lx_event_flags_create(OS_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr);
extern UINT lx_event_flags_delete(OS_EVENT_FLAGS_GROUP *group_ptr);
extern UINT lx_event_flags_set(OS_EVENT_FLAGS_GROUP *group_ptr,ULONG flags_to_set,UINT set_option);
extern UINT lx_event_flags_get(OS_EVENT_FLAGS_GROUP *group_ptr,ULONG requested_flags, UINT get_option,
							ULONG *actual_flags_ptr, ULONG wait_option);
extern UINT lx_event_flags_info_get(	OS_EVENT_FLAGS_GROUP *group_ptr,
								CHAR **name, ULONG *current_flags,
								OS_THREAD **first_suspended,
								ULONG *suspended_count,
								OS_EVENT_FLAGS_GROUP **next_group);	

extern UINT lx_semaphore_create(	OS_SEMAPHORE *semaphore_ptr,
							CHAR *name_ptr, ULONG initial_count);						
extern UINT lx_semaphore_delete(OS_SEMAPHORE *semaphore_ptr);
extern UINT lx_semaphore_get(	OS_SEMAPHORE *semaphore_ptr,
						ULONG wait_option);						
extern UINT lx_semaphore_info_get(	OS_SEMAPHORE *semaphore_ptr, CHAR **name,
							ULONG *current_value,
							OS_THREAD **first_suspended,
							ULONG *suspended_count,
							OS_SEMAPHORE **next_semaphore);							
extern UINT lx_semaphore_put(OS_SEMAPHORE *semaphore_ptr);

extern UINT        lx_mutex_create(OS_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit);
extern UINT        lx_mutex_delete(OS_MUTEX *mutex_ptr);
extern UINT        lx_mutex_get(OS_MUTEX *mutex_ptr, ULONG wait_option);
extern UINT        lx_mutex_info_get(	OS_MUTEX *mutex_ptr, CHAR **name, ULONG *count, OS_THREAD **owner,
                    			OS_THREAD **first_suspended, ULONG *suspended_count, 
                    			OS_MUTEX **next_mutex);                    			
extern UINT        lx_mutex_put(OS_MUTEX *mutex_ptr);



extern void lx_tx_thread_sleep(int time);
extern void lx_tx_thread_relinquish();
extern STATUS TimerInit();
extern UINT lx_timer_deactivate(OS_TIMER *timer_ptr);
extern UINT lx_thread_resume(OS_THREAD *thread_ptr);
extern UINT lx_timer_activate(OS_TIMER *timer_ptr);
extern UINT lx_queue_receive(OS_QUEUE *queue_ptr, void *destination_ptr, ULONG wait_option);
extern UINT lx_queue_send(OS_QUEUE *queue_ptr, void *source_ptr, ULONG wait_option);
extern ULONG lx_time_get(void);
extern UINT  lx_timer_create(OS_TIMER *timer_ptr, char *name_ptr, 
                 UINT expiration_function_id, ULONG expiration_input, ULONG initial_ticks,
                ULONG reschedule_ticks, UINT auto_activate);
extern UINT lx_timer_change(OS_TIMER *timer_ptr, ULONG initial_ticks, ULONG reschedule_ticks);
extern UINT lx_timer_delete(OS_TIMER *timer_ptr);
extern UINT lx_thread_sleep(ULONG timer_ticks);
extern void lx_thread_relinquish(void);  
extern UINT lx_thread_create(OS_THREAD *thread_ptr, CHAR *name_ptr, 
                void (*entry_function)(ULONG), ULONG entry_input,
                void *stack_start, ULONG stack_size, 
                UINT priority, UINT preempt_threshold, 
                ULONG time_slice, UINT auto_start);   
extern UINT lx_thread_suspend(OS_THREAD *thread_ptr);                                  
extern ULONG lx_time_get(void);   
#endif



#endif

