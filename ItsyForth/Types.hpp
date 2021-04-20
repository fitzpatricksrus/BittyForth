#ifndef Types_hpp
#define Types_hpp


class DictionaryWord;

typedef long Num;
typedef DictionaryWord** IPtr;
typedef char* Ptr;

typedef union XData {
	char* ptr;
	char c;
	Num l;
	DictionaryWord* w;
	void* v;
	
	XData() {}
	XData(long lIn) { l = lIn; }
//	XData(char* ptrIn) { ptr = ptrIn; }
//	XData(DictionaryWord* wIn) { w = wIn; }
	XData(void* pIn) { v = pIn; }
	operator long() { return l; }
	operator char*() { return ptr; }
	operator DictionaryWord*() { return w; }
	} XData;
	

#endif /* Types_hpp */
