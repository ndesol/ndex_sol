#include "nde/SolanaClient.hpp"

#include <functional>
#include <iostream>
#include <sstream>

namespace nde {

SolanaClient::SolanaClient(AppConfig config) : config_(std::move(config)) {}

bool SolanaClient::connect() {
    connected_ = true;
    std::cout << "Solana RPC boundary connected: " << config_.solanaRpcUrl << '\n';
    std::cout << "Solana program boundary selected: " << config_.solanaProgramId << '\n';
    std::cout << "Network route: " << config_.proxy.endpoint() << '\n';
    return connected_;
}

SolanaClusterStatus SolanaClient::fetchClusterStatus() {
    if (!connected_) {
        return {};
    }

    slotCursor_ += 64;
    return {slotCursor_, slotCursor_ / 432000, 4200.5};
}

std::string SolanaClient::requestValidationPacket(int round) {
    std::ostringstream packet;
    packet << "ledger-packet-round-" << round
           << "-slot-" << slotCursor_
           << "-program-" << config_.solanaProgramId;
    return packet.str();
}

std::string SolanaClient::buildProgramInstructionPayload(const std::string& functionName) const {
    std::ostringstream payload;
    payload << "{"
            << "\"jsonrpc\":\"2.0\","
            << "\"id\":\"" << config_.clientId << "\","
            << "\"method\":\"sendTransaction\","
            << "\"params\":[{"
            << "\"programId\":\"" << config_.solanaProgramId << "\","
            << "\"function\":\"" << functionName << "\","
            << "\"wallet\":\"" << config_.publicWalletAddress << "\","
            << "\"proxy\":\"" << config_.proxy.endpoint() << "\""
            << "}]"
            << "}";
    return payload.str();
}

bool SolanaClient::callProgramBaseFunction(const std::string& functionName) {
    if (!connected_) {
        return false;
    }

    std::cout << "Solana program demo call: " << buildProgramInstructionPayload(functionName) << '\n';
    return true;
}

bool SolanaClient::submitSyntheticProof(const Task& task) {
    if (!connected_) {
        return false;
    }

    std::cout << "Solana proof accepted for " << task.id
              << " program=" << config_.solanaProgramId
              << " proof=" << task.syntheticProof << '\n';
    return true;
}

} // namespace nde
