#include "mujoco/mujoco.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/ThreadSafeCounter.h"
#include "MujocoWorkerThread.h"
#include "Misc/ScopeLock.h"
#include <chrono>


FMujocoWorkerThread::FMujocoWorkerThread(mjData& InSharedmData, mjModel& InSharedmModel, FThreadSafeBool& InStopCondition, FThreadSafeCounter& InRunCount)
	: SharedmData(&InSharedmData)
	, SharedmModel(&InSharedmModel)
	, StopCondition(InStopCondition)
	, RunCount(InRunCount)
{}

// 工作线程运行函数
uint32 FMujocoWorkerThread::Run()
{
    const double TargetInterval = 1.0 / 10.0; // 100Hz = 每10毫秒一次

	double startTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    while (!StopCondition)
    {
		if (!SharedmData || !SharedmModel)
		{
			FPlatformProcess::Sleep(TargetInterval); // Sleep for 1ms to avoid busy waiting
			continue;
		}
		// 累加运行次数
        RunCount.Increment();
		// 执行MuJoCo模拟步骤
		
		double currentTime = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - startTime) / 1e9; // Convert to seconds
		while (SharedmData->time < currentTime)
		{			// 进行MuJoCo模拟
			mj_step(SharedmModel, SharedmData);
		}
	}
	return 0;
}

void FMujocoWorkerThread::Stop()
{
    StopCondition = true;
}
