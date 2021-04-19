#include "CountedString.hpp"

CountedString::CountedString(int len, char* dataIn)
{
	data[0] = len;
	for (int i = 0; i < len; i++) {
		data[i+1] = dataIn[i];
	}
}

CountedString::CountedString(char* where)
: data(where)
{
}
	
CountedString& CountedString::operator=(const CountedString& other) {
	if (this != &other) {
		int len = other.data[0];
		for (int i = 0; i <= len; i++) {
			data[i] = other.data[i];
		}
	}
	return *this;
}

bool CountedString::operator==(const CountedString& other) {
	compareTo(other) == 0;
}

int CountedString::compareTo(const CountedString& other) {
	if (this == &other) {
		return 0;
	} else {
		int len = data[0];
		int lenOther = other.data[0];
		int end = (len < lenOther) ? len : lenOther;
		for (int i = 1; i <= end; i++) {
			if (data[i] != other.data[i]) {
				return data[i] - other.data[i];
			}
		}
		return len - lenOther;
	}
}

int CountedString::length() const {
	return data[0];
}

const char& CountedString::operator[](int ndx) const {
	return data[ndx+1];
}

char& CountedString::operator[](int ndx) {
	return data[ndx+1];
}

