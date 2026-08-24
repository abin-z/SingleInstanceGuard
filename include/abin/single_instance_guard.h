/**
 * @file single_instance_guard.h
 *
 * @brief Provides a cross-platform single-instance guard.
 */
#pragma once

#include <memory>
#include <string>

namespace abin
{

/**
 * @brief RAII guard that prevents multiple instances of the same application
 *        from running simultaneously.
 *
 * The guard acquires an exclusive named process lock when created.
 * If another instance with the same name is already running,
 * the lock acquisition fails.
 *
 * The lock is automatically released when the object is destroyed
 * or when the process exits.
 *
 * Platform implementation:
 * - Linux/macOS: flock based file lock
 * - Windows: named mutex
 */
class SingleInstanceGuard {
 public:
  /**
   * @brief Create a single instance guard.
   *
   * @param name Unique identifier of the application instance.
   */
  explicit SingleInstanceGuard(const std::string &name);

  ~SingleInstanceGuard();

  explicit operator bool() const noexcept;

  /**
   * @brief Check whether this instance acquired the lock.
   *
   * @return true if this is the only running instance.
   */
  [[nodiscard]] bool acquired() const noexcept;

  SingleInstanceGuard(const SingleInstanceGuard &) = delete;
  SingleInstanceGuard &operator=(const SingleInstanceGuard &) = delete;
  SingleInstanceGuard(SingleInstanceGuard &&other) noexcept;
  SingleInstanceGuard &operator=(SingleInstanceGuard &&other) noexcept;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace abin