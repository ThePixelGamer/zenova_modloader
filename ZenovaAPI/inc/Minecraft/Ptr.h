//Thanks McMrARM :)

#pragma once

#include <atomic>
#include <utility>

template<typename T>
struct SharedCounter {
    /* volatile ? */ T* value;
    std::atomic<int> shared, weak;

    SharedCounter(T* value) : value(value) {}

    void addSharedRef() { ++shared; }

    void addWeakRef() { --shared; }

    bool releaseSharedRef() {
        if (--shared == 0) {
            if (value != nullptr) {
                T* oldValue = value;
                value = nullptr;
                delete oldValue;
            }
            return (weak == 0);
        }
        return false;
    }

    bool releaseWeakRef() {
        return (--weak == 0 && value);
    }
};

template<typename T>
struct SharedPtr {
    SharedCounter<T>* counter = nullptr;

    SharedPtr(T* val = nullptr) {
        if (val) {
            counter = new SharedCounter<T>(val);
			counter->shared = 0;
            counter->addSharedRef();
        }
    }

    SharedPtr(SharedPtr const& a) : counter(a.counter) {
        if (counter)
            counter->addSharedRef();
    }

	template<typename T2>
	SharedPtr(SharedPtr<T2> const& ptr) {
		counter = (SharedCounter<T>*)ptr.counter;
		counter->addSharedRef();
	}

    void reset() {
        if (counter && counter->releaseSharedRef())
            delete counter;
        counter = nullptr;
    }

    SharedPtr& operator=(SharedPtr const& ptr) {
        reset();
        this->counter = ptr.counter;
        if (counter)
            counter->addSharedRef();
        return *this;
    }

    ~SharedPtr() {
        reset();
    }

	template <typename... Args>
	static SharedPtr<T> make(Args&&... args) {
		return SharedPtr<T>(new T(std::forward<Args>(args)...));
	}

    T& operator*() {
        return *counter->value;
    }

    T* operator->() {
        return counter->value;
    }

    T* get() {
        if (!counter)
            return nullptr;
        return counter->value;
    }
};

template<typename T>
struct WeakPtr {
    SharedCounter<T>* counter = nullptr;

    WeakPtr(T* val = nullptr) {
        if (val) {
            counter = new SharedCounter<T>(val);
            counter->addWeakRef();
        }
    }

	WeakPtr(WeakPtr const& ptr) {
		counter = ptr.counter;
		++counter->weak;
	}

    WeakPtr(WeakPtr&& ptr) noexcept {
        counter = std::move(ptr.counter);
        ptr.counter = nullptr;
    }

	template<typename T2>
	WeakPtr(WeakPtr<T2> const& ptr) {
		counter = (SharedCounter<T>*)ptr.counter;
		++counter->weak;
	}

	template<typename T2>
	WeakPtr(SharedPtr<T2> const& ptr) {
		counter = (SharedCounter<T>*)ptr.counter;
		++counter->weak;
	}

    WeakPtr& operator=(WeakPtr const& ptr) {
        reset();
        this->counter = ptr.counter;
        if (counter)
            counter->addWeakRef();
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& ptr) {
        counter = std::move(ptr.counter);
        ptr.counter = nullptr;
        return *this;
    }

    void reset() {
        if (counter && counter->releaseWeakRef())
            delete counter;
        counter = nullptr;
    }

    ~WeakPtr() {
        reset();
    };

    template <typename... Args>
    static WeakPtr<T> make(Args&&... args) {
        return WeakPtr<T>(new T(std::forward(args...)));
    }

    T& operator*() {
        return *counter->value;
    }

    T* operator->() const {
        return counter->value;
    }

    T* get() {
        if (!counter)
            return nullptr;
        return counter->value;
    }

    T const* get() const {
        if (!counter)
            return nullptr;
        return counter->value;
    }
};