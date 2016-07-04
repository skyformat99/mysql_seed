#ifndef MYSQL_SEED_THREAD_THREAD_POOL_H_
#define MYSQL_SEED_THREAD_THREAD_POOL_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "thread/thread_utils.h"	/* threads API */
#include "thread/thread_queue.h"	/* ThreadQueue */
#include "thread/thread_log.h"		/* ThreadLog */

#define WORKERS_MAX 16lu

/* failure messages
 *─────────────────────────────────────────────────────────────────────────── */
#define WORKER_SPAWN_FAILURE_MESSAGE					\
"\n\nfailed to spawn new worker\nreason:\n"				\
"\t'WORKERS_MAX' (" EXPAND_STRINGIFY(SEED_WORKERS_MAX) ") "		\
"exceeded\n"

#define THREAD_POOL_EXIT_FAILURE_MESSAGE ANSI_BRIGHT ANSI_RED_BG	\
ANSI_YELLOW "\nTHREAD POOL EXITING ON FAILURE\n" ANSI_RESET

#define THREAD_POOL_EXIT_SUCCESS_MESSAGE ANSI_BRIGHT ANSI_WHITE_BG	\
ANSI_GREEN "\nTHREAD POOL EXITING ON SUCCESS\n" ANSI_RESET


/* typedefs
 *─────────────────────────────────────────────────────────────────────────── */
struct ThreadPool;

typedef void
ThreadPoolEvent(struct ThreadPool *const restrict pool);

union TaskFun {
	Routine *awaitable;
	Procedure *independent;
};

struct Task {
	union TaskFun fun;
	void *arg;
	void *result;
	bool busy;
	ThreadCond done;
	Mutex processing;
	struct ThreadQueueNode *node;
};

struct Worker {
	struct Task *task;
	ThreadKey key;
	Thread thread;
	struct ThreadQueueNode *node;
	struct ThreadPool *pool;
	struct HandlerClosure fail_cl;
};

struct Supervisor {
	Thread thread;
	ThreadKey key;
	ThreadPoolEvent *event;
	ThreadCond trigger;
	Mutex listening;
	struct ThreadPool *pool;
	struct HandlerClosure fail_cl;
};

struct TaskQueues {
	struct ThreadQueue vacant;
	struct ThreadQueue awaitable;
	struct ThreadQueue independent;
	struct ThreadQueue complete;
};


struct ThreadPool {
	bool busy;
	ThreadCond done;
	Mutex processing;
	struct Supervisor supervisor;
	struct ThreadQueue worker_queue;
	struct TaskQueues task_queues;
	struct ThreadLog log;
};

/* Supervisor operations, ThreadPoolEvents
 *─────────────────────────────────────────────────────────────────────────── */
/* should only be called from supervisor thread */
inline void
supervisor_cancel_workers_failure(struct ThreadPool *const restrict pool)
{
	struct ThreadQueueNode *restrict node;
	struct Worker *restrict worker;

	mutex_lock_try_catch_open(&pool->worker_queue.lock)

	mutex_lock_muffle(&pool->worker_queue.lock);

	thread_queue_peek(&pool->worker_queue,
			  &node);

	while (node != NULL) {
		worker = (struct Worker *restrict) node->payload;

		thread_cancel_muffle(worker->thread);

		node = node->next;
	}

	mutex_unlock_muffle(&pool->worker_queue.lock);

	mutex_lock_try_catch_close();
}

inline void
supervisor_cancel_workers_success(struct ThreadPool *const restrict pool)
{
	struct ThreadQueueNode *restrict node;
	struct Worker *restrict worker;

	mutex_lock_try_catch_open(&pool->worker_queue.lock)

	mutex_lock_handle_cl(&pool->worker_queue.lock,
			     &pool->supervisor.fail_cl);

	thread_queue_peek(&pool->worker_queue,
			  &node);

	while (node != NULL) {
		worker = (struct Worker *restrict) node->payload;

		thread_cancel_handle_cl(worker->thread,
					&pool->supervisor.fail_cl);

		node = node->next;
	}

	mutex_unlock_handle_cl(&pool->worker_queue.lock,
			       &pool->supervisor.fail_cl);

	mutex_lock_try_catch_close();
}

/* should only be called from supervisor thread */
void
supervisor_exit_cleanup(void *arg);
/* should only be called from supervisor thread */
inline void
supervisor_do_exit_failure(struct ThreadPool *const restrict pool)
{
	/* cancel all living workers */
	supervisor_cancel_workers_failure(pool);

	/* close the log with a failure message and dump to stderr */
	mutex_lock_try_catch_open(&pool->log.lock);

	mutex_lock_muffle(&pool->log.lock);

	thread_log_append_string(&pool->log,
				 THREAD_POOL_EXIT_FAILURE_MESSAGE);

	thread_log_append_close(&pool->log);

	thread_log_dump_muffle(&pool->log,
			       STDERR_FILENO);

	mutex_unlock_muffle(&pool->log.lock);

	mutex_lock_try_catch_close();

	/* wake up any threads waiting on 'thread_pool_process' */
	mutex_lock_try_catch_open(&pool->processing);

	mutex_lock_muffle(&pool->processing);

	pool->busy = false;

	thread_cond_signal_muffle(&pool->done);

	mutex_unlock_muffle(&pool->processing);

	mutex_lock_try_catch_close();
}

inline void
supervisor_do_exit_success(struct ThreadPool *const restrict pool)
{
	/* cancel all living workers */
	supervisor_cancel_workers_success(pool);

	/* close the log with a success message and dump to stdout */
	mutex_lock_try_catch_open(&pool->log.lock);

	mutex_lock_handle_cl(&pool->log.lock,
			     &pool->supervisor.fail_cl);

	thread_log_append_string(&pool->log,
				 THREAD_POOL_EXIT_SUCCESS_MESSAGE);

	thread_log_append_close(&pool->log);

	thread_log_dump_handle_cl(&pool->log,
				  STDOUT_FILENO,
				  &pool->supervisor.fail_cl);

	mutex_unlock_handle_cl(&pool->log.lock,
			       &pool->supervisor.fail_cl);

	mutex_lock_try_catch_close();

	/* ensure failure destructor functions aren't called on exit */
	thread_key_delete_handle_cl(pool->supervisor.key,
				    &pool->supervisor.fail_cl);

	/* exit */
	thread_exit_detached();
	__builtin_unreachable();
}

/* should only be called from supervisor thread */
void
supervisor_exit_on_failure(void *arg,
			   const char *restrict failure)
__attribute__((noreturn));

/* should only be called from supervisor thread */
inline void
supervisor_oversee_completion(struct ThreadPool *const restrict pool)
{
	/* wait for workers to complete tasks */
	thread_queue_await_empty_handle_cl(&pool->task_queues.awaitable,
					   &pool->supervisor.fail_cl);

	thread_queue_await_empty_handle_cl(&pool->task_queues.independent,
					   &pool->supervisor.fail_cl);

	thread_queue_await_empty_handle_cl(&pool->task_queues.complete,
					   &pool->supervisor.fail_cl);

	/* wake up any threads waiting on 'thread_pool_process' */
	mutex_lock_try_catch_open(&pool->processing);

	mutex_lock_handle_cl(&pool->processing,
			     &pool->supervisor.fail_cl);

	pool->busy = false;

	thread_cond_signal_handle_cl(&pool->done,
				     &pool->supervisor.fail_cl);

	mutex_unlock_handle_cl(&pool->processing,
			       &pool->supervisor.fail_cl);

	mutex_lock_try_catch_close();

	/* cancel workers, cleanup, exit */
	supervisor_do_exit_success(pool);
}


inline void
supervisor_init(struct Supervisor *const restrict supervisor,
		struct ThreadPool *const restrict pool)
{
	supervisor->event = NULL;
	thread_cond_init(&supervisor->trigger);
	mutex_init(&supervisor->listening);

	supervisor->fail_cl.handle = &supervisor_exit_on_failure;
	supervisor->fail_cl.arg	   = pool;

	supervisor->pool = pool;
}

inline void
supervisor_listen(struct Supervisor *const restrict supervisor)
{
	mutex_lock_try_catch_open(&supervisor->listening);

	while (1) {
		mutex_lock_handle_cl(&supervisor->listening,
				     &supervisor->fail_cl);

		while (supervisor->event == NULL)
			thread_cond_await_handle_cl(&supervisor->trigger,
						    &supervisor->listening,
						    &supervisor->fail_cl);

		supervisor->event(supervisor->pool);

		mutex_unlock_handle_cl(&supervisor->listening,
				       &supervisor->fail_cl);
	}

	mutex_lock_try_catch_close();
}

inline void
supervisor_signal_muffle(struct Supervisor *const restrict supervisor,
			 ThreadPoolEvent *const event)
{
	mutex_lock_try_catch_open(&supervisor->listening);

	mutex_lock_muffle(&supervisor->listening);

	supervisor->event = event;

	thread_cond_signal_muffle(&supervisor->trigger);

	mutex_unlock_muffle(&supervisor->listening);

	mutex_lock_try_catch_close();
}

inline void
supervisor_signal_handle_cl(struct Supervisor *const restrict supervisor,
			    ThreadPoolEvent *const event,
			    const struct HandlerClosure *const restrict fail_cl)
{
	mutex_lock_try_catch_open(&supervisor->listening);

	mutex_lock_handle_cl(&supervisor->listening,
			     fail_cl);

	supervisor->event = event;

	thread_cond_signal_handle_cl(&supervisor->trigger,
				     fail_cl);

	mutex_unlock_handle_cl(&supervisor->listening,
			       fail_cl);

	mutex_lock_try_catch_close();
}


/* Worker operations
 *─────────────────────────────────────────────────────────────────────────── */
void
worker_exit_cleanup(void *arg);

void
worker_exit_on_failure(void *arg,
		       const char *restrict failure)
__attribute__((noreturn));

inline void
worker_queue_init(struct ThreadQueue *const restrict queue,
		  struct ThreadQueueNode *restrict node,
		  struct ThreadQueueNode *const restrict last,
		  struct Worker *restrict worker,
		  struct ThreadPool *const restrict pool)
{
	struct ThreadQueueNode *restrict next;

	thread_queue_init(queue);

	last->next  = NULL;
	queue->last = last;

	node->prev  = NULL;
	queue->head = node;

	while (1) {
		worker->fail_cl.handle = &worker_exit_on_failure;
		worker->fail_cl.arg    = pool;

		worker->pool = pool;
		worker->node = node;

		node->payload = (void *) worker;

		if (node == last)
			return;

		next = node + 1l;

		node->next = next;
		next->prev = node;

		node = next;
		++worker;
	}
}

/* TaskQueues operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
task_queues_init_default(struct TaskQueues *const restrict task_queues)
{
	thread_queue_init_empty(&task_queues->awaitable);
	thread_queue_init_empty(&task_queues->independent);
	thread_queue_init_empty(&task_queues->complete);
}

inline void
vacant_task_queue_init(struct ThreadQueue *const restrict vacant,
		       struct ThreadQueueNode *restrict node,
		       struct ThreadQueueNode *const restrict last,
		       struct Task *restrict task)
{
	struct ThreadQueueNode *restrict next;

	thread_queue_init(vacant);

	last->next   = NULL;
	vacant->last = last;

	node->prev   = NULL;
	vacant->head = node;

	while (1) {
		task->node = node;

		node->payload = (void *) task;

		if (node == last)
			return;

		next = node + 1l;

		node->next = next;
		next->prev = node;

		node = next;
		++task;
	}
}


/* ThreadPool operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_pool_init_default(struct ThreadPool *const restrict pool)
{
	pool->busy = true;
	thread_cond_init(&pool->done);
	mutex_init(&pool->processing);

	thread_log_init(&pool->log,
			"thread pool");

	supervisor_init(&pool->supervisor,
			pool);

	task_queues_init_default(&pool->task_queues);
}

inline void
thread_pool_init_empty(struct ThreadPool *const restrict pool)
{
	thread_pool_init_default(pool);

	thread_queue_init_empty(&pool->task_queues.vacant);

	thread_queue_init_empty(&pool->worker_queue);
}

inline void
thread_pool_init(struct ThreadPool *const restrict pool,
		 const size_t count_workers,
		 const size_t count_tasks)
{
	thread_pool_init_default(pool);

	struct ThreadQueueNode *const restrict worker_queue_nodes
	= (struct ThreadQueueNode *const restrict) (pool + 1l);

	struct Worker *const restrict workers
	= (struct Worker *const restrict) (worker_queue_nodes + count_workers);

	struct ThreadQueueNode *const restrict last_worker_queue_node
	= ((struct ThreadQueueNode *const restrict) workers) - 1l;

	worker_queue_init(&pool->worker_queue,
			  worker_queue_nodes,
			  last_worker_queue_node,
			  workers,
			  pool);

	struct ThreadQueueNode *const restrict task_queue_nodes
	= (struct ThreadQueueNode *const restrict) (workers + count_workers);

	struct Task *const restrict tasks
	= (struct Task *const restrict) (task_queue_nodes + count_tasks);

	struct ThreadQueueNode *const restrict last_task_queue_node
	= ((struct ThreadQueueNode *const restrict) tasks) - 1l;

	vacant_task_queue_init(&pool->task_queues.vacant,
			       task_queue_nodes,
			       last_task_queue_node,
			       tasks);
}

inline struct ThreadPool *
thread_pool_create_empty(const struct HandlerClosure *const restrict fail_cl)
{
	struct ThreadPool *const restrict pool
	= malloc(sizeof(struct ThreadPool));

	if (pool != NULL) {
		thread_pool_init_empty(pool);
		return pool;
	}

	fail_cl->handle(fail_cl->arg,
			MALLOC_FAILURE_MESSAGE("thread_pool_create_empty"));
	__builtin_unreachable();
}

inline struct ThreadPool *
thread_pool_create(size_t count_workers,
		   const struct HandlerClosure *const restrict fail_cl)
{
	if (count_workers == 0lu)
		return thread_pool_create_empty(fail_cl);

	/* alloc space for:
	 *	1			ThreadPool
	 *	count_workers		Worker
	 *	count_workers		ThreadQueueNode (worker queue)
	 *	count_workers X 2	Task
	 *	count_workers X 2	ThreadQueueNode (task queues) */

	const size_t count_tasks = count_workers * 2lu;

	struct ThreadPool *const restrict pool
	= malloc(sizeof(struct ThreadPool)
		 + (sizeof(struct Worker)	   * count_workers)
		 + (sizeof(struct Task)		   * count_tasks)
		 + (sizeof(struct ThreadQueueNode) * (count_workers
						      + count_tasks)));

	if (pool != NULL) {
		thread_pool_init(pool,
				 count_workers,
				 count_tasks);
		return pool;
	}

	fail_cl->handle(fail_cl->arg,
			MALLOC_FAILURE_MESSAGE("thread_pool_create"));
	__builtin_unreachable();
}


inline void
thread_pool_process(struct ThreadPool *restrict pool,
		    const struct HandlerClosure *const restrict fail_cl)
{
	supervisor_signal_handle_cl(&pool->supervisor,
				    &supervisor_oversee_completion,
				    fail_cl);

	mutex_lock_try_catch_open(&pool->processing);

	mutex_lock_handle_cl(&pool->processing,
			     fail_cl);

	while (pool->busy)
		thread_cond_await_handle_cl(&pool->done,
					    &pool->processing,
					    fail_cl);

	mutex_unlock_handle_cl(&pool->processing,
			       fail_cl);

	mutex_lock_try_catch_close();
}


inline void
thread_pool_destroy(struct ThreadPool *restrict pool)
{
	free(pool);
}


/* Worker operations
 *─────────────────────────────────────────────────────────────────────────── */



/* void * */
/* worker_do_awaitable(void *arg); */


/* inline WorkerID */
/* worker_spawn_awaitable(Routine *const routine, */
/* 			    void *arg) */
/* { */
/* 	struct Worker *const restrict */
/* 	worker = worker_queue_handle_pop(&supervisor.idle); */

/* 	if (worker == NULL) */
/* 		supervisor_exit(SEED_WORKER_SPAWN_FAILURE_MESSAGE); */

/* 	worker->busy		  = true; */
/* 	worker->key		  = (ThreadKey) worker; */
/* 	worker->routine.awaitable = routine; */
/* 	worker->arg		  = arg; */

/* 	thread_handle_create(&worker->thread, */
/* 				  &worker_do_awaitable, */
/* 				  worker); */

/* 	worker_queue_handle_push(&supervisor.busy, */
/* 				 worker); */

/* 	return worker->id; */
/* } */

/* void * */
/* worker_do_independent(void *arg); */

/* inline void */
/* worker_spawn_independent(Procedure *const routine, */
/* 			      void *arg) */
/* { */
/* 	struct Worker *const restrict */
/* 	worker = worker_queue_handle_pop(&supervisor.idle); */

/* 	if (worker == NULL) */
/* 		supervisor_exit(SEED_WORKER_SPAWN_FAILURE_MESSAGE); */

/* 	worker->key		    = (ThreadKey) worker; */
/* 	worker->routine.independent = routine; */
/* 	worker->arg		    = arg; */

/* 	thread_handle_create(&worker->thread, */
/* 				  &worker_do_independent, */
/* 				  worker); */

/* 	worker_queue_handle_push(&supervisor.busy, */
/* 				 worker); */
/* } */



/* inline void * */
/* worker_await(const WorkerID id) */
/* { */


/* 	struct Worker *const restrict worker = worker_fetch(id); */

/* 	mutex_handle_lock(&worker->processing); */

/* 	while (worker->busy) */
/* 		thread_cond_handle_await(&worker->done, */
/* 					      &worker->processing); */

/* 	void *const restrict result = worker->result; */

/* 	mutex_handle_unlock(&worker->processing); */

/* 	worker_queue_handle_remove(&supervisor.done, */
/* 				   worker); */

/* 	worker_queue_handle_push(&supervisor.idle, */
/* 				 worker); */

/* 	return result; */
/* } */

/* inline void * */
/* worker_await_limit(const WorkerID id, */
/* 			const struct timespec *const restrict limit) */
/* { */


/* 	struct Worker *const restrict worker = worker_fetch(id); */

/* 	mutex_handle_lock(&worker->processing); */

/* 	while (worker->busy) */
/* 		thread_cond_handle_await_limit(&worker->done, */
/* 						    &worker->processing, */
/* 						    limit); */

/* 	void *const restrict result = worker->result; */

/* 	mutex_handle_unlock(&worker->processing); */

/* 	worker_queue_handle_remove(&supervisor.done, */
/* 				   worker); */

/* 	worker_queue_handle_push(&supervisor.idle, */
/* 				 worker); */

/* 	return result; */
/* } */

/* inline void * */
/* worker_await_span(const WorkerID id, */
/* 		       const struct timespec *const restrict span) */
/* { */
/* 	struct Worker *const restrict worker = worker_fetch(id); */

/* 	mutex_handle_lock(&worker->processing); */

/* 	while (worker->busy) */
/* 		thread_cond_handle_await_span(&worker->done, */
/* 						   &worker->processing, */
/* 						   span); */

/* 	void *const restrict result = worker->result; */

/* 	mutex_handle_unlock(&worker->processing); */

/* 	worker_queue_handle_remove(&supervisor.done, */
/* 				   worker); */

/* 	worker_queue_handle_push(&supervisor.idle, */
/* 				 worker); */

/* 	return result; */
/* } */

#endif /* ifndef MYSQL_SEED_THREAD_THREAD_POOL_H_ */
