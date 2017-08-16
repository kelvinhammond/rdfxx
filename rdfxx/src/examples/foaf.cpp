/* RDF C++ API 
 * -----------------------------------------------------------------------------
 * LICENSE
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * http://www.gnu.org
 * -----------------------------------------------------------------------------
 * AUTHOR
 *
 * Sebastian Faubel <sfaubel@users.sf.net>
 */

#include <rdfxx/rdfxx.h>
#include <iostream>

using namespace std;
using namespace rdf;

int main(void)
{
	bool rc = true;
    try
    {
        // Create and open RDF datastore.
        // cout << "Opening BDB store in /tmp/.." << endl;
        // Storage storage("hashes", "practrdf", "new='yes',hash-type='bdb',dir='/tmp/'");

        // Create a new model to add statements to.
        cout << "Creating new model.." << endl;
        // Model model = Model_::make(storage);
	Model model( "hashes", "practrdf", "new='yes',hash-type='bdb',dir='/tmp/'");
	cout << "created model" << endl;

        // Prepare parsing a RDF/XML file.
        Parser parser("raptor", "application/rdf+xml");
        URI file_uri("file:foaf.rdf");
	cout << "created parser" << endl;

        Node n1("http://organise.org/ofw/0.4/categories/documents");
        Node n2("http://purl.org/dc/0.1/title");
        Node n3("Dokumente", false);
        Node n4("Documents", false);
	cout << "created nodes" << endl;
        
        Statement s1(n1,n2,n3);
        Statement s2(n1,n2,n4);
	cout << "Created statements" << endl;
        rc = model->add(s1);
	if ( ! rc )
	{
		cerr << "Failed to add statement" << endl;
		return EXIT_FAILURE;
	}
	cout << "Added statement" << endl;
        rc = model->update(s1, s2);
	if ( ! rc )
	{
		cerr << "Failed to update statement" << endl;
		return EXIT_FAILURE;
	}
	cout << "Updated statement" << endl;

        // Parse the file.
        URI base_uri;
        unsigned int count = 0;
        Stream stream(parser, file_uri, base_uri);
	cout << "Created stream" << endl;
        while(!stream->end())
        {
             model->add(stream->current());
             cout << ++count << ": " << stream->current()->toString() << endl;
             stream->next();
        }
        cout << "Processed " << count << " statements." << endl;

        // Serialize as RDF/XML.
        Serializer serializer;
	URI ns_uri("http://xmlns.com/foaf/0.1/");
	serializer->setNamespace( ns_uri, "foaf" );
        serializer->toFile("foaf.rdf.new", model);

        // Prepare query string.
        QueryString qs;
        qs.addPrefix("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
        qs.addPrefix("foaf", "http://xmlns.com/foaf/0.1/");
        qs.setVariables("DISTINCT ?name ?homepage");
        qs.addCondition("?x rdf:type foaf:Person");
        qs.addCondition("?x foaf:name ?name");
        qs.addCondition("?x foaf:homepage ?homepage");
	cout << "Created query string:" << endl;
	cout << (string)qs << endl;

        // Setup query.
        Query query(qs);
        query->setLimit(5);
	cout << "Created query" << endl;

        // Execute query and get results.
        QueryResults results = query->execute( model );
	cout << "Executed query" << endl;

        // Display query results.
	for( auto &x : *results )
	{
		for ( int i=0; i<x.count(); i++ )
            	{
                	cout << x.getBoundName(i) << ": ";
                	cout << x.getBoundValue(i)->toString() << endl;
            	}
	}

    }
    catch(vx & e )
    {
    	cerr << e.what() << endl;
    }
    catch(...)
    {
	cerr << "Unknown exception" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
