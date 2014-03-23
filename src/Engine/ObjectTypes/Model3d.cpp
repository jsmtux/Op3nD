/**
 * @(#) Model3d.cpp
 */

#include <assimp/postprocess.h>

#include "Model3d.h"
#include "Image.h"
#include "Resource.h"
#include "../../ProjectManagement/MXML.h"
#include "../../ProjectManagement/Project.h"
#include "../Base.h"
#include "../Graphics/Shading.h"
map<string,Model3d*> Model3d::list;

Vertex::Vertex(){}

Vertex::Vertex(Vector3 pos, Vector2 tex){
    position = pos;
    coord = tex;
    normal = Vector3(0,0,0);
}

Vertex::Vertex(Vector3 pos, Vector2 tex, Vector3 norm){
    position = pos;
    coord = tex;
    normal = norm;
}


void VertexBoneData::AddBoneData(uint BoneID, float Weight){
    for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i]     = BoneID;
            Weights[i] = Weight;
            return;
        }        
    }
    
    //assert(0);
}

float Model3d::getBiggest(aiMesh* tMesh){
    float biggest=0;
    
    for (unsigned int i = 0 ; i < tMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(tMesh->mVertices[i]);
        
        Vector3 position =Vector3(pPos->x, pPos->y, pPos->z);
        
        float tmp=position.getBiggest();
        if(tmp>biggest)biggest=tmp;
    }
    return biggest;
}

Resource::ResourceType Model3d::getType()
{
  return MODEL3D;
}

Model3d::Model3d(const string filename){
    name =filename;
    numBones=0;
    valid=false;
    animTime=0;
    currentAnimation=0;
    loopAnimation=true;

    bool isDirectX=string(".x").compare(name.substr(name.find_last_of('.'),name.back()))==0;
    // Create the buffers for the vertices atttributes
    glGenBuffers(5, m_Buffers);
    
    scene=importer.ReadFile(filename,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    if(!scene){
        cout << "Error in file " << filename << endl;
        cout << importer.GetErrorString() << endl;
        valid=false;
        return;
    }
    m_GlobalInverseTransform = scene->mRootNode->mTransformation;
    m_GlobalInverseTransform.inverse();
    float ratio =0;
    for(int i=0;i<scene->mNumMeshes;i++){
        float tmp = getBiggest(scene->mMeshes[i]);
        if(tmp>ratio)ratio=tmp;
    }
    
    scaleTransform.setScaleTransform(1/ratio,1/ratio,1/ratio);
    
    vector<Vector3> Positions;
    vector<Vector3> Normals;
    vector<Vector2> TexCoords;
    vector<unsigned int> Indices;
    vector<VertexBoneData> Bones;
    
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
     meshes.resize(scene->mNumMeshes);
     for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        const aiMesh* paiMesh = scene->mMeshes[i];
        meshes[i].MaterialIndex = paiMesh->mMaterialIndex;
        meshes[i].NumIndices = paiMesh->mNumFaces * 3;
        meshes[i].BaseVertex = NumVertices;
        meshes[i].BaseIndex = NumIndices;
        
        NumVertices += paiMesh->mNumVertices;
        NumIndices += meshes[i].NumIndices;
    }
        
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Indices.reserve(NumIndices);
    Bones.resize(NumVertices);
    
    for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        int meshIndex=i;
        const aiMesh* paiMesh = scene->mMeshes[i];
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        // Populate the vertex attribute vectors
        for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
            const aiVector3D* pPos = &(paiMesh->mVertices[i]);
            const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
            const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ?
                                          &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

            Positions.push_back(Vector3(pPos->x, pPos->y, pPos->z));
            Normals.push_back(Vector3(pNormal->x, pNormal->y, pNormal->z));
            TexCoords.push_back(Vector2(pTexCoord->x, pTexCoord->y));
        }
        //populate bones
        for(int i=0;i< paiMesh->mNumBones;i++){
            
            unsigned int boneIndex=0;
            string boneName(paiMesh->mBones[i]->mName.data);
            if (boneMapping.find(boneName) == boneMapping.end()) {
                boneIndex = numBones;
                numBones++;
                BoneInfo bi;
                boneInfo.push_back(bi);
            }
            else {
                boneIndex = boneMapping[boneName];
            }

            boneMapping[boneName] = boneIndex;
            boneInfo[boneIndex].BoneOffset = paiMesh->mBones[i]->mOffsetMatrix;

            for (uint j = 0 ; j < paiMesh->mBones[i]->mNumWeights ; j++) {
                uint VertexID = meshes[meshIndex].BaseVertex + paiMesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = paiMesh->mBones[i]->mWeights[j].mWeight;
                Bones[VertexID].AddBoneData(boneIndex, Weight);
            }
        }
        

        // Populate the index buffer
        for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
            const aiFace& Face = paiMesh->mFaces[i];
            assert(Face.mNumIndices == 3);
	    if(!isDirectX){
	      Indices.push_back(Face.mIndices[0]);
	      Indices.push_back(Face.mIndices[1]);
	      Indices.push_back(Face.mIndices[2]);
	    }else{
	      Indices.push_back(Face.mIndices[2]);
	      Indices.push_back(Face.mIndices[1]);
	      Indices.push_back(Face.mIndices[0]);
	    }
        }
    }    
    initMaterials(scene);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0],
                    GL_STATIC_DRAW);
    glEnableVertexAttribArray(Shading::getActive()->getPosLocation());
    glVertexAttribPointer(Shading::getActive()->getPosLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0],
                    GL_STATIC_DRAW);
    glEnableVertexAttribArray(Shading::getActive()->getTexLocation());
    glVertexAttribPointer(Shading::getActive()->getTexLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0],
                    GL_STATIC_DRAW);
    glEnableVertexAttribArray(Shading::getActive()->getNormLocation());
    glVertexAttribPointer(Shading::getActive()->getNormLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size()*3, &Indices[0],
                    GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    valid=true;
}

void Model3d::Draw(){
#ifndef NODRAW
    if(!valid){
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glEnableVertexAttribArray(Shading::getActive()->getPosLocation());
    glVertexAttribPointer(Shading::getActive()->getPosLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glEnableVertexAttribArray(Shading::getActive()->getTexLocation());
    glVertexAttribPointer(Shading::getActive()->getTexLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    
    Shading::getActive()->setAnimated(scene->HasAnimations());
    
    vector<Matrix> transforms;
    boneTransform(animTime,transforms);
    animTime+=TIME_TO_SECONDS(Base::getInstance()->getStateManager()->getCurState()->getDiffTime());
    
    for(int i=0;i<transforms.size();i++){
        Shading::getActive()->setBoneTransform(i,transforms[i]);
    }
    
    for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        const unsigned int MaterialIndex = meshes[i].MaterialIndex;

       if (MaterialIndex < maps.size() && maps[MaterialIndex]) {
           dynamic_cast<Image*>(maps[MaterialIndex])->Bind();
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

bool Model3d::isValid(){
    return valid;
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
  cout << "Animation " << newAnimation << " not found\n";
}

float Model3d::getAnimationTime()
{
  return timeToAnimationTime(animTime)/float(scene->mAnimations[currentAnimation]->mDuration-1);
}

void Model3d::setLoop(bool isLoop)
{
  loopAnimation=isLoop;
}

Model3d* Model3d::loadM3d(string file ){
    map<string,Model3d*>::iterator ret;
    if((ret=list.find(file))!=list.end()){
      ret->second->copies++;
      return ret->second;
    }
    
    Model3d* tmp= new Model3d(file);
    if(!tmp->isValid()){
        delete tmp;
        return NULL;
    }
    list.insert(pair<string,Model3d*>(file,tmp));
    return tmp;
}


void Model3d::initMaterials(const aiScene *scene){
    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = scene->mMaterials[i];

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                cout << "loading image " << Path.data << " for " << name <<endl;
                maps.push_back(Resource::Load(Path.data));
            }
        }
    }
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

void Model3d::del(){
    for(Resource *i:maps){
        i->free();
        delete i;
    }
    maps.clear();
    list.erase(name);
}
