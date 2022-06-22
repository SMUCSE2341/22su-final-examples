#include <iostream>
#include <filesystem>
#include <fstream>
#include <iomanip>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using std::cout;
using std::endl;
using std::cin;
using std::ifstream;
using std::setw;
using std::left;

void testReadJsonFile(const char* fileName) {

    //open an ifstream on the file of interest and check that it could be opened.
    ifstream input(fileName);
    if(!input.is_open())
        std::cerr << "cannot open file" << endl;

    //Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    //Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    //Now that the document is parsed, we can access different elements the JSON using
    //familiar subscript notation.

    //This accesses the -title- element in the JSON. Since the value associated with title is a string (rather than
    // an array or something else), we call the GetString() function to return the actual title of the article
    // as a c-string.
    //
    auto val = d["title"].GetString();
    cout << "Title: " << val << endl;

    //The Persons entity for which you're building a specific inverted index is contained in
    // top level -entities- element.  So that's why we subscript with ["entities"]["persons"].
    // The value associated with entities>persons is an array.  So we call GetArray() to get
    // an iterable collection of elements
    auto persons = d["entities"]["persons"].GetArray();

    //We iterate over the Array returned from the line above.  Each element kind of operates like
    // a little JSON document object in that you can use the same subscript notation
    // to access particular values.
    for (auto& p : persons) {
        cout << "  >" << setw(30) << left << p["name"].GetString()
                      << setw(10) << left << p["sentiment"].GetString() << endl;
    }

    input.close();
}


int main() {

    testReadJsonFile("sample_data/news_0064567.json");
    return 0;
}
