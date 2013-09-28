#include "FileSys.h"

vector<string> listDirFiles( const path & directory, bool dirs ,bool recurse_into_subdirs){
    vector<string> ret;
    if( exists( directory ) ){
        directory_iterator end ;
        for( directory_iterator iter(directory) ; iter != end ; ++iter ){
            if ( is_directory( *iter ) ){
                if(dirs)ret.push_back(iter->path().filename().string());
                if( recurse_into_subdirs ) listDirFiles(*iter) ;
            }else{
                if(!dirs)ret.push_back(iter->path().filename().string());
            }
        }
    }
    return ret;
}
