#ifndef _XENON_ECS_CHUNKEDARRAY_
#define _XENON_ECS_CHUNKEDARRAY_

#include "devTools/logger_core.hpp"

#include <memory_resource>
#include <cmath>
#include <vector>

namespace Core {
template<typename T>
class ChunkedArray {
	static constexpr uint16_t ALLOCATION_SIZE = 4096;
public:
	class iterator;
	class const_iterator;

	explicit ChunkedArray(std::pmr::memory_resource* memoryRsrc = std::pmr::get_default_resource()) : m_resource(memoryRsrc) { 
		uint16_t maxSize = ALLOCATION_SIZE / sizeof(T);
		if(maxSize == 0) { XN_LOG_ERR("Single element size exceeds chunk size of 4kB. Creation of chunked array failed"); return; }
		m_maxPartitionSize = maxSize;
		auto* newData = static_cast<T*>(m_resource->allocate(ALLOCATION_SIZE));
		m_dataPtrs.push_back(newData);
	}
	~ChunkedArray() { for(auto ptr : m_dataPtrs) { m_resource->deallocate(ptr, ALLOCATION_SIZE); } }
	ChunkedArray(ChunkedArray &&) = delete;
	ChunkedArray(const ChunkedArray &) = delete;
	ChunkedArray &operator=(ChunkedArray &&) = delete;
	ChunkedArray &operator=(const ChunkedArray &) = delete;

	///////////////////////////////////////
	/// Iterators
	///////////////////////////////////////

	iterator begin() { return iterator(this, m_dataPtrs.front()); }
	iterator end() { return iterator(this, m_dataPtrs.back() + m_indexMinor); }
	const_iterator begin() const { return const_iterator(this, m_dataPtrs.front()); }
	const_iterator end() const { return const_iterator(this, m_dataPtrs.back() + m_indexMinor); }
	///////////////////////////////////////
	/// Modifiers 
	///////////////////////////////////////
	
	template<typename ...Args>
	T* emplace_back(Args&&... args) {
		T* ptr = &m_dataPtrs[m_indexMajor][m_indexMinor];
		new(ptr) T(std::forward<Args>(args)...);
		if(++m_indexMinor == m_maxPartitionSize) resize();
		return ptr;
	}
	void push_back(const T& value) { emplace_back(value); }
	void push_back(T&& value) { emplace_back(std::move(value)); }
	void pop_back() {
		back().~T();
		if(m_indexMinor == 0) {
			m_indexMajor--;
			m_indexMinor = m_maxPartitionSize - 1;
			m_resource->deallocate(m_dataPtrs.back(), ALLOCATION_SIZE);
			m_dataPtrs.pop_back();
		} else {
			m_indexMinor--; 
		}
	}

	// TODO: call stored objects destructor at clear()
	void clear() { 
		for(auto ptr : m_dataPtrs) { m_resource->deallocate(ptr, ALLOCATION_SIZE); }
		m_dataPtrs.clear();
		m_indexMinor = 0;
		m_indexMajor = 0;
	}

	// FIX: I don't know it these two work. They look like they don't.
	void push_chunk(T* ptr) {
		T* last = m_dataPtrs.back();
		m_dataPtrs.pop_back();
		m_dataPtrs.push_back(ptr);
		m_dataPtrs.push_back(last);
		m_indexMajor++;
	}
	void push_chunk(const std::vector<T*> ptrs) {
		T* last = m_dataPtrs.back();
		m_dataPtrs.pop_back();
		m_dataPtrs.append_range(ptrs);
		m_dataPtrs.push_back(last);
		m_indexMajor += ptrs.size();
	}

	///////////////////////////////////////
	/// Capacity 
	///////////////////////////////////////
	
	bool empty() { return m_indexMajor == 0 && m_indexMinor == 0; }
	size_t size() { return m_indexMajor * m_maxPartitionSize + static_cast<size_t>(m_indexMinor); }
	///////////////////////////////////////
	/// Access
	///////////////////////////////////////
	
	T& operator[](size_t index) {
		size_t inxMajor = index / m_maxPartitionSize;
		return m_dataPtrs[inxMajor][index - inxMajor * m_maxPartitionSize]; 
	}
	T& front() { return m_dataPtrs.front()[0]; }
	T& back() { return m_dataPtrs[m_indexMajor][m_indexMinor - 1]; }
private:
	uint16_t m_maxPartitionSize{};
	uint16_t m_indexMinor{};
	std::pmr::memory_resource* m_resource;
	size_t m_indexMajor{};
	std::vector<T*> m_dataPtrs;

	void resize() {
		auto* newData = static_cast<T*>(m_resource->allocate(ALLOCATION_SIZE));
		m_dataPtrs.push_back(newData);
		m_indexMajor++;
		m_indexMinor = 0;
	}
};

template<typename T>
class ChunkedArray<T>::iterator {
	public:
		explicit iterator(ChunkedArray<T>* CArr, T* ptr) : m_itPtr(ptr), m_itCArr(CArr) {}
		iterator operator++() {
			m_itPtr++;
			if(m_itPtr == m_itCArr->m_dataPtrs[m_itInxMajor] + m_itCArr->m_maxPartitionSize) m_itPtr = m_itCArr->m_dataPtrs[++m_itInxMajor];
			return *this;
		}
		iterator operator--() {
			m_itPtr--;
			if(m_itPtr == m_itCArr->m_dataPtrs[m_itInxMajor]) m_itPtr = m_itCArr->m_dataPtrs[--m_itInxMajor];
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
		iterator operator+(size_t val) {
			auto currInx = m_itPtr - m_itCArr->m_dataPtrs[m_itInxMajor];
			currInx += val;
			m_itInxMajor += currInx / m_itCArr->m_maxPartitionSize;
			m_itPtr = m_itCArr->m_dataPtrs[m_itInxMajor] + currInx % m_itCArr->m_maxPartitionSize;
			return *this;
		}
		iterator operator-(size_t val) {
			auto currInx = m_itPtr - m_itCArr->m_dataPtrs[m_itInxMajor];
			currInx -= val;
			auto divVal = currInx / m_itCArr->m_maxPartitionSize;
			m_itInxMajor += divVal;
			m_itPtr = (divVal != 0) ?
			(m_itCArr->m_dataPtrs[m_itInxMajor] + m_itCArr->m_maxPartitionSize - std::abs(currInx) % m_itCArr->m_maxPartitionSize) 
			: (m_itPtr - val);
			return *this;
		}
		
		bool operator==(const ChunkedArray<T>::iterator& other) { return m_itPtr == other.m_itPtr; }
		bool operator!=(const ChunkedArray<T>::iterator& other) { return m_itPtr != other.m_itPtr; }
		bool operator>(const ChunkedArray<T>::iterator& other) {
			return (m_itPtr > other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<(const ChunkedArray<T>::iterator& other) {
			return (m_itPtr < other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}
		bool operator>=(const ChunkedArray<T>::iterator& other) {
			return (m_itPtr >= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<=(const ChunkedArray<T>::iterator& other) {
			return (m_itPtr <= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}

		// T& operator[](size_t inx) { return *(this + inx); }
		T* operator->() { return m_itPtr; }
		T& operator*() { return *m_itPtr; }
	private:
		T* m_itPtr;
		size_t m_itInxMajor = 0;
		const ChunkedArray<T>* m_itCArr;
};

template<typename T>
class ChunkedArray<T>::const_iterator {
	public:
		explicit const_iterator(ChunkedArray<T>* CArr, T* ptr) : m_itPtr(ptr), m_itCArr(CArr) {}
		const_iterator operator++() {
			m_itPtr++;
			if(m_itPtr == m_itCArr->m_dataPtrs[m_itInxMajor] + m_itCArr->m_maxPartitionSize) m_itPtr = m_itCArr->m_dataPtrs[++m_itInxMajor];
			return *this;
		}
		const_iterator operator--() {
			m_itPtr--;
			if(m_itPtr == m_itCArr->m_dataPtrs[m_itInxMajor]) m_itPtr = m_itCArr->m_dataPtrs[--m_itInxMajor];
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
		const_iterator operator+(size_t val) {
			auto currInx = m_itPtr - m_itCArr->m_dataPtrs[m_itInxMajor];
			currInx += val;
			m_itInxMajor += currInx / m_itCArr->m_maxPartitionSize;
			m_itPtr = m_itCArr->m_dataPtrs[m_itInxMajor] + currInx % m_itCArr->m_maxPartitionSize;
			return *this;
		}
		const_iterator operator-(size_t val) {
			auto currInx = m_itPtr - m_itCArr->m_dataPtrs[m_itInxMajor];
			currInx -= val;
			auto divVal = currInx / m_itCArr->m_maxPartitionSize;
			m_itInxMajor += divVal;
			m_itPtr = (divVal != 0) ?
			(m_itCArr->m_dataPtrs[m_itInxMajor] + m_itCArr->m_maxPartitionSize - std::abs(currInx) % m_itCArr->m_maxPartitionSize) 
			: (m_itPtr - val);
			return *this;
		}
		
		bool operator==(const ChunkedArray<T>::const_iterator& other) { return m_itPtr == other.m_itPtr; }
		bool operator!=(const ChunkedArray<T>::const_iterator& other) { return m_itPtr != other.m_itPtr; }
		bool operator>(const ChunkedArray<T>::const_iterator& other) {
			return (m_itPtr > other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<(const ChunkedArray<T>::const_iterator& other) {
			return (m_itPtr < other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}
		bool operator>=(const ChunkedArray<T>::const_iterator& other) {
			return (m_itPtr >= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<=(const ChunkedArray<T>::const_iterator& other) {
			return (m_itPtr <= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}

		//T& operator[](size_t inx) const { return *(this + inx); }
		T* operator->() const { return m_itPtr; }
		T& operator*() const { return *m_itPtr; }
	private:
		T* m_itPtr;
		size_t m_itInxMajor = 0;
		const ChunkedArray<T>* m_itCArr;
};
}
#endif // !_XENON_ECS_CHUNKEDARRAY_
