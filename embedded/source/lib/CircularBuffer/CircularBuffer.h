#include <memory>

template <typename T, size_t bufferSize>
class CircularBuffer {
 public:
  void put(T item) {
    mBuffer[mHead] = item;

    if (mFull) {
      mTail = (mTail + 1) % bufferSize;
    }

    mHead = (mHead + 1) % bufferSize;

    mFull = (mHead == mTail);
  }

  T get() {
    if (empty()) {
      return T();
    }

    // Read data and advance the tail (we now have a free space)
    auto val = mBuffer[mTail];
    mFull    = false;
    mTail    = (mTail + 1) % bufferSize;

    return val;
  }

  void reset() {
    mHead = mTail;
    mFull = false;
  }

  bool empty() const {
    // if head and tail are equal, we are empty
    return (!mFull && (mHead == mTail));
  }

  bool full() const {
    // If tail is ahead the head by 1, we are full
    return mFull;
  }

  size_t capacity() const { return bufferSize; }

  size_t size() const {
    size_t size = bufferSize;

    if (!mFull) {
      if (mHead >= mTail) {
        size = mHead - mTail;
      } else {
        size = bufferSize + mHead - mTail;
      }
    }

    return size;
  }

  bool removeUntil(T item) {
    while (!empty()) {
      if (get() == item)
        return true;
    }
    return false;
  }

 private:
  T      mBuffer[bufferSize];
  size_t mHead = 0;
  size_t mTail = 0;
  bool   mFull = 0;
};
