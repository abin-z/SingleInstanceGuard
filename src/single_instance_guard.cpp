#include "abin/single_instance_guard.h"

#include <cstdint>
#include <filesystem>
#include <sstream>

namespace
{
bool ensure_directory(const std::string &path)
{
  namespace fs = std::filesystem;

  std::error_code ec;

  if (fs::is_directory(path, ec))
  {
    return true;
  }

  ec.clear();
  fs::create_directories(path, ec);

  if (!ec)
  {
    return true;
  }

  return fs::is_directory(path, ec);
}

std::string make_lock_id(const std::string &name)
{
  uint64_t hash = 14695981039346656037ULL;

  for (unsigned char c : name)
  {
    hash ^= c;
    hash *= 1099511628211ULL;
  }

  std::ostringstream oss;
  oss << std::hex << hash;

  return oss.str();
}

}  // namespace

#ifdef _WIN32

#include <windows.h>

namespace abin
{

class SingleInstanceGuard::Impl {
 public:
  explicit Impl(const std::string &name)
  {
    std::string mutex_name = "Global\\abin_single_instance_" + make_lock_id(name);

    SetLastError(ERROR_SUCCESS);
    handle_ = CreateMutexA(nullptr, TRUE, mutex_name.c_str());

    if (handle_ == nullptr)
    {
      acquired_ = false;
      return;
    }

    const DWORD error = GetLastError();

    acquired_ = error != ERROR_ALREADY_EXISTS;
  }

  ~Impl()
  {
    if (handle_ != nullptr)
    {
      if (acquired_)
      {
        ReleaseMutex(handle_);
      }

      CloseHandle(handle_);
    }
  }

  [[nodiscard]] bool acquired() const noexcept
  {
    return acquired_;
  }

  Impl(const Impl &) = delete;
  Impl &operator=(const Impl &) = delete;
  Impl(Impl &&) = delete;
  Impl &operator=(Impl &&) = delete;

 private:
  HANDLE handle_{nullptr};
  bool acquired_{false};
};

}  // namespace abin

#else

// Linux / macOS

#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>

namespace abin
{

class SingleInstanceGuard::Impl {
 public:
  explicit Impl(const std::string &name)
  {
    const std::string dir = "/tmp/.single_instance";
    if (!ensure_directory(dir))
    {
      return;
    }
    std::string path = dir + "/" + make_lock_id(name) + ".lock";

    fd_ = open(path.c_str(), O_CREAT | O_RDWR, 0666);

    if (fd_ < 0)
    {
      acquired_ = false;
      return;
    }

    if (flock(fd_, LOCK_EX | LOCK_NB) != 0)
    {
      close(fd_);
      fd_ = -1;
      acquired_ = false;
      return;
    }

    acquired_ = true;
  }

  ~Impl()
  {
    if (fd_ >= 0)
    {
      flock(fd_, LOCK_UN);
      close(fd_);
    }
  }

  [[nodiscard]] bool acquired() const noexcept
  {
    return acquired_;
  }

  Impl(const Impl &) = delete;
  Impl &operator=(const Impl &) = delete;
  Impl(Impl &&) = delete;
  Impl &operator=(Impl &&) = delete;

 private:
  int fd_{-1};
  bool acquired_{false};
};

}  // namespace abin

#endif

namespace abin
{
SingleInstanceGuard::SingleInstanceGuard(const std::string &name) : impl_(std::make_unique<Impl>(name)) {}
SingleInstanceGuard::~SingleInstanceGuard() = default;
SingleInstanceGuard::SingleInstanceGuard(SingleInstanceGuard &&other) noexcept = default;
SingleInstanceGuard &SingleInstanceGuard::operator=(SingleInstanceGuard &&other) noexcept = default;

bool SingleInstanceGuard::acquired() const noexcept
{
  return impl_ && impl_->acquired();
}

SingleInstanceGuard::operator bool() const noexcept
{
  return acquired();
}

}  // namespace abin