#include "nde/Task.hpp"

namespace nde {

std::string Task::typeName() const {
    switch (type) {
        case TaskType::CpuHashBatch:
            return "cpu-hash-batch";
        case TaskType::GpuLedgerScan:
            return "gpu-ledger-scan";
        case TaskType::DiskShardVerify:
            return "disk-shard-verify";
    }
    return "unknown";
}

std::string Task::statusName() const {
    switch (status) {
        case TaskStatus::Pending:
            return "pending";
        case TaskStatus::Running:
            return "running";
        case TaskStatus::Completed:
            return "completed";
        case TaskStatus::Skipped:
            return "skipped";
    }
    return "unknown";
}

} // namespace nde
