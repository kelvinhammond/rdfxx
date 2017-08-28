/*
 *     			rdf2dot.cpp
 *
 *     		Copyright 2012 - 2017 Brenton Ross
 *
 *
 *      This file is part of SASSY.
 *
 *   SASSY is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SASSY is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SASSY.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include "rdfxx/rdfxx.h"
#include "rdfxx/except.h"
#include <cfi/stringy.h>
#include <cfi/xini.h>
#include <vector>
#include <set>
#include <iostream>

using namespace std;
using namespace SASSY;
using namespace rdf;

// -------------------------------------------------------------------------

map< string, string > nodes;	// map id to label
map< string, string > ids;	// map labels to ids
vector< pair< string, string > > edges;
Model model;

// -------------------------------------------------------------------------

void
parseRDFfile( const Path &path )
{
	static int count = 1;
	World world = Universe::instance().world("rdf2dot");

	URI uri( string(path), world );
	Parser parser( world, "guess" );
	parser->parseIntoModel( model, uri, uri );

	Format format = { true, true, "blank", false, false, "en", false };
	Stream strm = model->toStream();
	while ( ! strm->end() )
	{
		Statement st( strm->current());
		Node subj( st->subject());
		Node pred( st->predicate());
		Node obj( st->object());

		string sid;
		if ( subj->isBlank() )
			sid = string("b") + std::to_string(count++);
		else
			sid = string("n") + std::to_string(count++);
		string pid = string("p") + std::to_string(count++);
		string oid; 
		if ( obj->isBlank())
			oid = string("b") + std::to_string(count++);
		else
			oid = string("n") + std::to_string(count++);

		string subjs( subj->toString(format));
		string preds( pred->toString(format));
		string objs( obj->toString(format));

		if ( ids[subjs].empty() ) ids[subjs] = sid;
		sid = ids[subjs];
		nodes[sid] = subjs;
		nodes[pid] = preds;
		if ( obj->isLiteral() )
		{
			nodes[oid] = objs;
		}
		else
		{
			if ( ids[objs].empty()) ids[objs] = oid;
			oid = ids[objs];
			nodes[oid] = objs;
		}

		edges.push_back( make_pair( sid, pid ));
		edges.push_back( make_pair( pid, oid ));

		strm->next();
	}
}

// -------------------------------------------------------------------------

void
makeDot()
{
	cout << "digraph {\n";
	cout << "\trankdir = LR;\n" << endl;

	for( auto &n : nodes )
	{
		switch ( n.first[0] )
		{
		case 'n':
			cout << "\t" << n.first << "[label=\"" << n.second << "\"]" << ";\n";
			break;
		case 'p':
			cout << "\t" << n.first << " [shape=box, label=\"" << n.second << "\"];\n";
			break;
		case 'b':
			cout << "\t" << n.first << " [label=\"  \"];\n";
			break;
		}
	}
	cout << "\n" << endl;

	for( auto &e : edges )
	{
		cout << "\t" << e.first << " -> " << e.second << ";\n";
	}
	cout << "\n" << endl;

	cout << "}\n" << endl;

}

// -------------------------------------------------------------------------

const string SASSY::cfi::XINI::config = "PECHD:"
			"-i:SASSY_CONF:"
			"sassy.xml:"
			".local/share/sassy/sassy.xml:"
			"$SASSY/projects/rdfxx/sassy.xml:"
			"$SASSY/share/sassy/sassy.xml:";

// -------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		cerr << "expected RDF file name" << endl;
		return EXIT_FAILURE;
	} 
	char *p = realpath(argv[1],NULL);
	if ( p == NULL )
	{
		cerr << "cannot find " << argv[1] << ": " << strerror(errno) << endl;
		return EXIT_FAILURE;
	}
	Path path(p);

	World world = Universe::instance().world("rdf2dot");
	world->prefixes().base( URI( string(path) + "#", world));
	world->prefixes().insert("w3c", URI(world,"http://www.w3.org/2000/03/example/classes#"));

	model = Model(world, "memory" );

	parseRDFfile( path );
	makeDot();

	return EXIT_SUCCESS;
}

// ---------------------------- end ----------------------------------------

