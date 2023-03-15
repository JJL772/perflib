
#pragma once

#include "perf-counters.h"

namespace perflib
{
	/*
	 * Simple wrapper around a perf counter
	 */
	class counter {
	public:
		counter() = delete;
		counter(const counter&) = delete;
		counter(counter&& c) {
			m_counter = c.m_counter;
			c.m_counter = -1;
		}

		counter(uint32_t type, uint64_t config, bool enable = false) {
			m_counter = perf_counter_open(type, config, enable);
		}

		~counter() {
			perf_counter_close(m_counter);
		}

		inline void reset() {
			perf_counter_reset(m_counter);
		}

		inline uint64_t read() const {
			return perf_counter_read(m_counter);
		}

		inline void enable() {
			perf_counter_enable(m_counter);
		}

		inline void disable() {
			perf_counter_disable(m_counter);
		}

	protected:
		int m_counter;
	};

	template<uint32_t type, uint64_t config, bool ENABLE = false>
	class counter_template : public counter {
	public:
		counter_template() :
			counter(type, config, ENABLE)
		{}
	};

	/*
	 * Special counter type to keep track of ratios
	 */
	template<uint32_t type1, uint64_t config1, uint32_t type2, uint64_t config2, bool ENABLE = false>
	class ratio_counter {
	public:
		ratio_counter() :
			m_counter1(type1, config1, ENABLE),
			m_counter2(type2, config2, ENABLE)
		{
		}

		auto refs() const {
			return m_counter1.read();
		}

		auto misses() const {
			return m_counter2.read();
		}

		void reset() {
			m_counter1.reset();
			m_counter2.reset();
		}

		void enable() {
			m_counter1.enable();
			m_counter2.enable();
		}

		void disable() {
			m_counter1.disable();
			m_counter2.disable();
		}

		/*
		 * Returns miss:ref ratio
		 */
		double ratio() const {
			auto r = refs();
			r = r == 0 ? 1 : r;
			return static_cast<double>(misses()) / r;
		}

	protected:
		counter m_counter1;
		counter m_counter2;
	};

	template<bool enable = false>
	using branch_miss_counter =
		ratio_counter<PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS, PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES, enable>;

	template<bool enable = false>
	using llc_miss_counter =
		ratio_counter<PERF_TYPE_HW_CACHE, PERF_CONFIG_LLC_REFS, PERF_TYPE_HW_CACHE, PERF_CONFIG_LLC_MISSES, enable>;
	
	template<bool enable = false>
	using cache_miss_counter = llc_miss_counter<enable>; // cache miss usually refers to LLC read miss

	template<bool enable = false>
	using l1d_miss_counter =
		ratio_counter<PERF_TYPE_HW_CACHE, PERF_CONFIG_L1D_REFS, PERF_TYPE_HW_CACHE, PERF_CONFIG_L1D_MISSES, enable>;

	template<bool enable = false>
	using instruction_counter = counter_template<PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS, enable>;
}