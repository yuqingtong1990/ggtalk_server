#include "UniqueId.h"
#include "Snowflake.h"

static SnowflakeIdWorker worker;

void UniqueId::Initialize(uint32_t datacenterId, uint32_t workid)
{
	worker.setDatacenterId(datacenterId);
	worker.setWorkerId(workid);
}

uint64_t UniqueId::get()
{
	return worker.getId();
}
