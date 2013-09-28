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


typedef enum {PHYSICAL=0,FONT,IMAGE,OBJECT, MAP, SOUND, SKELETON, SHADER, MESH, BOX}FileType;

class Project
{	
public:
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
     * Returns wether the project has to be portable to mobile devices or not
     * @return 
     */
    bool getPort();
    /**
     * Lists all avaliable projects
     * @return a vector of avaliable projects
     */
    static vector<string> listProjs();
    /**
     * Constructor
     * @param nome
     */
    Project( string nome);
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
private:
    string dir;
    string name;
    string desc;
    string vers;
    bool portable;
    string toString(FileType type);
    vector<string> remoteProjs;
};

#endif
