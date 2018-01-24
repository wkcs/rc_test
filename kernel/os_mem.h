#ifndef __OS_MEM_H__
#define __OS_MEM_H__

#include "os_cpu.h"
#include "os_config.h"

typedef struct __mem_block_obj{
	 /*拥有此内存块的任务的id*/
	os_uint32 task_id;          
	 /*占用的块数*/
	os_uint32 block_num;		
	struct __mem_block_obj *next;
}_mem_block_obj;                 /*内存块管理对象*/

extern _mem_block_obj *mem_block_obj_list_head;

void mem_init(void);
void mem_free(void *ptr);
void mem_free_for_task_id(os_uint32 task_id);							
void *mem_malloc(os_uint32 size, os_uint32 task_id); 
os_uint16 mem_ret_memory_usage_rates(void);
os_uint32 mem_ret_free_memory(void);
os_uint32 mem_ret_total_memory(void);
os_int8 mem_set_task_id(void *ptr, os_uint32 task_id);
void *get_mem_block_bottom_ptr(void *ptr);
#endif
