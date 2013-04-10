/* -*-Mode: C++;-*- */
//
// See stanford-copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "stanford-copyright.h"


//////////////////////////////////////////////////////////////////////
//  
//  list.h
//
//  This file implements a list template.
//  Adapted from similar templates written by Kathy Yelick and 
//  Paul Hilfinger.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LIST_H_
#define _LIST_H_

#include "cool-io.h"  //includes iostream
#include <stdlib.h>

template <class T>
class BasicListIterator;

template <class T>
class List {
private:
	T *head;
	List<T>* tail;
public:
	List(T *h,List<T>* t = NULL): head(h), tail(t) { }

	T *hd() const       { return head; }  
	List<T>* tl() const { return tail; }
  
	BasicListIterator<T> * getIterator() {
		return new BasicListIterator<T>(this);
	}
  
	List<T>* reverse()  {
		List<T> *ret = NULL;
		for (List<T> * l = this; l; l = l->tl()){
			ret = new List<T>(l->hd(), ret);
		}
		
		return ret;
	}
	
	void add(T & end){
		tail = new List<T>(&end);
	}
	
	static void remove(List<T> * &l, T * elem){
		List<T> * previous = NULL;
		List<T> * iter = l;
		bool found = false;
		while (!found && iter){
			if (iter->hd() == elem) {
				found = true;
			} else {
				previous = iter;
				iter = iter->tl();
			}
		}
		
		if (found) {
			if (previous == NULL){
				l = l->tail;
			} else {
				previous->tail = iter->tail;
			}
		}
	}
	
	static void replace(List<T> * &l, T * old, T * new_){
		List<T> * previous = NULL;
		List<T> * iter = l;
		bool found = false;
		while (!found && iter){
			if (iter->hd() == old) {
				found = true;
			} else {
				previous = iter;
				iter = iter->tl();
			}
		}
		
		if (found) {
			if (previous == NULL){
				l = new List<T>(new_, l->tail);
			} else {
				previous->tail = new List<T>(new_,iter->tail);
			}
		}
	}
	
	//Performs a deep-copy of the list passed as argument, returning a 
	//brand-new list, containing the same number of elements, in the same order, and using the same pointers. 
	//Because this list class is not really object-oriented, a copy constructor hasn't been used for this purpose.
	List<T> * copy(List<T> * &last_node) {
		List<T> *ret = NULL;
		last_node =NULL;
		for (List<T> * l = this; l; l = l->tl()){
			if (ret == NULL) {
				ret = new List<T>(l->hd(), NULL);
				last_node = ret;
			} else {
				last_node->tail = new List<T>(l->hd(), NULL);
				last_node= last_node->tail;
			}
		}
		
		return ret;
	}
	
	List<T> * copy () {
		List<T> * node;
		return copy(node);
	}
	
	static List<T> * concat (List<T> * begin, List<T> * end){
		
		if (begin == NULL) {
			return end;
		}
		
		List<T> * last_node = NULL;
		for (List<T> * l = begin; l; l = l->tl()){
			if (l->tl() != NULL){
				last_node = l;
			}
		}
		
		last_node->tail = end;
		
		return begin;
	}
};

template <class T>
class BasicListIterator {
private:
	List<T> * list_;
	
public:
	BasicListIterator(List<T> * l) : list_(l) {}
	
	bool more(){
		return list_ != NULL;
	}
	void move(){
		list_ = list_->tl();
	}
	
	T * current(){
		return list_->hd();
	}
};

/////////////////////////////////////////////////////////////////////////
// 
// list function templates
//
// To avoid potential problems with mutliple definitions of 
// the List<> class members, the list functions are not members of the
// list class.
//
/////////////////////////////////////////////////////////////////////////

//
// Map a function for its side effect over a list.
//
template <class T>
void list_map(void f(T*), List<T> *l)
{
  for (l; l != NULL; l = l->tl())
    f(l->hd());
}

//
// Print the given list on the standard output.
// Requires that "<<" be defined for the element type.
//
template <class S, class T>
void list_print(S &str, List<T> *l)
{
   str << "[\n";
   for(; l != NULL; l = l->tl())
	str << *(l->hd()) << " ";
   str << "]\n";
}

//
// Compute the length of a list.
//
template <class T>
int list_length(List<T> *l)
{
  int i = 0;
  for (; l != NULL; l = l->tl())
    i++;
  return i;
}

#endif

