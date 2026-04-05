#pragma once

#include <Engine.h>
#include <functional>
#include <vector>
#include <chrono>
#include <MinimalCore.h>

enum class TickStage : int {
	General,
	PreRender,
	Render,
	GUI,
	PostRender,
	NUM_STAGES
};

class ENGINE_API TickManager {
	friend class Game;

public:
	TickManager();
	~TickManager() = default;

	void RegisterTick(void* context, std::function<void(float)> func, TickStage stage = TickStage::General);
	void UnregisterTick(void* context, TickStage stage = TickStage::General);
	void Tick();
	
	inline float GetTimeScale() const {
		return TimeScale;
	}

	inline void SetTimeScale(float scale) {
		TimeScale = scale;
	}

private:
	struct TickCallback {
	public:
		TickCallback(void* context, std::function<void(float)> func) : ContextObject(context), Func(func) {}

		std::function<void(float)> Func;
		void* ContextObject;

		void operator()(float DeltaTime) {
			Func(DeltaTime);
		}
	};

	std::chrono::time_point<std::chrono::steady_clock> LastTick;
	std::vector<std::vector<TickCallback>> Callbacks;
	float TimeScale;
};