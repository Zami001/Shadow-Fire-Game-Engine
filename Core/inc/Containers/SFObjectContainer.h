#pragma once

#include <Core.h>
#include <vector>
#include <Asserts.h>
#include <iostream>
#include <Logging/Log.h>
#include <mutex>
#include <atomic>

template<typename T, bool Threadsafe = false, bool StrongLink = true, bool AllowNull = true>
struct SFObjectLink;

template<typename T, bool Threadsafe>
class SharedFromThis;

template<typename T, bool Threadsafe = false>
using SFSharedPtr = SFObjectLink<T, Threadsafe, true, true>;

template<typename T, bool Threadsafe = false>
using SFSharedRef = SFObjectLink<T, Threadsafe, true, false>;

template<typename T, bool Threadsafe = false>
using SFWeakPtr = SFObjectLink<T, Threadsafe, false, true>;

template<bool Threadsafe = false>
struct SFObjectContainer {
	friend SFObjectLink;
	friend SharedFromThis;

private:
	struct ContainerRef {
		void** Value;
		void** Container;

		ContainerRef() = delete;
		ContainerRef(void** value, void** container) : Value(value), Container(container) {}
	};

	int StrongRefCount = 0;
	std::vector<ContainerRef> WeakReferences;

	SFObjectContainer() {}

	~SFObjectContainer() {
		for (ContainerRef Reference : WeakReferences) {
			*Reference.Value = nullptr;
			*Reference.Container = nullptr;
		}
	}

	template<typename T>
	inline void RemoveWeakLink(SFObjectLink<T, Threadsafe, false, true>& Ptr) {
		for (int i = 0; i < WeakReferences.size(); ++i) {
			if (WeakReferences[i].Value == reinterpret_cast<void**>(&Ptr.Value)) {
				WeakReferences[i] = WeakReferences[WeakReferences.size() - 1];
				WeakReferences.pop_back();
				break;
			}
		}
	}

	template<typename T>
	inline void AddWeakLink(SFObjectLink<T, Threadsafe, false, true>& Ptr) {
		WeakReferences.emplace_back(reinterpret_cast<void**>(&Ptr.Value), reinterpret_cast<void**>(&Ptr.Container));
	}

	// returns true when object should be released
	inline void Increment() {
		++StrongRefCount;
	}

	// returns true when object should be released
	inline bool Decrement() {
		return --StrongRefCount == 0;
	}
};

template<>
struct SFObjectContainer<true> {
	friend SFObjectLink;
	friend SharedFromThis;

private:
	struct ContainerRef {
		void** Value;
		void** Container;
		std::mutex* Lock;

		ContainerRef() = delete;
		ContainerRef(void** value, void** container, std::mutex* lock) : Value(value), Container(container), Lock(lock) {}
	};

	std::mutex lock; // must be locked when modifying list of weak pointers
	std::atomic<int> StrongRefCount = 0;
	std::vector<ContainerRef> WeakReferences;

	SFObjectContainer() {}

	~SFObjectContainer() {
		lock.lock();
		
		for (int i = 0; i < WeakReferences.size(); ++i) {
			WeakReferences[i].Lock->lock();
		}

		for (ContainerRef Reference : WeakReferences) {
			*Reference.Value = nullptr;
			*Reference.Container = nullptr;
		}

		lock.unlock();
	}

	template<typename T>
	inline void RemoveWeakLink(SFObjectLink<T, true, false, true> Ptr) {
		lock.lock();
		for (int i = 0; i < WeakReferences.size(); ++i) {
			if (WeakReferences[i].Value == reinterpret_cast<void**>(&Ptr.Value)) {
				WeakReferences[i] = WeakReferences[WeakReferences.size() - 1];
				WeakReferences.pop_back();
				break;
			}
		}
		lock.unlock();
	}

	template<typename T>
	inline bool TryRemoveWeakLink(SFObjectLink<T, true, false, true> Ptr) {
		if (!lock.try_lock()) {
			return false;
		}

		for (int i = 0; i < WeakReferences.size(); ++i) {
			if (WeakReferences[i].Value == reinterpret_cast<void**>(&Ptr.Value)) {
				WeakReferences[i] = WeakReferences[WeakReferences.size() - 1];
				WeakReferences.pop_back();
				break;
			}
		}

		lock.unlock();
	}

	template<typename T>
	inline void AddWeakLink(SFObjectLink<T, true, false, true>& Ptr) {
		lock.lock();
		WeakReferences.emplace_back(reinterpret_cast<void**>(&Ptr.Value), reinterpret_cast<void**>(&Ptr.Container), &Ptr.lock);
		lock.unlock();
	}

	// returns true when object should be released
	inline void Increment() {
		++StrongRefCount;
	}

	// returns true when object should be released
	inline bool Decrement() {
		return --StrongRefCount == 0;
	}
};

template<typename BaseType, bool Threadsafe>
concept Sharable = requires(BaseType Value) {
	{ Value.AsShared() };
};

template<bool Threadsafe>
struct SFObjectLinkBase {
	inline void Lock() const {}
	inline void Unlock() const {}
};

template<>
struct SFObjectLinkBase<true> {
	mutable std::mutex lock;
	
	inline void Lock() const {
		lock.lock();
	}

	inline void Unlock() const {
		lock.unlock();
	}
};

template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
struct SFObjectLink : private SFObjectLinkBase<Threadsafe && !StrongLink> {
	static_assert(StrongLink || AllowNull, "Either StrongLink or AllowNull must be set true");
	using ContainerType = SFObjectContainer<Threadsafe>;
	using ThisType = SFObjectLink<T, Threadsafe, StrongLink, AllowNull>;

	friend struct SFObjectContainer<Threadsafe>;

	template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
	friend struct SFObjectLink;

	template<typename T, bool Threadsafe>
	friend class SharedFromThis;

private:
	ContainerType* Container = nullptr;
	T* Value = nullptr;

public:
	SFObjectLink() {
		SF_ASSERT(AllowNull, "Cannot use default constructor on smart pointers that do not allow null");
	}

	template<typename OtherT, bool StrongLinkOther, bool AllowNullOther>
	requires(std::derived_from<OtherT, T>)
	SFObjectLink(const SFObjectLink<OtherT, Threadsafe, StrongLinkOther, AllowNullOther>& other) {
		other.Lock();

		AssignContainer(other.Container, other.Value);

		other.Unlock();
	}

	SFObjectLink(const ThisType& other) {
		other.Lock();

		AssignContainer(other.Container, other.Value);

		other.Unlock();
	}

	SFObjectLink(SFObjectLink&& other) noexcept {
		other.Lock();

		Container = other.Container;
		other.Container = nullptr;
		Value = other.Value;

		if constexpr (!StrongLink) {
			if (Container) {
				Container->RemoveWeakLink(other);
				Container->AddWeakLink(*this);
			}
		}

		other.Unlock();
	}

	template<bool StrongLinkOther, bool AllowNullOther>
	SFObjectLink(SFObjectLink<T, Threadsafe, StrongLinkOther, AllowNullOther>&& other) {
		other.Lock();
		
		Container = other.Container;
		other.Container = nullptr;
		Value = other.Value;

		if constexpr ((!StrongLink) || (!StrongLinkOther)) {
			if (Container) {
				if constexpr (StrongLink) {
					Container->Increment();
				} else {
					Container->AddWeakLink(*this);
				}

				if constexpr (StrongLinkOther) {
					if (--Container->StrongRefCount == 0) {
						delete Container;
						delete Value;
						Container = nullptr;
						Value = nullptr;
						return;
					}
				}
			}
		}

		other.Unlock();
	}

	template<typename Ptr>
	requires(std::derived_from<Ptr, T>)
	SFObjectLink(Ptr* value) {
		if constexpr (Sharable<Ptr, Threadsafe>) {
			if constexpr (AllowNull) {
				if (value) {
					auto SharedValue = value->AsShared();
					AssignContainer(reinterpret_cast<ContainerType*>(SharedValue->Container), value);
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a shared pointer type that doesn't allow null");
				auto SharedValue = value->AsShared();
				AssignContainer(reinterpret_cast<ContainerType*>(SharedValue->Container), value);
			}
		} else if constexpr (AllowNull) {
			if (value) {
				Container = new ContainerType();
				Value = value;

				if constexpr (StrongLink) {
					Container->Increment();
				} else {
					static_assert(StrongLink, "Cannot directly create a weak pointer from a raw pointer");
				}
			}
		} else {
			SF_ASSERT(value, "Cannot assign nullptr to a shared pointer type that doesn't allow null");
			Container = new ContainerType();
			Container->Increment();
			Value = value;
		}
	}

	SFObjectLink(std::nullptr_t) {
		SF_ASSERT(AllowNull, "Cannot construct a shared pointer type that doesn't allow null from a null pointer type");
	}

private:
	inline void Release() {
		if constexpr (StrongLink) {
			if (Container) {
				if (Container->Decrement()) {
					delete Container;
					delete Value;
				}
			}
		} else {
			if constexpr (Threadsafe) {
				// has to be done in a loop to prevent rare spin lock upon simultaneous container destructor and weak pointer release call
				bool removed = false;
				while (!removed) {
					this->Lock();
					if (Container) {
						removed = Container->TryRemoveWeakLink(*this);
					}
					this->Unlock();
				}
			} else {
				if (Container) {
					Container->RemoveWeakLink(*this);
				}
			}
		}

		Container = nullptr;
		Value = nullptr;
	}

	inline void AssignContainer(ContainerType* NewContainer, T* NewValue) {
		if (NewContainer == Container) {
			return;
		}

		Release();
		Container = NewContainer;
		Value = NewValue;

		if constexpr (AllowNull) {
			SF_ASSERT(NewContainer, "Cannot assign a nullptr to smart pointers with AllowNull set to false");
			if constexpr (StrongLink) {
				Container->Increment();
			} else {
				Container->AddWeakLink(*this);
			}
		} else {
			if (Container) {
				if constexpr (StrongLink) {
					Container->Increment();
				} else {
					Container->AddWeakLink(*this);
				}
			}
		}
	}

public:
	~SFObjectLink() {
		Release();
	}

	T* Get() const {
		return Value;
	}

	T* operator->() {
		return Value;
	}

	T& operator*() {
		return *Value;
	}

	const T* operator->() const {
		return Value;
	}

	const T& operator*() const {
		return Value;
	}

	ThisType& operator=(const SFObjectLink& other) {
		other.Lock();

		AssignContainer(other.Container, other.Value);

		other.Unlock();
		return *this;
	}

	template<typename TOp, bool ThreadsafeOp, bool StrongLinkOp, bool AllowNullOp>
	ThisType& operator=(const SFObjectLink<TOp, ThreadsafeOp, StrongLinkOp, AllowNullOp>& other) {
		other.Lock();

		AssignContainer(other.Container, other.Value);

		other.Unlock();
		return *this;
	}

	ThisType& operator=(SFObjectLink&& other) noexcept {
		if (Container == other.Container) {
			return *this;
		}

		Release();

		other.Lock();

		// don't need to reset Value pointer since it will not be used since container is reset
		Container = other.Container;
		Value = other.Value;
		other.Container = nullptr;

		if constexpr (!StrongLink) {
			if (Container) {
				Container->RemoveWeakLink(other);
				Container->AddWeakLink(*this);
			}
		}

		other.Unlock();

		return *this;
	}

	template<typename Ptr>
	inline std::enable_if_t<std::is_base_of_v<T, Ptr> || std::is_same_v<T, Ptr>, ThisType&> operator=(Ptr* value) {
		static_assert(StrongLink, "Cannot assign a raw pointer to a weak pointer directly");

		if constexpr (Sharable<Ptr, Threadsafe>) {
			// todo: this can be optimised to avoid creating a shared pointer needlessly
			if constexpr (AllowNull) {
				if (value) {
					auto SharedValue = value->AsShared();
					AssignContainer(SharedValue->Container, value);
				} else {
					Release();
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a smart pointer that doesn't allow null");
				auto SharedValue = value->AsShared();
				AssignContainer(SharedValue->Container, value);
			}
		} else {
			if constexpr (AllowNull) {
				if (value) {
					AssignContainer(new ContainerType(), value);
				} else {
					Release();
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a smart pointer that doesn't allow null");
				AssignContainer(new ContainerType(), value);
			}
		}
	
		return *this;
	}

	inline ThisType& operator=(std::nullptr_t) {
		SF_ASSERT(AllowNull, "Cannot assign nullptr to a shared pointer type that doesn't allow null");
		Release();
		return *this;
	}

	template<typename Ptr, bool OtherThreadsafe, bool OtherStrong, bool OtherNullable>
	inline bool operator==(SFObjectLink<Ptr, OtherThreadsafe, OtherStrong, OtherNullable> other) {
		return Container == other.Container;
	}

	inline bool operator==(T* value) const {
		return value == Value;
	}

	template<typename To>
	requires(std::derived_from<To, T> || std::derived_from<T, To>)
	inline SFObjectLink<To, Threadsafe, StrongLink, false> Cast() {
		this->Lock();

		To* Casted = dynamic_cast<To*>(Value);
		if (Casted) {
			SFObjectLink<To, Threadsafe, StrongLink, false> Ptr;
			Ptr.AssignContainer(Container);
			Ptr.Value = Casted;
			this->Unlock();
			return Ptr;
		} else {
			this->Unlock();
			return SFObjectLink<To, Threadsafe, StrongLink, false>();
		}
	}

	inline operator bool() {
		return Value != nullptr;
	}

	inline operator T* () {
		return Get();
	}

	inline int StrongCount() const {
		this->Lock();

		if (Container) {
			int Count = Container->StrongRefCount;
			this->Unlock();
			return Count;
		}

		this->Unlock();
		return -1;
	}

	inline int WeakCount() const {
		this->Lock();

		if (Container) {
			int Count = Container->WeakReferences.size();
			this->Unlock();
			return Count;
		}

		this->Unlock();

		return -1;
	}

	constexpr bool IsThreadsafe() { return Threadsafe; }
};

template<typename T, bool Threadsafe = false>
class SharedFromThis {
	friend SFObjectLink;

public:
	SharedFromThis() = default;
	virtual ~SharedFromThis() = default;

private:
	SFObjectContainer<Threadsafe>* Container = nullptr;

public:
	inline SFSharedPtr<T, Threadsafe> AsShared() {
		if (Container) {
			SFSharedPtr<T, Threadsafe> ptr;
			ptr.AssignContainer(Container, static_cast<T*>(this));
			return ptr;
		} else {
			SFSharedPtr<T, Threadsafe> ptr;
			ptr.AssignContainer(new SFObjectContainer<Threadsafe>(), static_cast<T*>(this));
			Container = ptr.Container;
			return ptr;
		}
	}
};