#include <Math/Random.h>
#include <random>

uint64_t Random::GetInt64() {
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<uint64_t> randomizer;
	return randomizer(e2);
}

Random& Random::Get() {
	static Random instance;
	return instance;
}
