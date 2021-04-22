#ifndef CoreDictionary_hpp
#define CoreDictionary_hpp

class Runtime;

class CoreDictionary {
public:
	CoreDictionary(Runtime* runtime);

	void rebuildDictionary();
	
private:
	Runtime* runtime;
};

#endif /* CoreDictionary_hpp */
