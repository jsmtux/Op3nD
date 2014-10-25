/* 
 * File:   GLDebugDrawer.h
 * Author: jsmtux
 *
 * Created on November 1, 2012, 6:28 PM
 */

#ifndef GLDEBUGDRAWER_H
#define	GLDEBUGDRAWER_H

#include "LinearMath/btIDebugDraw.h"

class Shader;

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;
public:
    GLDebugDrawer();
    ~GLDebugDrawer(); 

    void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
    void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
    void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
    //void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
    void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
    void reportErrorWarning(const char* warningString);
    void draw3dText(const btVector3& location,const char* textString);
    void setDebugMode(int debugMode);
    int getDebugMode() const { return m_debugMode;}
private:
  Shader* shader;
};

#endif	/* GLDEBUGDRAWER_H */

