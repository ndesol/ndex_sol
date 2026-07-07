#include "nde/Engine.hpp"

#include <stdexcept>

namespace nde {

Engine::Engine(AppConfig config)
    : config_(std::move(config)),
      audit_("nde-audit.log"),
      wallet_(config_),
      solana_(config_),
      dashboard_(config_),
      profile_(ComputeProfile::detectSynthetic()),
      scheduler_(config_.resourcePolicy, profile_) {}

int Engine::run() {
    try {
        boot();
        runValidationRounds(3);
        shutdown();
        return 0;
    } catch (const std::exception& ex) {
        audit_.error("engine", ex.what());
        return 1;
    }
}

void Engine::boot() {
    audit_.info("engine", "Boot sequence started for Native Deployment Engine");
    audit_.info("profile", profile_.summary());

    if (!wallet_.initialize()) {
        throw std::runtime_error("Wallet adapter refused configuration");
    }
    audit_.info("wallet", "Public identity loaded: " + wallet_.publicIdentity());

    if (!solana_.connect()) {
        throw std::runtime_error("Solana connection boundary failed");
    }
    if (!solana_.callProgramBaseFunction("initialize_client")) {
        throw std::runtime_error("Solana program demo call failed");
    }
    audit_.info("solana", "Program initialized: " + config_.solanaProgramId);

    if (!dashboard_.connect()) {
        throw std::runtime_error("Dashboard connection boundary failed");
    }

    const auto session = dashboard_.registerClient(profile_);
    audit_.info("dashboard", "Client registered with session: " + session);
}

void Engine::runValidationRounds(int rounds) {
    auto tasks = scheduler_.planInitialTasks();

    for (int round = 1; round <= rounds; ++round) {
        const auto cluster = solana_.fetchClusterStatus();
        audit_.info("solana", "Cluster slot=" + std::to_string(cluster.currentSlot) +
                                  " epoch=" + std::to_string(cluster.epoch));

        const auto packet = solana_.requestValidationPacket(round);
        solana_.callProgramBaseFunction("request_validation_packet");
        for (auto& task : tasks) {
            task.validationPacket = packet;
        }

        scheduler_.executeRound(tasks, round);

        for (const auto& task : tasks) {
            if (task.status == TaskStatus::Completed) {
                solana_.submitSyntheticProof(task);
            }
        }

        dashboard_.sendHeartbeat(round, tasks);
        dashboard_.submitTelemetry(profile_, tasks);
    }
}

void Engine::shutdown() {
    audit_.info("engine", "Shutdown sequence completed");
}

} // namespace nde
