#pragma once

#include <mutex>
#include <atomic>
#include <chrono>
#include <exception>
#include <sstream>
#include <stdint.h>
/**
* @brief 分布式id生成类
* https://segmentfault.com/a/1190000011282426
* https://github.com/twitter/snowflake/blob/snowflake-2010/src/main/scala/com/twitter/service/snowflake/IdWorker.scala
*
* 64bit id: 0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
*           ||                                                           ||     ||     |  |              |
*           |└---------------------------时间戳--------------------------┘└中心-┘└机器-┘  └----序列号----┘
*           |
*         不用
* SnowFlake的优点: 整体上按照时间自增排序, 并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分), 并且效率较高, 经测试, SnowFlake每秒能够产生26万ID左右.
*/
//这里用来生成消息id
class SnowflakeIdWorker
{
public:
	//设置机器id
	void setWorkerId(uint32_t workerId){
		this->workerId = workerId;
	}
	//设置数据中心id
	void setDatacenterId(uint32_t datacenterId) {
		this->datacenterId = datacenterId;
	}

	uint64_t getId() {
		return nextId();
	}

	uint64_t nextId() {

		static std::atomic<int64_t> timestamp;

		timestamp = timeGen();

		// 如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过这个时候应当抛出异常
		if (timestamp < lastTimestamp) {
			std::ostringstream s;
			s << "clock moved backwards.  Refusing to generate id for " << lastTimestamp - timestamp << " milliseconds";
			throw std::exception(std::runtime_error(s.str()));
		}

		if (lastTimestamp == timestamp) {
			// 如果是同一时间生成的，则进行毫秒内序列
			sequence.store((sequence + 1) & sequenceMask);
			if (0 == sequence) {
				// 毫秒内序列溢出, 阻塞到下一个毫秒,获得新的时间戳
				timestamp = tilNextMillis(lastTimestamp);
			}
		}
		else {
			sequence = 0;
		}

		lastTimestamp = timestamp.load();

		return ((timestamp - twepoch) << timestampLeftShift)
			| (datacenterId << datacenterIdShift)
			| (workerId << workerIdShift)
			| sequence;

	}


	SnowflakeIdWorker() : workerId(0), datacenterId(0), sequence(0), lastTimestamp(0) {
		sequence.store(0);
		lastTimestamp.store(0);
	}
protected:
	uint64_t timeGen() const {
		auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
		return t.time_since_epoch().count();
	}
	uint64_t tilNextMillis(uint64_t lastTimestamp) const {
		uint64_t timestamp = timeGen();
		while (timestamp <= lastTimestamp) {
			timestamp = timeGen();
		}
		return timestamp;
	}

private:

	const uint64_t twepoch = 1514736000000;
	/**
	* 机器id所占的位数
	*/
	const uint32_t workerIdBits = 5;
	/**
	* 数据中心id所占的位数
	*/
	const uint32_t datacenterIdBits = 5;
	/**
	* 序列所占的位数
	*/
	const uint32_t sequenceBits = 12;
	/**
	* 机器ID向左移12位
	*/
	const uint32_t workerIdShift = sequenceBits;
	/**
	* 数据标识id向左移17位
	*/
	const uint32_t datacenterIdShift = workerIdShift + workerIdBits;
	/**
	* 时间截向左移22位
	*/
	const uint32_t timestampLeftShift = datacenterIdShift + datacenterIdBits;

	/**
	* 支持的最大机器id，结果是31
	*/
	const uint32_t maxWorkerId = 31;
	/**
	* 支持的最大数据中心id，结果是31
	*/
	const uint32_t maxDatacenterId = 31;
	/**
	* 生成序列的掩码，这里为4095
	*/
	const uint32_t sequenceMask = 4095;

	/**
	* 工作机器id(0~31)
	*/
	uint32_t workerId = 0;

	/**
	* 数据中心id(0~31)
	*/
	uint32_t datacenterId = 0;

	/**
	* 毫秒内序列(0~4095)
	*/
	std::atomic<uint32_t> sequence;

	/**
	* 上次生成ID的时间截
	*/
	std::atomic<int64_t> lastTimestamp;
};