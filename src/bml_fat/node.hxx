/*
 * node.hxx
 *
 *  Created on: 30 avr. 2011
 *      Author: gschwind
 */

#ifndef NODE_HXX_
#define NODE_HXX_

#include <stdint.h>
#include <list>
#include <fstream>
#include "exception.hxx"

namespace bml {

typedef std::list<node *> node_list;

class node {
	/* document is my friend ! */
	friend class document;

	/* hold nodes childs */
	node * child;
	/* hold the parent childs */
	node * parent_child_next;
	/* store data length */
	int64_t data_size;
	/* store data */
	char * data;

	/* used on write */
	mutable int64_t child_size;
	mutable int64_t next_size;

protected:

	/* read node from stream */
	node(std::fstream & f);


	/* compute size of current node in file
	 * this function avoid revert writing */
	int64_t update_write_size() const;

	/* write node to stream */
	void write(std::fstream &f) const;

	/* do not allow copy
	 * use clone instead
	 */
	node(node const &);
	node & operator=(node const &);

public:
	/* store id */
	int64_t id;

	/* create a node */
	node(int64_t id, uint64_t size = 0, char const * data = 0);

	/* destruct */
	~node();

	/* convenient template which allow
	 * recast.
	 */
	template<typename T>
	T& get() {
		if (this->data)
			return *(reinterpret_cast<T*>(this->data));
		else
			throw exception("no data for this node.");
	}

	/* find recusivly all node with same id */
	node_list get_nodes_by_id(int64_t id);

	/* get childs */
	node_list get_childs();

	void append_child(node * c);
	void remove_child(node * c);
	void replace_child(node * old, node * c);
	node * clone();
	void set_data(uint64_t size, char const * data);
};

/* create node from know types */
template<typename T>
class tnode: public node {
public:
	tnode(int64_t id, T& v) :
		node(id, sizeof(T), &v) {
	}

	tnode(int64_t id, T v) :
		node(id, sizeof(T), reinterpret_cast<char *>(&v)) {
	}
};

template<>
class tnode<std::string> : public node {
public:
	tnode(int64_t id, std::string & v) :
		node(id) {
		set_data(v.size() + 1, v.c_str());
	}
};

}

#endif /* NODE_HXX_ */
