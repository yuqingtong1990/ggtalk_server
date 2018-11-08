#pragma once

#include <mutex>
#include <atomic>
#include <chrono>
#include <exception>
#include <sstream>
#include <stdint.h>
/**
* @brief �ֲ�ʽid������
* https://segmentfault.com/a/1190000011282426
* https://github.com/twitter/snowflake/blob/snowflake-2010/src/main/scala/com/twitter/service/snowflake/IdWorker.scala
*
* 64bit id: 0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
*           ||                                                           ||     ||     |  |              |
*           |��---------------------------ʱ���--------------------------��������-��������-��  ��----���к�----��
*           |
*         ����
* SnowFlake���ŵ�: �����ϰ���ʱ����������, ���������ֲ�ʽϵͳ�ڲ������ID��ײ(����������ID�ͻ���ID������), ����Ч�ʽϸ�, ������, SnowFlakeÿ���ܹ�����26��ID����.
*/
//��������������Ϣid
class SnowflakeIdWorker
{
public:
	//���û���id
	void setWorkerId(uint32_t workerId){
		this->workerId = workerId;
	}
	//������������id
	void setDatacenterId(uint32_t datacenterId) {
		this->datacenterId = datacenterId;
	}

	uint64_t getId() {
		return nextId();
	}

	uint64_t nextId() {

		static std::atomic<int64_t> timestamp;

		timestamp = timeGen();

		// �����ǰʱ��С����һ��ID���ɵ�ʱ�����˵��ϵͳʱ�ӻ��˹����ʱ��Ӧ���׳��쳣
		if (timestamp < lastTimestamp) {
			std::ostringstream s;
			s << "clock moved backwards.  Refusing to generate id for " << lastTimestamp - timestamp << " milliseconds";
			throw std::exception(std::runtime_error(s.str()));
		}

		if (lastTimestamp == timestamp) {
			// �����ͬһʱ�����ɵģ�����к���������
			sequence.store((sequence + 1) & sequenceMask);
			if (0 == sequence) {
				// �������������, ��������һ������,����µ�ʱ���
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
	* ����id��ռ��λ��
	*/
	const uint32_t workerIdBits = 5;
	/**
	* ��������id��ռ��λ��
	*/
	const uint32_t datacenterIdBits = 5;
	/**
	* ������ռ��λ��
	*/
	const uint32_t sequenceBits = 12;
	/**
	* ����ID������12λ
	*/
	const uint32_t workerIdShift = sequenceBits;
	/**
	* ���ݱ�ʶid������17λ
	*/
	const uint32_t datacenterIdShift = workerIdShift + workerIdBits;
	/**
	* ʱ���������22λ
	*/
	const uint32_t timestampLeftShift = datacenterIdShift + datacenterIdBits;

	/**
	* ֧�ֵ�������id�������31
	*/
	const uint32_t maxWorkerId = 31;
	/**
	* ֧�ֵ������������id�������31
	*/
	const uint32_t maxDatacenterId = 31;
	/**
	* �������е����룬����Ϊ4095
	*/
	const uint32_t sequenceMask = 4095;

	/**
	* ��������id(0~31)
	*/
	uint32_t workerId = 0;

	/**
	* ��������id(0~31)
	*/
	uint32_t datacenterId = 0;

	/**
	* ����������(0~4095)
	*/
	std::atomic<uint32_t> sequence;

	/**
	* �ϴ�����ID��ʱ���
	*/
	std::atomic<int64_t> lastTimestamp;
};