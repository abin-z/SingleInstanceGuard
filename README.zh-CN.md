# SingleInstanceGuard

[English](README.md) | [中文](README-Zh.md) | [示例代码](example/main.cpp) | [许可证](LICENSE)

## 简介

SingleInstanceGuard 是一个跨平台的 C++17 单例运行保护库，用于确保同一个应用在同一时刻只能运行一个实例。

它会基于应用名称创建一个全局锁：

- Linux / macOS：使用 `flock` 文件锁
- Windows：使用命名互斥量（Named Mutex）

适用于桌面应用、后台服务、命令行工具等需要防止重复启动的场景。

## 目录

- [特性](#特性)
- [依赖要求](#依赖要求)
- [快速开始](#快速开始)
- [使用示例](#使用示例)
- [API 说明](#api-说明)
- [示例程序](#示例程序)
- [注意事项](#注意事项)
- [许可证](#许可证)

## 特性

- 跨平台单实例保护
- RAII 资源管理方式
- 简单直观的 API
- 可以直接集成到 CMake 项目中

## 依赖要求

- CMake 3.15 或更高版本
- 支持 C++17 的编译器
- 支持的平台：Linux、macOS、Windows

## 快速开始

```bash
git clone <repository-url>
cd SingleInstanceGuard
mkdir build && cd build
cmake ..
cmake --build .
```

如果你还想一起编译示例程序，可执行：

```bash
cmake -DSINGLE_INSTANCE_GURARD_BUILD_EXAMPLES=ON ..
cmake --build .
```

随后运行示例程序：

```bash
./example/main_test
```

## 使用示例

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

当对象销毁时，锁会自动释放；进程退出时也会自动释放相关资源。

## API 说明

```cpp
abin::SingleInstanceGuard guard("app_name");

bool acquired = guard.acquired();
bool ok = static_cast<bool>(guard);
```

### 主要方法

- `SingleInstanceGuard(const std::string &name)`
  - 使用指定应用名创建保护锁。
- `bool acquired() const noexcept`
  - 判断当前实例是否成功获取到锁。
- `operator bool() const noexcept`
  - 等价于判断当前是否为唯一运行实例。

## 示例程序

完整示例代码在 [example/main.cpp](example/main.cpp)。

它演示了以下内容：

- 创建单实例保护对象
- 如果已有实例正在运行，则立即退出
- 可通过 Ctrl+C 退出程序并释放锁

## 注意事项

- 锁的标识基于应用名称生成，因此不同名称的应用可以并发运行。
- 建议使用稳定且唯一的应用标识，例如产品名、应用 ID 或包名。
- 在类 Unix 系统中，锁文件会被创建在 `/tmp/.single_instance` 目录下。

## 许可证

本项目使用 [LICENSE](LICENSE) 中声明的许可证。

---

## English

[中文](README-Zh.md) | [Example Code](example/main.cpp) | [License](LICENSE)

SingleInstanceGuard is a small cross-platform C++17 library that ensures only one instance of an application can run at a time.

It creates a named lock based on the application name:

- Linux / macOS: file lock using `flock`
- Windows: named mutex

This is useful for desktop tools, background services, and command-line programs that should not be launched repeatedly.

### Features

- Cross-platform single-instance enforcement
- RAII-based lifecycle management
- Simple API
- Easy integration with CMake projects

### Requirements

- CMake 3.15 or newer
- C++17 compiler
- Supported OS: Linux, macOS, Windows

### Quick start

```bash
git clone <repository-url>
cd SingleInstanceGuard
mkdir build && cd build
cmake ..
cmake --build .
```

To also build the example program:

```bash
cmake -DSINGLE_INSTANCE_GURARD_BUILD_EXAMPLES=ON ..
cmake --build .
```

Run the sample:

```bash
./example/main_test
```

### Usage

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

The lock is released automatically when the guard is destroyed or the process exits.

### API

```cpp
abin::SingleInstanceGuard guard("app_name");

bool acquired = guard.acquired();
bool ok = static_cast<bool>(guard);
```

#### Methods

- `SingleInstanceGuard(const std::string &name)`
  - Creates the lock for the provided application name.
- `bool acquired() const noexcept`
  - Returns whether this instance successfully acquired the lock.
- `operator bool() const noexcept`
  - Equivalent to checking whether this is the only running instance.

### Example

A complete example is provided in [example/main.cpp](example/main.cpp).

It demonstrates how to:

- create the guard
- exit immediately when another instance is already running
- shut down cleanly via signal handling

### Notes

- The lock ID is derived from the application name, so different names can run concurrently.
- Use a stable and unique application identifier such as your product name or bundle ID.
- On Unix-like systems, lock files are stored in `/tmp/.single_instance`.

### License

This project is distributed under the license described in [LICENSE](LICENSE).
