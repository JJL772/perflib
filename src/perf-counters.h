
#pragma once

#include <linux/perf_event.h>
#include <stdint.h>


#undef PERF_EVENT_CONFIG
#define PERF_EVENT_CONFIG(cache_id, cache_op_id, cache_result_id) \
	(cache_id) | (cache_op_id << 8) | (cache_result_id << 16)

/*
 * Some pre-defined event types, for ease of use
 */

/* last-level cache misses/refs  */
#define PERF_CONFIG_LLC_MISSES PERF_EVENT_CONFIG(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_MISS)
#define PERF_CONFIG_LLC_REFS PERF_EVENT_CONFIG(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_ACCESS)

/* L1 instruction misses/refs */
#define PERF_CONFIG_L1I_MISSES PERF_EVENT_CONFIG(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_MISS)
#define PERF_CONFIG_L1I_REFS PERF_EVENT_CONFIG(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_ACCESS)

/* L1 data misses/refs */
#define PERF_CONFIG_L1D_MISSES PERF_EVENT_CONFIG(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_MISS)
#define PERF_CONFIG_L1D_REFS PERF_EVENT_CONFIG(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_ACCESS)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Opens a counter based on the passed in config
 * Returns a file descriptor that may be passed into sprof_counter_read to get the current
 * counter value.
 * If start is 1, we'll start sampling immediately
 * Returns -1 if the counter could not be opened for some reason
*/
int perf_counter_open(uint32_t type, uint64_t config, int start);

/*
 * Reads the current value of the performance counter
 * returns 0 if read failed
 */
uint64_t perf_counter_read(int counter);

/*
 * Close the performance counter
 */
void perf_counter_close(int counter);

/*
 * Reset the counter value
 */
void perf_counter_reset(int counter);

/*
 * Pause sampling on the counter
 */
void perf_counter_enable(int counter);

/*
 * Resume sampling on the counter
 */
void perf_counter_disable(int counter);

#ifdef __cplusplus
}
#endif
