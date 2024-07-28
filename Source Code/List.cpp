#include <iostream>
#include "List.h"
#include "Student.h"

using namespace std;


List::List() {
	head = NULL;
	count = 0;
}


bool List::empty() {
	if (count==0) return true;
	return false;
}


int List::size() {
	return count;
}


Node *List::find(int position) {
	Node	*cur;
	
	if (position > count) return NULL;
	cur = head;
	for (int count=1; count<position; count++) 
		cur = cur->next;
	return cur;
}


bool List::get(int position, type &result) {
	if (position > count) return false;
	result = find(position)->item;
	return true;
}


bool List::set(int position, type newItem) {
	if (position > count) return false;
	find(position)->item = newItem;
	return true;
}


bool List::insert(int at, type newItem) {// Any simplification can be done on code below?
	Node	*pre, *cur, *tmp = new Node(newItem);

	if (at < 1 || at > count+1) return false;
	if (!tmp) return false;

	if (empty()) {
		head = tmp;
		count++;
		return true;
	}
	if (at == 1) {
		tmp->next = head;
		head = tmp;
		count++;
		return true;
	}
	pre = find(at-1);
	cur = pre->next;
	tmp->next = cur;
	pre->next = tmp;
	count++;
	return true;
}


bool List::remove(int from) {
	Node	*pre, *cur;

	if (from < 1 || from > count) return false;
	if (from == 1) {
		cur = head;
		head = head->next;
		count--;
		free(cur);
		return true;
	}
	pre = find(from-1);
	cur = pre->next;
	pre->next = cur->next;
	free(cur);
	count--;
	return true;
}

//insert Student according to name
bool List::insert(type newItem) {
	Node	*pre, *cur, *tmp;

	tmp = new Node(newItem);

	if (!tmp) return false;
	if (empty()) {
		head = tmp;
		count++;
		return true;
	}
	count++;
	if (head->item.compareName1(newItem)) {
		tmp->next = head;
		head = tmp;
		return true;
	}
	pre = head;
	cur = pre->next;
	for (; cur != NULL;) {
		if (cur->item.compareName1(newItem)) break;
		pre = cur;	cur = cur->next;
	}
	tmp->next = cur;
	pre->next = tmp;
	return true;
}
