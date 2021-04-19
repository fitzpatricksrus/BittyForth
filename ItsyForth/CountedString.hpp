//
//  CountedString.hpp
//  ItsyForth
//
//  Created by Dad on 4/19/21.
//

#ifndef CountedString_hpp
#define CountedString_hpp

class CountedString {
public:
	CountedString(int len, char* dataIn);
	CountedString(char* where);
	
	CountedString& operator=(const CountedString& other);
	bool operator==(const CountedString& other);
	int compareTo(const CountedString& other);
	
	int length() const;
	const char& operator[](int ndx) const;
	char& operator[](int ndx);
	
	char* data;
};

#endif /* CountedString_hpp */
