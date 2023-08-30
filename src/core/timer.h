#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <vector>

namespace core {

	class Timer {
	public:
		Timer(int numFrames = 1);
		void tick();
		float getFps() const;
	private:
		int mCurrentFrame;
		float mFps;
		std::chrono::steady_clock::time_point mLastTick;
		std::vector<float> mLastFrames;
	};

}

#endif