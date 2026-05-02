# Toolchain file for macOS ARM (Apple Silicon)
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR arm64)

# Use clang which ships with Xcode on macOS
set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)

# Target arm64 architecture
set(CMAKE_C_FLAGS_INIT   "-arch arm64")
set(CMAKE_CXX_FLAGS_INIT "-arch arm64")

# macOS deployment target – adjust as needed
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0")
