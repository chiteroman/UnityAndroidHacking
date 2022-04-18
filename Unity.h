#include <iostream>
#include <string>
#include <codecvt>
#include <cassert>
#include <locale>

std::string utf16_to_utf8(std::u16string const& s)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
			std::codecvt_mode::little_endian>, char16_t> cnv;
	std::string utf8 = cnv.to_bytes(s);
	if(cnv.converted() < s.size())
		throw std::runtime_error("incomplete conversion");
	return utf8;
}

template <typename T>
struct monoArray
{
	void* klass;
	void* monitor;
	void* bounds;
	int   max_length;
	void* vector[1];
	int getLength()
	{
		return max_length;
	}
	T getPointer()
	{
		return (T)vector;
	}
};

template <typename T>
struct monoList {
	void* unk0;
	void* unk1;
	monoArray<T>* items;
	int size;
	int version;

	T getItems() {
		return items->getPointer();
	}

	int getSize() {
		return size;
	}

	int getVersion() {
		return version;
	}
};

typedef struct _monoString {
    void *klass;
    void *monitor;
    int length;
    char chars[1];

    int getLength() {
        return length;
    }

    char *getChars() {
        return chars;
    }

    const char *getCS() {
    	std::u16string str = reinterpret_cast<const char16_t *>(getChars());
    	return utf16_to_utf8(str).c_str();
    }
} monoString;

template <typename K, typename V>
struct monoDictionary {
	void *unk0;
	void *unk1;
	monoArray<int **> *table;
	monoArray<void **> *linkSlots;
	monoArray<K> *keys;
	monoArray<V> *values;
	int touchedSlots;
	int emptySlot;
	int size;

	K getKeys(){
		return keys->getPointer();
	}

	V getValues(){
		return values->getPointer();
	}

	int getNumKeys(){
		return keys->getLength();
	}

	int getNumValues(){
		return values->getLength();
	}

	int getSize(){
		return size;
	}
};