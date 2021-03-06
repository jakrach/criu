#ifndef __CR_RST_INFO_H__
#define __CR_RST_INFO_H__

#include "lock.h"
#include "common/list.h"
#include "vma.h"

struct task_entries {
	int nr_threads, nr_tasks, nr_helpers;
	futex_t nr_in_progress;
	futex_t start;
	atomic_t cr_err;
	mutex_t userns_sync_lock;
};

struct fdt {
	int			nr;		/* How many tasks share this fd table */
	pid_t			pid;		/* Who should restore this fd table */
	/*
	 * The fd table is ready for restoing, if fdt_lock is equal to nr
	 * The fdt table was restrored, if fdt_lock is equal to nr + 1
	 */
	futex_t			fdt_lock;
};

struct _MmEntry;

struct rst_info {
	struct list_head	used;
	struct list_head	fds;
	struct list_head	eventpoll;
	struct list_head	tty_slaves;
	struct list_head	tty_ctty;

	void			*premmapped_addr;
	unsigned long		premmapped_len;
	unsigned long		clone_flags;

	void			*munmap_restorer;

	int service_fd_id;
	struct fdt		*fdt;

	struct vm_area_list	vmas;
	struct _MmEntry		*mm;

	u32			cg_set;

	union {
		struct pstree_item	*pgrp_leader;
		futex_t			pgrp_set;
	};

	struct file_desc	*cwd;
	struct file_desc	*root;
	bool			has_umask;
	u32			umask;

	/*
	 * We set this flag when process has seccomp filters
	 * so that we know to suspend them before we unmap the
	 * restorer blob.
	 */
	bool			has_seccomp;

	void			*breakpoint;
};

#endif /* __CR_RST_INFO_H__ */
