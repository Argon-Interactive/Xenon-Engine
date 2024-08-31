#ifndef _XENON_ECS_COMPONENTLIST_
#define _XENON_ECS_COMPONENTLIST_

#include "chunkedArray.hpp"
#include "devTools/logger_core.hpp"

namespace Core {
template<typename T>
class ComponentReferenceList {
	struct Node;
public:
	class iterator;
	class const_iterator;

	explicit ComponentReferenceList(std::pmr::memory_resource* memRsrc = std::pmr::get_default_resource())
	: m_resource(memRsrc), m_head(nullptr), m_tail(nullptr) {}

	~ComponentReferenceList() {
		for(Node* node = m_head; node != nullptr; ) {
			Node* temp = node;
			node = node->next;
			m_resource->deallocate(temp, sizeof(Node));
		} }

	ComponentReferenceList(ComponentReferenceList &&) = delete;
	ComponentReferenceList(const ComponentReferenceList&) = delete;
	ComponentReferenceList &operator=(ComponentReferenceList &&) = delete;
	ComponentReferenceList &operator=(const ComponentReferenceList &) = delete;

	[[nodiscard]] uint32_t size() { return m_size; }
	[[nodiscard]] bool contains(ChunkedArray<T>* elem) const {
		for(Node* node = m_head; node != nullptr; node = node->next) { if(node->data == elem) return true; }
		return false;
	}	
	[[nodiscard]] iterator begin() { return iterator(m_head); }
	[[nodiscard]] iterator end() { return iterator(this); }
	[[nodiscard]] const_iterator begin() const { return const_iterator(m_head); }
	[[nodiscard]] const_iterator end() const { return const_iterator(this); }
private:
	std::pmr::memory_resource* m_resource;
	Node* m_head;
	Node* m_tail;
	//PERF: size probably won't be used in the dist but might be useful for debugging 
	uint32_t m_size{};
	void push(ChunkedArray<T>* elem) {
		Node* newNode = static_cast<Node*>(m_resource->allocate(sizeof(Node)));
		new(newNode) Node(elem);
		if(m_head == nullptr) {
			m_head = newNode;
			m_head->prev = nullptr;
		}
		else {
			m_tail->next = newNode;
			newNode->prev = m_tail;
		}
		m_tail = newNode;
		m_tail->next = nullptr;
		m_size++;
	}
	void pop(ChunkedArray<T>* elem) {
		Node* temp = nullptr;
		if(m_head->data == elem && m_head == m_tail) {
			m_resource->deallocate(m_head, sizeof(Node));
			m_head = m_tail = nullptr;
		}
		else if(m_head->data == elem) {
			temp = m_head;
			m_head = m_head->next;
			m_head->prev = nullptr;
			m_resource->deallocate(temp, sizeof(Node));
			m_size--;
		}
		else if(m_tail->data == elem) {
			temp = m_tail;
			m_tail = m_tail->prev;
			m_tail->next = nullptr;
			m_resource->deallocate(temp, sizeof(Node));
			m_size--;
		}
		else if(m_head->next) {
			for(temp = m_head->next; temp != m_tail; temp = temp->next) {
				if(temp->data == elem) {
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
					m_resource->deallocate(temp, sizeof(Node));
					m_size--;
					return;
				}
			}
		}
		else XN_LOG_ERR("Trying to delete an element form ComponentReferenceList which it doesn't contain");
	}

	friend class ComponentManager;
	friend struct ComponentCluster;
};

template<typename T>
struct ComponentReferenceList<T>::Node {
	explicit Node(ChunkedArray<T>* elem) : data(elem) {}
	ChunkedArray<T>* data;
	Node* next;
	Node* prev;
};

template<typename T>
class ComponentReferenceList<T>::iterator {
	friend ComponentReferenceList<T>;
	explicit iterator(ComponentReferenceList<T>* CRL) : m_ptr(CRL->m_tail), m_caIt(CRL->m_tail->data->end()) {}
public:
	explicit iterator(Node* ptr) : m_ptr(ptr), m_caIt(ptr->data->begin()) {}
	iterator operator++() {
		m_caIt++;
		if(m_caIt == m_ptr->data->end()) {
			m_ptr = m_ptr->next;
			if(m_ptr) m_caIt = m_ptr->data->begin();
		}
		return *this;
	}
	iterator operator--() {
		m_caIt--;
		if(m_caIt == m_ptr->data->begin()) {
			m_ptr = m_ptr->prev;
			if(m_ptr) m_caIt = m_ptr->data->end();
			return *this;
		}
		return *this;
	}
	iterator operator++(int) {
		iterator it = *this;
		++(*this);
		return it;
	}
	iterator operator--(int) {
		iterator it = *this;
		--(*this);
		return it;
	}
	
	bool operator==(const ComponentReferenceList<T>::iterator& other) { return m_caIt == other.m_caIt; }
	bool operator!=(const ComponentReferenceList<T>::iterator& other) { return m_caIt != other.m_caIt; }

	T* operator->() { return m_caIt; }
	T& operator*() { return *m_caIt; }
private:
	Node* m_ptr;
	ChunkedArray<T>::iterator m_caIt;
};

template<typename T>
class ComponentReferenceList<T>::const_iterator {
	friend ComponentReferenceList<T>;
	explicit const_iterator(ComponentReferenceList<T>* CRL) : m_ptr(CRL->m_tail), m_caIt(CRL->m_tail->data->end()) {}
public:
	explicit const_iterator(Node* ptr) : m_ptr(ptr), m_caIt(ptr->data->begin()) {}
	const_iterator operator++() {
		m_caIt++;
		if(m_caIt == m_ptr->data->end()) {
			m_ptr->next;
			if(m_ptr) m_caIt = m_ptr->data->begin();
		}
		return *this;
	}
	const_iterator operator--() {
		m_caIt--;
		if(m_caIt == m_ptr->data->begin()) {
			m_ptr->prev;
			if(m_ptr) m_caIt = m_ptr->data->end();
		}
		return *this;
	}
	const_iterator operator++(int) {
		const_iterator it = *this;
		++(*this);
		return it;
	}
	const_iterator operator--(int) {
		const_iterator it = *this;
		--(*this);
		return it;
	}
	
	bool operator==(const ComponentReferenceList<T>::const_iterator& other) { return m_caIt == other.m_caIt; }
	bool operator!=(const ComponentReferenceList<T>::const_iterator& other) { return m_caIt != other.m_caIt; }

	T* operator->() const { return m_caIt; }
	T& operator*() const { return *m_caIt; }
private:
	Node* m_ptr;
	ChunkedArray<T>::const_iterator m_caIt;
};
}
#endif // !_XENON_ECS_COMPONENTLIST_
