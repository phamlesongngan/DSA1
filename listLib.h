/*
* =========================================================================================
* Name        : listLib.h
* Author      : Duc Dung Nguyen
* Email       : nddung@hcmut.edu.vn
* Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
* Description : library for Assignment 1 - Data structures and Algorithms - Spring 2018
* =========================================================================================
*/

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>

using namespace std;

class DSAException {
	int     _error;
	string  _text;
public:

	DSAException() : _error(0), _text("Success") {}
	DSAException(int err) : _error(err), _text("Unknown Error") {}
	DSAException(int err, const char* text) : _error(err), _text(text) {}

	int getError() { return _error; }
	string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
	T data;
	L1Item<T> *pNext;
	L1Item() : pNext(NULL) {}
	L1Item(L1Item<T>* p) : pNext(p) {}
	L1Item(T &a) : data(a), pNext(NULL) {}
	L1Item(T &&a) : data(std::move(a)), pNext(NULL) {}
};

template <class T>
class L1List {
public:
	L1Item<T>   *_pHead, *pCur;// The head pointer of linked list
	size_t      _size;// number of elements in this list

	L1List() : _pHead(NULL), pCur(NULL), _size(0) {}
	~L1List() {
		clean();
	};

	void    clean();
	bool    isEmpty() {
		return _pHead == NULL;
	}
	size_t  getSize() {
		return _size;
	}

	//custom function
	bool      storeCurrent(T& ret) {
		if (this->pCur == NULL) {
			return false;
		}
		else {
			T temp = this->pCur->data;
			memcpy(&ret, &temp, sizeof(T));
			this->pCur = this->pCur->pNext;
			return true;
		}
	}
	void initCur() { this->pCur = this->_pHead; }
	void resetCur() { this->pCur = this->_pHead; }
	//T*      find(T& a, std::function<int(*eqCmp) (T&, T&)> eqCmp);
	T& find(T& a, int(*eqCmp) (T&, T&));

	T&      at(int i);
	T&      operator[](int i);

	bool    find(T& a, int& idx);
	int     insert(int i, T& a);
	int     remove(int i);
	
	int     push_back(T& a);
	int     insertHead(T& a);
	int     insertHead() {
		_pHead = new L1Item<T>(_pHead);
		_size++;
		return 0;
	}

	int     removeHead();
	int     removeLast();

	void    reverse();

	void    traverse(void(*op)(T&)) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data);
			p = p->pNext;
		}
	}
	L1Item<T>*  traverse1(bool(*op)(T&, void*), void* pParam) {
		L1Item<T>   *p = _pHead;
		while (p) {
			if (op(p->data, pParam)) return p;
			p = p->pNext;
		}
		return NULL;
	}
	void    traverse(void(*op)(T&, void*), void* pParam) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data, pParam);
			p = p->pNext;
		}
	}

	T* _find(T&a, bool & found, int(*eqcmp)(T&, T&)) {
		L1Item<T> * current = this->_pHead;
		while (current) {
			if (eqCmp(a, current->data) == 0) {
				found = true;
				return &current->data;
			}
			current = current->pNext;
		}
		found = false;
	}
	bool find(T&a, T*ret, int(*eqcmp)(T&, T&)) {
		L1Item<T> * current = this->_pHead;
		while (current) {
			if (eqCmp(a, current->data) == 0) {
				memcpy(ret, &current->data, sizeof(current->data));
				return true;
			}
			current = current->pNext;
		}
		return false;
	}

	bool _remove1(T& a, int(*eqCmp)(T&, T&)) {
		if (eqCmp(a, this->_pHead->data) == 0) {
			L1Item<T> * current = this->_pHead;
			this->_pHead = this->_pHead->pNext;
			this->_size--;
			delete current;
			return true;
		}
		else {
			L1Item<T> * current = this->_pHead;
			L1Item<T> * preCurrent = NULL;
			this->_size--;
			while (current) {
				if (eqCmp(a, current->data) == 0) {
					break;
				}
				preCurrent = current;
				current = current->pNext;
			}
			if (current) {
				preCurrent->pNext = current->pNext;
				current = NULL;
			}
			else 
				return false;
		}
		return true;
	}
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
	if (_pHead == NULL) {
		_pHead = new L1Item<T>(a);
	}
	else {
		L1Item<T>   *p = _pHead;
		while (p->pNext) p = p->pNext;
		p->pNext = new L1Item<T>(a);
	}
	_size++;
	return 0;
}
template <class T>
T& L1List<T>::find(T& a, int(*eqCmp)(T&, T&)) {
	L1Item<T> * current = this->_pHead;
	while (current) {
		if (eqCmp(a, current->data) == 0) {
			return current->data;
		}
		current = current->pNext;
	}
}


/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
	L1Item<T>   *p = new L1Item<T>(a);
	p->pNext = _pHead;
	_pHead = p;
	_size++;
	return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
	if (_pHead) {
		L1Item<T>* p = _pHead;
		_pHead = p->pNext;
		delete p;
		_size--;
		return 0;
	}
	return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
	if (_pHead) {
		if (_pHead->pNext) {
			L1Item<T>* prev = _pHead;
			L1Item<T>* pcur = prev->pNext;
			while (pcur->pNext) {
				prev = pcur;
				pcur = pcur->pNext;
			}
			delete pcur;
			prev->pNext = NULL;
		}
		else {
			delete _pHead;
			_pHead = NULL;
		}
		_size--;
		return 0;
	}
	return -1;
}

template <class T>
void L1List<T>::clean() {
	L1Item<T> * current = this->_pHead;
	while (this->_pHead) {
		this->_pHead = this->_pHead->pNext;
		delete current;
		current = this->_pHead;
		this->_size--;
	}
	delete this->pCur;
}

template <class T>
bool L1List<T>::find(T& a, int &idx) {
	L1Item<T> * current = this->_pHead;
	while (current) {
		if (a == current->data) {
			return true;
		}
		idx++;
		current = current->pNext;
	}
	return false;
}

template <class T>
int L1List<T>::insert(int i, T&a) {
	L1Item<T> * current = this->_pHead;
	int idx = 0;
	while (current) {
		if (i == idx) {
			break;
		}
		current = current->pNext;
		idx++;
	}
	L1Item<T> * pNew = new L1Item<T>(a);
	pNew->pNext = current->pNext;
	current->pNext = pNew;
	this->_size++;
	return idx;
}

template <class T>
int L1List<T>::remove(int i) {
	if (!(isEmpty()) && (i > 0) && (i < (int)_size)) {
		L1Item<T>* p = _pHead;
		L1Item<T>* ptr = NULL;
		for (int count = 0; count < i - 1; count++)
			p = p->pNext;
		ptr = ptr->pNext;
		p->pNext = ptr->pNext;
		delete ptr;
		return 0;
	}
	else return -1;
}

template <class T>
T& L1List<T>::at(int i) {
	if (this->_pHead != NULL) {
		L1Item<T> * pRet = this->_pHead;
		int idx = 0;
		while (pRet) {
			if (i != idx) {
				pRet = pRet->pNext;
				idx++;
			}
			else  return (pRet->data);
		}
	}
	T ret;
	return ret;
}

template <class T>
T& L1List<T>::operator[](int i) {
	return at(i);
}

template <class T>
void L1List<T>::reverse() {
	L1Item<T> * current = this->_pHead;
	L1Item<T> * next = current->pNext;
	while (next) {
		L1Item<T> * pre = current;
		current = next;
		next = current->pNext;
		current->pNext = pre;
	}
	this->_pHead->pNext = NULL;
	this->_pHead = current;
}




#endif //A01_LISTLIB_H