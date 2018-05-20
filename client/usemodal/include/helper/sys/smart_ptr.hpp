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

	// �Զ�����ת��
	operator T*() const throw() {
		return ptr;
	}

	// ����Ϊ�˿��Է�������������У�������Ϊ map �� key ֵ.
	// ���� msvc 6.0 ���벻��ȥ���� msvc71 ȴ���ԡ�(������ msvc60 �� map �� ʵ������)
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