#include "GLDebugDrawer.h"
#include "../ObjectTypes/Line.h"

#include <stdio.h> //printf debugging
GLDebugDrawer::GLDebugDrawer()
:m_debugMode(btIDebugDraw::DBG_DrawWireframe)
{

}

GLDebugDrawer::~GLDebugDrawer()
{
}

void	GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor){
    Line::drawLine(from,to,fromColor);
}

void	GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color){
    drawLine(from,to,color,color);
}

void GLDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color){
	//glTranslatef (p.getX(), p.getY(), p.getZ());

	int lats = 5;
	int longs = 5;

	int i, j;
	for(i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius*sin(lat0);
		btScalar zr0 =  radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);

                btScalar prev=(0,0,0);
		for(j = 0; j <= longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
                        Line::drawLine(prev,(x * zr0, y * zr0, z0),color);
                        Line::drawLine((x * zr0, y * zr0, z0),(x * zr1, y * zr1, z1),color);//strip
                        prev=(x * zr1, y * zr1, z1);
		}
	}

}


//
//void	GLDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
//{
////	if (m_debugMode > 0)
////	{
////		const btVector3	n=btCross(b-a,c-a).normalized();
////		glBegin(GL_TRIANGLES);		
////		glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
////		glNormal3d(n.getX(),n.getY(),n.getZ());
////		glVertex3d(a.getX(),a.getY(),a.getZ());
////		glVertex3d(b.getX(),b.getY(),b.getZ());
////		glVertex3d(c.getX(),c.getY(),c.getZ());
////		glEnd();
////	}
//}

void	GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;

}

void GLDebugDrawer::draw3dText(const btVector3& location,const char* textString){;}

void	GLDebugDrawer::reportErrorWarning(const char* warningString){
	printf("%s\n",warningString);
}

void	GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color){
	{
//		btVector3 to=pointOnB+normalOnB*1;//distance;
//		const btVector3&from = pointOnB;
//		glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
//		//glColor4f(0,0,0,1.f);
//		glBegin(GL_LINES);
//		glVertex3d(from.getX(), from.getY(), from.getZ());
//		glVertex3d(to.getX(), to.getY(), to.getZ());
//		glEnd();
	}
}





