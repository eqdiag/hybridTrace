#include "timer.h"
#include <iostream>

core::Timer::Timer(int numFrames) :
	mCurrentFrame{0},
	mFps{ 0.0f },
	mLastTick{ std::chrono::steady_clock::now() }
{
	mLastFrames.resize(numFrames);
	for (int i = 0; i < mLastFrames.size(); i++) {
		mLastFrames[i] = 0.0f;
	}
}


void core::Timer::tick()
{
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> dt = now - mLastTick;
	mLastFrames[mCurrentFrame] = 1.0f / dt.count();
	mCurrentFrame = (mCurrentFrame + 1) % mLastFrames.size();
	mFps = 0.0;
	for (int i = 0; i < mLastFrames.size(); i++) {
		mFps += mLastFrames.at(i);
	}
	mFps /= mLastFrames.size();
	mLastTick = now;
}

float core::Timer::getFps() const
{
	return mFps;
}
