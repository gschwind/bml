/*
 * node.cxx
 *
 *  Created on: 30 avr. 2011
 *      Author: gschwind
 */

#include "node.hxx"
#include <cstring>

namespace bml {

struct node_header {
	int64_t id;
	int64_t child_offset;
	int64_t next_offset;
	uint64_t data_length;
}__attribute__((packed));

node::node(std::fstream & f) {
	/* f is at top of header */
	node_header h;
	f.read(reinterpret_cast<char *>(&h), sizeof(node_header));
	id = h.id;
	data_size = h.data_length;
	data = new char[data_size];
	f.read(data, data_size);
	/* back to the begin of head */
	f.seekg(0 - data_size - sizeof(node_header), std::ios::cur);

	/* go to the child */
	f.seekg(h.child_offset, std::ios::cur);
	child = new node(f);
	/* back to the header */
	f.seekg(0 - h.child_offset, std::ios::cur);
	/* go to the next */
	f.seekg(h.next_offset, std::ios::cur);
	parent_child_next = new node(f);
	/* back to the head */
	f.seekg(0 - h.next_offset);
}

int64_t node::update_write_size() const {
	if (child)
		child_size = child->update_write_size();
	else
		child_size = 0;
	if (next_size)
		next_size = parent_child_next->update_write_size();
	else
		next_size = 0;

	return data_size + sizeof(node_header) + child_size + next_size;
}

void node::write(std::fstream &f) const {
	node_header h;
	h.id = id;
	h.data_length = data_size;
	h.child_offset = data_size + sizeof(node_header);
	h.next_offset = data_size + sizeof(node_header) + child_size;
	f.write(reinterpret_cast<char *>(&h), sizeof(node_header));
	if (data_size > 0)
		f.write(data, data_size);
	if (child)
		child->write(f);
	if (parent_child_next)
		parent_child_next->write(f);
}

node::node(int64_t id, uint64_t length, char const * data) {
	this->data = 0;
	set_data(length, data);
	this->id = id;
	child = 0;
}

node::~node() {
	if(data)
		delete data;
	data = 0;
}

node_list node::get_nodes_by_id(int64_t id) {
	node_list l;
	if (child) {
		node * cur = child;
		while (cur) {
			if (cur->id == id) {
				l.push_back(cur);
			}
			node_list subl = cur->get_nodes_by_id(id);
			l.splice(l.end(), subl);
			cur = cur->parent_child_next;
		}
	}
	return l;
}

node_list node::get_childs() {
	node_list l;
	node * cur = child;
	while (cur) {
		l.push_back(cur);
		cur = cur->parent_child_next;
	}
	return l;
}

void node::append_child(node * c) {
	c->parent_child_next = 0;
	if (child) {
		node * cur = child;
		while (cur->parent_child_next) {
			cur = cur->parent_child_next;
		}
		cur->parent_child_next = c;
	} else {
		child = c;
	}
}

void node::remove_child(node * c) {
	node * cur = child;
	while (cur) {
		if (cur->parent_child_next == c) {
			cur->parent_child_next = c->parent_child_next;
		}
	}
}

void node::replace_child(node * old, node * c) {
	node * cur = child;
	while (cur) {
		if (cur->parent_child_next == old) {
			c->parent_child_next = old->parent_child_next;
			cur->parent_child_next = c;
		}
	}
}

node * node::clone() {
	node * result = new node(id, data_size, data);
	if (child)
		result->child = child->clone();
	else
		child = 0;
	result->parent_child_next = 0;
	return result;
}

void node::set_data(uint64_t size, char const * data) {
	if (this->data)
		delete[] this->data;
	data_size = size;
	if (size) {
		this->data = new char[size];
		memcpy(this->data, data, size);
	} else {
		this->data = 0;
	}
}

}
