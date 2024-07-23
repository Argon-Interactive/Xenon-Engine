#ifndef _XENON_SRC_ECS_CHUNKEDARRAY_
#define _XENON_SRC_ECS_CHUNKEDARRAY_

#include <memory_resource>
#include <cmath>
#include <vector>

namespace Core {
template<typename T>
class ChunkedArray {
public:
	class MMindex;
	class iterator;
	class const_iterator;

	explicit ChunkedArray(std::pmr::memory_resource* memoryRsrc) : m_resource(memoryRsrc), m_allocationSize(4096) { 
		//To change the default chunk size change the m_allocationSize value above, it is the only place where it needs to be changed
		uint32_t maxSize = m_allocationSize / sizeof(T);
		while(maxSize == 0) {
			m_allocationSize *= 4;
			maxSize = m_allocationSize / sizeof(T);
		}
		m_maxPartitionSize = maxSize;
		m_dataPtrs.reserve(8);
		m_dataPtrs.push_back(m_resource->allocate(m_allocationSize));
	}
	~ChunkedArray() { for(auto ptr : m_dataPtrs) { m_resource->deallocate(ptr, m_allocationSize); } }
	ChunkedArray(const ChunkedArray &) = delete;
	ChunkedArray(ChunkedArray &&) = delete;
	ChunkedArray &operator=(const ChunkedArray &) = delete;
	ChunkedArray &operator=(ChunkedArray &&) = delete;
	///////////////////////////////////////
	/// Iterators
	///////////////////////////////////////

	iterator begin() { return iterator(this, m_dataPtrs.front()); }
	iterator end() { return iterator(this, m_dataPtrs.back() + m_indexMajor); }
	const_iterator begin() const { return const_iterator(this, m_dataPtrs.front()); }
	const_iterator end() const { return const_iterator(this, m_dataPtrs.back() + m_indexMajor); }
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
	T& operator[](const MMindex& index) { m_dataPtrs[index.m_inxMajor][index.m_inxMinor]; }
	T& front() { return m_dataPtrs.front()[0]; }
	T& back() { return m_dataPtrs[m_indexMajor][--m_indexMinor]; }
	///////////////////////////////////////
	/// MMindex
	///////////////////////////////////////
	
	MMindex getMMindex(size_t inx) { 
		size_t inxMajor = inx / m_maxPartitionSize;
		return { inxMajor, inx - inxMajor * m_maxPartitionSize }; 
	}
	MMindex getMMindexBack() { return { m_indexMajor, --m_indexMinor }; }
	size_t getIndex(const MMindex& mminx) { return mminx.m_inxMajor * m_maxPartitionSize + mminx.m_inxMinor; }
private:
	std::pmr::memory_resource* m_resource;
	uint16_t m_allocationSize; //NOLINT
	uint16_t m_maxPartitionSize{};
	size_t m_indexMajor{};
	uint16_t m_indexMinor{};
	std::pmr::vector<T*> m_dataPtrs{m_resource};

	void resize() {
		m_dataPtrs.push_back(m_resource->allocate(m_allocationSize));
		if(m_dataPtrs.capacity == m_dataPtrs.size()) m_dataPtrs.reserve(8);
		m_indexMajor++;
		m_indexMinor = 0;
	}
};

template<typename T>
class ChunkedArray<T>::MMindex {
	MMindex(size_t major, uint16_t minor) : m_inxMajor(major), m_inxMinor(minor) {}
	size_t m_inxMajor;
	uint16_t m_inxMinor;
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

		T& operator[](size_t inx) const { return *(this + inx); }
		T* operator->() const { return m_itPtr; }
		T& operator*() const { return *m_itPtr; }
	private:
		T* m_itPtr;
		size_t m_itInxMajor = 0;
		const ChunkedArray<T>* m_itCArr;
};
}
#endif
