#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
template <typename T> class ThreadQueue {
  private:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> internal_queue;

  public:
    void send_message(T msg) {
      mtx.lock();
      internal_queue.push(msg);
      cv.notify_one();
      mtx.unlock();
    }

    std::optional<T> try_receive_message() {
      mtx.lock();
      std::optional<T> msg = (internal_queue.empty()) ? std::nullopt : std::optional(internal_queue.front());
      mtx.unlock();
      if (msg.has_value()) {
        internal_queue.pop();
      }
      return msg;
    }

    T wait_message() {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [this] { return !internal_queue.empty(); });
      auto msg = internal_queue.front();
      internal_queue.pop();
      return msg;
    }

};
