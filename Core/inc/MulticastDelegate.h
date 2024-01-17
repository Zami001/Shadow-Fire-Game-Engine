#pragma once

#include <map>
#include <functional>

typedef size_t DelegateHandle;

struct DelegateBase {
protected:
	DelegateBase() = default;
	~DelegateBase() = default;

private:
	DelegateHandle NextHandle = 0;

protected:
	DelegateHandle GetNewHandle() {
		return NextHandle++;
	}
};

template<typename... Args>
struct MulticastDelegate final : private DelegateBase {
	MulticastDelegate() = default;
	~MulticastDelegate() = default;

private:
	std::map<DelegateHandle, std::function<void(Args...)>> callbacks;

public:
	inline void operator()(Args... args) {
		for (auto& callback : callbacks) {
			callback.second(args...);
		}
	}

	inline DelegateHandle Add(std::function<void(Args...)> callback) {
		DelegateHandle handle = GetNewHandle();
		callbacks[handle] = callback;
		return handle;
	}

	inline void Remove(DelegateHandle handle) {
		auto listener = callbacks.find(handle);
		if (listener != callbacks.end()) {
			callbacks.erase(listener);
		}
	}
};