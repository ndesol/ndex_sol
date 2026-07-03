#include "nde/ResourceScheduler.hpp"

#include <algorithm>
#include <functional>
#include <sstream>
#include <thread>

namespace nde {

namespace {

std::string pseudoProof(const Task& task, int round) {
    std::hash<std::string> hash;
    const auto seed = task.id + ":" + std::to_string(round) + ":" + task.validationPacket;
    std::ostringstream output;
    output << "nde-proof-" << std::hex << hash(seed);
    return output.str();
}

} // namespace

ResourceScheduler::ResourceScheduler(ResourcePolicy policy, ComputeProfile profile)
    : policy_(policy), profile_(std::move(profile)) {}

std::vector<Task> ResourceScheduler::planInitialTasks() {
    std::vector<Task> tasks;
    tasks.push_back({"cpu-hash-batch-001", TaskType::CpuHashBatch, TaskStatus::Pending, cpuBudgetUnits()});

    if (policy_.allowGpu && gpuBudgetUnits() > 0) {
        tasks.push_back({"gpu-ledger-scan-001", TaskType::GpuLedgerScan, TaskStatus::Pending, gpuBudgetUnits()});
    }

    if (policy_.allowDiskTasks && diskBudgetUnits() > 0) {
        tasks.push_back({"disk-shard-verify-001", TaskType::DiskShardVerify, TaskStatus::Pending, diskBudgetUnits()});
    }

    return tasks;
}

void ResourceScheduler::executeRound(std::vector<Task>& tasks, int round) {
    for (auto& task : tasks) {
        if (task.budgetUnits <= 0) {
            task.status = TaskStatus::Skipped;
            continue;
        }

        task.status = TaskStatus::Running;
        const int roundUnits = std::max(1, task.budgetUnits / 3);
        task.completedUnits = std::min(task.budgetUnits, task.completedUnits + roundUnits);
        task.simulatedDuration += std::chrono::milliseconds(25 * roundUnits);
        task.syntheticProof = pseudoProof(task, round);

        if (task.completedUnits >= task.budgetUnits) {
            task.status = TaskStatus::Completed;
        } else {
            task.status = TaskStatus::Pending;
        }
    }
}

int ResourceScheduler::cpuBudgetUnits() const {
    return std::max(1, profile_.cpuThreads * policy_.maxCpuPercent / 10);
}

int ResourceScheduler::gpuBudgetUnits() const {
    const auto available = std::count_if(profile_.gpuDevices.begin(), profile_.gpuDevices.end(), [](const GpuDevice& gpu) {
        return gpu.available;
    });
    return static_cast<int>(available) * std::max(0, policy_.maxGpuPercent / 5);
}

int ResourceScheduler::diskBudgetUnits() const {
    return std::min(profile_.estimatedDiskMbps, policy_.maxDiskMbps) / 2;
}

} // namespace nde
