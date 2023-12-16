#include <SFGUID.h>
#include <Math/Random.h>
#include <chrono>
#include <Asserts.h>

static std::set<uint64_t> UsedIdentifiers;

SFGUID::SFGUID() {
	do {
		//long long time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		//srand(time);
		Identifier = Random::GetInt64(); //rand();
		//Identifier <<= 32;
		//Identifier ^= rand();
	} while (Identifier == 0 || UsedIdentifiers.count(Identifier) != 0);

	UsedIdentifiers.insert(Identifier);
}

SFGUID::SFGUID(uint64_t id) {
	Identifier = id;

	UsedIdentifiers.insert(Identifier);
}

SFGUID::SFGUID(const SFGUID& id) {
	Identifier = id.Identifier;
}

SFGUID::SFGUID(SFGUID&& id) : Identifier(id.Identifier) {}

SFGUID::~SFGUID() {}

SFGUID::operator uint64_t() const {
	return Identifier;
}