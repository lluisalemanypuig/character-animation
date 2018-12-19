#pragma once

// C++ includes
#include <memory>
#include <vector>

// This is an implementation of an indexed min-heap-0

template<class T, class Allocator = std::allocator<T> >
class indexed_heap {
	private:
	
		class core_elem {
			private:
			public:
				T value;
				size_t index;
				
				core_elem() {}
				core_elem(const T& v, size_t i) : value(v), index(i) {}
				
				inline core_elem& operator= (const core_elem& e) {
					value = e.value;
					index = e.index;
					return *this;
				}
				
				inline bool operator< (const core_elem& e) const
				{ return value < e.value; }
		};
		
		/// where_is[i] = k : the i-th element
		/// inserted is at the k-th position of the heap
		std::vector<size_t> where_is;
		/// the element T is the i-th element inserted
		std::vector<core_elem> t;
		/// index of the last core_element pushed
		size_t max_index;
		
		size_t j;
		bool blind;
		
		/// Sink element at position p and update @ref where_is.
		void sink(size_t p);
		/// Float element at position p and update @ref where_is.
		void make_float(size_t p);
		
		/// Sink element at position p.
		void blind_sink(size_t p);
		/// Float element at position p.
		void blind_make_float(size_t p);
		
	public:
		indexed_heap<T, Allocator>();
		~indexed_heap<T, Allocator>();
		
		// non-blind operations:
		
		// Pushes a new element into the heap
		// Returns the insertion index of this element
		size_t push(const T& x);
		
		// Pops the element at the top of the heap
		void pop();
		
		// Modifies the i-th pushed element
		void modify_th(size_t i, const T& x);
		
		// Builds a min-heap using the elements in elems
		// pre: The heap must be empty
		// post: Returns the index of the first pushed element in elems
		size_t make_heap(const std::vector<T, Allocator>& elems);
		
		// blind operations:
		
		// Pushes a new element into the heap
		// Returns the insertion index of this element
		size_t blind_push(const T& x);
		
		// Pops the element at the top of the heap
		void blind_pop();
		
		// Builds a min-heap using the elements in elems
		// pre: The heap must be empty
		// post: Returns the index of the first pushed element in elems
		size_t blind_make_heap(const std::vector<T, Allocator>& elems);
		
		// Constructs the where_is table
		// post: the heap is not blind anymore
		void construct_where_is();
		
		// Gets the element at the top of the heap
		// post: the index of the first element pushed after calling this
		//       function is 0
		const T& top() const;
		
		// Actually frees the memory occupied by the heap
		// post: the index of the first element pushed after calling this
		//       function is 0
		void force_flush();
		
		// Frees the memory occupied by the heap (but may not take place
		// at the moment of callig this method)
		void flush();
		
		size_t size() const;
		bool empty() const;
		bool is_blind() const;
};

#include <anim/utils/indexed_heap.cpp>

