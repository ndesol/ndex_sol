#pragma once

#include "nde/AppConfig.hpp"
#include "nde/ComputeProfile.hpp"
#include "nde/Task.hpp"

#include <vector>

namespace nde {

class ResourceScheduler {
public:
    ResourceScheduler(ResourcePolicy policy, ComputeProfile profile);

    std::vector<Task> planInitialTasks();
    void executeRound(std::vector<Task>& tasks, int round);

private:
    int cpuBudgetUnits() const;
    int gpuBudgetUnits() const;
    int diskBudgetUnits() const;

    ResourcePolicy policy_;
    ComputeProfile profile_;
};

} // namespace nde
