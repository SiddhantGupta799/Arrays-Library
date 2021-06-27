#ifndef ARRAYS_H
#define ARRAYS_H

#include <iostream>
#include <string>
using namespace std;

// just a piece of code that determines if the passed parameter is an iterator
template <class _iter>
using _iter_cat_t = typename iterator_traits<_iter>::iterator_category;

template <class... _Types>
using void_tt = void;

template <class _Ty, class = void>
constexpr bool _is_iterator_v_array = false;

template <class _Ty>
constexpr bool _is_iterator_v_array<_Ty, void_tt<_iter_cat_t<_Ty>>> = true;


namespace Py {
// ================================================= Array Class Iterators: ======================================================= 

	// Random Access Iterator
	template<typename T>
	class _Array_Iterator_ {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

	protected:
		pointer _iter_;

	public:
		_Array_Iterator_(pointer ptr) { 
			_iter_ = ptr;
		}

		_Array_Iterator_(const _Array_Iterator_<T>& rawIterator) {
			this->_iter_ = rawIterator._iter_;
		}
		
		_Array_Iterator_<T>& operator=(const _Array_Iterator_<T>& rawIterator) {
			this->_iter_ = rawIterator._iter_;
			return *this;
		}

		_Array_Iterator_<T>& operator=(pointer ptr) {
			_iter_ = ptr; 
			return *this; 
		}

		operator bool() const {
			if (_iter_) return true;
			else return false;
		}

		bool operator==(const _Array_Iterator_<T>& rawIterator) const { return (_iter_ == rawIterator.getConstPtr()); }
		bool operator!=(const _Array_Iterator_<T>& rawIterator) const { return (_iter_ != rawIterator.getConstPtr()); }

		_Array_Iterator_<T>& operator+=(const difference_type& movement) { _iter_ += movement; return (*this); }
		_Array_Iterator_<T>& operator-=(const difference_type& movement) { _iter_ -= movement; return (*this); }
		_Array_Iterator_<T>& operator++() { ++_iter_; return (*this); }
		_Array_Iterator_<T>& operator--() { --_iter_; return (*this); }

		_Array_Iterator_<T> operator++(int) { auto temp(*this); ++_iter_; return temp; }
		_Array_Iterator_<T> operator--(int) { auto temp(*this); --_iter_; return temp; }
		_Array_Iterator_<T> operator+(const difference_type& movement) { auto oldPtr = _iter_; _iter_ += movement; auto temp(*this); _iter_ = oldPtr; return temp; }
		_Array_Iterator_<T> operator-(const difference_type& movement) { auto oldPtr = _iter_; _iter_ -= movement; auto temp(*this); _iter_ = oldPtr; return temp; }

		difference_type operator-(const _Array_Iterator_<T>& rawIterator) { return std::distance(rawIterator.getPtr(), this->getPtr()); }

		T& operator*() { return *_iter_; }
		const T& operator*() const { return *_iter_; }
		T* operator->() { return _iter_; }

		T* getPtr()const { return _iter_; }
		const T* getConstPtr() const { return _iter_; }

		~_Array_Iterator_() { _iter_ = nullptr; }
	};

	// Random Access Reverse Iterator
	template<typename T>
	class _Array_Reverse_Iterator_ : public _Array_Iterator_<T> {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		_Array_Reverse_Iterator_(T* ptr) : _Array_Iterator_<T>(ptr) {}

		_Array_Reverse_Iterator_(const _Array_Iterator_<T>& rawIterator) { 
			this->_iter_ = rawIterator._iter_; 
		}

		_Array_Reverse_Iterator_(const _Array_Reverse_Iterator_<T>& rawReverseIterator) = default;
		
		_Array_Reverse_Iterator_<T>& operator=(const _Array_Reverse_Iterator_<T>& rawReverseIterator) = default;
		_Array_Reverse_Iterator_<T>& operator=(const _Array_Iterator_<T>& rawIterator) { this->_iter_ = rawIterator.getPtr(); return (*this); }
		_Array_Reverse_Iterator_<T>& operator=(T* ptr) { this->setPtr(ptr); return (*this); }

		_Array_Reverse_Iterator_<T>& operator+=(const difference_type& movement) { this->_iter_ -= movement; return (*this); }
		_Array_Reverse_Iterator_<T>& operator-=(const difference_type& movement) { this->_iter_ += movement; return (*this); }
		_Array_Reverse_Iterator_<T>& operator++() { --this->_iter_; return (*this); }
		_Array_Reverse_Iterator_<T>& operator--() { ++this->_iter_; return (*this); }
		
		_Array_Reverse_Iterator_<T> operator++(int) { auto temp(*this); --this->_iter_; return temp; }
		_Array_Reverse_Iterator_<T> operator--(int) { auto temp(*this); ++this->_iter_; return temp; }
		_Array_Reverse_Iterator_<T> operator+(const int& movement) { auto oldPtr = this->_iter_; this->_iter_ -= movement; auto temp(*this); this->_iter_ = oldPtr; return temp; }
		_Array_Reverse_Iterator_<T> operator-(const int& movement) { auto oldPtr = this->_iter_; this->_iter_ += movement; auto temp(*this); this->_iter_ = oldPtr; return temp; }

		difference_type operator-(const _Array_Reverse_Iterator_<T>& rawReverseIterator) { return std::distance(this->getPtr(), rawReverseIterator.getPtr()); }

		~_Array_Reverse_Iterator_() {}
	};

// ================================================= Array Class: ======================================================= 
	constexpr int MAX_ARRAY_CAPACITY = INT32_MAX;
	constexpr int EXTRA_RESERVE = 10;
	/*
	This Array Class is designed to behave like a vector but in a permitted range of 0 - MAX_ARRAY_CAPACITY.
	Compatibility:
		- With the algorithm library
		- for each loops
		- any other Cpp library that accesses a contigious memory block using begin() and end() method.

	Supports:
		- Negative Index Accessibility
		- Negative Index Insertability
		- Sorted Insertability
		- Input via operator>>
		- Appending
		- Concatenation
		- Replacment
		- Poping
		- Sorting
		- Reversal
		- Erasing
		- Max and Min
		- Duplicates Removal
		- Count and Conditional Count
		- Conditional Removal and Complete removal (all occurrences) of an Element
		- Linear and Binary Search
		- Lower and Upper Bound Search
		- Rotation in Clockwise and Anti Clockwise Direction
		- Negative Positive Rearrangement

	Operators Supported:
		- operator<<	: shows elements to the console
		- operator>> 	: requires that the size is set during initialization
		- operator=		: Assignment
		- operator+		: Momentary concatenation
		- operator+=	: by reference concatenation of a single element or initialiser_list
		- operator[]	: subscripting
		- all of the relational operators < , > , <= , >= , == , !=
				> Uses std::lexicographical_compare
		- operator() 	: it takes a function as a parameter and maps the function to all the array elements

	Other memory Manipulation functions Supported:
		- resize()
		- clear()
		- shrink_to_fit()

	Note:
		operators '+', '+=' can concatenate single values as
		well as values passed in the form of initializer_list.
		ex:
			// operator +=
			Array<int> arr{4,5,3};
			print(arr);
			arr += 5;
			print(arr);
			arr += {4,8,1};
			print(arr);

			// operator +
			print(arr + Array<int>{67, 89, 54});

		output:
			4 5 3
			4 5 3 5
			4 5 3 5 4 8 1
			4 5 3 5 4 8 1 67 89 54

		operator '+'  doesn't support direct initializer_list

	Since Raw arrays and std::array have a problem of not being dynamically declarable.
	for ex.: we cannot pass a variable array size in the templated std::array<{typename},{size_t}>
		i.e. we cannot increase a counter variable and then declare the Array with counter as the value of size.
		ex:
			int counter_val = function(param);
			std::array<int,counter_val> arr;   // this results in a compiler error.
			to avoid this. Array class is designed to not have the size parameter passed to it as template declaration.
			But Size can be passed as a param to constructor

	-	The Array Class provides transferabilty among functions without size becoming a hurdle in between.

	*/

	// defined for Array class only, but can be used for any object that has 'name' as its public variable
#define init_name(x) x.name = #x

	template <class T = int>
	class Array {
		size_t visible_size = 0;							 // Visible Size
		T* values = nullptr;			 // Actual array that holds the info
		int capacity = 0;					// Holding capacity of the array
		int multiplier = 0;		   // factor by which the capacity increases

	public:
		const char* name = "none";

		using iterator = _Array_Iterator_<T>;
		using const_iterator = _Array_Iterator_<const T>;
		using reverse_iterator = _Array_Reverse_Iterator_<T>;
		using const_reverse_iterator = _Array_Reverse_Iterator_<const T>;

	private:
		// this helps in determining whether a Array<char> was intended to behave like a string
		bool is_string = false;

		// To Print the Array. it can be used with all the print functions.
		friend std::ostream& operator<< (std::ostream& os, const Array<T>& arr) {
			if (arr.is_string) {
				for (size_t i = 0; i < arr.visible_size; i++) {
					os << arr.values[i];
				}
			}
			else {
				os << '[';
				arr.show();
				os << ']';
			}
			return os;
		}

		// for support of input
		friend istream& operator>>(std::istream& is, Array<T>& arr) {
			arr.input();
			return is;
		}

		// all relational operators
		friend bool operator<(const Array<T>& lhs, const Array<T>& rhs) {
			return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		friend bool operator>(const Array<T>& lhs, const Array<T>& rhs) {
			return rhs < lhs;
		}

		friend bool operator<=(const Array<T>& lhs, const Array<T>& rhs) {
			return !(rhs < lhs);
		}

		friend bool operator>=(const Array<T>& lhs, const Array<T>& rhs) {
			return !(lhs < rhs);
		}

		// for momentary or assignable join of two Arrays
		friend Array<T> operator+ (Array<T> lhs, Array<T> rhs) {
			lhs.concat(rhs);
			return lhs;
		}

		/* flattens A 2D matrix into a 1D array
		* This looks like a drunken approach to a certain type of problem i.e. flattening
		  a matrix down to 1D
		*/
		friend Array<T> operator~(Array<Array<T>>& matrix) {
			Array<T> flattened_matrix;
			for (Array<T>& ar : matrix) {
				for (T& t : ar) {
					flattened_matrix.append(t);
				}
			}
			return flattened_matrix;
		}

		// yet another copy of swap :(
		template<typename Ty>
		void _swap(Ty& a, Ty& b) {
			Ty temp = a;
			a = b;
			b = temp;
		}

		void _allocate(int bytes = 0) {
			if (this->capacity == 0) {
				// shows new allocation
				this->capacity = bytes + EXTRA_RESERVE;
				this->values = new T[this->capacity];
			}
			else {
				this->capacity = ((++this->multiplier) * 10) + this->capacity + bytes;

				T* sample = new T[this->capacity];

				for (size_t i = 0; i < this->visible_size; i++) {
					sample[i] = this->values[i];
				}

				delete[]this->values;
				this->values = sample;
				sample = nullptr;
			}
		}

		void _de_allocate() {
			if (this->capacity == 0) {
				// nothing can be done
			}
			else {
				// clearing previous memory
				delete[]this->values;
				// allocating basic memory
				this->visible_size = 0;
				this->capacity = EXTRA_RESERVE;
				this->values = new T[this->capacity];
				this->multiplier = 0;
				// Allocates 10 units of space;
			}
		}

		/*
		To fill the default values. passed by the constructor
		This has to be done because for raw-arrays initialiser_list does it automatically.
		*/
		void _filler(initializer_list<T> init_l) {
			int i = 0;
			int size = init_l.size();

			if (this->capacity == 0) {
				this->_allocate(size);
			}

			for (auto& il : init_l) {
				this->values[i] = il;
				i++;
			}

			visible_size = i;
		}

	public:
			/////////////////////////////////////
	///////////////    Constructors    ///////////////
			////////////////////////////////////

			// By default allocates atleast 10 units of memory
		Array() {
			this->_allocate();
		}

		// allocates s units of memory
		Array(size_t s) {
			this->visible_size = s;
			this->_allocate(s);
			this->fill(T{});
		}

		// allocates s units of memory and initializes all of them to t
		Array(size_t s, T t) {
			this->visible_size = s;
			this->_allocate(s);
			this->fill(t);
		}

		// for management of Strings
		Array(string s) {
			int len = (s.size() + 1);
			this->is_string = true;
			this->visible_size = (len - 1);
			this->_allocate(len);
			for (size_t i = 0; i < this->visible_size; i++) {
				this->values[i] = s[i];
			}
		}

		Array(char const* s) : Array(string(s)) {}

		// for initialization from sections of a raw array
		Array(T* begin, T* end, size_t additional_bytes = 0) {
			int len = end - begin;
			this->_allocate(len + additional_bytes);
			this->visible_size = len;
			for (int i = 0; i < len; i++) {
				this->values[i] = *(begin + i);
			}
		}

		// for conversion of a raw array completely to Array Class object
		template<size_t s>
		Array(T(&_arr)[s]) {
			int len = s;
			this->_allocate(len);
			this->visible_size = len;
			for (int i = 0; i < len; i++) {
				this->values[i] = *(_arr + i);
			}
		}

		// Iterator Based Inititalization, supports iterators that have an overloaded ++ and * and - operator
		template<class _Iter, enable_if_t<_is_iterator_v_array<_Iter>, int> = 0>
		Array(_Iter begin, _Iter end) {
			_Iter it = begin;
			int size = end - begin;
			this->_allocate(size);
			this->visible_size = size;
			int i = 0;
			while (it != end) {
				this->values[i++] = *it;
				it++;
			}
		}

		// Initializer list initialization constructor
		Array(initializer_list<T> init_l) { this->_filler(init_l); }

			///////////////////////////////////////
	///////////////   Copy Constructor    ///////////////
			//////////////////////////////////////

		Array(const Array<T>& obj) :
			multiplier{ obj.multiplier },
			visible_size{ obj.visible_size },
			capacity{ obj.capacity }
		{
			this->values = new T[obj.capacity];

			for (size_t i = 0; i < obj.visible_size; i++) {
				this->values[i] = obj.values[i];
			}

			/*cout << "copy constructor" << endl;
			this->log_properties();*/
		}

			///////////////////////////////////////
	///////////////   Move Constructor    ///////////////
			//////////////////////////////////////

		Array(Array&& obj) noexcept {
			// cout << "Move Constructor" << endl;
			// changing ownership
			this->values = obj.values;
			// nulling out sample
			obj.values = nullptr;
			// assigning all the values accordingly
			this->capacity = obj.capacity;
			this->visible_size = obj.visible_size;
			this->multiplier = obj.multiplier;
		}


			/////////////////////////////////////////////
	///////////////    Basic Utility Functions    ///////////////
			////////////////////////////////////////////

		// Fills the Array upto its visible size.
		void fill(T value) {
			for (size_t i = 0; i < this->visible_size; i++) {
				values[i] = value;
			}
		}

		// Returns visible size 
		int size() { return (int)this->visible_size; }

		// returns capacity
		int get_capacity() { return capacity; }

			//////////////////////////////
	///////////////    Operators    ///////////////
			//////////////////////////////

			// Supports positive and negative indices within the range of 0 - visible_size
		T& operator[] (int index) {
			// supports negative indices
			if (index >= 0) {
				assert(index >= 0 and index < (int)this->visible_size);
			}
			if (index < 0) {
				index = this->visible_size - ((-1) * index);
				assert(index >= 0 and index < (int)this->visible_size);
			}
			return this->values[index];
		}

		T& at(int index) {
			return this->operator[](index);
		}

		/*
		Maps a function passed as parameter to all the array elements.
		Note: Type conversions other than the implicit ones are restricted by C++ itself.
		Ex: The passed function should look like one of these
			> int square (int n) {return n*n;}
			> void square (int& n) {n = n*n;}
			> int square (int& n) {n = n*n; return n;}
		*/
		// Ex -> int square (int n) {return n*n;}
		// This operator returns a copy of the elements mapped with the function passed as parameter
		Array<T>  operator()(T(function)(T)) {
			Array<T> arrt(this->visible_size);
			for (size_t i = 0; i < this->visible_size; i++) {
				arrt[i] = function(this->values[i]);
			}
			return arrt;
		}

		// Ex -> void square (int& n) {n = n*n;}
		void operator()(void(function)(T&)) {
			for (size_t i = 0; i < this->visible_size; i++) {
				function(this->values[i]);
			}
		}

		// Ex -> int square (int& n) {n = n*n; return n;}
		Array& operator()(T(function)(T&)) {
			for (size_t i = 0; i < this->visible_size; i++) {
				function(this->values[i]);
			}
			return *this;
		}

		template<typename U>
		Array<U> operator()(U(function)(T)) {
			Array<U> arru = Array<U>(this->visible_size);
			for (size_t i = 0; i < this->visible_size; i++) {
				arru[i] = function(this->values[i]);
			}
			return arru;
		}

		// Copy Assignment, Making two exact copies
		Array& operator= (const Array<T>& obj) {
			// acquiring memory equivalent to assignee
			T* sample = new T[obj.capacity];

			// filling the memory with data
			for (size_t i = 0; i < obj.visible_size; i++) {
				sample[i] = obj.values[i];
			}

			// removing old data
			delete[]this->values;

			// changing ownership
			values = sample;

			// nulling out sample
			sample = nullptr;

			// assigning all the values accordingly
			this->capacity = obj.capacity;
			this->visible_size = obj.visible_size;
			this->multiplier = obj.multiplier;

			// returning the reference for current array
			return *this;
		}

		// Move Assignment, Changing the owner ship
		Array& operator= (Array<T>&& obj) noexcept {
			// removing old data
			delete[]this->values;

			// changing ownership
			this->values = obj.values;

			// nulling out sample
			obj.values = nullptr;

			// assigning all the values accordingly
			this->capacity = obj.capacity;
			this->visible_size = obj.visible_size;
			this->multiplier = obj.multiplier;

			// returning the reference of the current array
			return *this;
		}

		// Compares Visible Size and All the values
		bool operator== (const Array<T>& arr) {
			if (this->visible_size != arr.visible_size) {
				return false;
			}
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] != arr.values[i]) {
					return false;
				}
			}
			return true;
		}

		operator bool() { return this->values != nullptr; }

		// operator format for appending
		Array& operator+=(T t) {
			this->append(t);
			return *this;
		}

				////////////////////////////////////////
	///////////////   Memory/Size Manipulators    ///////////////
				///////////////////////////////////////

		Array& resize(int bytes = 0) noexcept {
			// allocates more bytes than actually passed as parameter, just in case
			this->_allocate(bytes);
			return *this;
		}

		// clears the data;
		Array& clear() noexcept {
			this->_de_allocate();
			return *this;
		}

		// shrinks the size of the array to visible_size
		Array& shrink_to_fit() noexcept {
			// allocating required memory
			T* sample = new T[this->visible_size];

			// initializing the memory
			for (size_t i = 0; i < this->visible_size; i++) {
				sample[i] = this->values[i];
			}

			// clearing old data
			delete[] this->values;

			// changing ownership
			this->values = sample;

			// nullfying sample
			sample = nullptr;

			// setting all the parameters right
			this->capacity = this->visible_size;
			this->multiplier = 0;

			// returning the reference to current object
			return *this;
		}

		// return the address of first element
		T* begin() {
			return &(this->values[0]);
		}

		// return the address of last element
		T* end() {
			return &this->values[visible_size];
		}

		T* cbegin() {
			return &(this->values[0]);
		}

		T* cend() {
			return &this->values[visible_size];
		}

		// for reverse iterator support

		T* rbegin() {
			return &this->values[visible_size - 1];
		}

		T* rend() {
			return &this->values[- 1];
		}

		T* crbegin() {
			return &this->values[visible_size - 1];
		}

		T* crend() {
			return &this->values[-1];
		}

			///////////////////////////////////////
	///////////////   Member Functions   ///////////////
			//////////////////////////////////////

		Array& input() {
			if (this->is_string) {
				// declaring a temp string
				string s;
				// taking input
				cout << "Enter a string (ending with \\n): ";
				std::getline(cin, s);

				// deleting previous data
				delete[] this->values;

				// getting the size of the input
				int size = s.size();

				// initializing things right
				this->capacity = size + EXTRA_RESERVE;
				this->values = new T[this->capacity];
				this->visible_size = size;
				this->multiplier = 0;

				// finally initializing the values
				for (size_t i = 0; i < size; i++) {
					this->values[i] = s[i];
				}
			}
			else {
				// cout << "Enter " << this->visible_size << " values: ";
				for (size_t i = 0; i < this->visible_size; i++) {
					cin >> this->values[i];
				}
			}
			return *this;
		}

		// Appends and Element to the last and increases visible size by 1
		Array& append(T t) noexcept {
			this->concat({ t });
			return *this;
		}

		// inserts elem at the given index
		Array& insert(int index, T elem) noexcept {
			assert((this->visible_size + 1) < MAX_ARRAY_CAPACITY);

			index >= 0 ? (index) : (index = (this->visible_size - ((-1) * index)));

			assert(index >= 0 and index < (int)this->visible_size);

			if (this->visible_size == this->capacity) {
				this->_allocate();
			}

			for (size_t i = this->visible_size; (int)i > index; i--) {
				this->values[i] = this->values[i - 1];
			}

			this->values[index] = elem;
			this->visible_size++;
			return *this;
		}

		// Sorts the Array, finds the position of elem and inserts it there
		Array& sorted_insert(T elem) {
			// sorting
			this->sort();

			// checking that insertion doesn't overflow the buffer
			if ((int)this->visible_size == this->capacity) {
				this->_allocate();
			}

			int i = (this->visible_size - 1);

			// finding the place
			while (i >= 0 and this->values[i] > elem) {
				this->values[i + 1] = this->values[i];
				i--;
			}

			this->visible_size++;
			this->values[i + 1] = elem;

			return *this;
		}

		Array& concat(char const* s) noexcept {
			if (this->is_string) {
				this->concat(Array<char>(s));
			}
			return *this;
		}

		// concatenates Array at the back, also reallocates any extra memory if required
		// important: if T = string and concatenation of only one element is required, consider using append()
		Array& concat(Array<T> arr) noexcept {
			int totalSize = this->visible_size + arr.visible_size;

			// concatenation shouldn't make the array more than MAX_ARRAY_CAPACITY
			assert(totalSize < MAX_ARRAY_CAPACITY);

			if (totalSize >= this->capacity) {
				// uses arr.visible_size to increase the size accordingly to fit the array 
				// and still have enough space to concatenate more before another reallocation
				this->_allocate(arr.visible_size);
			}

			int  i = this->visible_size;
			int arrIndex = 0;
			for (; i < totalSize; i++) {
				this->values[i] = arr[arrIndex];
				arrIndex++;
			}
			this->visible_size += arrIndex;
			return *this;
		}

		// finds and replaces the value, if unable to find the value no changes take place
		// faster replacements if array is sorted
		// Note:
		//	> replaces the first occurrence in an unsorted array.
		//	> replaces the last occurrence in a sorted array.
		Array& replace(T elem1, T elem2) {
			int index = -1;
			if (this->is_sorted_ascend()) {
				index = this->upper_bound(elem1);
			}
			else {
				index = this->linear_search(elem1);
			}
			index >= 0 ? (this->values[index] = elem2) : (elem2);
			return *this;
		}

		// replaces all occurrences of elem1 with elem2
		Array& replace_all(T elem1, T elem2) {
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] == elem1) this->values[i] = elem2;
			}
			return *this;
		}

		// outputs all the occurrences of elem
		Array<T> find_all(T elem) {
			Array<T> arr;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] == elem) arr.append(i);
			}
			return arr;
		}

		// replaces all occurrences like this:
		//		elem1 -> elem2
		//		elem2 -> elem1
		Array& interchange(T elem1, T elem2) {
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] == elem1) { this->values[i] = elem2; }
				else if (this->values[i] == elem2) { this->values[i] = elem1; }
			}
			return *this;
		}

		// initializes the last element with the default value and reduces the visible size by 1
		Array& pop() {
			this->values[--this->visible_size] = T{};
			return *this;
		}

	private:
		void _shift_left(int begin) {
			for (size_t i = begin; i < this->visible_size; i++) {
				this->values[i] = this->values[i + 1];
			}
			this->pop();
		}

	public:
		// removes the element at a given index and returns removed element
		T remove(int index) {
			T val = this->values[index];
			this->_shift_left(index);
			return val;
		}

		// conditional remove
		//	> removes elements if a certain condition is met
		// making this adpatable to functor and lamdas
		template<class Func>
		Array& remove_if(Func function) {
			size_t temp_size = 0;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (!function(this->values[i])) {
					this->values[temp_size++] = this->values[i];
				}
			}

			// defaulting deleted section
			for (size_t i = temp_size; i < this->visible_size; i++) {
				this->values[i] = T{};
			}

			this->visible_size = temp_size;
			return *this;
		}

	private:
		// for remove_all
		struct is_elem {
			T t;
			is_elem(T _t) : t{ _t } {}
			bool operator()(T e) {
				return t == e;
			}
		};

	public:
		// to remove all the occurrences of elem
		// time complexity: O(n)
		Array& remove_all(T elem) {
			this->remove_if(is_elem(elem));
			return *this;
		}

		// mathematical intersection of two sets
		// works linearly in sorted arr, at most 
		Array& intersect(Array<T> arr) {
			assert(this->visible_size > arr.visible_size);
			// sorting the other set before hand
			arr.sort();
			size_t temp_size = 0;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (!arr.binary_search(this->values[i])) {
					this->values[temp_size++] = this->values[i];
				}
			}

			// defaulting deleted section
			for (size_t i = temp_size; i < this->visible_size; i++) {
				this->values[i] = T{};
			}

			this->visible_size = temp_size;
			return *this;
		}

		// Removes all the duplicates
		// Time Complexity: O(n) [if we keep (part: 1) out of equation]
		// with a positive point that it doesn't use hash table and maintains the order of elements
		Array& remove_duplicates() {
			int* _register = new int[this->visible_size]{ 0 };

			// [part: 1] = useful initialization of _register
			for (size_t i = 0; i < this->visible_size; i++) {
				T temp = this->values[i];
				for (size_t j = i + 1; j < this->visible_size; j++) {
					if (temp == this->values[j]) {
						_register[j]++;
					}
				}
			}

			// removal of elements
			size_t temp_size = 0;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (_register[i] == 0) {
					this->values[temp_size++] = this->values[i];
				}
			}

			// defaulting deleted section
			for (size_t i = temp_size; i < this->visible_size; i++) {
				this->values[i] = T{};
			}

			this->visible_size = temp_size;

			// nulling out _register
			delete[]_register;
			_register = nullptr;

			return *this;
		}

		// counts all th occurrences of elem
		int count(T elem) {
			int count = 0;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] == elem) count++;
			}
			return count;
		}

		// conditional count
		//	Counts only if a certain condition is met.
		//	Condition should be passed as a function.
		template<class Func>
		int count_if(Func function) {
			int count = 0;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (function(this->values[i])) count++;
			}
			return count;
		}

		// erases a part of the array marked by [begin,end)
		Array& erase(size_t begin, size_t end) {	// false parameters cause no change to Array
			// the parameters should be inbound
			if (begin < this->visible_size and end < this->visible_size and (end - begin < this->visible_size)) {
				int base = begin;

				// shifting everything towards left
				for (size_t i = end; i < this->visible_size; i++) {
					this->values[base++] = this->values[i];
				}

				// clearing and defaulting out deleted section
				for (size_t i = base; i < this->visible_size; i++) {
					// initiailizing with default value
					this->values[i] = T{};
				}

				this->visible_size -= (end - begin);
			}
			return *this;
		}

		// erases everything starting from begin
		Array& erase(int begin) {
			this->erase(begin, this->visible_size);
			return *this;
		}

	private:
		void _place_sorted(T elem, int end) {
			end--;

			// finding the place
			while (end >= 0 and this->values[end] > elem) {
				*(this->values + end + 1) = this->values[end];
				end--;
			}

			// putting the elem in its sorted position
			this->values[end + 1] = elem;
		}

	public:
		// Sorts the array (using insertion sort), only if it is not sorted
		// since it is adaptive and stable
		Array& sort() {
			if (this->is_sorted_ascend()) {
			}
			else if (this->is_sorted_descend()) {
				this->reverse();
			}

#ifdef _ALGORITHM_
			else if (this->visible_size >= 200) {
				/* 
				since algorithm library is being included and compiled here just for std::sort, this manuever 
				is done to exclude it in small projects, that dont require large sized arrays 
				but if the library is not included the sort function will work but will use insertion sort
				
				If for Some reason this part of code doesn't get enabled make sure the official libraries are included first 
				and then include this library
				ex: 
					#include <algorithm>
					#include "Arrays.h"
				*/
				std::sort(this->begin(), this->end());

			}
#endif
			else {

				// insertion sort
				for (size_t i = 1; i != this->visible_size; i++) {
					this->_place_sorted(this->values[i], i);
				}

				// bubbble sort (just in case)
				/*for (size_t i = 0; i < this->visible_size; i++) {
					for (size_t j = 1; j < this->visible_size - i; j++) {
						if (this->values[j - 1] > this->values[j])
							_swap(this->values[j - 1], this->values[j]);
					}
				}*/
			}
			return *this;
		}

	private:
		// odd_indice_val even_indice_val placement
		// reversal with half rotation
		// odd_indice_val even_indice_val placement
		void odd_even_place_concatenation() {
			int even_size = 0, odd_size = 0;
			auto iseven = [](int n) -> bool { return n % 2 == 0; };

			if (iseven(this->visible_size)) even_size = odd_size = (this->visible_size / 2);
			else odd_size = (this->visible_size / 2), even_size = ((this->visible_size / 2) + 1);

			// say even_size with odd_size done;
			T* even_indice_val = new T[even_size]{};
			T* odd_indice_val = new T[odd_size]{};

			int e_ct = 0, o_ct = 0;
			for (size_t i = 0; i < this->visible_size; i++) {
				if (iseven(i))even_indice_val[e_ct++] = this->values[i];
				else odd_indice_val[o_ct++] = this->values[i];
			}

			for (int i = 0; i < even_size; i++) {
				//say even_indice_val[i] with "";
				this->values[i] = even_indice_val[i];
			}

			//say "" wrap;

			for (int i = 0; i < odd_size; i++) {
				//say odd_indice_val[i] with "";
				this->values[i + e_ct] = odd_indice_val[i];
			}

			//say "" wrap;

			delete[] even_indice_val;
			delete[] odd_indice_val;
			even_indice_val = odd_indice_val = nullptr;
		}

	public:
		Array& shuffle() {
			// doesn't use libraries random shuffle functions
			this->odd_even_place_concatenation();
			this->reverse();
			this->rotate_anti_clockwise(this->visible_size / 2);
			this->odd_even_place_concatenation();
			return *this;
		}

		// swaps the entire owner ship with the passed param
		Array& swap(Array<T>& obj) {
			this->_swap(this->visible_size, obj.visible_size);
			this->_swap(this->multiplier, obj.multiplier);
			this->_swap(this->capacity, obj.capacity);
			this->_swap(this->is_string, obj.is_string);
			this->_swap(this->values, obj.values);
			this->_swap(this->name, obj.name);
			return *this;
		}

		// returns index of the element
		int linear_search(T elem) {
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] == elem) {
					return i;
				}
			}
			return -1;
		}

		// Return: true if the array is sorted in ascending order
		bool is_sorted_ascend() {
			for (size_t i = 0; i < visible_size - 1; i++) {
				if (this->values[i] > this->values[i + 1]) {
					return false;
				}
			}
			return true;
		}

		// Return: true if the array is sorted in descending order
		bool is_sorted_descend() {
			for (size_t i = 0; i < visible_size - 1; i++) {
				if (this->values[i] < this->values[i + 1]) {
					return false;
				}
			}return true;
		}

	private:
		// recursive version
		int __binary_search(T* arr, int low, int high, T elem) {
			int mid = (low + high) / 2;
			if (low <= high) {
				if (arr[mid] == elem) {
					return mid;
				}
				else if (elem < arr[mid]) {
					return __binary_search(arr, low, mid - 1, elem);
				}
				else if (elem > arr[mid]) {
					return __binary_search(arr, mid + 1, high, elem);
				}
			}
			return -1;
		}

		// for debugging
		void show() const {
			if (this->visible_size >= 1) {
				for (size_t i = 0; i < this->visible_size - 1; i++) {
					cout << this->values[i] << ", ";
				}cout << this->values[visible_size - 1];
			}
		}

	public:
		/* Note: Always search for a value one degree lesser
		Ex: search 2 in Array<int>{5,4,3,2,1}
			search {1,2,3} in Array<Array<int>> {{1,2,3}, {3,4,5}}
		No guarantee for presence of a 2D matrix in 3D matrix or above
		*/
		// returns true if the element is present, false if not.
		bool binary_search(T elem) {
			// temporary sample for binary searching
			T* sample = new T[visible_size];

			// copying the values
			for (size_t i = 0; i < visible_size; i++) {
				sample[i] = this->values[i];
			}

			// swapping the ownership
			this->_swap(this->values, sample);

			// because sorting works on this->values so to let sorting be handled by sort()
			this->sort();

			// again swapping the ownership
			this->_swap(this->values, sample);

			// finding, the idx here merely represents the presence of the element and not the actual index where it exists
			int idx = this->__binary_search(sample, 0, this->visible_size, elem);

			// clearing the sample
			delete[] sample;
			sample = nullptr;

			// returning the presence of element
			return idx >= 0 ? true : false;
		}

		int lower_bound(T elem) {
			// assumes the array is already sorted
			// this code implemented similar to lower_bound function in xutility library
			int _count = this->visible_size;
			T* _first = &this->values[0];

			while (_count > 0) {
				int _count2 = _count / 2;
				T* _mid = (_first + _count2);
				if (*_mid < elem) {
					_first = (_mid + 1);
					_count -= _count2 + 1;
				}
				else {
					_count = _count2;
				}
			}

			return (_first - &this->values[0]);
		}

		// returns the upper bound of the passed element, greater than elem
		int upper_bound(T elem) {
			// assumes the array is already sorted
			int _count = this->visible_size;
			T* _last = &this->values[this->visible_size - 1];

			while (_count > 0) {
				int _count2 = _count / 2;
				T* _mid = (_last - _count2);
				if (*_mid > elem) {
					_last = _mid - 1;
					_count -= _count2 + 1;
				}
				else {
					_count = _count2;
				}
			}

			return (_last - &this->values[0]);
		}

		// Max Element
		T max() {
			T temp = this->values[0];
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] > temp) {
					temp = this->values[i];
				}
			}
			return temp;
		}

		// Min Element
		T min() {
			T temp = this->values[0];
			for (size_t i = 0; i < this->visible_size; i++) {
				if (this->values[i] < temp) {
					temp = this->values[i];
				}
			}
			return temp;
		}

		// logs all the properties of the array on console
		void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {

			static int _i = 0;

			if (show_log_count)
				cout << "Log Record Count: " << _i << endl;

			if (entry != "")
				cout << "Debug Entry: " << entry << endl;

			if (log_obj_details) {
				cout << "Object Address: " << this << endl;
				cout << "Object Type: " << typeid(*this).name() << endl;
			}

			cout << "Object Name: " << this->name << endl;
			cout << "Content: "; this->show(); cout << endl;
			cout << "Size: " << this->visible_size << endl;
			cout << "Current Capacity Multiplier: " << this->multiplier << endl;
			cout << "Current Capacity: " << this->capacity << endl;
			cout << endl;
			_i++;
		}

		// Arranges negatives on left side, positives on right side
		Array& neg_pos_rearrange() {
			int i = 0, j = visible_size - 1;
			while (i < j) {

				while (this->values[i] < 0) { i++; }
				while (this->values[j] >= 0) { j--; }

				if (i < j)
					_swap(this->values[j], this->values[i]);
			}
			return *this;
		}

		// rotates the array by the given number of times anticlockwise
		// time complexity: O(n), auxilliary space required: 5 * sizeof(int)
		Array& rotate_anti_clockwise(int no_of_times = 1) {
			// using times as pivot
			int first_mid = no_of_times / 2, second_mid = ((this->visible_size + no_of_times) / 2);
			int first_last = no_of_times - 1, second_last = (this->visible_size + no_of_times - 1);
			int i = 0;

			// reversing [0,times) // no. of iterations = no_of_times/2
			for (i = 0; i < first_mid; i++) {
				_swap(this->values[i], this->values[first_last - i]);
			}

			// reversing [times,visible_size) // no. of iterations = (no_of_times + visible_size)/2
			for (i = no_of_times; i < second_mid; i++) {
				_swap(this->values[i], this->values[second_last - i]);
			}

			// reversing whole array
			this->reverse();
			return *this;
		}

		// rotates the array by the given number of times clockwise
		// time complexity: O(n)
		Array& rotate_clockwise(int times = 1) {
			this->rotate_anti_clockwise(this->visible_size - times);
			return *this;
		}

		// Reverses the Array
		Array& reverse() {
			int mid = this->visible_size / 2;
			for (int i = 0; i < mid; i++) {
				_swap(this->values[i], this->values[this->visible_size - i - 1]);
			}
			return *this;
		}

		// Gets Rid of allocated memory and sets everything right
		~Array() {
			this->visible_size = 0;
			this->capacity = 0;
			this->multiplier = 0;
			delete[]this->values;
			this->values = nullptr;
		}
	};
}

#endif
