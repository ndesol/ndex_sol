cmake_minimum_required(VERSION 3.20)

project(NativeDeploymentEngine
    VERSION 0.1.0
    DESCRIPTION "Demonstration client for community-powered Solana validator compute coordination"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(native-deployment-engine
    src/AppConfig.cpp
    src/AuditLog.cpp
    src/ComputeProfile.cpp
    src/DashboardClient.cpp
    src/Engine.cpp
    src/ResourceScheduler.cpp
    src/SafeWalletAdapter.cpp
    src/SolanaClient.cpp
    src/Task.cpp
    src/main.cpp
)

target_include_directories(native-deployment-engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_options(native-deployment-engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(native-deployment-engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
