/**
 * Example code related to 2341 final project for summer 2022
 */
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <dirent.h>
#include <sys/stat.h>

//RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

#include "BinarySearchTree.h"

using namespace rapidjson;
using std::cout;
using std::endl;
using std::cin;
using std::ifstream;
using std::setw;
using std::left;

//Function Prototypes
void testFileSystem(const char* path);
void testCStyleFileSystem(const char* path);
void testReadJsonFile(const char* fileName);


int main() {

//    cout << "-------------------------------------------" << endl;
//    cout << "------ RapidJSON Doc Parsing Example ------" << endl;
//    cout << "-------------------------------------------" << endl;
//    testReadJsonFile("sample_data/news_0064567.json");
//
//    cout << "\n";
//    cout << "-------------------------------------------" << endl;
//    cout << "------     File System Example       ------" << endl;
//    cout << "-------------------------------------------" << endl;
//    testFileSystem("sample_data/");

    cout << "\n";
    cout << "-------------------------------------------" << endl;
    cout << "------  c-style File System Example  ------" << endl;
    cout << "-------------------------------------------" << endl;
    testCStyleFileSystem("sample_data");

//    cout << "\n";
//    cout << "-------------------------------------------" << endl;
//    cout << "------      Binary Search Tree       ------" << endl;
//    cout << "-------------------------------------------" << endl;
//    BinarySearchTree<int, int> bst;
//    bst.insert(3, 10);
//    bst.insert(5, 20);
//    bst.insert(1, 30);
//    bst.insert(8, 80);
//    bst.insert(10, 90);
//    bst.insert(4, 40);
//    bst.print();

    return 0;
}


/**
 * example code that reads and parses a json file and extracts the title and person
 * entities.
 * @param fileName filename with relative or absolute path included.
 */
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

    //This accesses the -title- element in the JSON. Since the value
    // associated with title is a string (rather than
    // an array or something else), we call the GetString()
    // function to return the actual title of the article
    // as a c-string.
    auto val = d["title"].GetString();
    cout << "Title: " << val << endl;

    //The Persons entity for which you're building a specific
    // inverted index is contained in top level -entities- element.
    // So that's why we subscript with ["entities"]["persons"].
    // The value associated with entities>persons is an array.
    // So we call GetArray() to get an iterable collection of elements
    auto persons = d["entities"]["persons"].GetArray();

    //We iterate over the Array returned from the line above.
    // Each element kind of operates like a little JSON document
    // object in that you can use the same subscript notation
    // to access particular values.
    cout << "  Person Entities:" << endl;
    for (auto& p : persons) {
        cout << "    > " << setw(30) << left << p["name"].GetString()
             << setw(10) << left << p["sentiment"].GetString() << endl;
    }

    input.close();
}


/**
 * example code for how to traverse the filesystem using std::filesystem
 * @param path an absolute or relative path to a folder containing files
 * you want to parse.
 */
void testFileSystem(const char* path) {

    //recursive_director_iterator used to "access" folder at parameter -path-
    //we are using the recursive iterator so it will go into subfolders.
    auto it = std::filesystem::recursive_directory_iterator(path);

    //loop over all the entries.
    for(const auto& entry : it) {

        cout << "--- " << setw(60) << left << entry.path().c_str() << " ---" << endl;

        //We only want to attempt to parse files that end with .json...
        if (entry.is_regular_file() && entry.path().extension().string() == ".json") {
            testReadJsonFile(entry.path().c_str());
        }

    }
}

/**
 * example code for how to traverse the filesystem using c-style DIRENT code
 * @param path an absolute or relative path to a folder containing files
 * you want to parse.
 */
void testCStyleFileSystem(const char* path) {

    //set up some structures to hold info
    DIR* directory;
    struct dirent* dir_entry;
    struct stat file_stat{};

    //temporary buffer to hold directory name + file name;
    char buffer[2048];

    //open the directory/folder based on the parameter
    directory = opendir(path);

    //check if opening directory was successful
    if(directory == nullptr) {
        cout << "Error opening " << path << "!!!" << endl;
        return;
    }

    while ((dir_entry = readdir(directory)) != nullptr) {

        strcpy(buffer, path);
        strcat(buffer, "/");
        strcat(buffer, dir_entry->d_name);
        //for debugging
        cout << "Processing " << buffer << endl;

        //we want to ignore the . and .. directories.
        if(dir_entry->d_name[0] == '.') {
            cout << "Ignoring . or .." << endl;
            continue;
        }

        //retrieve some information (stats) about the current file or folder
        auto retval = stat(buffer, &file_stat);
        if(retval != 0)
            cout << "Houston, we have a problem..." << endl;

        //using the information retrieved above, check to see if the current
        // thing is a folder/directory using (S_ISDIR(...) function).
        // If it is, we want to recursively descend into that folder using this
        // same function (testCStyleFileSystem(...)).
        if(S_ISDIR(file_stat.st_mode)){
            cout << "   Found a directory" << endl;
            testCStyleFileSystem(buffer);
        } else {

            //otherwise, we parse the JSON in the file.
            testReadJsonFile(buffer); //process file here.
        }
    }

    //close the directory pointer at the end.
    closedir(directory);
}