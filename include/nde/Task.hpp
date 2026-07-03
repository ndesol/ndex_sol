#pragma once

#include <chrono>
#include <string>

namespace nde {

enum class TaskType {
    CpuHashBatch,
    GpuLedgerScan,
    DiskShardVerify
};

enum class TaskStatus {
    Pending,
    Running,
    Completed,
    Skipped
};

struct Task {
    std::string id;
    TaskType type = TaskType::CpuHashBatch;
    TaskStatus status = TaskStatus::Pending;
    int budgetUnits = 0;
    int completedUnits = 0;
    std::string validationPacket;
    std::string syntheticProof;
    std::chrono::milliseconds simulatedDuration{0};

    std::string typeName() const;
    std::string statusName() const;
};

} // namespace nde
