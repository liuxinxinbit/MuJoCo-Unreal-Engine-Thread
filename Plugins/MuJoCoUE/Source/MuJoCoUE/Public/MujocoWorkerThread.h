
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/ThreadSafeCounter.h"
#include "HAL/PlatformProcess.h"

// 自定义线程类
class FMujocoWorkerThread : public FRunnable
{
public:
    FMujocoWorkerThread(mjData& InSharedmData, mjModel& InSharedmModel, FThreadSafeBool& InStopCondition, FThreadSafeCounter& InRunCount);
    virtual ~FMujocoWorkerThread() {}

    // FRunnable接口实现
    virtual uint32 Run() override;
    virtual void Stop() override;

private:
    mjData* SharedmData;
    mjModel* SharedmModel;
    FThreadSafeBool& StopCondition;
    FThreadSafeCounter& RunCount;
};
