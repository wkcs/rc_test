#include "os_mem.h"
#include "os_define.h"

_mem_block_obj *mem_block_obj_list_head;

static os_uint32 mem_block_free_num;              /*空闲内存块数目*/

/*内存池,按最小块大小对齐*/
os_uint8 __attribute__ ((aligned (MEM_BLOCK_SIZE))) membase[MEM_MAX_SIZE];


static os_int8 mem_clr(void *addr_start, void *addr_end)
{
    os_cpu_sr *addr_now;
    for (addr_now = (os_cpu_sr *)addr_start; addr_now < (os_cpu_sr *)addr_end; addr_now++)
        *addr_now = 0;
    return 0;
}

static void mem_dev_init(void)
{
    os_uint32 i;
    _mem_block_obj *mem_block_obj_list;
    for (i = 0; i < MEM_BLOCK_NUM - 1; i++)
    {
        mem_block_obj_list = (_mem_block_obj *)(membase + MEM_BLOCK_SIZE * i);
        mem_block_obj_list->task_id = 0;
        mem_block_obj_list->block_num = 0;
        mem_block_obj_list->next = (_mem_block_obj *)(membase + MEM_BLOCK_SIZE * (i + 1));
    }
    mem_block_obj_list = (_mem_block_obj *)(membase + MEM_BLOCK_SIZE * i);
    mem_block_obj_list->task_id = 0;
    mem_block_obj_list->block_num = 0;
    mem_block_obj_list->next = NULL;
}

void mem_init(void)
{
    mem_block_obj_list_head = (_mem_block_obj *)membase;
    mem_block_free_num = MEM_BLOCK_NUM;
    mem_clr((void *)membase, (void *)(membase + MEM_MAX_SIZE));
    mem_dev_init();
}

void mem_free(void *ptr)
{
    os_uint32 i, block_num;
    _mem_block_obj *mem_block_obj;
    if ((ptr > (void *)membase) && (ptr < (void *)(membase + MEM_MAX_SIZE)) \
    && (((os_cpu_sr)ptr - (os_cpu_sr)(mem_block_obj_list_head + 1)) % MEM_BLOCK_SIZE == 0))           
    {
        mem_block_obj = (_mem_block_obj *)ptr - 1;
        block_num = mem_block_obj->block_num;
        if (block_num > 1) {
            for (i = 0; i < block_num - 1; i++)
            {
                mem_block_obj -> task_id = 0;
                mem_block_obj -> block_num = 0;
                mem_block_obj -> next = (_mem_block_obj *)((os_uint8 *)mem_block_obj + MEM_BLOCK_SIZE);
                mem_block_obj = mem_block_obj -> next;
            }
            mem_block_obj->task_id = 0;
            mem_block_obj->block_num = 0;
            if (mem_block_obj == (_mem_block_obj *)(membase + MEM_MAX_SIZE - 32))
                mem_block_obj -> next = NULL;
            else
                mem_block_obj -> next = (_mem_block_obj *)((os_uint8 *)mem_block_obj + MEM_BLOCK_SIZE);
        } else if (block_num > 0) {
            mem_block_obj -> task_id = 0;
            mem_block_obj -> block_num = 0;
        }
        mem_block_free_num += block_num;
        if (mem_block_free_num > MEM_BLOCK_NUM)
            mem_block_free_num = MEM_BLOCK_NUM;
    }
}

/*
 * 根据任务id释放内存块
 */
void mem_free_for_task_id(os_uint32 task_id)
{
    os_uint32 i, block_num;
    _mem_block_obj *mem_block_obj, *mem_block_obj_list;
    /*遍历内存管理表，找到相应的内存块并释放*/
    mem_block_obj_list = mem_block_obj_list_head;
    while (mem_block_obj_list != NULL) {
        if ((mem_block_obj_list -> task_id == task_id) && (mem_block_obj_list -> block_num > 0)) {
            mem_block_obj = mem_block_obj_list;
            mem_block_obj_list = mem_block_obj_list -> next;
            block_num = mem_block_obj -> block_num;
            if (block_num > 1) {
                for (i = 0; i < block_num - 1; i++)
                {
                    mem_block_obj -> task_id = 0;
                    mem_block_obj -> block_num = 0;
                    mem_block_obj -> next = (_mem_block_obj *)((os_uint8 *)mem_block_obj + MEM_BLOCK_SIZE);
                    mem_block_obj = mem_block_obj -> next;
                }
                mem_block_obj -> task_id = 0;
                mem_block_obj -> block_num = 0;
                if (mem_block_obj == (_mem_block_obj *)(membase + MEM_MAX_SIZE - 32))
                    mem_block_obj -> next = NULL;
                else
                    mem_block_obj -> next = (_mem_block_obj *)((os_uint8 *)mem_block_obj + MEM_BLOCK_SIZE);
            } else {
                mem_block_obj -> task_id = 0;
                mem_block_obj -> block_num = 0;
            }
            mem_block_free_num += block_num;
            if (mem_block_free_num > MEM_BLOCK_NUM)
                mem_block_free_num = MEM_BLOCK_NUM;
        } else 
            mem_block_obj_list = mem_block_obj_list -> next;
    }
}

void *mem_malloc(os_uint32 size, os_uint32 task_id)
{
    os_uint32 i, need_block_num;
    _mem_block_obj *mem_block_obj, *mem_block_obj_list;
    if (size < MEM_BLOCK_SIZE - sizeof(_mem_block_obj))
        need_block_num = 1;
    else
    {
        i = size - MEM_BLOCK_SIZE + sizeof(_mem_block_obj);
        if (i % MEM_BLOCK_SIZE == 0)
            need_block_num = i / MEM_BLOCK_SIZE + 1;
        else
            need_block_num = i / MEM_BLOCK_SIZE + 2;
    }

    /*遍历链表查找满足要求的内存区域*/
    mem_block_obj_list = mem_block_obj_list_head;
    i = 0;
    while (mem_block_obj_list != NULL)
    {
        if (mem_block_obj_list->block_num == 0)
        {
            if (i == 0)
                mem_block_obj = mem_block_obj_list;
            i++;
        }
        else
            i = 0;
        if (i == need_block_num)
            break;
        else
            mem_block_obj_list = mem_block_obj_list->next;
    }
    if (i < need_block_num)
        return NULL;
    else
    {
        mem_block_obj->task_id = task_id;
        mem_block_obj->block_num = need_block_num;
        mem_block_obj->next = mem_block_obj_list->next;
        mem_clr((void *)(mem_block_obj + 1), (void *)(mem_block_obj_list->next));
        mem_block_free_num -= need_block_num;
        return (void *)(mem_block_obj + 1);
    }
}

os_uint16 mem_ret_memory_usage_rates(void)
{
    return (os_uint16)((double)(MEM_BLOCK_NUM - mem_block_free_num) / (double)MEM_BLOCK_NUM * 10000);
}

os_uint32 mem_ret_free_memory(void)
{
    return mem_block_free_num * MEM_BLOCK_SIZE - sizeof(_mem_block_obj);
}

os_uint32 mem_ret_total_memory(void)
{
    return MEM_MAX_SIZE - sizeof(_mem_block_obj);
}

os_int8 mem_set_task_id(void *ptr, os_uint32 task_id)
{
    if ((ptr > (void *)membase) && (ptr < (void *)(membase + MEM_MAX_SIZE)) && \
    (((os_cpu_sr)ptr - (os_cpu_sr)(mem_block_obj_list_head + 1)) % MEM_BLOCK_SIZE == 0)) {             /*判断内存地址是否合法*/
        if (((_mem_block_obj *)ptr - 1) -> block_num > 0) {                    /*判断内存是否分配*/
            ((_mem_block_obj *)ptr - 1) -> task_id = task_id;
            return 0;
        } else
            return -2;
    } else 
        return -1;
}

/*
 * 获取内存块最底部指针，一般用于下增长的栈使用
 */
void *get_mem_block_bottom_ptr(void *ptr)
{
    if ((ptr > (void *)membase) && (ptr < (void *)(membase + MEM_MAX_SIZE)) && \
    (((os_cpu_sr)ptr - (os_cpu_sr)(mem_block_obj_list_head + 1)) % MEM_BLOCK_SIZE == 0)) {             /*判断内存地址是否合法*/
        if (((_mem_block_obj *)ptr - 1) -> block_num > 0) {                    /*判断内存是否分配*/
            return (void *)((os_cpu_sr *)((os_uint8 *)((_mem_block_obj *)ptr - 1) + (((_mem_block_obj *)ptr - 1) -> block_num) * MEM_BLOCK_SIZE) - 1);
        } else
            return NULL;
    } else
        return NULL;
}

