#include <cmath>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <vector>

namespace Core {
template<typename T>
class LinekdArray {
public:
	LinekdArray() : m_maxsize(8388608/sizeof(T)) {
		if(m_maxsize == 0 || m_maxsize == 1) {
			m_maxsize = 2;
			m_startingSize = 2;
		}
		else {
		for(size_t i = 1; m_startingSize == 0; i*=4)
			m_startingSize = 512 * i / sizeof(T) / 2;
		}
		makeNewArray();
	}
    ~LinekdArray() {
        for (auto ptr : m_dataArrays)
            delete[] ptr;
    }
    //TODO deleted for now. should be implemented at some point;
	LinekdArray(const LinekdArray&) = delete;
	LinekdArray& operator=(const LinekdArray&) = delete;
	LinekdArray(LinekdArray&&) noexcept = delete;
	LinekdArray& operator=(LinekdArray&&) noexcept = delete;

	T& operator[](size_t inx) { return m_dataArrays[inx / m_maxsize][inx % m_maxsize]; }
	void pushBack(const T& newElement) {
		m_dataArrays[m_currInxMajor][m_currInxMinor++] = newElement;
		if(m_currInxMinor >= m_reserved) resize();
	}
	size_t size() { return m_currInxMajor * m_maxsize + m_currInxMinor; }
	bool empty() { return m_currInxMinor == 0 && m_currInxMajor == 0; }
	void clear() {
		for(auto ptr : m_dataArrays) 
			delete[] ptr;
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
		T* newArray = new T[m_reserved];
		m_dataArrays.push_back(newArray);
	}
	void resize() {
		size_t oldsize = m_reserved;
		std::cout << "resize\n";
		if(m_reserved * 4 <= m_maxsize) m_reserved *= 4;
		else if (m_reserved < m_maxsize) m_reserved = m_maxsize;
		else { makeNewArray(); return; }
		T* newArray = new T[m_reserved];
		std::memmove(newArray, m_dataArrays.back(), oldsize);
		delete[] m_dataArrays.back();
		m_dataArrays[m_dataArrays.size() - 1] = newArray;
	}
};
}
