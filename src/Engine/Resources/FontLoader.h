#ifndef _FONTLOADER_H_
#define _FONTLOADER_H_

#include "BaseResourceLoader.h"

class FontLoader: public BaseResourceLoader {
public:
  Project::FileType getFileType() {return Project::FONT;}
private:
  Resource* loadInternal(State* state, string dir);
};

#endif