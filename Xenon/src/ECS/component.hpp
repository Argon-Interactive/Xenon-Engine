#ifndef _XENON_ECS_COMPONENT_
#define _XENON_ECS_COMPONENT_

#include <type_traits>

namespace Core {

template<typename T>
class Reference;

template<typename T>
class Component {
public:
	Component() = default;

	virtual ~Component() {
		p_atDelete();
	}

	Component([[maybe_unused]] const Component& other) {}

	Component(Component&& other) noexcept : m_listHead(other.m_listHead) {
		other.m_listHead = nullptr;
		p_atMove();
	}

	Component &operator=(const Component& other) {
		if(this == &other) return *this;
		p_atDelete();
		m_listHead = nullptr;
		return *this;
	}

	Component &operator=(Component&& other) noexcept {
		if(this == &other) return *this;
		p_atDelete();
		m_listHead = other.m_listHead;
		other.m_listHead = nullptr;
		p_atMove();
		return *this;
	}

private:
	Reference<T>* m_listHead = nullptr;

	void p_atDelete() {
		if(m_listHead == nullptr) return;
		m_listHead->p_atCompDelete();
	}

	void p_atMove() {
		if(m_listHead == nullptr) return;
		m_listHead->p_atCompMove(static_cast<T*>(this));
	}

	friend class Reference<T>;
};

template<typename T>
constexpr bool is_proper_component() {
	static_assert(std::is_base_of_v<Component<T>, T>, "T has to be declared with `COMPONENT(T)` macro ( equivalent to `struct T : Component<T>` )");
	static_assert(std::is_default_constructible_v<T>, "T has to be trivially default constructible");
	static_assert(std::is_copy_constructible_v<T>, "T has to be trivially copy constructible");
	static_assert(std::is_move_constructible_v<T>, "T has to be trivially move constructible");
	static_assert(std::is_copy_assignable_v<T>, "T has to be trivially copy assignable");
	static_assert(std::is_move_assignable_v<T>, "T has to be trivially move assignable");
    return true;
}

}

#define COMPONENT(NAME) struct NAME : public Component<NAME>

#endif // !_XENON_ECS_COMPONENT_
