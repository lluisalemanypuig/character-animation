#include <anim/utils/indexed_heap.hpp>

/// Private

#define __ih_left_child(i) (i << 1) + 1	// 2*i + 1
#define __ih_right_child(i) (i + 1) << 1	// 2*(i + 1) = 2*i + 2
#define __ih_parent(i) ((i & 0x1) == 0 ? (i >> 1) - 1 : i >> 1)

template<class T, class Allocator>
void indexed_heap<T, Allocator>::sink(size_t p) {
	size_t c = __ih_left_child(p);
	
	bool last = false;
	while (c < j and not last) {
		if (c + 1 < j and t[c + 1] < t[c]) {
			++c;
		}
		if (t[c] < t[p]) {
			
			swap(where_is[t[p].index], where_is[t[c].index]);
			swap(t[p], t[c]);
			
			p = c;
			c = __ih_left_child(p);
		}
		else {
			last = true;
		}
	}
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::make_float(size_t p) {
	while (p != 0 and t[p] < t[__ih_parent(p)]) {
		swap(where_is[t[p].index], where_is[t[__ih_parent(p)].index]);
		swap(t[p], t[__ih_parent(p)]);
		p = __ih_parent(p);
	}
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::blind_sink(size_t p) {
	size_t c = __ih_left_child(p);
	
	bool last = false;
	while (c < j and not last) {
		if (c + 1 < j and t[c + 1] < t[c]) {
			++c;
		}
		if (t[c] < t[p]) {
			swap(t[p], t[c]);
			p = c;
			c = __ih_left_child(p);
		}
		else {
			last = true;
		}
	}
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::blind_make_float(size_t p) {
	while (p != 0 and t[p] < t[__ih_parent(p)]) {
		swap(t[p], t[__ih_parent(p)]);
		p = __ih_parent(p);
	}
}

/// Public

template<class T, class Allocator>
indexed_heap<T, Allocator>::indexed_heap() {
	j = 0;
	max_index = 0;
	blind = false;
}

template<class T, class Allocator>
indexed_heap<T, Allocator>::~indexed_heap() { }

template<class T, class Allocator>
size_t indexed_heap<T, Allocator>::push(const T& x) {
	if (blind) construct_where_is();
	
	if (j < t.size()) {
		t[j] = core_elem(x, max_index);
	}
	else {
		t.push_back(core_elem(x, max_index));
	}
	
	where_is.push_back(j);
	make_float(j);
	++max_index;
	++j;
	
	return max_index - 1;
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::pop() {
	if (blind) construct_where_is();
	
	where_is[t[j - 1].index] = 0;
	t[0] = t[j - 1];
	sink(0);
	
	--j;
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::modify_th(size_t i, const T& x) {
	if (blind) construct_where_is();
	
	size_t p = where_is[i];
	t[p].value = x;
	
	if (p > 0 and x < t[__ih_parent(p)].value) {
		make_float(p);
	}
	else {
		sink(p);
	}
}

template<class T, class Allocator>
size_t indexed_heap<T, Allocator>::make_heap
(const std::vector<T, Allocator>& elems)
{
	size_t first_index = max_index;
	for (size_t i = 0; i < elems.size(); ++i) {
		t.push_back(core_elem(elems[i], max_index + i));
	}
	max_index += elems.size();
	j = t.size();
	
	for (size_t i = t.size() - 2; i >= 1; --i) {
		blind_sink(i);
	}
	blind_sink(0);
	
	where_is = std::vector<size_t>(t.size());
	construct_where_is();
	
	return first_index;
}

template<class T, class Allocator>
size_t indexed_heap<T, Allocator>::blind_push(const T& x) {
	blind = true;
	
	if (j < t.size()) {
		t[j] = core_elem(x, max_index);
	}
	else {
		t.push_back(core_elem(x, max_index));
	}
	
	blind_make_float(j);
	
	++max_index;
	++j;
	
	return max_index - 1;
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::blind_pop() {
	blind = true;
	
	t[0] = t[j - 1];
	blind_sink(0);
	--j;
}

template<class T, class Allocator>
size_t indexed_heap<T, Allocator>::blind_make_heap
(const std::vector<T, Allocator>& elems)
{
	blind = true;
	size_t first_index = max_index;
	for (size_t i = 0; i < elems.size(); ++i) {
		t.push_back(core_elem(elems[i], i));
	}
	max_index = elems.size();
	j = t.size();
	
	for (size_t i = t.size() - 2; i >= 1; --i) {
		blind_sink(i);
	}
	blind_sink(0);
	
	where_is = std::vector<size_t>(t.size());
	return first_index;
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::flush() {
	max_index = 0;
	j = 0;
	where_is.clear();
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::force_flush() {
	std::vector<T, Allocator>().swap(t);
	std::vector<T>().swap(where_is);
	j = 0;
	max_index = 0;
}

template<class T, class Allocator>
void indexed_heap<T, Allocator>::construct_where_is() {
	blind = false;
	for (size_t i = 0; i < t.size(); ++i) {
		where_is[t[i].index] = i;
	}
}

template<class T, class Allocator>
const T& indexed_heap<T, Allocator>::top() const {
	return t[0].value;
}

template<class T, class Allocator>
size_t indexed_heap<T, Allocator>::size() const {
	return j;
}

template<class T, class Allocator>
bool indexed_heap<T, Allocator>::empty() const {
	return j == 0;
}

template<class T, class Allocator>
bool indexed_heap<T, Allocator>::is_blind() const {
	return blind;
}
