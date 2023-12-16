#include <TickManager.h>

TickManager::TickManager() : LastTick(std::chrono::steady_clock::now()), TimeScale(1.f) {}

void TickManager::RegisterTick(void* context, std::function<void(float)> func) {
	Callbacks.emplace_back(context, func);
}

void TickManager::UnregisterTick(void* context) {
	for (int i = 0; i < Callbacks.size(); ++i) {
		if (Callbacks[i].ContextObject == context) {
			Callbacks.erase(Callbacks.begin() + i);
			i--;
		}
	}
}

void TickManager::Tick() {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> step = now - LastTick;

	for (TickManager::TickCallback callback : Callbacks) {
		callback(step.count());
	}

	LastTick = now;
}