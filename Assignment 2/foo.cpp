#include <memory>
using namespace std;

template <typename T>
class SmartArray {  // Using smart pointer.
private:
    unique_ptr< T[] > arr;
	unsigned capacity;
public:
    SmartArray(int size)
        : arr(new T[size])
	    , capacity(size)
	{ }
	// Strong exception safety.
	// Without copy-and-swap idiom.
    SmartArray& operator=(const SmartArray& other) {
        if (this != &other) {
			// Make a new array.
            unique_ptr< T[] > newArr (new T[other.capacity]);
			// Copy over.
            copy(other.arr.get(), other.arr.get() + other.capacity, newArr.get());
			// Take over that copy.
            arr = move(newArr);
            capacity = other.capacity;
        }
		return *this;
    }
};

// Exactly equivalent.
template <typename T>
class EquallySmartArray {  // Using raw pointer.
private:
    T* arr;
    unsigned capacity;
public:
    EquallySmartArray(int size)
        : arr(new T[size])  // Same.
	    , capacity(size)
	{ }
	// Strong exception safety.
	// Without copy-and-swap idiom.
    EquallySmartArray& operator=(const EquallySmartArray& other) {
        if (this != &other) {
			// Make a new array.
            T* newArr = new T[other.capacity];
            try {
				// Copy over.
                copy(other.arr, other.arr + other.capacity, newArr);
            } catch (...) {
				// Clean up, rethrow.
                delete[] newArr;
                throw;
            }
            delete[] arr;
			// Take over that copy.
            arr = newArr;
			newArr = nullptr;  // Optional.
            capacity = other.capacity;
        }
		return *this;
    }
    ~EquallySmartArray() { delete[] arr; }  // One extra method.
};

int main() {
    SmartArray<int> a(5), b(2);
    b = a;
    EquallySmartArray<int> A(5), B(2);
    B = A;
}
