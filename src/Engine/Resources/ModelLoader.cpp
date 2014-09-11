#include "ModelLoader.h"
#include "Model3d.h"
#include "Graphics/Shading.h"
#include <Base.h>
#include <assimp/postprocess.h>

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
}

float getBiggest(aiMesh* tMesh){
    float biggest=0;
    
    for (unsigned int i = 0 ; i < tMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(tMesh->mVertices[i]);
        
        Vector3 position =Vector3(pPos->x, pPos->y, pPos->z);
        
        float tmp=position.getBiggest();
        if(tmp>biggest)biggest=tmp;
    }
    return biggest;
}

Resource* ModelLoader::loadInternal(State* state, string dir)
{
  Assimp::Importer *importer = new Assimp::Importer();
  Resource* ret = nullptr;
  string name =dir;
  int numBones=0;
  float animTime=0;
  bool loopAnimation=true;
  const aiScene* scene;
  Matrix m_GlobalInverseTransform;
  Matrix scaleTransform;
  vector<Resource*> maps;
  vector<MeshEntry> meshes;
  map<string,unsigned int> boneMapping;
  vector<BoneInfo> boneInfo;
  GLuint* m_Buffers = new GLuint[5];

  bool isDirectX=string(".x").compare(name.substr(name.find_last_of('.'),name.back()))==0;
  // Create the buffers for the vertices atttributes
  glGenBuffers(5, m_Buffers);

  scene=importer->ReadFile(dir,
	  aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
  if(!scene){// TODO This should throw an exception
    cout << "Error in file " << dir << endl;
    cout << importer->GetErrorString() << endl;
  } else{
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
    maps = initMaterials(state, scene);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(Shading::getActive()->getPosLocation());
    glVertexAttribPointer(Shading::getActive()->getPosLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(Shading::getActive()->getTexLocation());
    glVertexAttribPointer(Shading::getActive()->getTexLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
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

    ret = new Model3d(importer, m_Buffers, maps, meshes, boneMapping, numBones, boneInfo, m_GlobalInverseTransform, scene, animTime, scaleTransform);
  }
  return ret;
}

vector<Resource*> ModelLoader::initMaterials(State* state, const aiScene* scene){
  vector<Resource*> maps;
  for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++) {
    const aiMaterial* pMaterial = scene->mMaterials[i];

    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString Path;

      if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
	maps.push_back(state->loadResource(Path.data));
      }
    }
  }
  return maps;
}