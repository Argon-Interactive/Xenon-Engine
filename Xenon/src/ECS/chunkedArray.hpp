#ifndef _XENON_SRC_ECS_CHUNKEDARRAY_
#define _XENON_SRC_ECS_CHUNKEDARRAY_

#include <memory_resource>
#include <cmath>
#include <vector>

#include "devTools/logger_core.hpp"

namespace Core {
template<typename T>
class ChunkedArray {
public:
	class iterator;
	class const_iterator;

	explicit ChunkedArray(std::pmr::memory_resource* memoryRsrc = std::pmr::get_default_resource()) : m_resource(memoryRsrc), m_allocationSize(4096) { 
		//To change the default chunk size change the m_allocationSize value above, it is the only place where it needs to be changed
		uint32_t maxSize = m_allocationSize / sizeof(T);
		while(maxSize == 0) {
			m_allocationSize *= 4;
			maxSize = m_allocationSize / sizeof(T);
		}
		m_maxPartitionSize = maxSize;
		auto* newData = static_cast<T*>(m_resource->allocate(m_allocationSize));
		m_dataPtrs.push_back(newData);
	}
	~ChunkedArray() { for(auto ptr : m_dataPtrs) { m_resource->deallocate(ptr, m_allocationSize); } }
	ChunkedArray(ChunkedArray &&) = delete;
	ChunkedArray(const ChunkedArray &) = delete;
	ChunkedArray &operator=(ChunkedArray &&) = delete;
	ChunkedArray &operator=(const ChunkedArray &) = delete;

///////////////////?DEB?//////////////////////////////////
	void loginx() {XN_LOG_DEB("Major: {0}, Minor: {0}", m_indexMajor, m_indexMinor); }


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
	void emplace_back(Args&&... args) {
		new(&m_dataPtrs[m_indexMajor][m_indexMinor]) T(std::forward<Args>(args)...);
		if(++m_indexMinor == m_maxPartitionSize) resize();
	}
	void push_back(const T& value) { emplace_back(value); }
	void push_back(T&& value) { emplace_back(std::move(value)); }
	void pop_back() {
		if(m_indexMajor == 0) {
			m_indexMajor--;
			m_indexMinor = m_maxPartitionSize - 1;
			m_resource->deallocate(m_dataPtrs.back(), m_allocationSize);
			m_dataPtrs.pop_back();
		}
		else { m_indexMinor--; }
	}
	void clear() { 
		for(auto ptr : m_dataPtrs) { m_resource->deallocate(ptr, m_allocationSize); }
		m_dataPtrs.clear();
		m_indexMinor = 0;
		m_indexMajor = 0;
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
	T& back() { return m_dataPtrs[m_indexMajor][--m_indexMinor]; }
private:
	std::pmr::memory_resource* m_resource;
	uint16_t m_allocationSize; //NOLINT
	uint16_t m_maxPartitionSize{};
	size_t m_indexMajor{};
	uint16_t m_indexMinor{};
	std::pmr::vector<T*> m_dataPtrs{m_resource};

	void resize() {
		auto* newData = static_cast<T*>(m_resource->allocate(m_allocationSize));
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
#endif
