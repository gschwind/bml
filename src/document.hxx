/*
 * document.hxx
 *
 *  Created on: 30 avr. 2011
 *      Author: gschwind
 */

#ifndef DOCUMENT_HXX_
#define DOCUMENT_HXX_

#include "node.hxx"

namespace bml {

class document : public node {

public:
	/* create document from file name */
	document(char const * filename);
	/* create an empty document */
	document();
	/* write the document to a new file */
	void write(char const * filename) const;

};

}

#endif /* DOCUMENT_HXX_ */
