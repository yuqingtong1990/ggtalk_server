#pragma once
#include <stdint.h>
#include <string>

/*
 * 分布式id生成类,需要自己的代码
 * workid 支持的最大机器id，结果是31
 * datacenterId 支持的最大数据中心id 结果是31
 * 如果传了大于31的数字则取余
 * */
uint64_t sfUniqueId(uint32_t datacenterId = 5, uint32_t workid = 31);

std::string CreateGuidStr();

//创建一个随机字符串
std::string CreateRandomStr(uint32_t len = 32);
