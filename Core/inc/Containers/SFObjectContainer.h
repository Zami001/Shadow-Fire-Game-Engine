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

	static_assert(!Threadsafe, "Thread safe references not yet implemented");

	int StrongRefCount = 0;
	std::vector<ContainerRef> WeakReferences;

	SFObjectContainer() {}

	~SFObjectContainer() {
		for (ContainerRef Reference : WeakReferences) {
			*Reference.Value = nullptr;
			*Reference.Container = nullptr;
		}
	}

	inline void RemoveWeakLink(void** WeakRef) {
		for (int i = 0; i < WeakReferences.size(); ++i) {
			if (WeakReferences[i].Value == WeakRef) {
				WeakReferences[i] = WeakReferences[WeakReferences.size() - 1];
				WeakReferences.pop_back();
				break;
			}
		}
	}
};

template<typename BaseType, bool Threadsafe> //, bool Threadsafe
concept Sharable = requires(BaseType Value) {
	{ Value.AsShared() };
	
};

template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
struct SFObjectLink {
	static_assert(!Threadsafe, "Threadsafe links are currently unimplemented");
	static_assert(StrongLink || AllowNull, "Either StrongLink or AllowNull must be set true");
	using ContainerType = SFObjectContainer<Threadsafe>;
	using ThisType = SFObjectLink<T, Threadsafe, StrongLink, AllowNull>;

	template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
	friend class SFObjectLink;

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
		AssignContainer(other.Container);
		Value = other.Value;
	}

	SFObjectLink(const ThisType& other){
		AssignContainer(other.Container);
		Value = other.Value;
	}

	SFObjectLink(SFObjectLink&& other) noexcept {
		Container = other.Container;
		other.Container = nullptr;
		Value = other.Value;

		if constexpr (!StrongLink) {
			if (Container) {
				Container->RemoveWeakLink(reinterpret_cast<void**>(&Value));
				Container->WeakReferences.emplace_back(reinterpret_cast<void**>(&Value), reinterpret_cast<void**>(&Container));
			}
		}
	}

	template<bool StrongLinkOther, bool AllowNullOther>
	SFObjectLink(SFObjectLink<T, Threadsafe, StrongLinkOther, AllowNullOther>&& other) {
		Container = other.Container;
		other.Container = nullptr;
		Value = other.Value;

		if constexpr ((!StrongLink) || (!StrongLinkOther)) {
			if (Container) {
				if constexpr (StrongLink) {
					Container->StrongRefCount++;
				} else {
					Container->WeakReferences.emplace_back(reinterpret_cast<void**>(&Value), reinterpret_cast<void**>(&Container));
				}

				if constexpr (StrongLinkOther) {
					if (--Container->StrongRefCount == 0) {
						delete Container;
						Container = nullptr;
						Value = nullptr;
						return;
					}
				}
			}
		}
	}

	template<typename Ptr>
	requires(std::derived_from<Ptr, T>)
	SFObjectLink(Ptr* value) {
		if constexpr (Sharable<Ptr, Threadsafe>) {
			if constexpr (AllowNull) {
				if (value) {
					auto SharedValue = value->AsShared();
					AssignContainer(reinterpret_cast<ContainerType*>(SharedValue->Container));
					Value = value;
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a shared pointer type that doesn't allow null");
				auto SharedValue = value->AsShared();
				AssignContainer(reinterpret_cast<ContainerType*>(SharedValue->Container));
				Value = value;
			}
		} else if constexpr (AllowNull) {
			if (value) {
				Container = new ContainerType();
				Value = value;

				if constexpr (StrongLink) {
					Container->StrongRefCount++;
				} else {
					static_assert(StrongLink, "Cannot directly create a weak pointer from a raw pointer");
				}
			}
		} else {
			SF_ASSERT(value, "Cannot assign nullptr to a shared pointer type that doesn't allow null");
			Container = new ContainerType();
			Container->StrongRefCount++;
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
				if (--Container->StrongRefCount == 0) {
					delete Container;
					delete Value;
				}
			}
		} else {
			if (Container) {
				Container->RemoveWeakLink(reinterpret_cast<void**>(&Value));
			}
		}

		Container = nullptr;
		Value = nullptr;
	}

	inline void AssignContainer(ContainerType* NewContainer) {
		if (NewContainer == Container) {
			return;
		}

		Release();
		Container = NewContainer;

		if constexpr (AllowNull) {
			SF_ASSERT(NewContainer, "Cannot assign a nullptr to smart pointers with AllowNull set to false");
			if constexpr (StrongLink) {
				Container->StrongRefCount++;
			} else {
				Container->WeakReferences.emplace_back(reinterpret_cast<void**>(&Value), reinterpret_cast<void**>(&Container));
			}
		} else {
			if (Container) {
				if constexpr (StrongLink) {
					Container->StrongRefCount++;
				} else {
					Container->WeakReferences.emplace_back(reinterpret_cast<void**>(&Value), reinterpret_cast<void**>(&Container));
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
		AssignContainer(other.Container);
		Value = other.Value;
		return *this;
	}

	template<typename TOp, bool ThreadsafeOp, bool StrongLinkOp, bool AllowNullOp>
	ThisType& operator=(const SFObjectLink<TOp, ThreadsafeOp, StrongLinkOp, AllowNullOp>& other) {
		AssignContainer(other.Container);
		Value = other.Value;
		return *this;
	}

	ThisType& operator=(SFObjectLink&& other) noexcept {
		if (Container == other.Container) {
			return *this;
		}

		Release();

		Container = other.Container;
		other.Container = nullptr;

		if constexpr (!StrongLink) {
			if (Container) {
				Container->RemoveWeakLink(reinterpret_cast<void**>(&other.Container));
				Container->WeakReferences.emplace_back(reinterpret_cast<void**>(&Value), reinterpret_cast<void**>(&Container));
			}
		}

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
					AssignContainer(SharedValue->Container);
					Value = value;
				} else {
					Release();
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a smart pointer that doesn't allow null");
				auto SharedValue = value->AsShared();
				AssignContainer(Container);
				Value = value;
			}
		} else {
			if constexpr (AllowNull) {
				if (value) {
					AssignContainer(new ContainerType());
					Value = value;
				} else {
					Release();
				}
			} else {
				SF_ASSERT(value, "Cannot assign nullptr to a smart pointer that doesn't allow null");
				AssignContainer(new ContainerType());
				Value = value;
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
		To* Casted = dynamic_cast<To*>(Value);
		if (Casted) {
			SFObjectLink<To, Threadsafe, StrongLink, false> Ptr;
			Ptr.AssignContainer(Container);
			Ptr.Value = Casted;
			return Ptr;
		} else {
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
		if (Container) {
			return Container->StrongRefCount;
		}

		return -1;
	}

	inline int WeakCount() const {
		if (Container) {
			return Container->WeakReferences.size();
		}

		return -1;
	}
};

template<typename T, bool Threadsafe = false>
class SharedFromThis {
	friend SFObjectLink;

public:
	SharedFromThis() = default;
	~SharedFromThis() = default;

private:
	SFObjectContainer<Threadsafe>* Container = nullptr;

public:
	inline SFSharedPtr<T, Threadsafe> AsShared() {
		if (Container) {
			SFSharedPtr<T, Threadsafe> ptr;
			ptr.AssignContainer(Container);
			ptr.Value = static_cast<T*>(this);
			return ptr;
		} else {
			SFSharedPtr<T, Threadsafe> ptr;
			ptr.AssignContainer(new SFObjectContainer<Threadsafe>());
			Container = ptr.Container;
			ptr.Value = static_cast<T*>(this);
			return ptr;
		}
	}
};