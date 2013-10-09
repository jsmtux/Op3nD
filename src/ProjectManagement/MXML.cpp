#include "MXML.h"

using namespace MXML;
mutex DTD::DTDmt;

#include <iostream>
#include <fstream>
#include <sstream>
#include<stack>

Attribute::Attribute() {
    type = NODE;
    val.s = NULL;
    count=new int(1);
}

Attribute::~Attribute(){
    (*count)--;
    if(count==0){
        delete count;
        if(type==STRING){
            delete val.s;
        }
    }
}

Attribute::Attribute(float f) {
    val.f = f;
    type = FLOAT;
    count=new int(1);
}

Attribute::Attribute(int i) {
    val.i = i;
    type = INT;
    count=new int(1);
}

Attribute::Attribute(string &s) {
    val.s = strdup(s.c_str());
    type = STRING;
    count=new int(1);
}

Attribute::Attribute(const char *s) {
    val.s = strdup(s);
    type = STRING;
    count=new int(1);
}

tData Attribute::getType() {
    return type;
}

float Attribute::getFloat() {
    return val.f;
}

int Attribute::getInt() {
    return val.i;
}

string Attribute::getString() {
    return string(val.s);
}

Tag::Tag(string n) : name(n) {
}

void Tag::setAttrib(Attribute a) {
    if (!children.empty())return;
    attrib = a;
}

void Tag::addChildren(Tag t) {
    if (attrib.getType() != NODE)return;
    children.push_back(t);
}

string Tag::getName() {
    return name;
}

vector<Tag> &Tag::getChildren() {
    return children;
}

Attribute &Tag::getAttrib() {
    return attrib;
}

Tag& Tag::operator[] (string name) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i].getName().compare(name) == 0)return children[i];
    }
    cout << name << "Not found in tag " << Tag::name << endl;
}

bool Tag::contains(string s) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i].getName().compare(s) == 0)return true;
    }
    return false;
}

string printTabs(int n = 0) {
    string ret;
    for (int i = 0; i < n; i++)ret += ' ';
    return ret;
}

string Tag::print(int l) {
    string ret;
    ret += printTabs(l);
    ret += " <" + name + "> \n";
    for (int i = 0; i < children.size(); i++)
        ret += children[i].print(l + 1);
    stringstream ss;
    if (attrib.getType() != NODE) {
        ret += printTabs(l);
        switch (attrib.getType()) {
            case INT:
                ss << attrib.getInt();
                ret += ss.str() + "\n";
                break;
            case FLOAT:
                ss << attrib.getFloat();
                ret += ss.str() + "\n";
                break;
            case STRING:
                ret += attrib.getString() + "\n";
                break;
        }
    }
    ret += printTabs(l);
    ret += " </" + name + ">  \n";
    return ret;
}

void Tag::clear() {
    children.clear();
}

bool Tag::isEmpty(){
    return children.empty();
}

MXMLFile::MXMLFile(Tag &r, string d) : root(r) {
    dtd = d;
}

Tag* MXMLFile::getRoot() {
    return &root;
}

XMLFile::XMLFile(string file, Tag &r, string d) : MXMLFile(r, d) {
    dir = file;
}

bool XMLFile::write() {
    std::ofstream file;
    file.open(dir.c_str());
    if(!file.is_open()){
      cerr << "file " << dir << " can't be opened or created" << endl;
      return false;
    }
    file << "<!DOCTYPE " << root.getName() << " SYSTEM \"" << dtd << "\">" << endl;
    file << root.print() << endl;
    file.close();
    return true;
}

string prevToken = "", currToken;

string nextToken(string full, string delimiters = " \t\n") {
    if (prevToken.compare(full) != 0) {
        currToken = full;
    }

    prevToken = full;
    string ret;
    string::size_type b, e;
    b = currToken.find_first_not_of(delimiters, 0);
    e = currToken.find_first_of(delimiters, b);
    if (b == string::npos)return string("");
    if (e == string::npos) {
        ret = currToken.substr(b, currToken.size());
        currToken.clear();
    } else {
        ret = currToken.substr(b, (e - b));
        currToken.erase(0, e);
    }
    return ret;
}

#include <stdlib.h>
#include <stdexcept>
#include <bits/stl_vector.h>

void XMLFile::read() {
    std::ifstream file(dir.c_str());
    if (!file.is_open()) {
        cerr << "File "<< dir <<" does not exist" << endl;
        return;
    }
    string line;
    int s = 0;
    stack<Tag*> sTag;
    string token;
    string ntag;
    string att;
    Tag curTag;
    string rootName;
    string prevName;
    float parseF;
    bool prev=true;

    while (!file.eof()) {
        getline(file, line);
        if (line.empty())continue;
        try {
            while (!(token = nextToken(line)).empty()) {
                switch (s) {
                    case 0://Doctype not found yet
                        if (token.compare("<!DOCTYPE") != 0)throw "Error on doctype";
                        rootName = nextToken(line);
                        nextToken(line);
                        dtd = nextToken(line);
                        dtd = dtd.substr(1, dtd.size() - 3);
                        DTD::newDTD(dtd);
                        while (!(token = nextToken(line)).empty());
                        s++;
                        break;
                    case 1:
                        switch (getTokenType(token)) {
                            case BEGIN://found opening tag
                                ntag = token.substr(1, token.size() - 2);
                                curTag = Tag(ntag);
                                if (sTag.empty()) {
                                    root = curTag;
                                    sTag.push(&root);
                                } else {
                                    sTag.top()->addChildren(curTag);
                                    sTag.push(&sTag.top()->getChildren().back());
                                }
                                break;
                            case NONE://inside tag
                                if(prev){
                                    //att=att+string("\n");
                                    prevName=sTag.top()->getName();
                                }else{
                                    att = string("");
                                    prev=true;
                                }
                                while (token[0] != '<' && !token.empty()) {//adds until newline or end tag
                                    if(att.size()>0)att+=string(" ");
                                    att += token;
                                    token = nextToken(line);
                                }
                                
                                switch (DTD::getType(dtd, sTag.top()->getName())) {//inserts the read string into the structure
                                    case INT:
                                        sTag.top()->setAttrib(Attribute(atoi(att.c_str())));
                                        break;
                                    case FLOAT:
                                        istringstream(att) >> parseF;
                                        sTag.top()->setAttrib(Attribute(parseF));
                                        break;
                                    case STRING:
                                        sTag.top()->setAttrib(Attribute(att));
                                        break;
                                }
                                if (token.empty())
                                    break;
                            case END://found closing tag
                                prev=false;
                                ntag = token.substr(2, token.size() - 3);
                                if (sTag.empty())throw "Tags don't match";
                                string last = sTag.top()->getName();
                                sTag.pop();
                                if (last.compare(ntag) != 0)throw "Tags don't match";
                                break;
                        }
                        break;
                }
            }
        } catch (const char* error) {
            cout << error << endl;
            break;
        }
    }
    file.close();
}

tToken XMLFile::getTokenType(string tok) {
    if (tok[0] != '<')return NONE;
    if (tok[1] != '/')return BEGIN;
    return END;
}

void BINType::writeString(string s) {
    int size = s.size() + 1;
    write((char*) &(size), sizeof (int));
    write(s.c_str(), sizeof (char) *(size));
}

string BINType::readString() {
    int n;
    static char str[256];
    read((char*) &n, sizeof (int));
    read(str, sizeof (char) *n);
    return string(str);
}

MEM::MEM(char* f, streamsize max) {
    file = f;
    maxSize = max;
    currPos = 0;
}

int MEM::read(char* r, streamsize size) {
    for (int i = 0; i < size && currPos < maxSize; i++) {
        r[i] = file[currPos + i];
    }
    currPos += size;
    return currPos < maxSize;
}

void MEM::write(const char* w, streamsize size) {
    int i;
    for (i = 0; i < size && currPos < maxSize; i++) {
        file[currPos + i] = w[i];
    }
    if (i < size)throw "file too short";
    currPos += size;
}

streamsize MEM::getSize() {
    return currPos;
}

Stream::Stream(std::ifstream* f) : fi(f), fo(NULL) {
}

Stream::Stream(std::ofstream* f) : fo(f), fi(NULL) {
}

int Stream::read(char* d, streamsize size) {
    if (!fi)return 0;
    return fi->read(d, size) != 0;
}

void Stream::write(const char* d, streamsize size) {
    if (!fo)return;
    fo->write(d, size);
}

BINFile::BINFile(BINType &f, Tag&r, string d) : MXMLFile(r, d), file(f) {
    dtd = d;
}

void BINFile::read() {
    static char str[256];
    char n;
    int ti;
    float tf;
    dtd = file.readString();
    DTD::newDTD(dtd);

    string rootName = file.readString();
    Tag tRoot(rootName);

    stack<Tag*> readSt;
    readSt.push(&tRoot);
    while (file.read(&n, sizeof (char)) && n != -1) {
        if (n == 0) {
            readSt.pop();
            continue;
        }
        string tag;
        try {
            tag = DTD::getName(dtd, readSt.top()->getName(), n - 1);
        } catch (exception e) {
            cout << "exception reading tag" << e.what();
        }
        tData td;
        try {
            td = DTD::getType(dtd, tag);
        } catch (std::out_of_range r) {
            cout << "Tag " << tag << "type not found" << endl;
            exit(1);
        }
        char* ts = NULL;
        readSt.top()->addChildren(Tag(tag));
        readSt.push(&readSt.top()->getChildren().back());
        switch (td) {
            case INT:
                file.read((char*) &ti, sizeof (int));
                readSt.top()->setAttrib(Attribute(ti));
                break;
            case FLOAT:
                file.read((char*) &tf, sizeof (float));
                readSt.top()->setAttrib(Attribute(tf));
                break;
            case STRING:
                readSt.top()->setAttrib(Attribute(file.readString().c_str()));
                break;
        }
    }
    root = tRoot;
}

bool BINFile::write() {
    file.writeString(dtd);
    file.writeString(root.getName());
    write(root);
    char n = 255;
    file.write(&n, sizeof (char));
    return true;
}

void BINFile::write(Tag &r) {
    vector<Tag> childs = r.getChildren();
    if (r.getAttrib().getType() == NODE) {
        for (int i = 0; i < childs.size(); i++) {
            char n = DTD::getNo(dtd, r.getName(), childs[i].getName()) + 1;
            file.write((char*) &n, sizeof (char));
            write(childs[i]);
            n = 0;
            file.write((char*) &n, sizeof (char));
        }
    } else {
        Attribute att = r.getAttrib();
        float tf;
        int ti;
        switch (att.getType()) {
            case FLOAT:
                tf = att.getFloat();
                file.write((char*) &tf, sizeof (float));
                break;
            case INT:
                ti = att.getInt();
                file.write((char*) &ti, sizeof (int));
                break;
            case STRING:
                file.writeString(att.getString());
                break;
        }
    }
}

tData stringToType(string t) {
    if (t.compare("integer") == 0) return INT;
    if (t.compare("float") == 0) return FLOAT;
    if (t.compare("string") == 0) return STRING;
    throw "unhandled type";
}

string DefLine::getTag(int no) {
    return lines.at(no);
}

map<string, DTD> DTD::dtdList;

void DTD::newDTD(string dir) {
    DTDmt.lock();
    if (dtdList.find(dir) == dtdList.end()) {
        try {
            dtdList[dir] = DTD(O3D_FILES_DIR+string("dtd/") + dir);
        } catch (char* ex) {
            cout << "error initialising " << dir << ":" << ex << endl;
        }
    }
    DTDmt.unlock();
}

tData DTD::getType(string dtd, string tag) {
    try {
        return dtdList.at(dtd).lines.at(tag).getType();
    } catch (std::out_of_range r) {
        cout << "Error in getType\n" << "dtd " << dtd << ", tag " << tag;
        exit(1);
    }
}

string DTD::getName(string dtd, string tag, int no) {
    try {
        return dtdList.at(dtd).lines.at(tag).getTag(no);
    } catch (std::out_of_range r) {
        cout << "Error in getName\n" << "dtd " << dtd << ", tag " << tag << " at line " << no << endl;
        exit(1);
    }
}

int DTD::getNo(string dtd, string tag, string search) {
    if(tag.empty()){
        cout << "Empty tag name in getNo\n";
        return -1;
    }
    try {
        return dtdList.at(dtd).lines.at(tag).getNo(search);
    } catch (std::out_of_range r) {
        cout << "Error in getNo\n" << "dtd " << dtd << ", tag " << tag << " word " << search << endl;
        exit(1);
    }
}

DTD::DTD(string file) {
    ifstream text(file.c_str());
    if (!text.is_open()) {
        cout << "File " << file << " does not exist\n";
        throw "DTD does not exist";
    }
    string line, token;

    while (!text.eof()) {
        getline(text, line);
        if (line.empty() || line[0] == '#')continue;
        if (nextToken(line).compare("<!ELEMENT") != 0)throw "unexpected line in dtd file";
        string name = nextToken(line);
        string desc = nextToken(line);
        switch (desc[0]) {
            case '#':
                lines[name] = DefLine(stringToType(desc.substr(1, desc.size() - 2)));
                break;
            case '(':
                DefLine tmp;
                string str;
                char mult;
                for (int i = 1; i < desc.size(); i++) {
                    switch (desc[i]) {
                        case ',':
                        case ')':
                            tmp.addLine(str, mult);
                            str = "";
                            break;
                        case '*':
                            mult = '*';
                            break;
                        case '?':
                            mult = '?';
                            break;
                        case ' ':
                            break;
                        default:
                            str += desc[i];
                    }
                }
                lines[name] = tmp;
                break;
        }
    }
    text.close();
}
