
#ifndef DOMACI1_H_
#define DOMACI1_H_

#define THREAD_COUNT 120

/*
 * Performs a test for the spinlock implementation.
 * There are ~30K locks and unlocks in total.
 * The timeout is set to 200ms.
 * The argument for this function - timeout - is in seconds.
 * The return value is 0 if mutex implementation is valid,
 * and return value is 1 if mutex implementation is not valid.
 *
 * The function expects the following to be implemented:
 * int lock_n_threads_with_timeout(int id, int* local, double timeout)
 * void unlock_n_threads_with_timeout(int id, int* local)
 *
 * The lock function should return 1 if lock is successful,
 * and 0 if the timeout period expires.
 * The third argument ("timeout") is in seconds.
 */
extern int start_timeout_mutex_n_threads_test(double timeout);

/*
 * Performs a compare of variable var with value old_val
 * If they are same, var is set to new_val, and 1 is returned
 * If they are not same, var is unchanged and 0 is returned
 */
extern int lrk_compare_and_set(int* var, int old_val, int new_val);

/*
 * Sets the current thread into sleep state for the given number
 * of milliseconds.
 */
extern void lrk_sleep(long millis);

/*
 * Gets the value of variable var and performs
 * the specified operation afterwards, atomically
 */
extern int lrk_get_and_increment(int* var);
extern int lrk_get_and_decrement(int* var);
extern int lrk_get_and_add(int* var, int to_add);
extern int lrk_get_and_sub(int* var, int to_sub);
extern int lrk_get_and_add_with_mod(int* var, int to_add, int mod);
extern int lrk_get_and_set(int* var, int to_set);

/*
 * Retrieves the time elapsed from some point in the past.
 * If you want to have time elapsed from the start of the process,
 * you must call this function at the beginning.
 * Time is retrieved in seconds.
 */
extern double lrk_get_time();

#endif /* DOMACI1_H_ */
