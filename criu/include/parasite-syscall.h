#ifndef __CR_PARASITE_SYSCALL_H__
#define __CR_PARASITE_SYSCALL_H__

#include "pid.h"
#include "common/list.h"
#include "config.h"
#include "asm/parasite-syscall.h"

#define BUILTIN_SYSCALL_SIZE	8

struct parasite_dump_thread;
struct parasite_dump_misc;
struct parasite_drain_fd;
struct vm_area_list;
struct pstree_item;
struct _CredsEntry;
struct _CoreEntry;
struct list_head;
struct cr_imgset;
struct fd_opts;
struct pid;
struct parasite_dump_cgroup_args;
struct rt_sigframe;

struct thread_ctx {
	k_rtsigset_t		sigmask;
	user_regs_struct_t	regs;
};

struct parasite_ctl;

extern int parasite_dump_sigacts_seized(struct parasite_ctl *ctl, struct cr_imgset *cr_imgset);
extern int parasite_dump_itimers_seized(struct parasite_ctl *ctl, struct pstree_item *);

struct proc_posix_timers_stat;
extern int parasite_dump_posix_timers_seized(struct proc_posix_timers_stat *proc_args,
		struct parasite_ctl *ctl, struct pstree_item *);

extern int parasite_send_fd(struct parasite_ctl *ctl, int fd);

extern int parasite_dump_misc_seized(struct parasite_ctl *ctl, struct parasite_dump_misc *misc);
extern int parasite_dump_creds(struct parasite_ctl *ctl, struct _CredsEntry *ce);
extern int parasite_dump_thread_leader_seized(struct parasite_ctl *ctl, int pid, struct _CoreEntry *core);
extern int parasite_dump_thread_seized(struct parasite_ctl *ctl, int id,
					struct pid *tid, struct _CoreEntry *core);
extern int dump_thread_core(int pid, CoreEntry *core, bool native,
					const void *dump_thread);

extern int parasite_drain_fds_seized(struct parasite_ctl *ctl,
					struct parasite_drain_fd *dfds, int nr_fds, int off,
					int *lfds, struct fd_opts *flags);
extern int parasite_get_proc_fd_seized(struct parasite_ctl *ctl);

extern struct parasite_ctl *parasite_infect_seized(pid_t pid,
						   struct pstree_item *item,
						   struct vm_area_list *vma_area_list);
extern void parasite_ensure_args_size(unsigned long sz);
extern unsigned long get_exec_start(struct vm_area_list *);

extern int parasite_dump_cgroup(struct parasite_ctl *ctl, struct parasite_dump_cgroup_args *cgroup);

extern struct parasite_tty_args *parasite_dump_tty(struct parasite_ctl *ctl, int fd, int type);

extern int parasite_init_threads_seized(struct parasite_ctl *ctl, struct pstree_item *item);
extern int parasite_fini_threads_seized(struct parasite_ctl *ctl);

extern int syscall_seized(struct parasite_ctl *ctl, int nr, unsigned long *ret,
			  unsigned long arg1, unsigned long arg2,
			  unsigned long arg3, unsigned long arg4,
			  unsigned long arg5, unsigned long arg6);

extern bool arch_can_dump_task(pid_t pid);

#endif /* __CR_PARASITE_SYSCALL_H__ */
