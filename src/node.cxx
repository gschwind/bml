/*
 * Copyright 2010 Benoit Gschwind <gschwind@gnu-log.net>
 *
 * This file is part of libbml.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "node.hxx"
#include <cstring>
#include <iostream>

namespace bml {

node::node(std::fstream & f) {

	/* read the node header */
	read_header(f);
	if (_is_invalid)
		return;

	/* read data */
	if (_data_size > 0) {
		_data = new char[_data_size];
		f.read(_data, _data_size);
	} else {
		_data = 0;
	}

	/* read children node */
	for (;;) {
		node * n = new node(f);
		if (n->_is_invalid) {
			delete n;
			return;
		}
		child.push_back(n);
	}
}

void node::prepare_write() const {
	_content_size = 0;
	node_list::const_iterator i = child.begin();
	while (i != child.end()) {
		(*i)->prepare_write();
		_content_size += (*i)->_content_size + (*i)->_header_size;
		++i;
	}
	_content_size += _data_size + 1;
	prepare_header();
}

void node::write(std::fstream & f) const {
	/* write data */
	int k = 0;
	unsigned char id_length = get_length((char const *)&id);
	unsigned char s_length = get_length((char const *)&_data_size);
	char x = ((id_length << 4) & 0xf0) | (s_length & 0x0f);
	f.write((char *)&x, 1);
	char * data;

	f.write((char*) &id, id_length);
	f.write((char*) &_data_size, s_length);

	/* write data */
	if (_data_size > 0)
		f.write(_data, _data_size);

	/* write children node */
	node_list::const_iterator i = child.begin();
	while (i != child.end()) {
		(*i)->write(f);
		++i;
	}

	/* write the last node */
	char invalid_node = 0;
	f.write(&invalid_node, 1);

}

node::node(int64_t id, uint64_t length, char const * data) {
	this->_data = 0;
	set_data(length, data);
	this->id = id;
	std::cout << "created node: " << this->id << " " << this->_data_size << " "
			<< (void*) this->_data << std::endl;
}

node::~node() {
	if (_data != 0)
		delete _data;
	_data = 0;
}

node_list node::get_nodes_by_id(int64_t id) {
	node_list l;
	node_list::iterator i = child.begin();
	while (i != child.end()) {
		if ((*i)->id == id) {
			l.push_back((*i));
		}
		node_list subl = (*i)->get_nodes_by_id(id);
		l.splice(l.end(), subl);
		++i;
	}
	return l;
}

void node::append_child(node * c) {
	child.push_back(c);
}

void node::remove_child(node * c) {
	child.remove(c);
}

void node::replace_child(node * old, node * c) {
	node_list::iterator i = child.begin();
	while (i != child.end()) {
		if (*i == old) {
			child.insert(i, c);
			child.remove(c);
			break;
		}
		++i;
	}
}

node * node::clone() {
	node * result = new node(id, _data_size, _data);
	result->child = child;
	return result;
}

void node::set_data(uint64_t size, char const * data) {
	if (this->_data)
		delete[] this->_data;
	_data_size = size;
	if (size > 0) {
		this->_data = new char[size];
		memcpy(this->_data, data, size);
	} else {
		this->_data = 0;
	}
}

void node::read_header(std::fstream & f) {
	id = 0;
	_data_size = 0;

	unsigned char x;
	f.read((char*)&x, sizeof(char));
	unsigned char id_length = (x >> 4) & 0x0F;
	unsigned char size_length = (x) & 0x0F;

	if (id_length == 0 || id_length == 0) {
		_header_size = 1;
		_is_invalid = true;
		return;
	}

	_is_invalid = false;

	/* read the id */
	f.read((char*) &id, id_length);

	/* read the content size */
	f.read((char*) &_data_size, size_length);

}

inline char node::get_length(char const * v) const {
	unsigned char z = 7;
	while (z != 0 && v[z] == 0)
		--z;
	return z + 1;
}

char node::prepare_header() const {
	int k = 0;
	unsigned char id_length = get_length((char const *)&id);
	unsigned char s_length = get_length((char const *)&_data_size);
	char x = ((id_length << 4) & 0xF0) | (s_length & 0x0F);
	header_data[k++] = x;
	char * data;
	data = (char*) &id;
	for (int i = 0; i < id_length; ++i) {
		header_data[k++] = data[i];
	}
	data = (char*) &_data_size;
	for (int i = 0; i < s_length; ++i) {
		header_data[k++] = data[i];
	}
	_header_size = k;
}

void node::write_header(std::fstream & f) const {
	f.write(header_data, _header_size);
}

node & node::operator[](int64_t id) {
	node_list::iterator i = child.begin();
	while (i != child.end()) {
		if ((*i)->id == id)
			return *(*i);
		++i;
	}

	throw exception("ID not found");

}

char * node::operator*() {
	return _data;
}

}
