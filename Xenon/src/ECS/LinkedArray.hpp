#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace Core {
template<typename T>
class LinekdArray {
public:
	class iterator;
	class const_iterator;
	class index;
	LinekdArray() : m_maxsize(8388608/sizeof(T)) {
		if(m_maxsize == 0 || m_maxsize == 1) {
			m_maxsize = 2;
			m_startingSize = 2;
		}
		else for(size_t i = 1; m_startingSize == 0; i*=4)
			m_startingSize = 512 * i / sizeof(T) / 2;
		makeNewArray();
	}
    ~LinekdArray() { for (auto ptr : m_dataArrays) delete[] ptr; } //NOLINT
    //TODO deleted for now. should be implemented at some point;
	LinekdArray(const LinekdArray&) = delete;
	LinekdArray& operator=(const LinekdArray&) = delete;
	LinekdArray(LinekdArray&&) noexcept = delete;
	LinekdArray& operator=(LinekdArray&&) noexcept = delete;

	void pushBack(const T& newElement) {
		m_dataArrays[m_currInxMajor][m_currInxMinor++] = newElement;
		if(m_currInxMinor >= m_reserved) resize();
	}
	void popBack() {
		if(m_currInxMinor != 0) m_currInxMinor--;
		else { m_currInxMajor--; m_currInxMinor = m_maxsize; }
	}
	[[nodiscard]] T& operator[](size_t inx) { return m_dataArrays[inx / m_maxsize][inx % m_maxsize]; }
	[[nodiscard]] T& at(size_t inx) { return this[inx]; }
	[[nodiscard]] T& at(const index& inx) { return m_dataArrays[inx.m_inxMajor][inx.m_inxMinor]; }
	[[nodiscard]] index getMMIndex(size_t inx) { return index(this, inx); }
	iterator begin() { return iterator(this, m_dataArrays[0]); }
	iterator end() { return iterator(this, m_dataArrays.back() + m_currInxMinor); }
	const_iterator begin() const { return const_iterator(this, m_dataArrays[0]); }
	const_iterator end() const { return const_iterator(this, m_dataArrays.back() + m_currInxMinor); }
	T& front() { return m_dataArrays[0][0]; }
	T& back() { return m_dataArrays[m_currInxMajor][m_currInxMinor - 1]; }
	size_t size() { return m_currInxMajor * m_maxsize + m_currInxMinor; }
	bool empty() { return m_currInxMinor == 0 && m_currInxMajor == 0; }
	void clear() {
		for(auto ptr : m_dataArrays) 
			delete[] ptr; //NOLINT
		m_dataArrays.clear();
		m_currInxMajor = 0;
		m_currInxMinor = 0;
	}
private:
	size_t m_currInxMinor = 0;
	size_t m_currInxMajor = -1UL;
	size_t m_maxsize = 0;
	size_t m_startingSize = 0;
	size_t m_reserved = 0;
	std::vector<T*> m_dataArrays;
	void makeNewArray() {
		m_currInxMinor = 0;
		m_currInxMajor++;
		m_reserved = m_startingSize;
		T* newArray = new T[m_reserved]; //NOLINT
		m_dataArrays.push_back(newArray);
	}
	void resize() {
		size_t oldsize = m_reserved;
		if(m_reserved * 4 <= m_maxsize) m_reserved *= 4;
		else if (m_reserved < m_maxsize) m_reserved = m_maxsize;
		else { makeNewArray(); return; }
		T* newArray = new T[m_reserved]; //NOLINT
		std::memmove(newArray, m_dataArrays.back(), oldsize);
		delete[] m_dataArrays.back();
		m_dataArrays[m_dataArrays.size() - 1] = newArray;
	}
};

template<typename T>
class LinekdArray<T>::index {
	explicit index(const LinekdArray<T>* LA, size_t inx) : m_inxMajor(inx/LA->m_maxsize), m_inxMinor(inx%LA->m_maxsize) {}
	uint32_t m_inxMajor;
	uint32_t m_inxMinor;
	friend class LinekdArray;
};

template<typename T>
class LinekdArray<T>::iterator {
	public:
		explicit iterator(LinekdArray<T>* LA, T* ptr) : m_itPtr(ptr), m_itLA(LA) {}
		iterator operator++() {
			m_itPtr++;
			if(m_itPtr == m_itLA->m_dataArrays[m_itInxMajor] + m_itLA->m_maxsize) m_itPtr = m_itLA->m_dataArrays[++m_itInxMajor];
			return *this;
		}
		iterator operator--() {
			m_itPtr--;
			if(m_itPtr == m_itLA->m_dataArrays[m_itInxMajor]) m_itPtr = m_itLA->m_dataArrays[--m_itInxMajor];
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
			auto currInx = m_itPtr - m_itLA->m_dataArrays[m_itInxMajor];
			currInx += val;
			m_itInxMajor += currInx / m_itLA->m_maxsize;
			m_itPtr = m_itLA->m_dataArrays[m_itInxMajor] + currInx % m_itLA->m_maxsize;
			return *this;
		}
		iterator operator-(size_t val) {
			auto currInx = m_itPtr - m_itLA->m_dataArrays[m_itInxMajor];
			currInx -= val;
			auto divVal = currInx / m_itLA->m_maxsize;
			m_itInxMajor += divVal;
			m_itPtr = (divVal != 0) ? (m_itLA->m_dataArrays[m_itInxMajor] + m_itLA->m_maxsize - std::abs(currInx) % m_itLA->m_maxsize) : (m_itPtr - val);
			return *this;
		}
		
		bool operator==(const LinekdArray<T>::iterator& other) {
			return m_itPtr == other.m_itPtr;
		}
		bool operator!=(const LinekdArray<T>::iterator& other) {
			return m_itPtr != other.m_itPtr;
		}
		bool operator>(const LinekdArray<T>::iterator& other) {
			return (m_itPtr > other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<(const LinekdArray<T>::iterator& other) {
			return (m_itPtr < other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}
		bool operator>=(const LinekdArray<T>::iterator& other) {
			return (m_itPtr >= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<=(const LinekdArray<T>::iterator& other) {
			return (m_itPtr <= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}

		T& operator[](size_t inx) { return *(this + inx); }
		T* operator->() { return m_itPtr; }
		T& operator*() { return *m_itPtr; }

	private:
		T* m_itPtr;
		size_t m_itInxMajor = 0;
		const LinekdArray<T>* m_itLA;

};
template<typename T>
class LinekdArray<T>::const_iterator {
	public:
		explicit const_iterator(LinekdArray<T>* LA, T* ptr) : m_itPtr(ptr), m_itLA(LA) {}
		const_iterator operator++() {
			m_itPtr++;
			if(m_itPtr == m_itLA->m_dataArrays[m_itInxMajor] + m_itLA->m_maxsize) m_itPtr = m_itLA->m_dataArrays[++m_itInxMajor];
			return *this;
		}
		const_iterator operator--() {
			m_itPtr--;
			if(m_itPtr == m_itLA->m_dataArrays[m_itInxMajor]) m_itPtr = m_itLA->m_dataArrays[--m_itInxMajor];
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
			auto currInx = m_itPtr - m_itLA->m_dataArrays[m_itInxMajor];
			currInx += val;
			m_itInxMajor += currInx / m_itLA->m_maxsize;
			m_itPtr = m_itLA->m_dataArrays[m_itInxMajor] + currInx % m_itLA->m_maxsize;
			return *this;
		}
		const_iterator operator-(size_t val) {
			auto currInx = m_itPtr - m_itLA->m_dataArrays[m_itInxMajor];
			currInx -= val;
			auto divVal = currInx / m_itLA->m_maxsize;
			m_itInxMajor += divVal;
			m_itPtr = (divVal != 0) ? (m_itLA->m_dataArrays[m_itInxMajor] + m_itLA->m_maxsize - std::abs(currInx) % m_itLA->m_maxsize) : (m_itPtr - val);
			return *this;
		}
		
		bool operator==(const LinekdArray<T>::const_iterator& other) {
			return m_itPtr == other.m_itPtr;
		}
		bool operator!=(const LinekdArray<T>::const_iterator& other) {
			return m_itPtr != other.m_itPtr;
		}
		bool operator>(const LinekdArray<T>::const_iterator& other) {
			return (m_itPtr > other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<(const LinekdArray<T>::const_iterator& other) {
			return (m_itPtr < other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}
		bool operator>=(const LinekdArray<T>::const_iterator& other) {
			return (m_itPtr >= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor > other.m_itInxMajor;
		}
		bool operator<=(const LinekdArray<T>::const_iterator& other) {
			return (m_itPtr <= other.m_itPtr && m_itInxMajor == other.m_itInxMajor) || m_itInxMajor < other.m_itInxMajor;
		}

		T& operator[](size_t inx) const { return *(this + inx); }
		T* operator->() const { return m_itPtr; }
		T& operator*() const { return *m_itPtr; }

	private:
		T* m_itPtr;
		size_t m_itInxMajor = 0;
		const LinekdArray<T>* m_itLA;

};
}
