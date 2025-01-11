#pragma once

#include <mutex>
#include <optional>
#include <queue>
template <typename T> class ThreadQueue {
  private:
    std::mutex mtx;
    std::queue<T> internal_queue;

  public:
    void send_message(T msg) {
      mtx.lock();
      internal_queue.push(msg);
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

};
