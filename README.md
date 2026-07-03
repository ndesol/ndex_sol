# Native Deployment Engine
Native Deployment Engine is a C++ client architecture for coordinating community-provided compute capacity around one or more Solana validator initiatives.

## Concept

The project is a client application that can:

- Load an explicit user configuration.
- Register the local machine as an opted-in compute contributor.
- Coordinate with a Solana RPC endpoint boundary.
- Coordinate with the Native Deployment Engine dashboard boundary.
- Build a local resource profile for CPU, GPU, memory, and disk.
- Schedule local verification workloads within configured limits.
- Report telemetry and task status to the dashboard layer.

The product concept is to let many users pool computing capacity to support Solana validator operations. In this repository, validator and compute behavior is represented with local deterministic control flow so the architecture can be reviewed without external dependencies.

## Safety Model

The client design avoids unsafe behavior:

- No automatic private key discovery or import.
- No wallet secret parsing.
- No hidden resource usage.
- No background persistence.
- No remote command execution.
- No production network dependency.

The client accepts only a public wallet identifier in the local configuration. Any production-grade wallet integration should use explicit user consent, hardware wallets or secure signing providers, and never read private keys from disk automatically.

## Repository Layout

```text
NativeDeploymentEngine/
  CMakeLists.txt
  README.md
  config/
    ndengine.local.json
  include/nde/
    AppConfig.hpp
    AuditLog.hpp
    ComputeProfile.hpp
    DashboardClient.hpp
    Engine.hpp
    ResourceScheduler.hpp
    SafeWalletAdapter.hpp
    SolanaClient.hpp
    Task.hpp
  src/
    AppConfig.cpp
    AuditLog.cpp
    ComputeProfile.cpp
    DashboardClient.cpp
    Engine.cpp
    ResourceScheduler.cpp
    SafeWalletAdapter.cpp
    SolanaClient.cpp
    Task.cpp
    main.cpp
```

## Runtime Flow

1. The executable loads `config/ndengine.local.json`.
2. The wallet adapter validates that only a public wallet address is present.
3. The Solana client performs an RPC handshake through the integration boundary.
4. The dashboard client performs a registration with `ndesol.top` through the integration boundary.
5. The compute profiler builds a hardware profile.
6. The scheduler creates CPU, GPU, and disk verification tasks.
7. The engine runs several rounds of local validation work.
8. Telemetry is printed to the console and written to the audit log.

## Configuration

```json
{
  "clientId": "nde-node-001",
  "dashboardUrl": "https://ndesol.top",
  "solanaRpcUrl": "https://api.mainnet-beta.solana.com",
  "publicWalletAddress": "PUBLIC_WALLET_ADDRESS_ONLY",
  "resourcePolicy": {
    "maxCpuPercent": 35,
    "maxGpuPercent": 25,
    "maxDiskMbps": 10,
    "allowGpu": true,
    "allowDiskTasks": true
  }
}
```

## Building

A typical CMake build uses the following commands:

```bash
cmake -S . -B build
cmake --build build --config Release
```

## Design Notes

### Wallet Handling

`SafeWalletAdapter` is intentionally restrictive. It rejects any configuration field that appears to contain private key material. It keeps only a public wallet identifier for accounting and attribution.

### Solana Integration

`SolanaClient` is the boundary around Solana RPC behavior. It handles handshake state, slot fetches, epoch status, and validator packet submission paths.

### Dashboard Integration

`DashboardClient` models registration, heartbeat, and telemetry calls. A production implementation would use TLS, signed messages, strict schemas, retry budgets, and transparent user-facing status.

### Resource Scheduling

`ResourceScheduler` creates tasks based on user policy and compute profile. It shows how a client can respect CPU, GPU, and disk limits before accepting work.

### Verification Pipeline

Each `Task` has a type, target resource, budget, status, and proof string. The proof field is where attestation or work-result metadata would be attached.

## Production Considerations

A production implementation would need:

- Explicit opt-in screens and visible runtime controls.
- Secure wallet integration through external signers.
- Sandboxed workload execution.
- Cryptographic task attestation.
- Auditable source builds.
- Rate limits and thermal safeguards.
- Clear uninstall and pause behavior.
- Legal review for compute sharing and token incentives.


