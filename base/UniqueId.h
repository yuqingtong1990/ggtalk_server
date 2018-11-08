#pragma once
#include <stdint.h>

namespace UniqueId
{
	void Initialize(uint32_t datacenterId = 5, uint32_t workid = 31);
	uint64_t get();
};

