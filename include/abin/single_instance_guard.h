/**
 * @file single_instance_guard.h
 * @brief Provides a cross-platform single-instance guard.
 * @author abin
 * @date 2026-08-15
 */

#ifndef __GUARD_SINGLE_INSTANCE_GUARD_H_INCLUDE_GUARD__
#define __GUARD_SINGLE_INSTANCE_GUARD_H_INCLUDE_GUARD__
#pragma once

#include <memory>
#include <string>

namespace abin
{

/**
 * @brief RAII guard that prevents multiple instances of the same application
 *        from running simultaneously.
 *
 * The guard acquires an exclusive named process lock when constructed.
 * If another instance with the same name is already running, lock acquisition
 * fails.
 *
 * The lock is automatically released when the guard is destroyed.
 *
 * Platform implementation:
 * - Linux/macOS: file lock based on flock
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

  /**
   * @brief Release the acquired lock.
   */
  ~SingleInstanceGuard();

  /**
   * @brief Check whether the guard successfully acquired the lock.
   *
   * @return true if the lock was acquired.
   * @return false if another instance is already running.
   */
  explicit operator bool() const noexcept;

  /**
   * @brief Check whether this instance acquired the lock.
   *
   * @return true if this is the only running instance.
   */
  [[nodiscard]] bool acquired() const noexcept;

  SingleInstanceGuard(const SingleInstanceGuard &) = delete;
  SingleInstanceGuard &operator=(const SingleInstanceGuard &) = delete;
  /**
   * @brief Move-construct a single instance guard.
   *
   * @param other Guard to move from.
   */
  SingleInstanceGuard(SingleInstanceGuard &&other) noexcept;
  /**
   * @brief Move-assign a single instance guard.
   *
   * @param other Guard to move from.
   *
   * @return Reference to this guard.
   */
  SingleInstanceGuard &operator=(SingleInstanceGuard &&other) noexcept;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace abin

#endif  // __GUARD_SINGLE_INSTANCE_GUARD_H_INCLUDE_GUARD__