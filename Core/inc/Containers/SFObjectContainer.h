#pragma once

#include <Core.h>
#include <vector>
#include <Asserts.h>
#include <iostream>
#include <Logging/Log.h>

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

template<typename T, bool Threadsafe = false>
struct SFObjectContainer {
	friend SFObjectLink;
	friend SharedFromThis;

private:
	static_assert(!Threadsafe, "Thread safe references not yet implemented");

	int StrongRefCount = 0;
	T* value;
	std::vector<SFWeakPtr<T, Threadsafe>*> WeakReferences;

	SFObjectContainer() : value(new T()) {}

	SFObjectContainer(T* object) : value(object) {
		SF_ASSERT(object, "SFObjectContainer should never be initialized with a nullptr");
	}

	~SFObjectContainer() {
		delete value;
	}

	inline void RemoveWeakLink(SFWeakPtr<T, Threadsafe>* WeakRef) {
		for (int i = 0; i < WeakReferences.size(); ++i) {
			if (WeakReferences[i] == WeakRef) {
				WeakReferences[i] = WeakReferences[WeakReferences.size() - 1];
				WeakReferences.pop_back();
				break;
			}
		}
	}
};

template<typename BaseType, bool Threadsafe> //, bool Threadsafe
concept Sharable = requires(BaseType Value) {
	{ Value.AsShared() } -> std::convertible_to<SFSharedPtr<BaseType, Threadsafe>>;
};

template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
struct SFObjectLink {
	static_assert(!Threadsafe, "Threadsafe links are currently unimplemented");
	static_assert(StrongLink || AllowNull, "StrongLink cannot be set to true and allownull to be false");
	using ContainerType = SFObjectContainer<T, Threadsafe>;
	using ThisType = SFObjectLink<T, Threadsafe, StrongLink, AllowNull>;

	template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
	friend class SFObjectLink;

	template<typename T, bool Threadsafe>
	friend class SharedFromThis;

private:
	ContainerType* Container = nullptr;

public:
	SFObjectLink() {
		SF_ASSERT(AllowNull, "Cannot use default constructor on smart pointers that do not allow null");
	}

	template<bool StrongLinkOther, bool AllowNullOther>
	SFObjectLink(const SFObjectLink<T, Threadsafe, StrongLinkOther, AllowNullOther>& other) {
		AssignContainer(other.Container);
	}

	SFObjectLink(const ThisType& other) {
		AssignContainer(other.Container);
	}

	SFObjectLink(SFObjectLink&& other) noexcept {
		Container = other.Container;
		other.Container = nullptr;

		if constexpr (!StrongLink) {
			if (Container) {
				Container->RemoveWeakLink(&other);
				Container->WeakReferences.push_back(this);
			}
		}
	}

	template<bool StrongLinkOther, bool AllowNullOther>
	SFObjectLink(SFObjectLink<T, Threadsafe, StrongLinkOther, AllowNullOther>&& other) {
		Container = other.Container;
		other.Container = nullptr;

		if constexpr ((!StrongLink) || (!StrongLinkOther)) {
			if (Container) {
				if constexpr (StrongLinkOther) {
					if (--Container->StrongRefCount == 0) {
						delete Container;
						Container = nullptr;
						return;
					}
				}

				if constexpr (StrongLink) {
					Container->StrongRefCount++;
				} else {
					Container->WeakReferences.push_back(this);
				}
			}
		}
	}

	SFObjectLink(T* value) {
		if constexpr (AllowNull) {
			if (value) {
				Container = new ContainerType(value);

				if constexpr (StrongLink) {
					Container->StrongRefCount++;
				} else {
					static_assert(StrongLink, "Cannot directly create a weak pointer from a raw pointer");
				}
			}
		} else {
			SF_ASSERT(value, "Cannot assign nullptr to a shared pointer type that doesn't allow null");
			Container = new ContainerType(value);
			Container->StrongRefCount++;
		}
	}

private:
	inline void Release() {
		if constexpr (StrongLink) {
			if (Container) {
				if (--Container->StrongRefCount == 0) {
					for (int i = 0; i < Container->WeakReferences.size(); ++i) {
						Container->WeakReferences[i]->Container = nullptr;
					}

					delete Container;
				}
			}
		} else {
			if (Container) {
				Container->RemoveWeakLink(this);
			}
		}
	}

	template<typename Ptr>
	inline std::enable_if_t<std::is_base_of_v<T, Ptr> || std::is_same_v<T, Ptr>, void> AssignContainer(SFObjectContainer<Ptr, Threadsafe>* NewContainer) {
		static_assert(sizeof(ContainerType) == sizeof(SFObjectContainer<Ptr, Threadsafe>), "The pointer reinterpret hack will fail if there is a change which affects the container structure based on type");

		ContainerType* _NewContainer = reinterpret_cast<ContainerType*>(NewContainer); // hack: container should always be the same format internally, only c++ semantics affect pointer type
		if (_NewContainer == Container) {
			return;
		}

		Release();
		Container = _NewContainer;

		if constexpr (AllowNull) {
			SF_ASSERT(NewContainer, "Cannot assign a nullptr to smart pointers with AllowNull set to false");
			if constexpr (StrongLink) {
				Container->StrongRefCount++;
			} else {
				Container->WeakReferences.push_back(this);
			}
		} else {
			if (Container) {
				if constexpr (StrongLink) {
					Container->StrongRefCount++;
				} else {
					Container->WeakReferences.push_back(this);
				}
			}
		}
	}

public:
	~SFObjectLink() {
		Release();
	}

	T* Get() const {
		if (Container) {
			return Container->value;
		}

		return nullptr;
	}

	T* operator->() {
		return Container->value;
	}

	T& operator*() {
		return *Container->value;
	}

	const T* operator->() const {
		return Container->value;
	}

	const T& operator*() const {
		return *Container->value;
	}

	ThisType& operator=(const SFObjectLink& other) {
		AssignContainer(other.Container);
		return *this;
	}

	template<typename TOp, bool ThreadsafeOp, bool StrongLinkOp, bool AllowNullOp>
	ThisType& operator=(const SFObjectLink<TOp, ThreadsafeOp, StrongLinkOp, AllowNullOp>& other) {
		AssignContainer(other.Container);
		return *this;
	}

	ThisType& operator=(SFObjectLink&& other) {
		if (Container == other.Container) {
			return *this;
		}

		Release();

		Container = other.Container;
		other.Container = nullptr;

		if constexpr (!StrongLink) {
			if (Container) {
				Container->RemoveWeakLink(*other);
				Container->WeakReferences.push_back(this);
			}
		}

		return *this;
	}

	template<typename Ptr>
	inline std::enable_if_t<std::is_base_of_v<T, Ptr>, ThisType&> operator=(Ptr* value) {
		static_assert(StrongLink, "Cannot assign a raw pointer to a weak pointer directly");

		if constexpr (Sharable<Ptr, Threadsafe>) {
			// todo: this can be optimised to avoid creating a shared pointer needlessly
			if constexpr (AllowNull) {
				if (value) {
					AssignContainer(value->AsShared()->Container);
				} else {
					Release();
					Container = nullptr;
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a smart pointer that doesn't allow null");
				AssignContainer(value->AsShared()->Container);
			}
		} else {
			if constexpr (AllowNull) {
				if (value) {
					AssignContainer(new ContainerType(value));
				} else {
					Release();
					Container = nullptr;
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a smart pointer that doesn't allow null");
				AssignContainer(new ContainerType(value));
			}
		}
	
		return *this;
	}

	template<typename Ptr, bool OtherThreadsafe, bool OtherStrong, bool OtherNullable>
	inline bool operator==(SFObjectLink<Ptr, OtherThreadsafe, OtherStrong, OtherNullable> other) {
		return Container == other.Container;
	}

	inline bool operator==(T* value) const {
		if (Container) {
			return Container->value == value;
		}

		return value == nullptr;
	}

	inline operator bool() {
		return Container != nullptr;
	}

	inline operator T* () {
		return Get();
	}
};

template<typename T, bool Threadsafe>
class SharedFromThis {
	friend SFObjectLink;

public:
	SharedFromThis() = default;
	~SharedFromThis() = default;

private:
	SFObjectContainer<T, Threadsafe>* Container = nullptr;

public:
	inline SFSharedPtr<T, Threadsafe> AsShared() {
		if (Container) {
			SFSharedPtr<T, Threadsafe> ptr;
			ptr.AssignContainer(Container);
			return ptr;
		} else {
			SFSharedPtr<T, Threadsafe> ptr;
			ptr.AssignContainer(new SFObjectContainer<T, Threadsafe>(static_cast<T*>(this)));
			Container = ptr.Container;
			return ptr;
		}
	}
};