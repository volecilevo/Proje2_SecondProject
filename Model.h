//
//  Model.h
//  Ders 5
//
//  Created by Gurel Erceis on 3/11/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//
// Assimp kutuphanesini kullaniyoruz.

#ifndef Ders_5_Model_h
#define Ders_5_Model_h
#include <Windows.h>
#include <GL\glew.h>
#include <GL/gl.h>
#include <GL\glut.h>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#include "Camera.h"
#define BUFFER_OFFSET(i) ((void*)(i))
using namespace std;

class Model{
public:
    Model(char *filename, char *vert_filename, char *frag_filename):filename(filename),vert_filename(vert_filename),frag_filename(frag_filename){
		glewInit();
        printf("Loading Model : %s\n", filename);
        if( loadGeometry() ){
            programId = LoadShaders(vert_filename,frag_filename);
            generateBuffers(); 
        }
        modelMatrix = glm::mat4x4(1.0f);
        
        
    }
    ~Model(){
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &normalBufferID);
        glDeleteBuffers(1, &indiceBufferID);
        glDeleteProgram(programId);
    }
    void draw(Camera *camera){
        
        glUseProgram(programId);
        
        glm::mat4 MVPmatrix = camera->projectionMatrix * camera->viewMatrix * modelMatrix;
        
        glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVPmatrix[0][0]);
       
        
        // vertex bufferi bagla
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(AttributePosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // normal buffer i bagla
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
		glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // indiceleri bagla
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBufferID);
        
        // Attributelari enable et
        glEnableVertexAttribArray(AttributePosition);
        glEnableVertexAttribArray(AttributeNormal);
        
        // ciz
        glDrawElements(GL_TRIANGLES, int(faceArray.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        

        // baglantilari kopar
        glDisableVertexAttribArray(AttributeNormal);
        glDisableVertexAttribArray(AttributePosition);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
        glUseProgram(0);
    }
    glm::mat4 modelMatrix;
private:
    char* filename;
    char* vert_filename;
    char* frag_filename;
    
    GLuint vertexBufferID,normalBufferID,indiceBufferID;
    GLuint vertexAttributePos,normalAttributePos;
    GLuint programId;
    GLuint UniformMVP;
    GLuint AttributePosition,AttributeNormal;
    
    vector<GLuint> faceArray;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
	int numFaces;
    
    void generateBuffers(){
        
        UniformMVP = glGetUniformLocation(programId, "MVP");
        printf("UniformMVP: %d\n",UniformMVP);
        
        AttributePosition = glGetAttribLocation(programId, "Position");
        printf("Position Attribute: %d\n", AttributePosition);
        
        AttributeNormal = glGetAttribLocation(programId, "Normal");
        printf("Normal Attribute: %d\n", AttributeNormal);
        
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &normalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &indiceBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceArray.size() * sizeof(GLuint), &faceArray[0], GL_STATIC_DRAW);
        
        printf("VertexBufferID:%d\n NormalBufferID:%d\n IndiceBufferID:%d\n", vertexBufferID, normalBufferID, indiceBufferID);
    }
    bool loadGeometry();
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    void readScene(const struct aiScene *sc);
};

#endif
