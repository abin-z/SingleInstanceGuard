# SingleInstanceGuard

[English](README.md) | [中文](README-Zh.md) | [Example Code](example/main.cpp) | [License](LICENSE)

A small cross-platform C++17 library for ensuring that only one instance of an application is running at a time.

It uses a named lock per application ID:

- Linux / macOS: file lock via `flock`
- Windows: named mutex

This is useful for desktop tools, background services, and any program that should not be launched multiple times concurrently.

## Features

- Cross-platform single-instance enforcement
- RAII-based lifecycle management
- Simple API
- Easy to integrate into existing CMake projects

## Requirements

- CMake 3.15 or newer
- C++17 compiler
- Operating system: Linux, macOS, or Windows

## Quick Start

```bash
git clone <repository-url>
cd SingleInstanceGuard
mkdir build && cd build
cmake ..
cmake --build .
```

If you want to build the example program as well:

```bash
cmake -DSINGLE_INSTANCE_GURARD_BUILD_EXAMPLES=ON ..
cmake --build .
```

Then run the sample:

```bash
./example/main_test
```

## Usage

```cpp
#include <iostream>
#include "abin/single_instance_guard.h"

int main()
{
    abin::SingleInstanceGuard guard("my_application");

    if (!guard)
    {
        std::cerr << "Another instance is already running." << std::endl;
        return 1;
    }

    std::cout << "Application started successfully." << std::endl;
    return 0;
}
```

The guard is automatically released when it is destroyed or when the process exits.

## API

```cpp
abin::SingleInstanceGuard guard("app_name");

bool acquired = guard.acquired();
bool ok = static_cast<bool>(guard);
```

### Methods

- `SingleInstanceGuard(const std::string &name)`
  - Creates the lock for the given application name.
- `bool acquired() const noexcept`
  - Returns whether this instance successfully acquired the lock.
- `operator bool() const noexcept`
  - Equivalent to checking whether the application is the only active instance.

## Example

A complete example is provided in [example/main.cpp](example/main.cpp).

It demonstrates how to:

- create the guard
- exit immediately if another instance is already running
- wait for user interrupt to exit cleanly

## Notes

- The lock key is generated from the app name, so different names can run concurrently.
- Use a stable, unique application identifier such as your product name or bundle ID.
- On Unix-like systems, the implementation stores lock files under `/tmp/.single_instance`.

## License

This project is distributed under the project license in [LICENSE](LICENSE).
