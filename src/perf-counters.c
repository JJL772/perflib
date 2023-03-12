
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <memory.h>

#include "perf-counters.h"

static int perf_event_open(struct perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
	return syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}

int perf_counter_open(uint32_t type, uint64_t config, int start) {
	struct perf_event_attr attr;
	memset(&attr, 0, sizeof(attr));
	attr.disabled = 1;
	attr.type = type;
	attr.size = sizeof(attr);
	attr.exclude_kernel = 1;
	attr.exclude_hv = 1;
	attr.config = config;

	int fd = perf_event_open(&attr, 0, -1, -1, 0);
	if (fd == -1)
		return fd;
	
	if (start)
		ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
	perf_counter_reset(fd);
	return fd;
}

uint64_t perf_counter_read(int counter) {
	uint64_t buf = 0;
	if (read(counter, &buf, sizeof(buf)) != sizeof(buf))
		return 0;
	return buf;
}

void perf_counter_close(int counter) {
	close(counter);
}

void perf_counter_reset(int counter) {
	ioctl(counter, PERF_EVENT_IOC_RESET, 0);
}

void perf_counter_enable(int counter) {
	ioctl(counter, PERF_EVENT_IOC_ENABLE, 0);
}

void perf_counter_disable(int counter) {
	ioctl(counter, PERF_EVENT_IOC_DISABLE, 0);
}