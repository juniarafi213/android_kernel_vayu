// SPDX-License-Identifier: GPL-2.0
/*
 * Anxiety I/O scheduler
 *
 * Prioritizes latency over everything else. Uses FIFO queues
 * implemented as doubly linked lists and prioritizes reads over writes.
 *
 * Copyright (C) 2019-2020 Tyler Nijmeh <tyler@nijmeh.com>
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct anxiety_data {
	struct list_head queue[2];
};

enum {
	ANXIETY_READ = 0,
	ANXIETY_WRITE = 1,
};

static void anxiety_merged_requests(struct request_queue *q, struct request *rq,
				    struct request *next)
{
	list_del_init(&next->queuelist);
}

static int anxiety_dispatch(struct request_queue *q, int force)
{
	struct anxiety_data *nd = q->elevator->elevator_data;
	struct request *rq;

	/* Always serve read requests first to minimize UI latency */
	rq = list_first_entry_or_null(&nd->queue[ANXIETY_READ], struct request, queuelist);
	if (rq) {
		list_del_init(&rq->queuelist);
		elv_dispatch_sort(q, rq);
		return 1;
	}

	/* Then serve write requests */
	rq = list_first_entry_or_null(&nd->queue[ANXIETY_WRITE], struct request, queuelist);
	if (rq) {
		list_del_init(&rq->queuelist);
		elv_dispatch_sort(q, rq);
		return 1;
	}

	return 0;
}

static void anxiety_add_request(struct request_queue *q, struct request *rq)
{
	struct anxiety_data *nd = q->elevator->elevator_data;
	const int rw = (rq_data_dir(rq) == READ) ? ANXIETY_READ : ANXIETY_WRITE;

	list_add_tail(&rq->queuelist, &nd->queue[rw]);
}

static struct request *
anxiety_former_request(struct request_queue *q, struct request *rq)
{
	struct anxiety_data *nd = q->elevator->elevator_data;
	const int rw = (rq_data_dir(rq) == READ) ? ANXIETY_READ : ANXIETY_WRITE;

	if (rq->queuelist.prev == &nd->queue[rw])
		return NULL;

	return list_prev_entry(rq, queuelist);
}

static struct request *
anxiety_latter_request(struct request_queue *q, struct request *rq)
{
	struct anxiety_data *nd = q->elevator->elevator_data;
	const int rw = (rq_data_dir(rq) == READ) ? ANXIETY_READ : ANXIETY_WRITE;

	if (rq->queuelist.next == &nd->queue[rw])
		return NULL;

	return list_next_entry(rq, queuelist);
}

static int anxiety_init_queue(struct request_queue *q, struct elevator_type *e)
{
	struct anxiety_data *nd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	eq->elevator_data = nd;

	INIT_LIST_HEAD(&nd->queue[ANXIETY_READ]);
	INIT_LIST_HEAD(&nd->queue[ANXIETY_WRITE]);

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

static void anxiety_exit_queue(struct elevator_queue *e)
{
	struct anxiety_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue[ANXIETY_READ]));
	BUG_ON(!list_empty(&nd->queue[ANXIETY_WRITE]));
	kfree(nd);
}

static struct elevator_type elevator_anxiety = {
	.ops.sq = {
		.elevator_merge_req_fn		= anxiety_merged_requests,
		.elevator_dispatch_fn		= anxiety_dispatch,
		.elevator_add_req_fn		= anxiety_add_request,
		.elevator_former_req_fn		= anxiety_former_request,
		.elevator_latter_req_fn		= anxiety_latter_request,
		.elevator_init_fn		= anxiety_init_queue,
		.elevator_exit_fn		= anxiety_exit_queue,
	},
	.elevator_name = "anxiety",
	.elevator_owner = THIS_MODULE,
};

static int __init anxiety_init(void)
{
	return elv_register(&elevator_anxiety);
}

static void __exit anxiety_exit(void)
{
	elv_unregister(&elevator_anxiety);
}

module_init(anxiety_init);
module_exit(anxiety_exit);

MODULE_AUTHOR("Tyler Nijmeh");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Anxiety I/O scheduler");
