#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "abin/single_instance_guard.h"

namespace
{
std::atomic_bool running{true};

void signal_handler(int signal)
{
  if (signal == SIGINT || signal == SIGTERM)
  {
    running.store(false);
  }
}

}  // namespace

int main()
{
  if (std::signal(SIGINT, signal_handler) == SIG_ERR || std::signal(SIGTERM, signal_handler) == SIG_ERR)
  {
    std::cerr << "Failed to install signal handler." << std::endl;
    return 1;
  }

  abin::SingleInstanceGuard guard("test_application");

  if (!guard)
  {
    std::cout << "Another instance is already running." << std::endl;

    return 1;
  }

  std::cout << "Instance acquired successfully." << std::endl;

  std::cout << "Press Ctrl+C to exit." << std::endl;

  while (running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  std::cout << "\nReceived Ctrl+C, exiting...\n";
  std::cout << "Guard will be destroyed, lock released." << std::endl;

  return 0;
}