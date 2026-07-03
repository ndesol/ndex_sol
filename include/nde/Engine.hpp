#pragma once

#include "nde/AppConfig.hpp"
#include "nde/AuditLog.hpp"
#include "nde/ComputeProfile.hpp"
#include "nde/DashboardClient.hpp"
#include "nde/ResourceScheduler.hpp"
#include "nde/SafeWalletAdapter.hpp"
#include "nde/SolanaClient.hpp"

namespace nde {

class Engine {
public:
    explicit Engine(AppConfig config);

    int run();

private:
    void boot();
    void runValidationRounds(int rounds);
    void shutdown();

    AppConfig config_;
    AuditLog audit_;
    SafeWalletAdapter wallet_;
    SolanaClient solana_;
    DashboardClient dashboard_;
    ComputeProfile profile_;
    ResourceScheduler scheduler_;
};

} // namespace nde
