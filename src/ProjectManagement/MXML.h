/* 
 * File:   xml.h
 * Author: jsmtux
 *
 * Created on September 5, 2012, 11:58 AM
 */

#ifndef MXML_H
#define	MXML_H

#include <string>
#include <vector>
#include <map>
#include <exception>
#include <cstring>
#include <mutex>
using namespace std;

namespace MXML{
    
    typedef enum {NODE, INT, FLOAT, STRING}tData;
    
    class Attribute{
    public:
        Attribute();
        Attribute(float f);
        Attribute(int i);
        Attribute(string &s);
        Attribute(const char *s);
        ~Attribute();
        tData getType();
        float getFloat();
        int getInt();
        string getString();
    private:
        union{
            int i;
            float f;
            const char* s;
        }val;
        int *count;
        tData type;
    };
    
    class Tag{
    public:
        Tag(string n="");
        void setAttrib(Attribute a);
        void addChildren(Tag t);
        string getName();
        vector<Tag> &getChildren();
        Tag& operator[] (string name);
        Attribute &getAttrib();
        bool contains(string s);
        string  print(int l=0);
        void clear();
        bool isEmpty();
    private:
        string name;
        Attribute attrib;
        vector<Tag> children;
    };
    
    typedef enum{BEGIN, END, NONE}tToken;
    
    class MXMLFile{
    public:
        /**
         * Constructor used for writing files
         * @param d d is the name of the dtd
         * @param r is the root tag
         */
        MXMLFile(Tag &r, string d="");
        /**
         * constructor used for reading files
         */
        MXMLFile();
        /**
         * Performs the reading of the file
         */
        virtual void read()=0;
        /**
         * Writes the file
         */
        virtual void write()=0;  
        /**
         * Returns the main tag
         * @return main tag
         */
        Tag* getRoot(); 
    protected:
        Tag &root;
        string dir, dtd;        
    };
    
    class XMLFile:public MXMLFile{
    public:
        XMLFile(string file, Tag &r, string d=NULL);
        void read();
        void write();
    private:
        tToken getTokenType(string tok);
        string dir;
    };
    
    class BINType{
    public:
        virtual int read(char* r, streamsize size)=0;
        virtual void write(const char* w, streamsize size)=0;
        void writeString(string s);
        string readString();
    };
    
    class MEM:public BINType{
    public:
        MEM(char* f, streamsize max);
        int read(char* r, streamsize size);
        void write(const char* w, streamsize size);
        streamsize getSize();
    private:
        char* file;
        streamsize maxSize, currPos;
    };
    
    class Stream:public BINType{
    public:
        Stream(std::ifstream *f);
        Stream(std::ofstream *f);
        int read(char* d, streamsize size);
        void write(const char* d, streamsize size);
    private:
        std::ifstream *fi;
        std::ofstream *fo;
    };
    
    class BINFile: public MXMLFile{
    public:
        BINFile(BINType &f, Tag &r, string d="");
        void read();
        void write();
    private:
        void write(Tag &r);
        BINType &file;
    };
    
    class DefLine{
    public:
        DefLine(){type=NODE;}
        DefLine(tData t){type=t;}
        void addLine(string l, char m){lines.push_back(l),multiplicity.push_back(m);}
        tData getType(){return type;}
        string getTag(int no);
        int getNo(string tag){for(int i=0;i<lines.size();i++)if(lines[i].compare(tag)==0)return i;return -1;}
    private:
        vector<string> lines;
        vector<char> multiplicity;
        tData type;
    };
    
    class DTD{
    public:
        static tData getType(string dtd, string tag);
        static string getName(string dtd, string tag, int no);
        static int getNo(string dtd,string tag, string search);
        static void newDTD(string dir);
        DTD(){;}
    private:
        static map<string, DTD> dtdList;
        DTD(string file);
        std::map<string,DefLine> lines;
        static mutex DTDmt;
    };
}


#endif	/* XML_H */

