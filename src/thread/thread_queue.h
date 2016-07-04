#ifndef MYSQL_SEED_THREAD_THREAD_QUEUE_H_
#define MYSQL_SEED_THREAD_THREAD_QUEUE_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "thread/thread_utils.h"	/* threads API */


/* typedefs, struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
struct ThreadQueueNode {
	void *payload;
	struct ThreadQueueNode *prev;
	struct ThreadQueueNode *next;
};

struct ThreadQueue {
	struct ThreadQueueNode *restrict head;
	struct ThreadQueueNode *restrict last;
	Mutex lock;
	ThreadCond node_ready;
	ThreadCond empty;
};

/* initialize
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_queue_init(struct ThreadQueue *const restrict queue)
{
	mutex_init(&queue->lock);
	thread_cond_init(&queue->node_ready);
	thread_cond_init(&queue->empty);
}

inline void
thread_queue_init_empty(struct ThreadQueue *const restrict queue)
{
	thread_queue_init(queue);

	queue->head = NULL;
	queue->last = NULL;
}

inline void
thread_queue_init_populated(struct ThreadQueue *const restrict queue,
			    struct ThreadQueueNode *restrict nodes,
			    void *restrict payloads,
			    const size_t payload_count,
			    const size_t payload_width)
{
	struct ThreadQueueNode *restrict next;

	thread_queue_init(queue);

	struct ThreadQueueNode *const restrict last = nodes
						    + (payload_count - 1l);
	last->next  = NULL;
	queue->last = last;

	nodes->prev = NULL;
	queue->head = nodes;


	while (1) {
		nodes->payload = payloads;

		if (nodes == last)
			return;

		next = nodes + 1l;

		nodes->next = next;
		next->prev  = nodes;

		nodes = next;

		payloads = ((char *restrict) payloads) + payload_width;
	}
}

/* locking the queue... */
inline bool
thread_queue_lock_status(struct ThreadQueue *const restrict queue)
{
	return mutex_lock_status(&queue->lock);
}

inline void
thread_queue_lock_muffle(struct ThreadQueue *const restrict queue)
{
	mutex_lock_muffle(&queue->lock);
}

inline bool
thread_queue_lock_report(struct ThreadQueue *const restrict queue,
			 const char *restrict *const restrict failure)
{
	return mutex_lock_report(&queue->lock,
				 failure);
}

inline void
thread_queue_lock_handle(struct ThreadQueue *const restrict queue,
			 Handler *const handle,
			 void *arg)
{
	mutex_lock_handle(&queue->lock,
			  handle,
			  arg);
}

inline void
thread_queue_lock_handle_cl(struct ThreadQueue *const restrict queue,
			    const struct HandlerClosure *const restrict cl)
{
	mutex_lock_handle_cl(&queue->lock,
			     cl);
}

/* locking the queue (no block on failure) */
inline bool
thread_queue_try_lock_status(struct ThreadQueue *const restrict queue)
{
	return mutex_try_lock_status(&queue->lock);
}

inline void
thread_queue_try_lock_muffle(struct ThreadQueue *const restrict queue)
{
	mutex_try_lock_muffle(&queue->lock);
}

inline enum ThreadFlag
thread_queue_try_lock_report(struct ThreadQueue *const restrict queue,
			     const char *restrict *const restrict failure)
{
	return mutex_try_lock_report(&queue->lock,
				     failure);
}

inline void
thread_queue_try_lock_handle(struct ThreadQueue *const restrict queue,
			     Handler *const handle,
			     void *arg)
{
	mutex_try_lock_handle(&queue->lock,
			      handle,
			      arg);
}

inline void
thread_queue_try_lock_handle_cl(struct ThreadQueue *const restrict queue,
				const struct HandlerClosure *const restrict cl)
{
	mutex_try_lock_handle_cl(&queue->lock,
				 cl);
}

/* unlocking the queue... */
inline bool
thread_queue_unlock_status(struct ThreadQueue *const restrict queue)
{
	return mutex_unlock_status(&queue->lock);
}

inline void
thread_queue_unlock_muffle(struct ThreadQueue *const restrict queue)
{
	mutex_unlock_muffle(&queue->lock);
}

inline bool
thread_queue_unlock_report(struct ThreadQueue *const restrict queue,
			   const char *restrict *const restrict failure)
{
	return mutex_unlock_report(&queue->lock,
				   failure);
}

inline void
thread_queue_unlock_handle(struct ThreadQueue *const restrict queue,
			   Handler *const handle,
			   void *arg)
{
	mutex_unlock_handle(&queue->lock,
			    handle,
			    arg);
}

inline void
thread_queue_unlock_handle_cl(struct ThreadQueue *const restrict queue,
			      const struct HandlerClosure *const restrict cl)
{
	mutex_unlock_handle_cl(&queue->lock,
			       cl);
}

/* LIFO peek
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_queue_peek(struct ThreadQueue *const restrict queue,
		  struct ThreadQueueNode *restrict *const restrict node)
{
	*node = queue->head;
}


/* LIFO push
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_queue_push_muffle(struct ThreadQueue *const restrict queue,
			 struct ThreadQueueNode *const restrict node)
{
	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_muffle(&queue->lock);

	node->next = NULL;

	if (queue->last == NULL) {
		node->prev  = NULL;
		queue->head = node;
		queue->last = node;

		thread_cond_signal_muffle(&queue->node_ready);
	} else {
		node->prev	  = queue->last;
		queue->last->next = node;
		queue->last	  = node;
	}

	mutex_unlock_muffle(&queue->lock);

	mutex_lock_try_catch_close();
}


inline void
thread_queue_push_handle_cl(struct ThreadQueue *const restrict queue,
			    struct ThreadQueueNode *const restrict node,
			    const struct HandlerClosure *const restrict h_cl)
{
	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_handle_cl(&queue->lock,
			     h_cl);

	node->next = NULL;

	if (queue->last == NULL) {
		node->prev  = NULL;
		queue->head = node;
		queue->last = node;

		thread_cond_signal_handle_cl(&queue->node_ready,
					     h_cl);
	} else {
		node->prev	  = queue->last;
		queue->last->next = node;
		queue->last	  = node;
	}

	mutex_unlock_handle_cl(&queue->lock,
			       h_cl);

	mutex_lock_try_catch_close();
}


/* LIFO pop
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_queue_pop_muffle(struct ThreadQueue *const restrict queue,
			struct ThreadQueueNode *restrict *const restrict node)
{
	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_muffle(&queue->lock);

	if (queue->head == NULL) {
		do {
			thread_cond_await_muffle(&queue->node_ready,
						 &queue->lock);
		} while (queue->head == NULL);

		*node = queue->head;

		queue->head = NULL;
		queue->last = NULL;

		thread_cond_signal_muffle(&queue->empty);
	} else {
		*node = queue->head;

		queue->head = (*node)->next;

		if (queue->head == NULL) {
			queue->last = NULL;

			thread_cond_signal_muffle(&queue->empty);
		} else {
			queue->head->prev = NULL;
		}
	}

	mutex_unlock_muffle(&queue->lock);

	mutex_lock_try_catch_close();
}

inline void
thread_queue_pop_handle_cl(struct ThreadQueue *const restrict queue,
			   struct ThreadQueueNode *restrict *const restrict node,
			   const struct HandlerClosure *const restrict h_cl)
{
	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_handle_cl(&queue->lock,
			     h_cl);

	if (queue->head == NULL) {
		do {
			thread_cond_await_handle_cl(&queue->node_ready,
						    &queue->lock,
						    h_cl);
		} while (queue->head == NULL);

		*node = queue->head;

		queue->head = NULL;
		queue->last = NULL;

		thread_cond_signal_handle_cl(&queue->empty,
					     h_cl);
	} else {
		*node = queue->head;

		queue->head = (*node)->next;

		if (queue->head == NULL) {
			queue->last = NULL;

			thread_cond_signal_handle_cl(&queue->empty,
						     h_cl);
		} else {
			queue->head->prev = NULL;
		}
	}

	mutex_unlock_handle_cl(&queue->lock,
			       h_cl);

	mutex_lock_try_catch_close();
}


/* random access delete
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_queue_remove_muffle(struct ThreadQueue *const restrict queue,
			   struct ThreadQueueNode *const restrict node)
{
	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_muffle(&queue->lock);

	if (node->prev == NULL) {
		if (node->next == NULL) {
			queue->head = NULL;
			queue->last = NULL;

			thread_cond_signal_muffle(&queue->empty);
		} else {
			node->next->prev = NULL;
			queue->head	 = node->next;
		}
	} else {
		node->prev->next = node->next;

		if (node->next == NULL)
			queue->last	 = node->prev;
		else
			node->next->prev = node->prev;
	}

	mutex_unlock_muffle(&queue->lock);

	mutex_lock_try_catch_close();
}

inline void
thread_queue_remove_handle_cl(struct ThreadQueue *const restrict queue,
			      struct ThreadQueueNode *const restrict node,
			      const struct HandlerClosure *const restrict h_cl)
{
	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_handle_cl(&queue->lock,
			     h_cl);

	if (node->prev == NULL) {
		if (node->next == NULL) {
			queue->head = NULL;
			queue->last = NULL;

			thread_cond_signal_handle_cl(&queue->empty,
						     h_cl);
		} else {
			node->next->prev = NULL;
			queue->head	 = node->next;
		}
	} else {
		node->prev->next = node->next;

		if (node->next == NULL)
			queue->last	 = node->prev;
		else
			node->next->prev = node->prev;
	}

	mutex_unlock_handle_cl(&queue->lock,
			       h_cl);

	mutex_lock_try_catch_close();
}


/* block until emptied
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_queue_await_empty_muffle(struct ThreadQueue *const restrict queue)
{
	if (queue->head == NULL)
		return;

	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_muffle(&queue->lock);

	do {
		thread_cond_await_muffle(&queue->empty,
					 &queue->lock);
	} while (queue->head != NULL);

	mutex_unlock_muffle(&queue->lock);

	mutex_lock_try_catch_close();
}

inline void
thread_queue_await_empty_handle_cl(struct ThreadQueue *const restrict queue,
				   const struct HandlerClosure *const restrict h_cl)
{
	if (queue->head == NULL)
		return;

	mutex_lock_try_catch_open(&queue->lock);

	mutex_lock_handle_cl(&queue->lock,
			     h_cl);

	do {
		thread_cond_await_handle_cl(&queue->empty,
					    &queue->lock,
					    h_cl);
	} while (queue->head != NULL);

	mutex_unlock_handle_cl(&queue->lock,
			       h_cl);

	mutex_lock_try_catch_close();
}

#endif /* ifndef MYSQL_SEED_THREAD_THREAD_QUEUE_H_ */
