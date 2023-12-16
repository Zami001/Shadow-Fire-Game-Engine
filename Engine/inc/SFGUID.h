#pragma once

#include <Engine.h>
#include <stdint.h>
#include <set>

class ENGINE_API SFGUID {
	friend class SerializedAsset;

public:
	SFGUID();
	SFGUID(uint64_t id);
	SFGUID(const SFGUID& id);
	SFGUID(SFGUID&& id);

	~SFGUID();

	uint64_t GetID() const;

	operator uint64_t() const;

	void operator=(SFGUID&& id) = delete;
	void operator=(const SFGUID& id) = delete;

private:
	uint64_t Identifier;
};