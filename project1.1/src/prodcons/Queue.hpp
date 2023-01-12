/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>
/**
 * @brief A encapsulation of a queue to make it thread safe
 * using a mutex and a semaphore
 * Produces network messages and push them to the queue
 * @details this is based on the unbouded buffer pattern
 * to make it bounded, the queue must implemented with a semaphore to produce
 */
#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <mutex>
#include <queue>

#include "common.hpp"
#include "Semaphore.hpp"

/**
 * @brief A thread-safe generic queue for consumer-producer pattern.
 *
 * @remark None of the methods of this class can be const because all
 * methods require lock the mutex to avoid race-conditions
 */
template <typename DataType>
class Queue {
  /*! \cond PRIVATE */
  // copies are not allow for this class, disables the 5 points rule
  DISABLE_COPY(Queue);
  /*! \endcond */

 protected:
  /// All read or write operations are mutually exclusive
  std::mutex mutex;
  /// Indicates if there are consumable elements in the queue
  Semaphore canConsume;
  /// Contains the actual data shared between producer and consumer
  std::queue<DataType> queue;

 public:
  /// Constructor
  Queue()
    : canConsume(0) {
  }

  /// Destructor
  ~Queue() {
    // TODO(jhc): clear()?
  }

  /// Produces an element that is pushed in the queue
  /// The semaphore is increased to wait potential consumers
  // here we encapsulate the push operation in a mutex
  // and we increase the semaphore to indicate that there is a new element
  void push(const DataType& data) {
    this->mutex.lock();
    this->queue.push(data);
    this->mutex.unlock();
    this->canConsume.signal();
  }

  /// Consumes the next available element. If the queue is empty, blocks the
  /// calling thread until an element is produced and enqueue
  /// @return A copy of the element that was removed from the queue
  DataType pop() {
    // consumers arrive here and wait until there is an element to consume
    this->canConsume.wait();
    // when the semaphore is signaled, the consumer can continue
    // and consume the element
    // but it locks the mutex so that there is no condition race
    this->mutex.lock();
    DataType result = this->queue.front();
    this->queue.pop();
    this->mutex.unlock();
    return result;
  }
};

#endif  // QUEUE_HPP
