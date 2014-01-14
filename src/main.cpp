#ifndef NOGUI
#include <GL/glew.h>
#include <QDebug>
#include <QtWidgets/QApplication>
#include "Op3nD.h"
#else
#include <iostream>
using namespace std;
#include "Engine/Base.h"
#endif

int main(int argc, char** argv)
{
#ifndef NOGUI
    QApplication app(argc, argv);
    app.setOrganizationName("Op3nD");
    app.setOrganizationDomain("Op3nD.com");
    app.setApplicationName("Op3nD");
    Op3nD op3nd;
    op3nd.show();
    return app.exec();
#else
    /*argc=6;
    argv=new char*[6];
    argv[1]="/home/jsmtux/O3D/Naves";
    argv[2]="-map";
    argv[3]="new.xml";
    argv[4]="-res";
    argv[5]="640x480";*/
    
    if(argc<2){
      cout << "usage: " << argv[0] << " " << "<project path> <options>" << endl
	   << "Options: \n"
	   << "\t-map <map> : specify map name\n"
	   << "\t-res <resolution>: specify resolution\n"
	   << "\t-fullscreen: set fullscreen\n";
      
      return 0;
    }
    
    cout << argv[1] << endl;
    
    string pPath=argv[1];
    cout << "New path is " << pPath << endl;
    string map="main.xml";
    string res="640x480";
    bool fullscreen=false;
    for(int i=2;i<argc;i++){
      if(string(argv[i]).compare("-map")==0){
	map= argv[++i];
      }
      if(string(argv[i]).compare("-res")==0){
	res= argv[++i];
      }
      if(string(argv[i]).compare("-fullscreen")==0){
	fullscreen=true;
      }
    }
    
    int resX=atoi(res.substr(0,res.find('x')).c_str());
    int resY=atoi(res.substr(res.find('x')+1,res.size()-res.find('x')+1).c_str());
    
    Base::getInstance()->setResolution(Vector3(resX,resY,32));
    Base::getInstance()->init();
    Base::getInstance()->setProj(pPath);
    Base::getInstance()->newState(map,GAMEST);
    Base::getInstance()->changeState(map,GAMEST);
    Base::getInstance()->getCurState()->loadFile();
    Base::getInstance()->beginState();
    Base::getInstance()->gameLoop();
#endif
}
