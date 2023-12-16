#pragma once

#include <Core.h>
#include <stdint.h>

class CORE_API Random {
public:
	static uint64_t GetInt64();

private:
	static Random& Get();
};