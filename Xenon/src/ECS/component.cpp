#include "component.hpp"
#include "reference.hpp"

namespace Core {

Component::Component() = default;

Component::~Component() {
	p_atMove();
}

Component::Component(const Component& other) = default;

Component::Component(Component&& other) noexcept : m_listHead(other.m_listHead) {
	p_atMove();
}

Component& Component::operator=(const Component& other) {
	if(this == &other) return *this;
	p_atDelete();
	m_listHead = nullptr;
	return *this;
}

Component& Component::operator=(Component&& other) noexcept{
	if(this == &other) return *this;
	p_atDelete();
	m_listHead = other.m_listHead;
	p_atMove();
	return *this;
}

void Component::p_atDelete() {
	if(m_listHead == nullptr) return;
	m_listHead->p_atCompDelete();
}

void Component::p_atMove() {
	if(m_listHead == nullptr) return;
	m_listHead->p_atCompMove(this);
}

}
