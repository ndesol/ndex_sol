#pragma once

#include "nde/AppConfig.hpp"
#include "nde/ComputeProfile.hpp"
#include "nde/Task.hpp"

#include <string>
#include <vector>

namespace nde {

class DashboardClient {
public:
    explicit DashboardClient(AppConfig config);

    bool connect();
    std::string registerClient(const ComputeProfile& profile);
    void sendHeartbeat(int round, const std::vector<Task>& tasks);
    void submitTelemetry(const ComputeProfile& profile, const std::vector<Task>& tasks);

private:
    AppConfig config_;
    bool connected_ = false;
    std::string sessionId_;
};

} // namespace nde
