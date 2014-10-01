#include "Line.h"
#include "Math/Vector3.h"
#include "Graphics/Shader.h"
#include "Project.h"

#ifndef NODRAW
GLuint Line::VBO;
#endif

void Line::Draw(Shader* shader){
    drawLine(shader, begin,end,color);
}

void Line::drawLine(Shader* shader, Vector3 b, Vector3 e, Vector3 c){
#ifndef NODRAW
#ifndef ANDROID
    Vector3 Vertices[2];
    Vertices[0] = b;
    Vertices[1] = e;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, 2);
#endif
#endif
}

Resource::ResourceType Line::getType(){
    return LINE;
}

void Line::init(){    
#ifndef NODRAW
#ifndef ANDROID
    glGenBuffers(1, &VBO);
#endif
#endif
}

void Line::del(){
    ;
}
