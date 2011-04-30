/*
 * document.cxx
 *
 *  Created on: 30 avr. 2011
 *      Author: gschwind
 */

#include "exception.hxx"
#include "document.hxx"
#include <fstream>

namespace bml {

document::document(char const * filename) : node((uint64_t)-1, 0, 0) {
	std::fstream f;
	char head[4];
	f.open(filename, std::fstream::in | std::fstream::binary);
	f.read(head, 4);
	if (head[0] != 'B' || head[1] != 'M' || head[2] != 'L' || head[3] != 1) {
		throw exception("bml signature not found");
	}
	new (this) node(f);
	f.close();
	if(id != -1)
		throw exception("bad bml format");
}

document::document() : node((uint64_t)-1, 0, 0) {

}

void document::write(char const * filename) const {
	std::fstream f;
	f.open(filename, std::fstream::out | std::fstream::binary);
	char const head[] = { 'B', 'M', 'L', 1 };
	f.write(head, sizeof(head));
	update_write_size();
	node::write(f);
	f.close();
}

}
