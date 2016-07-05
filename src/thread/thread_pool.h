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
#define THREAD_POOL_EXIT_FAILURE_MESSAGE ANSI_BRIGHT ANSI_RED_BG	\
ANSI_YELLOW "\nTHREAD POOL EXITING ON FAILURE\n" ANSI_RESET

#define THREAD_POOL_EXIT_SUCCESS_MESSAGE ANSI_BRIGHT ANSI_WHITE_BG	\
ANSI_GREEN "\nTHREAD POOL EXITING ON SUCCESS\n" ANSI_RESET


/* typedefs
 *─────────────────────────────────────────────────────────────────────────── */
struct ThreadPool;

typedef void
ThreadPoolEvent(struct ThreadPool *const restrict pool);

/* ProcedureClosure
 * - storage of result must be handled by caller,
 * - set NULL to indicate task is complete to threads awaiting 'done' */
struct Task {
	struct ThreadQueueNode *node;
	const struct ProcedureClosure *closure;
	ThreadCond done;
	Mutex processing;
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
	struct ThreadQueue awaiting;
	struct ThreadQueue complete;
};


struct ThreadPool {
	bool busy;
	ThreadCond done;
	Mutex processing;
	struct ThreadLog log;
	struct Supervisor supervisor;
	struct ThreadQueue worker_queue;
	struct TaskQueues task_queues;
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
	thread_queue_await_empty_handle_cl(&pool->task_queues.awaiting,
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
	__builtin_unreachable();
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
worker_init(struct Worker *const restrict worker,
	    struct ThreadQueueNode *const restrict node,
	    struct ThreadPool *const restrict pool)
{
	worker->fail_cl.handle = &worker_exit_on_failure;
	worker->fail_cl.arg    = pool;
	worker->pool	       = pool;
	worker->node	       = node;
}

inline void
worker_process_tasks(struct Worker *const restrict worker)
{
	struct TaskQueues *const restrict
	task_queues = &worker->pool.task_queues;

	while (1) {
		/* pop next assigned task */
		thread_queue_pop_handle_cl(&task_queues->awaiting,
					   &worker->task,
					   &worker->fail_cl);

	}

	__builtin_unreachable();
}

void *
worker_start(void *arg)
__attribute__((noreturn));


/* Task operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
task_init(struct Task *const restrict task,
	  struct ThreadQueueNode *const restrict node,
	  const struct ProcedureClosure *const restrict closure)
{
	task->node    = node;
	task->closure = closure;

	thread_cond_init(&task->done);
	mutex_init(&task->processing);
}


/* ThreadQueue operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
task_queues_vacant_init(struct ThreadQueue *const restrict vacant,
			struct ThreadQueueNode *restrict node,
			struct Task *restrict task)
{
	if (node == (struct ThreadQueueNode *const restrict) (task)) {
		thread_queue_init_empty(vacant);
		return;
	}

	thread_queue_init(vacant);

	struct ThreadQueueNode *restrict next;
	struct ThreadQueueNode *const restrict last
	= ((struct ThreadQueueNode *const restrict) task) - 1l;

	last->next   = NULL;
	vacant->last = last;

	node->prev   = NULL;
	vacant->head = node;

	while (1) {
		task_init(task,
			  node,
			  NULL);

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

inline void
task_queues_awaiting_init(struct ThreadQueue *const restrict awaiting,
			  struct ThreadQueueNode *restrict node,
			  struct Task *restrict task,
			  const struct ProcedureClosure *restrict task_cl)
{
	if (node == (struct ThreadQueueNode *const restrict) (task)) {
		thread_queue_init_empty(awaiting);
		return;
	}

	struct ThreadQueueNode *restrict next;

	struct ThreadQueueNode *const restrict last
	= ((struct ThreadQueueNode *const restrict) task) - 1l;

	last->next     = NULL;
	awaiting->last = last;

	node->prev     = NULL;
	awaiting->head = node;

	while (1) {
		task_init(task,
			  node,
			  task_cl);

		node->payload = (void *) task;

		if (node == last)
			return;

		next = node + 1l;

		node->next = next;
		next->prev = node;

		node = next;
		++task;
		++task_cl;
	}
}

inline void
worker_queue_init(struct ThreadQueue *const restrict worker_queue,
		  struct ThreadQueueNode *restrict node,
		  struct Worker *restrict worker,
		  struct ThreadPool *const restrict pool)
{
	struct ThreadQueueNode *restrict next;

	thread_queue_init(worker_queue);

	struct ThreadQueueNode *const restrict last
	= ((struct ThreadQueueNode *const restrict) worker) - 1l;

	last->next	   = NULL;
	worker_queue->last = last;

	node->prev	   = NULL;
	worker_queue->head = node;

	while (1) {
		worker_init(worker,
			    node,
			    pool);

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

inline void
worker_queue_start(struct ThreadQueue *const restrict worker_queue,
		   const struct HandlerClosure *const restrict fail_cl)
{
	struct ThreadQueueNode *restrict node;
	struct Worker *restrict worker;

	mutex_lock_try_catch_open(&worker_queue->lock);

	mutex_lock_handle_cl(&worker_queue->lock,
			     fail_cl);

	thread_queue_peek(&pool->worker_queue,
			  &node);

	do {
		worker = (struct Worker *restrict) node->payload;

		thread_create_handle_cl(&worker->thread,
					&worker_start,
					worker);

		node = node->next;

	} while (node != NULL);



	mutex_unlock_handle_cl(&worker_queue->lock,
			       fail_cl);

	mutex_lock_try_catch_close();
}


/* TaskQueues operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
task_queues_init(struct TaskQueues *const restrict task_queues,
		 struct ThreadQueueNode *const restrict vacant_task_nodes,
		 struct ThreadQueueNode *const restrict init_task_nodes,
		 struct Task *const restrict vacant_tasks,
		 struct Task *const restrict init_tasks,
		 const struct ProcedureClosure *const restrict task_cls)
{

	task_queues_vacant_init(&task_queues->vacant,
				vacant_task_nodes,
				vacant_tasks);

	task_queues_awaiting_init(&task_queues->awaiting,
				  init_task_nodes,
				  init_tasks,
				  task_cls);

	thread_queue_init_empty(&task_queues->complete);
}


/* ThreadPool operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
thread_pool_init(struct ThreadPool *const restrict pool,
		 const struct ProcedureClosure *const restrict task_cls,
		 const size_t count_workers,
		 const size_t count_init_tasks,
		 const size_t count_vacant_tasks)
{
	/* initialize overall completion condition variables */
	thread_cond_init(&pool->done);
	mutex_init(&pool->processing);

	/* initialize thread log */
	thread_log_init(&pool->log,
			"thread pool");

	/* initialize supervisor */
	supervisor_init(&pool->supervisor,
			pool);

	/* divvy up memory for task queue nodes, tasks */
	struct ThreadQueueNode *const restrict vacant_task_nodes
	= (struct ThreadQueueNode *const restrict) (pool + 1l);

	struct Task *const restrict vacant_tasks
	= (struct Task *const restrict) (vacant_task_nodes
					 + count_vacant_tasks);

	struct ThreadQueueNode *const restrict init_task_nodes
	= (struct ThreadQueueNode *const restrict) (vacant_tasks
						    + count_vacant_tasks);

	struct Task *const restrict init_tasks
	= (struct Task *const restrict) (init_task_nodes
					 + count_init_tasks);

	/* initialize task queues */
	task_queues_init(&pool->task_queues,
			 vacant_task_nodes,
			 init_task_nodes,
			 vacant_tasks,
			 init_tasks,
			 task_cls);

	/* divvy up memory for worker queue nodes, workers */
	struct ThreadQueueNode *const restrict worker_nodes
	= (struct ThreadQueueNode *const restrict) (init_tasks
						    + count_init_tasks);

	struct Worker *const restrict workers
	= (struct Worker *const restrict) (worker_nodes
					   + count_workers);

	/* initialize worker queue */
	worker_queue_init(&pool->worker_queue,
			  worker_nodes,
			  workers,
			  pool);
}

inline struct ThreadPool *
thread_pool_create(const struct ProcedureClosure *const restrict task_cls,
		   const size_t count_workers,
		   const size_t count_init_tasks,
		   const size_t count_vacant_tasks,
		   const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (count_workers == 0lu) {
		failure = FAILURE_REASON("thread_pool_create",
					 "no space allocated for pool workers");
		goto HANDLE_FAILURE;
	}

	/* allocate space for:
	 *	1			ThreadPool
	 *	count_workers		Worker
	 *	count_workers		ThreadQueueNode (worker queue)
	 *	count_total_tasks	Task
	 *	count_total_tasks	ThreadQueueNode (task queues) */

	const size_t count_total_tasks = count_init_tasks + count_vacant_tasks;

	if (count_total_tasks == 0lu) {
		failure = FAILURE_REASON("thread_pool_create",
					 "no space allocated for pool tasks");
		goto HANDLE_FAILURE;
	}

	struct ThreadPool *const restrict pool
	= malloc(sizeof(struct ThreadPool)
		 + (sizeof(struct Worker)	   * count_workers)
		 + (sizeof(struct Task)		   * count_total_tasks)
		 + (sizeof(struct ThreadQueueNode) * (count_workers
						      + count_total_tasks)));
	if (pool != NULL) {
		/* initialize, but do not start */
		thread_pool_init(pool,
				 task_cls,
				 count_workers,
				 count_init_tasks,
				 count_vacant_tasks);
		return pool;
	}

	failure = MALLOC_FAILURE_MESSAGE("thread_pool_create");

HANDLE_FAILURE:
	fail_cl->handle(fail_cl->arg,
			failure);
	__builtin_unreachable();
}

inline void
thread_pool_start(struct ThreadPool *restrict pool,
		  const struct HandlerClosure *const restrict fail_cl)
{
	pool->busy = true;

	worker_queue_start(&pool->worker_queue,
			   fail_cl);

	supervisor_start(&pool->supervisor,
			 fail_cl);
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
/* 	worker->routine.awaiting = routine; */
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
