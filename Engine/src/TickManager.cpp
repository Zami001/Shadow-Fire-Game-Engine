#include <TickManager.h>

TickManager::TickManager() : LastTick(std::chrono::steady_clock::now()), TimeScale(1.f) {
	for (int i = 0; i < static_cast<int>(TickStage::NUM_STAGES); ++i) {
		Callbacks.emplace_back();
	}
}

void TickManager::RegisterTick(void* context, std::function<void(float)> func, TickStage stage) {
	Callbacks[static_cast<int>(stage)].emplace_back(context, func);
}

void TickManager::UnregisterTick(void* context, TickStage stage) {
	for (int i = 0; i < Callbacks[static_cast<int>(stage)].size(); ++i) {
		if (Callbacks[static_cast<int>(stage)][i].ContextObject == context) {
			Callbacks[static_cast<int>(stage)].erase(Callbacks[static_cast<int>(stage)].begin() + i);
			i--;
		}
	}
}

void TickManager::Tick() {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> step = now - LastTick;

	for (int i = 0; i < static_cast<int>(TickStage::NUM_STAGES); ++i) {
		for (TickManager::TickCallback callback : Callbacks[i]) {
			callback(step.count());
		}
	}

	LastTick = now;
}