/**
 * @(#) Project.h
 */

#ifndef PROJECT_H_H
#define PROJECT_H_H
#include <string>
#include <fstream>
#include "FileSys.h"
#include "MXML.h"
using namespace std;

class Project
{	
public:
    typedef enum {IMAGE,OBJECT, MAP, SOUND, SHADER, MESH, BOX, FILETYPE_END}FileType;
    /**
     * Constructor
     * @param nome
     */
    Project(string dir);
    static bool createProject( string name, string dir, string vers, string desc);
    /**
     * Returns the working directory of the project
     * @return a string to the project
     */
    string getDir();
    /**
     * Returns the location of a file of a certain kind
     * @param file The file we are asking for
     * @param type the type of the file, FileType
     * @return a string to the file
     */
    string getDir(string file, FileType type);
    string getDir(string file);
    /**
     * Returns the name of the current project
     * @return the name of the project
     */
    string getName();
    /**
     * Returns the version of the project
     * @return version of the project
     */
    string getVers();
    /**
     * Returns the description of the project
     * @return the description
     */
    string getDesc();
    /**
     * Reads the project contents
     */
    void read( );
    /**
     * Returns the type of a file by its extension
     */
    static FileType getType(string dir);
    void setRemoteProjs(vector<string> p);
    vector<string> listFiles(FileType file);
    static bool isProject(string dir);
    static string toString(FileType type);
    
private:
    string dir;
    string name;
    string desc;
    string vers;
    vector<string> remoteProjs;
};

#endif
