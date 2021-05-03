#ifndef CoreDictionary_hpp
#define CoreDictionary_hpp

#include <string>
class Runtime;
class DictionaryWord;

class CoreDictionary {
public:
	CoreDictionary(Runtime* runtime);

	DictionaryWord* find(const std::string& name);
	void rebuildDictionary();
	
private:
	Runtime* runtime;
};

#endif /* CoreDictionary_hpp */
