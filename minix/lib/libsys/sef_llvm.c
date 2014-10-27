#include "syslib.h"
#include <assert.h>
#include <unistd.h>
#include <minix/sysutil.h>

/* Stack refs definitions. */
extern char **environ;
extern char **env_argv;
extern int env_argc;

#define sef_llvm_stack_refs_save_one(P, T, R) { *((T*)P) = R; P += sizeof(T); }
#define sef_llvm_stack_refs_restore_one(P, T, R) { R = *((T*)P); P += sizeof(T); }

/*===========================================================================*
 *      	              sef_llvm_magic_enabled                         *
 *===========================================================================*/
int sef_llvm_magic_enabled()
{
    extern void __attribute__((weak)) magic_init();
    if (!magic_init)
        return 0;
    return 1;
}

/*===========================================================================*
 *      	                sef_llvm_real_brk                            *
 *===========================================================================*/
int sef_llvm_real_brk(char *newbrk)
{
    extern int __attribute__((weak)) _magic_real_brk(char*);
    if (!_magic_real_brk)
        return brk(newbrk);
    return _magic_real_brk(newbrk);
}

/*===========================================================================*
 *      	              sef_llvm_state_cleanup                         *
 *===========================================================================*/
int sef_llvm_state_cleanup()
{
    return OK;
}

/*===========================================================================*
 *      	                sef_llvm_dump_eval                           *
 *===========================================================================*/
void sef_llvm_dump_eval(char *expr)
{
    extern void __attribute__((weak)) _magic_dump_eval_bool(char*);
    if (!_magic_dump_eval_bool)
        return;
    return _magic_dump_eval_bool(expr);
}

/*===========================================================================*
 *      	               sef_llvm_eval_bool                            *
 *===========================================================================*/
int sef_llvm_eval_bool(char *expr, char *result)
{
    extern int __attribute__((weak)) magic_eval_bool(char*, char*);
    if (!magic_eval_bool)
        return 0;
    return magic_eval_bool(expr, result);
}

/*===========================================================================*
 *      	            sef_llvm_state_table_addr                        *
 *===========================================================================*/
void *sef_llvm_state_table_addr()
{
    extern void* __attribute__((weak)) _magic_vars_addr(void);
    if (!_magic_vars_addr)
        return NULL;
    return _magic_vars_addr();
}

/*===========================================================================*
 *      	            sef_llvm_state_table_size                        *
 *===========================================================================*/
size_t sef_llvm_state_table_size()
{
    extern size_t __attribute__((weak)) _magic_vars_size(void);
    if (!_magic_vars_size)
        return 0;
    return _magic_vars_size();
}

/*===========================================================================*
 *      	            sef_llvm_stack_refs_save                         *
 *===========================================================================*/
void sef_llvm_stack_refs_save(char *stack_buff)
{
    extern void __attribute__((weak)) st_stack_refs_save_restore(char*, int);
    char *p = stack_buff;

    sef_llvm_stack_refs_save_one(p, char**, environ);
    sef_llvm_stack_refs_save_one(p, char**, env_argv);
    sef_llvm_stack_refs_save_one(p, int, env_argc);

    if (st_stack_refs_save_restore)
        st_stack_refs_save_restore(p, 1);
}

/*===========================================================================*
 *      	           sef_llvm_stack_refs_restore                       *
 *===========================================================================*/
void sef_llvm_stack_refs_restore(char *stack_buff)
{
    extern void __attribute__((weak)) st_stack_refs_save_restore(char*, int);
    char *p = stack_buff;

    sef_llvm_stack_refs_restore_one(p, char**, environ);
    sef_llvm_stack_refs_restore_one(p, char**, env_argv);
    sef_llvm_stack_refs_restore_one(p, int, env_argc);

    if (st_stack_refs_save_restore)
        st_stack_refs_save_restore(p, 0);
}

/*===========================================================================*
 *      	            sef_llvm_state_transfer                          *
 *===========================================================================*/
int sef_llvm_state_transfer(sef_init_info_t *info)
{
    extern int __attribute__((weak)) _magic_state_transfer(sef_init_info_t *info);
    if (!_magic_state_transfer)
        return ENOSYS;
    return _magic_state_transfer(info);
}

/*===========================================================================*
 *      	             sef_llvm_ltckpt_enabled                         *
 *===========================================================================*/
int sef_llvm_ltckpt_enabled()
{
    extern int __attribute__((weak)) ltckpt_get_offset();
    if (!ltckpt_get_offset)
        return 0;
    return 1;
}

/*===========================================================================*
 *      	            sef_llvm_ltckpt_get_offset                       *
 *===========================================================================*/
int sef_llvm_get_ltckpt_offset()
{
    extern int __attribute__((weak)) ltckpt_get_offset();
    if (!ltckpt_get_offset)
        return 0;
    return ltckpt_get_offset();
}
