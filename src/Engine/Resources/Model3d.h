/**
 * @(#) Model3d.h
 */

#ifndef MODEL3D_H_H
#define MODEL3D_H_H


#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#define INVALID_OGL_VALUE 0xFFFFFFFF    

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define NUM_BONES_PER_VERTEX 4
#define INDEX_BUFFER 0
#define POS_VB 1
#define NORMAL_VB 2
#define TEXCOORD_VB 3
#define BONE_VB 4

#define BONE_ID_LOCATION 3
#define BONE_WEIGHT_LOCATION 4

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix.h"
#include "Resource.h"

#include <string>
#include <memory>

struct MeshEntry {
    MeshEntry()
    {
      NumIndices = 0;
      BaseVertex = 0;
      BaseIndex = 0;
      MaterialIndex = INVALID_OGL_VALUE;
    }

    unsigned int BaseVertex;
    unsigned int BaseIndex;
    unsigned int NumIndices;
    unsigned int MaterialIndex;
};

struct BoneInfo
{
    Matrix BoneOffset;
    Matrix FinalTransformation;        

    BoneInfo(){
	BoneOffset.setZero();
	FinalTransformation.setZero();            
    }
};

struct Vertex{
    Vector3 position;
    Vector2 coord;
    Vector3 normal;

    Vertex();
    Vertex(Vector3 pos, Vector2 tex);
    Vertex(Vector3 pos, Vector2 tex, Vector3 norm);
};

struct VertexBoneData{
    uint IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
    
    VertexBoneData()
    {
        Reset();
    };

    void Reset()
    {
        ZERO_MEM(IDs);
        ZERO_MEM(Weights);        
    }
    
    void AddBoneData(uint BoneID, float Weight);
};

class Model3d: public Resource
{
public:
    virtual ~Model3d();
    void Draw(Shader* shader);
    ResourceType getType();
    //animation
    vector<string> getAnimations();
    string getCurrentAnimation();
    void changeAnimation(string newAnimation);
    float getAnimationTime();
    void setLoop(bool isLoop);
    Model3d(string name, Assimp::Importer *_importer, GLuint *_buffers, vector<Resource*> _maps, vector<MeshEntry> _meshes, map<string,unsigned int> _boneMapping, 
    unsigned int _numBones, vector<BoneInfo> _boneInfo, Matrix _globalInverseTransform, const aiScene* _scene,
    float _animTime, Matrix _scaleTransform);
    string getDefaultShader(){return "normal.sfx";}
private:
    void boneTransform(float timeInSeconds, vector<Matrix>& transforms);
    float timeToAnimationTime(float t);
    void readNodeHierarchy(float animationTime, const aiNode* pNode, const Matrix& parentTransform);
    Matrix getTransform();
    
    void calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    
    GLuint* m_Buffers;

    vector<Resource*> maps;
    vector<MeshEntry> meshes;
    map<string,unsigned int> boneMapping;
    unsigned int numBones;
    vector<BoneInfo> boneInfo;
    Matrix m_GlobalInverseTransform;
    const aiScene* scene;
    float animTime;
    Matrix scaleTransform;
    //animation
    bool activeAnimation;
    unsigned int currentAnimation;
    bool loopAnimation;
    Assimp::Importer *importer;
};

#endif
