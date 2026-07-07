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
3. The Solana client performs an RPC handshake through the configured direct or proxy route.
4. The Solana client selects program `FkMPbGB3bs2at6cdytVAeX4s9beCUd7wtReUntQSzket`.
5. The dashboard client performs a registration with `ndesol.top` through the integration boundary.
6. The compute profiler builds a hardware profile.
7. The scheduler creates CPU, GPU, and disk verification tasks.
8. The engine runs several rounds of local validation work.
9. Telemetry is printed to the console and written to the audit log.

## Configuration

```json
{
  "clientId": "nde-node-001",
  "dashboardUrl": "https://ndesol.top",
  "solanaRpcUrl": "https://api.mainnet-beta.solana.com",
  "solanaProgramId": "FkMPbGB3bs2at6cdytVAeX4s9beCUd7wtReUntQSzket",
  "publicWalletAddress": "PUBLIC_WALLET_ADDRESS_ONLY",
  "proxy": {
    "type": "none",
    "host": "127.0.0.1",
    "port": 7890,
    "username": "",
    "password": ""
  },
  "resourcePolicy": {
    "cpuCoreCount": 2,
    "memoryMb": 2048,
    "maxDiskGb": 5,
    "maxCpuPercent": 35,
    "maxGpuPercent": 25,
    "maxDiskMbps": 10,
    "allowGpu": true,
    "allowDiskTasks": true
  }
}
```

### Configuration Fields

- `clientId`: Local client identifier. Change it to any readable node name.
- `solanaRpcUrl`: Solana RPC endpoint. The default uses the public mainnet-beta RPC.
- `publicWalletAddress`: Public wallet address only. Do not enter a private key, mnemonic, secret, or seed.
- `proxy.type`: Set to `none`, `http`, or `socks`.
- `proxy.host`: Proxy server host, such as `127.0.0.1`.
- `proxy.port`: Proxy server port, such as `7890`.
- `proxy.username` / `proxy.password`: Optional proxy credentials. Leave both empty when authentication is not required.
- `resourcePolicy.maxCpuPercent`: CPU usage percentage limit.
- `resourcePolicy.maxGpuPercent`: GPU usage percentage limit.
- `resourcePolicy.maxDiskMbps`: Disk throughput limit, in MB/s.

### Proxy Examples

No proxy:

```json
"proxy": {
  "type": "none",
  "host": "",
  "port": 0,
  "username": "",
  "password": ""
}
```

HTTP proxy:

```json
"proxy": {
  "type": "http",
  "host": "127.0.0.1",
  "port": 7890,
  "username": "",
  "password": ""
}
```

SOCKS proxy:

```json
"proxy": {
  "type": "socks",
  "host": "127.0.0.1",
  "port": 1080,
  "username": "",
  "password": ""
}
```

## Building

Install a C++20 compiler and CMake 3.20 or newer, then run these commands from the repository root:

```bash
cmake -S . -B build
cmake --build build --config Release
```

On Windows with Visual Studio generators, the executable is usually created at:

```text
build/Release/native-deployment-engine.exe
```

On Linux or macOS, the executable is usually created at:

```text
build/native-deployment-engine
```

## Running

Run with the default config file:

```bash
./build/native-deployment-engine
```

On Windows PowerShell:

```powershell
.\build\Release\native-deployment-engine.exe
```

Run with a custom config path:

```bash
./build/native-deployment-engine config/ndengine.local.json
```

On Windows PowerShell:

```powershell
.\build\Release\native-deployment-engine.exe .\config\ndengine.local.json
```

## Design Notes

### Wallet Handling

`SafeWalletAdapter` is intentionally restrictive. It rejects any configuration field that appears to contain private key material. It keeps only a public wallet identifier for accounting and attribution.

### Solana Integration

`SolanaClient` is the boundary around Solana RPC behavior. It handles handshake state, slot fetches, epoch status, base program-call payload generation, and validator packet submission paths.

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


