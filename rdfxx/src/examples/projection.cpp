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

#define PREFIX_PP "http://www.organise-fw.org/pp/1.0/terms#"
#define PREFIX_NFO "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#"
#define PREFIX_RDF "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define PREFIX_RDFS "http://www.w3.org/2000/01/rdf-schema#"

#include <stack>
#include <algorithm>
#include <string.h>
#include <iostream>

#include <rdfxx/rdfxx.h>

using namespace std;
using namespace rdf;

class FolderProjection
{
private:

	string base;
	string path;

public:

	FolderProjection()
	{
		setBase(getenv("HOME"));
	}

	string getBase() { return base; }
	void setBase(string _base)
	{
		if(_base[_base.length() - 1] != '/')
			_base += '/';

		base = _base;
	}

	string getPath() { return path; }
	void setPath(string _path) { path = _path; }
	void appendToPath(string _path) { path += _path; }
};

class FileProjection
{
private:

	string uri;
	string name;
	string extension;

public:

	FileProjection(string _uri)
	{
		setUri(_uri);
	}

	string getUri() { return uri; }
	void setUri(string _uri)
	{
		uri = _uri;

		// Calculate the file extension..
		unsigned int pos = uri.rfind('.');
		extension = (pos != string::npos) ? uri.substr(pos) : "";
	}

	string getName() { return name; }
	void setName(string _name) { name = _name + getExtension(); }

	string getExtension()
	{
		return extension;
	}
};

class PathProjection
{
private:

	// MemoryStore model;
	// Storage store;
	// Model model = Model_::make(memStore);

	World world;
	Model model;

	// Possible path transformations.
	bool transformUpperCase;
	bool transformLowerCase;
	bool substituteWhitespace;
	bool removeWhitespace;

	// There is only one file Projection, hence the file name is global.
	FileProjection fileProjection;
	// There may be several path projections.
	stack<FolderProjection> folderProjections;

	void traverseClassHierarchy(string& _class)
	{
		/* NOTE: Filter isURI(?parentClass) is required since some OWL classes 
         * return blank nodes. */
		QueryString qs0;
		qs0.addPrefix("rdfs", PREFIX_RDFS);
		qs0.addPrefix("pp", PREFIX_PP);
		qs0.setVariables("?parentClass ?classType");
		qs0.addCondition(string("<") + _class + "> rdfs:subClassOf ?parentClass");
		qs0.addCondition(string("<") + _class + "> a ?classType");
		qs0.addCondition("FILTER isURI(?parentClass)");

		Query q0(world, qs0);
		QueryResults qr0 = q0->execute( model );

		/* The end of a classification hierarchy corresponds to one directory 
         * tree in the file system. Subsequently the following loop can access the
         * new path from the top of the stack when the recursion returns. */
		if(! qr0->success())
		{
			folderProjections.push(FolderProjection());
			return;
		}

		// while(qr0->success())
		for( auto &qi0 : *qr0 )
		{
			string classUri = qi0.getBoundValue("parentClass")->toURI()->toString();

			// Recursively traverse the classification hierarchy.
			traverseClassHierarchy(classUri);
			// Print out the classification hierarchy from bottom to top.
			cout << _class << ": ";

			// Compute the path projection of the current class.
			// NOTE: Error handling for empty projection strings.
			computeClassProjection(_class, folderProjections.top()); 

			cout << endl;

			// qr0->next();
		}
	}

	void computeClassProjection(string& _class, FolderProjection& _projection)
	{
		QueryString qs0;
		qs0.addPrefix("nfo", PREFIX_NFO);
		qs0.addPrefix("pp",PREFIX_PP);
		qs0.setVariables("?node ?containerType ?baseFolder");
		qs0.addCondition(string("?projection pp:projectionFor <") + _class + ">");
		qs0.addCondition("?node nfo:belongsToContainer ?projection");
		qs0.addCondition("?node a ?containerType");
		qs0.addCondition("OPTIONAL { ?projection pp:projectionBaseFolder ?baseFolder }");

    	Query q0(world, qs0);
    	QueryResults qr0 = q0->execute( model );

		// Multiple projections may occur for different projection types (Folder/File).
		// while(qr0->success())
	// cout << "QueryResults: " << qr0->toString() << endl;
	// return;
	for ( auto &qi0 : *qr0 )
    	{
			string node = qi0.getBoundValue("node")->toURI()->toString();
			string containerType = qi0.getBoundValue("containerType")->toURI()->toString();

			try { _projection.setBase(qi0.getBoundValue("baseFolder")->toString()); }
			catch(...) {}

			string nodeProjection = computeNodeProjection(node, containerType);

			if(containerType == PREFIX_NFO"Folder")
			{
        		_projection.appendToPath(nodeProjection);
				cout << nodeProjection;
			}
			else if(containerType == PREFIX_NFO"FileDataObject")
				fileProjection.setName(nodeProjection);

        	// qr0->next();
    	}

		return;
	}

	string computeNodeProjection(string& _node, string& _containerType)
	{
		string path;

		/* Compute the current node name */
		QueryString qs0;
		qs0.addPrefix("rdf", PREFIX_RDF);
		qs0.addPrefix("pp", PREFIX_PP);
		qs0.setVariables("?nameComponent");
		qs0.addCondition(string("<") + _node + "> pp:nodeName [ ?li ?nameComponent ]");
		qs0.addCondition("FILTER( ?li != rdf:type )");
			
		Query q0(world, qs0);
		QueryResults qr0 = q0->execute( model );

		// while(qr0->success())
		for( auto &qi0 : *qr0 )
		{
			Node nameComponent = qi0.getBoundValue("nameComponent");

			if(nameComponent->isLiteral())
			{
				path += nameComponent->toString();
			}
			else if(nameComponent->isResource())
			{
				QueryString qs1;
				qs1.setVariables("?propertyValue");
				qs1.addCondition(string("<") + fileProjection.getUri()
					+ "> <" + nameComponent->toURI()->toString() 
					+ "> ?propertyValue");

				Query q1(world, qs1);
				QueryResults qr1 = q1->execute( model );

				// NOTE: Improve error handling.
				// while(qr1->success())
				for( auto &qi1 : *qr1 )
				{
					path += qi1.getBoundValue("propertyValue")->toString();
					// qr1->next();
				}
			}

			// qr0->next();
		}

		if(_containerType == PREFIX_NFO"Folder")
		{
			path += '/';

			/* Query sub folderProjections */
			QueryString qs2;
			qs2.addPrefix("nfo", PREFIX_NFO);
			qs2.setVariables("?node");
			qs2.addCondition(string("?node nfo:belongsToContainer <") +  _node + ">");
			qs2.addCondition(string("?node a <") + _containerType + ">");

			Query q2(world, qs2);
			QueryResults qr2 = q2->execute( model );

			// while(qr2->success())
			for( auto &qi2 : *qr2 )
			{
				string node = qi2.getBoundValue("node")->toURI()->toString();
				path += computeNodeProjection(node, _containerType);
				// qr2->next();
			}
		}

		return path;
	}

protected:

	void parseRdfFile(URI _file)
	{
		Parser p(world, "rdfxml");
		p->parseIntoModel(model, _file, _file);
	}

	void computePathProjection()
	{
		QueryString qs0;
		qs0.addPrefix("nfo", PREFIX_NFO);
		qs0.setVariables("?fileType");
		qs0.addCondition(string("<") + fileProjection.getUri() + "> a ?fileType");
		qs0.addCondition("FILTER (?fileType != <" PREFIX_NFO "FileDataObject>)");

		Query q0(world, qs0);
		QueryResults qr0 = q0->execute( model );

		cout << "--- CLASS HIERARCHIES" << endl;

		// while(qr0->success())
		for( auto &qi0 : *qr0 )
		{
			string fileType = qi0.getBoundValue("fileType")->toURI()->toString();
			traverseClassHierarchy(fileType);
			// qr0->next();
		}
	}

public:

	PathProjection(string _uri) :
		world(Universe::instance().world("Projection")),
		model(world, "memory"),
		fileProjection(_uri),
		transformLowerCase(false),
		transformUpperCase(false),
		substituteWhitespace(false),
		removeWhitespace(false)
	{
		parseRdfFile( URI(world, "file:../../src/examples/model_projections.rdf"));
		parseRdfFile( URI(world, "file:../../src/examples/model_files.rdf"));
		parseRdfFile( URI(world, "file:../../src/examples/ontology_nfo.rdf"));
		parseRdfFile( URI(world, "file:../../src/examples/ontology_pub.rdf"));

		computePathProjection();
	}

	void setLowerCaseTransformation(bool _value=true)
	{
		transformLowerCase = _value;
	}

	void setUpperCaseTransformation(bool _value=true)
	{
		transformUpperCase = _value;
	}

	void setWhitespaceSubstitution(bool _value=true)
	{
		substituteWhitespace = _value;
	} 

	void setWhitespaceRemoval(bool _value=true)
	{
		removeWhitespace = _value;
	} 

	void printPathProjections()
	{
		cout << endl << "--- PATH PROJECTIONS" << endl;

		while(!folderProjections.empty())
		{
			string path;
			path += folderProjections.top().getBase();
			path += folderProjections.top().getPath();
			path += fileProjection.getName();

			if(transformLowerCase)
				transform(path.begin(), path.end(), path.begin(), ::tolower);

			if(transformUpperCase)
				transform(path.begin(), path.end(), path.begin(), ::toupper);

			if(removeWhitespace)
				replace(path.begin(), path.end(), ' ', '\0');

			if(substituteWhitespace)
				replace(path.begin(), path.end(), ' ', '_');

			folderProjections.pop();

			cout << path << endl;
		}
	}
};

int main(int _argc, char** _argv)
{
    try
    {
    	std::vector< std::string > parsers;
    	Parser_::listParsers( Universe::instance().world("Projection"), parsers );
	for( auto &x : parsers )
	{
		cout << x << endl;
	}

		PathProjection pp("file:///home/root66/documents/organise-fw/diplomarbeit_vorschlag.odt");

		for(int i=0; i < _argc; i++)
		{
			if(strcmp(_argv[i], "-l") == 0)
				pp.setLowerCaseTransformation();

			if(strcmp(_argv[i], "-u") == 0)
				pp.setUpperCaseTransformation();

			if(strcmp(_argv[i], "-rw") == 0)
				pp.setWhitespaceRemoval(); 

			if(strcmp(_argv[i], "-sw") == 0)
				pp.setWhitespaceSubstitution(); 
		}

		pp.printPathProjections();
  
  }
    catch( vx & e )
    {
    	cerr << "\n" << e.what() << endl;
    }
    catch( std::exception & e )
    {
    	cerr << "\n Exception: " << e.what() << endl;
    }
    catch(...)
    {
		cerr << "\nException caught!" << endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
