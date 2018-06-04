//
// Created by kiva on 2018/6/4.
//
#pragma once

#include <kivm/kivm.h>
#include <kivm/runtime/vmThread.h>
#include <shared/monitor.h>

namespace kivm {
    enum GCState {
        GC_RUNNING,
        ENJOYING_HOLIDAY,
        WAITING_FOR_SAFEPOINT,
    };

    class GCThread : public VMThread {
    private:
        static GCThread *sGCThreadInstance;

    public:
        inline static GCThread *get() {
            if (sGCThreadInstance == nullptr) {
                PANIC("GCThread not initialized");
            }
            return sGCThreadInstance;
        }

        static void initialize();

    private:
        GCState _gcState;
        Monitor _triggerMonitor;
        Monitor _gcWaitMonitor;

    private:
        bool isAllThreadInSafePoint();

        void doGarbageCollection();

    protected:
        void run() override;

    public:
        void required();

        void wait();

        inline GCState getState() const {
            return _gcState;
        }
    };
}
