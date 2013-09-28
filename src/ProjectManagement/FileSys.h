/* 
 * File:   fileSys.h
 * Author: jsmtux
 *
 * Created on August 16, 2012, 12:05 PM
 */

#ifndef FILESYS_H
#define	FILESYS_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace boost::filesystem;
using namespace std;

vector<string> listDirFiles( const path & directory, bool dirs=true ,bool recurse_into_subdirs = false );

#endif	/* FILESYS_H */

