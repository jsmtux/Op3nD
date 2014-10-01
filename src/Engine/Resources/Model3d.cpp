/**
 * @(#) Model3d.cpp
 */

#include <assimp/postprocess.h>

#include "Model3d.h"
#include "Image.h"
#include "Resource.h"
#include "../../ProjectManagement/MXML.h"
#include "../../ProjectManagement/Project.h"
#include "Base.h"
#include "Graphics/Shader.h"

Resource::ResourceType Model3d::getType()
{
  return MODEL3D;
}

Model3d::Model3d(Assimp::Importer *_importer, GLuint *_buffers, vector<Resource*> _maps, vector<MeshEntry> _meshes, map<string,unsigned int> _boneMapping, 
	    unsigned int _numBones, vector<BoneInfo> _boneInfo, Matrix _globalInverseTransform, const aiScene* _scene,
	    float _animTime, Matrix _scaleTransform): importer(_importer){
    currentAnimation = 0;
    m_Buffers = _buffers;
    maps = _maps;
    meshes = _meshes;
    boneMapping = _boneMapping;
    numBones = _numBones;
    boneInfo = _boneInfo;
    m_GlobalInverseTransform = _globalInverseTransform;
    scene = _scene;
    animTime = _animTime;
    scaleTransform = _scaleTransform;
}

void Model3d::Draw(Shader *shader){
#ifndef NODRAW
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glEnableVertexAttribArray(shader->getVarLocation("Position"));
    glVertexAttribPointer(shader->getVarLocation("Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glEnableVertexAttribArray(shader->getVarLocation("TexCoord"));
    glVertexAttribPointer(shader->getVarLocation("TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    
    shader->setBool(scene->HasAnimations(), "animated");
    
    vector<Matrix> transforms;
    boneTransform(animTime,transforms);
    animTime+=TIME_TO_SECONDS(Base::getInstance()->getStateManager()->getCurState()->getDiffTime());
    
    for(int i=0;i<transforms.size();i++){
	shader->setMatrix(transforms[i], "gBones", i);
    }
    
    for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        const unsigned int MaterialIndex = meshes[i].MaterialIndex;

       if (MaterialIndex < maps.size() && maps[MaterialIndex]) {
           dynamic_cast<Image*>(maps[MaterialIndex])->Bind(shader);
       }
       #ifndef ANDROID
        glDrawElementsBaseVertex(GL_TRIANGLES,
                                  meshes[i].NumIndices,
                                  GL_UNSIGNED_INT,
                                  (void*)(sizeof(unsigned int) * meshes[i].BaseIndex),
                                  meshes[i].BaseVertex);
		#else
        glDrawElements(GL_TRIANGLES,
                      meshes[i].NumIndices,
                      GL_UNSIGNED_INT,
                      (void*)(sizeof(unsigned int) * meshes[i].BaseIndex));

		#endif
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    Image::unBind();
#endif
}

vector< string > Model3d::getAnimations()
{
  vector<string> ret;
  for(int i=0;i<scene->mNumAnimations;i++){
    ret.push_back(scene->mAnimations[i]->mName.C_Str());
  }
  return ret;
}

string Model3d::getCurrentAnimation()
{
  if(!scene->HasAnimations()){
    return "";
  }
  return getAnimations()[currentAnimation];
}

void Model3d::changeAnimation(string newAnimation)
{
  for(int i=0;i<scene->mNumAnimations;i++){
    if(string(scene->mAnimations[i]->mName.C_Str()).compare(newAnimation)==0){
      currentAnimation=i;
      animTime=0;
      return;
    }
  }
}

float Model3d::getAnimationTime()
{
  return timeToAnimationTime(animTime)/float(scene->mAnimations[currentAnimation]->mDuration-1);
}

void Model3d::setLoop(bool isLoop)
{
  loopAnimation=isLoop;
}

float Model3d::timeToAnimationTime(float t)
{
  
    float TicksPerSecond = scene->mAnimations[currentAnimation]->mTicksPerSecond != 0 ?
                            scene->mAnimations[currentAnimation]->mTicksPerSecond : 25.0f;
    float TimeInTicks = t * TicksPerSecond;
    float totalTime=TimeInTicks;
    if(TimeInTicks>scene->mAnimations[currentAnimation]->mDuration){
      if(loopAnimation)
	totalTime = std::fmod(float(TimeInTicks), float(scene->mAnimations[currentAnimation]->mDuration));
      else
	totalTime = float(scene->mAnimations[currentAnimation]->mDuration)-1;
    }
    return totalTime;
}

void Model3d::boneTransform(float timeInSeconds, vector<Matrix>& transforms){
    Matrix Identity;
    Identity.setIdentity();
    if(!scene->HasAnimations()){
        return;
    }
    
    float animationTime=timeToAnimationTime(timeInSeconds);
    readNodeHierarchy(animationTime, scene->mRootNode, Identity);

    transforms.resize(numBones);
    for (uint i = 0 ; i < numBones ; i++) {
        transforms[i] = boneInfo[i].FinalTransformation;
    }    
}

void Model3d::readNodeHierarchy(float animationTime, const aiNode* pNode, const Matrix& parentTransform){
    string NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = scene->mAnimations[currentAnimation];

    Matrix nodeTransformation(pNode->mTransformation);
    
    const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        calcInterpolatedScaling(Scaling, animationTime, pNodeAnim);
        Matrix scalingM;
        scalingM.setScaleTransform(Scaling.x, Scaling.y, Scaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ=aiQuaternion(1,0,0,0);
        calcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
        Matrix rotationM = Matrix(RotationQ.GetMatrix());

	
        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        calcInterpolatedPosition(Translation, animationTime, pNodeAnim);
        Matrix translationM;
        translationM.setTranslationTransform(Translation.x, Translation.y, Translation.z);
	
        // Combine the above transformations
        nodeTransformation = translationM * rotationM * scalingM;
    }

    Matrix globalTransformation = parentTransform * nodeTransformation;

    if (boneMapping.find(NodeName) != boneMapping.end()) {
        unsigned int boneIndex = boneMapping[NodeName];
        boneInfo[boneIndex].FinalTransformation = m_GlobalInverseTransform * globalTransformation *
                                                    boneInfo[boneIndex].BoneOffset;
    }

    for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) {
        readNodeHierarchy(animationTime, pNode->mChildren[i], globalTransformation);
    }
}

void Model3d::calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim){
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = findScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

const aiNodeAnim* Model3d::findNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}

void Model3d::calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim){
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }
    
    uint RotationIndex = findRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    aiQuaternion StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    aiQuaternion EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}

void Model3d::calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim){    
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }
            
    uint PositionIndex = findPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

uint Model3d::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


uint Model3d::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


uint Model3d::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    
    //assert(0);

    return 0;
}

Matrix Model3d::getTransform(){
    return scaleTransform;
}

Model3d::~Model3d(){
    for(Resource *i:maps){
        delete i;
    }
    maps.clear();
}
