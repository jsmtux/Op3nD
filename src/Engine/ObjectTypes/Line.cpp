#include "Line.h"
#include "../Math/Vector3.h"
#include "../Graphics/Shading.h"

#ifndef NODRAW
GLuint Line::VBO;
Shading* Line::colorShader;
#endif

void Line::Draw(){
    drawLine(begin,end,color);
}

void Line::drawLine(Vector3 b, Vector3 e, Vector3 c){
#ifndef NODRAW
#ifndef ANDROID
    Shading::push();
    colorShader->useProgram();
    Vector3 Vertices[2];
    Vertices[0] = b;
    Vertices[1] = e;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, 2);


    Shading::pop();
#endif
#endif
}

string Line::getType(){
    return "line";
}

void Line::init(){    
#ifndef NODRAW
#ifndef ANDROID
    colorShader = new Shading();
    colorShader->initShader("color.sfx");
    glGenBuffers(1, &VBO);
#endif
#endif
}

void Line::del(){
    ;
}
