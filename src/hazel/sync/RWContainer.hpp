#pragma once

#include <mutex>
#include <shared_mutex>
#include <functional>
#include <type_traits>

namespace hazel {


/**
 * Utility container for a data object with a mutex wrapper.
 *
 * Note that if T is a pointer type, the behaviour of this class is undefined,
 * and does not guarantee synchronisation.
 */
template <typename T>
class RWContainer {
private:
    T data;
    std::shared_mutex mutex;

public:
    RWContainer() : data() {}
    RWContainer(const T& initVal) : data(initVal) {}
    RWContainer(T&& initVal) : data(std::move(initVal)) {}

    void read(std::function<void(const T&)> callback) {
        std::shared_lock<std::shared_mutex> lock(mutex);
        callback(data);
    }

    /**
     * Returns a copy of the underlying data, if copyable.
     */
    template <
        typename S = T,
        typename = std::enable_if_t<std::is_copy_assignable_v<S>>
    >
    S copy() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data;
    }

    void write(std::function<void(T&)> callback) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        callback(data);
    }

    /**
     * Utility function that returns the object without locking it.
     * This is fundamentally unsafe and should not be used unless
     * there's a very good reason to do so.
     */
    T& raw() {
        return data;
    }
    
};

}
