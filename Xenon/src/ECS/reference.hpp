#ifndef _XENON_ECS_REFERENCE_
#define _XENON_ECS_REFERENCE_

#include "component.hpp"

namespace Core {

template<typename T>
class Reference {
	static_assert(is_proper_component<T>());
public:
	explicit Reference(T *dep = nullptr) : m_ref(dep) {
		p_addReference();
	}

	~Reference() {
		p_removeReference();
	}

	Reference(const Reference& other) : m_ref(other.m_ref) {
		p_addReference();
	}

	Reference(Reference&& other) noexcept
	: m_ref(other.m_ref), m_next(other.m_next), m_prev(other.m_prev) {
		other.m_ref = nullptr;
		if(m_prev != nullptr)
			m_prev->m_next = this;
		if(m_next != nullptr)
			m_next->m_prev = this;
		else if(m_ref != nullptr)
			m_ref->m_listHead = this;
	}

	Reference &operator=(const Reference& other) {
		if(this == &other) return *this;
		set(other.m_ref);
		return *this;
	}

	Reference &operator=(Reference&& other) noexcept {
		if(this == &other) return *this;
		p_removeReference();
		m_ref = other.m_ref;
		m_next = other.m_next;
		m_prev = other.m_prev;
		other.m_ref = nullptr;
		if(m_prev != nullptr)
			m_prev->m_next = this;
		if(m_next != nullptr)
			m_next->m_prev = this;
		else if(m_ref != nullptr)
			m_ref->m_listHead = this;
		return *this;
	}

	[[nodiscard]] T& get() const {
		return *static_cast<T*>(m_ref);
	}

	[[nodiscard]] T* getPtr() const {
		return static_cast<T*>(m_ref);
	}

	void set(T* dep = nullptr) {
		p_removeReference();
		m_ref = dep;
		p_addReference();
	}

	void set(T& dep) {
		set(&dep);
	}

private:
	T* m_ref;
	Reference<T>* m_next = nullptr;
	Reference<T>* m_prev = nullptr;

	void p_addReference() {
		if(m_ref == nullptr) return;
		if(m_ref->m_listHead == nullptr) {
			m_ref->m_listHead = this;
		} else {
			m_prev = m_ref->m_listHead;
			m_ref->m_listHead->m_next = this;
			m_ref->m_listHead = this;
		}
    }

	void p_removeReference() {
		if(m_ref == nullptr) return;
		if(m_prev != nullptr)
			m_prev->m_next = m_next;
		if(m_next != nullptr)
			m_next->m_prev = m_prev;
		else
			m_ref->m_listHead = m_prev;
	}

	void p_atCompMove(T* pos) {
		m_ref = pos;
		if(m_prev != nullptr) m_prev->p_atCompMove(pos);
	}

	void p_atCompDelete() {
		if(m_prev != nullptr) m_prev->p_atCompDelete();
		m_ref = nullptr;
		m_next = nullptr;
		m_prev = nullptr;
	}

	friend class Component<T>;
};

}

#endif // !_XENON_ECS_REFERENCE_
