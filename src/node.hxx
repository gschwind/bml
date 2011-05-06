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

#ifndef NODEX_HXX_
#define NODEX_HXX_

#include "exception.hxx"
#include <list>
#include <stdint.h>
#include <fstream>

namespace bml {

const size_t HEADER_MAX_SIZE = sizeof(char) + sizeof(int64_t)
		+ sizeof(uint64_t);

class node;
typedef std::list<node *> node_list;

class node {
	/* document is my friend ! */
	friend class document;
	/* store data length */
	int64_t _data_size;
	/* store data */
	char * _data;

	/* node content */
	mutable uint64_t _content_size;
	/* header size */
	mutable char _header_size;
	mutable char header_data[HEADER_MAX_SIZE];

	bool _is_invalid;

protected:

	/* read node from stream */
	node(std::fstream & f);

	/* compute size of current node in file
	 * this function avoid revert writing */
	void prepare_write() const;

	/* write node to stream */
	void write(std::fstream &f) const;

	/* do not allow copy
	 * use clone instead
	 */
	node(node const &);
	node & operator=(node const &);

public:
	/* store id */
	uint64_t id;
	/* child nodes */
	node_list child;

	/* create a node */
	node(int64_t id, uint64_t size = 0, char const * data = 0);

	/* destruct */
	virtual ~node();

	/* convenient template which allow
	 * recast.
	 */
	template<typename T>
	T& get() {
		if (this->_data)
			return *(reinterpret_cast<T*> (this->_data));
		else
			throw exception("no data for this node.");
	}

	/* find recusivly all node with same id */
	node_list get_nodes_by_id(int64_t id);

	void append_child(node * c);
	void remove_child(node * c);
	void replace_child(node * old, node * c);

	node * clone();
	void set_data(uint64_t size, char const * data);
	void read_header(std::fstream & f);

	void write_header(std::fstream & f) const;

	node & operator[] (int64_t id);
	char * operator*();

};

/* create node from know types */
template<typename T>
class tnode: public node {
public:
	virtual ~tnode() { };
	tnode(int64_t id, T & v) :
		node(id, sizeof(T), reinterpret_cast<char *> (&v)) {
	}
	tnode(int64_t id, T v) :
		node(id, sizeof(T), reinterpret_cast<char *> (&v)) {
	}
};

template<>
class tnode<std::string> : public node {
public:
	tnode(int64_t id, std::string & v) :
		node(id, v.size() + 1, v.c_str()) {
	}

	tnode(int64_t id, std::string v) :
		node(id, v.size() + 1, v.c_str()) {
	}
};

}

#endif /* NODEX_HXX_ */
