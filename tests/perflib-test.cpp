
#include "perflib/perflib.h"

#include <stdio.h>
#include <list>

int main() {
	using namespace perflib;

	std::list<int> l;

	branch_miss_counter<true> branchCounter;
	instruction_counter<true> instCounter;
	cache_miss_counter<> cacheCounter;

	
	printf("cache misses: %lu, refs: %lu\n", cacheCounter.misses(), cacheCounter.refs());
	printf("branch misses: %lu, refs: %lu\n", branchCounter.misses(), branchCounter.refs());
	printf("inst: %lu\n", instCounter.read());

	printf("\n\n");

	cacheCounter.enable();

	for (int i = 0; i < 1000; ++i) {
		//if (i % 10) {
			l.push_back(i);
		//}
	}

	printf("cache misses: %lu, refs: %lu, %f%% miss rate\n", cacheCounter.misses(), cacheCounter.refs(), cacheCounter.ratio() * 100.0f);
	printf("branch misses: %lu, refs: %lu, %f%% miss rate\n", branchCounter.misses(), branchCounter.refs(), branchCounter.ratio() * 100.0f);
	printf("inst: %lu\n", instCounter.read());
}