#ifndef __SMART_PTR_INCLUDE__
#define __SMART_PTR_INCLUDE__

ZDEV_NAMESPACE_BEGIN

template <typename T>
class smart_ptr {
private:
	T* ptr;
	long* count;
public:
	explicit smart_ptr(T* p=NULL)
		: ptr(p), count(new long(1)) {
	}

	smart_ptr(const smart_ptr<T>& p) throw()
		: ptr(p.ptr), count(p.count) {
		++*count;
	}

	~smart_ptr() throw() {
		dispose();
	}
	
	smart_ptr<T>& operator= (T* p) throw() {
		if (ptr != p) {
			dispose();
			ptr = p;
			count = new long(1);
		}
		return *this;
	}
	
	smart_ptr<T>& operator= (const smart_ptr<T>& p) throw() {
		if (this != &p) {
			dispose();
			ptr = p.ptr;
			count = p.count;
			++*count;
		}
		return *this;
	}

	T& operator* () const throw() {
		return *ptr;
	}
	
	T* operator-> () const throw() {
		return ptr;
	}

	bool operator== (const T* p) const throw() {
		return ptr == p;
	}
	
	bool operator!= (const T* p) const throw() {
		return ptr != p;
	}

	// 自动类型转换
	operator T*() const throw() {
		return ptr;
	}

	// 仅仅为了可以放在排序的容器中，比如作为 map 的 key 值.
	// 否则 msvc 6.0 编译不过去，而 msvc71 却可以。(可能是 msvc60 的 map 的 实现问题)
	bool operator< (const smart_ptr<T>& p) const throw() {
		return ptr < p.ptr;
	}
private:
	void dispose() {
		if (--*count == 0) {
			delete count;
			if (ptr) delete ptr;
		}
	}
};


ZDEV_NAMESPACE_END

#endif // __SMART_PTR_INCLUDE__