#include "nde/DashboardClient.hpp"

#include <iostream>
#include <sstream>

namespace nde {

DashboardClient::DashboardClient(AppConfig config) : config_(std::move(config)) {}

bool DashboardClient::connect() {
    connected_ = true;
    std::cout << "Dashboard boundary connected: " << config_.dashboardUrl << '\n';
    return connected_;
}

std::string DashboardClient::registerClient(const ComputeProfile& profile) {
    if (!connected_) {
        return {};
    }

    std::ostringstream session;
    session << "nde-session-" << config_.clientId << "-" << profile.cpuThreads << "c";
    sessionId_ = session.str();
    std::cout << "Dashboard boundary registered client: " << sessionId_ << '\n';
    return sessionId_;
}

void DashboardClient::sendHeartbeat(int round, const std::vector<Task>& tasks) {
    if (!connected_) {
        return;
    }

    std::cout << "Dashboard heartbeat round=" << round << " tasks=" << tasks.size() << '\n';
}

void DashboardClient::submitTelemetry(const ComputeProfile& profile, const std::vector<Task>& tasks) {
    if (!connected_) {
        return;
    }

    int completed = 0;
    for (const auto& task : tasks) {
        if (task.status == TaskStatus::Completed) {
            ++completed;
        }
    }

    std::cout << "Dashboard telemetry session=" << sessionId_
              << " profile={" << profile.summary() << "}"
              << " completedTasks=" << completed << "/" << tasks.size() << '\n';
}

} // namespace nde
