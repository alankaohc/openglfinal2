#pragma once
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad\glad.h>
#include "MyShader.h"
#include "MyComputeShader.h"
#include "src/MyPoissonSample.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src\MyCameraManager.h"
#include "src\MyImGuiPanel.h"

// shader 
MyShader* myShaderPointer;
MyComputeShader* myCompShaderPointer;
MyComputeShader* myResetCompShaderPointer;

// load model
std::vector<float> vertexData;
std::vector<float> uvData;
std::vector<float> normalData;
std::vector<unsigned int> indices;
int vertexCount[5] = { 0 };
int indexCount[5] = { 0 };


// SSBO : draw commands, for compute instanceCount, pass it to compute shader
struct DrawElementsIndirectCommand {
	unsigned int count;
	unsigned int instanceCount;
	unsigned int firstIndex;
	unsigned int baseVertex;
	unsigned int baseInstance;
};
DrawElementsIndirectCommand drawCommands[5];

// SSBO : position, for culling 
struct InstanceProperties {
	glm::vec4 position;
};
InstanceProperties* rawInsData;

// SSBO : rotation 
struct Colume {
	glm::vec4 vector;
};
Colume* rotCol0;
Colume* rotCol1;
Colume* rotCol2;
Colume* rotCol3;



// buffer
GLuint vaoHandle;       // attribute
GLuint vboHandle;       // vertex
GLuint uvboHandle;      // texture coordinate
GLuint nboHandle;       // normal 
GLuint iboHandle;       // index
GLuint cmdBufferHandle; // draw command, for indirect drawing, pass it to vertex shader

int NUM_TOTAL_INSTANCE = 0;
int NUM_SAMPLE0 = 0;
int NUM_SAMPLE1 = 0;
int NUM_SAMPLE2 = 0;
int NUM_SAMPLE3 = 0;
int NUM_SAMPLE4 = 0;

// uniform location
GLuint viewProjMatLoc; // for culling, only from player camera
GLuint viewMatLoc;     // rotate, identity
GLuint projMatLoc;     // projective, from player/god camera
GLuint modelMatLoc;    // model, from player/godd camera
GLuint albedoLoc;      // texture image

// load texture
const int NUM_TEXTURE = 5;
const int IMG_WIDTH = 1024;
const int IMG_HEIGHT = 1024;
const int IMG_CHANNEL = 4;
unsigned char* textureArrayData = new unsigned char[IMG_WIDTH * IMG_HEIGHT * IMG_CHANNEL * NUM_TEXTURE]; // texture data
GLuint textureArrayHandle; // texture buffer




// rock & plane

// shader  
MyShader* rockShaderPointer;
MyShader* planeShaderPointer;

// load model 
std::vector<float> rockVertexData;
std::vector<float> rockUvData;
std::vector<float> rockNormalData;
std::vector<unsigned int> rockIndices;

int rockIndexCount = 0;
int rockVertexCount = 0;

std::vector<float> planeVertexData;
std::vector<float> planeUvData;
std::vector<float> planeNormalData;
std::vector<unsigned int> planeIndices;

int planeIndexCount = 0;
int planeVertexCount = 0;

// load texture
unsigned char* rockTextureData;
GLuint rockTextureHandle;

unsigned char* planeTextureData;
GLuint planeTextureHandle;

// buffer
GLuint rockVaoHandle;       // attribute
GLuint rockVboHandle;       // vertex
GLuint rockUvboHandle;      // texture coordinate
GLuint rockNboHandle;       // normal 
GLuint rockIboHandle;       // index

GLuint planeVaoHandle;       // attribute
GLuint planeVboHandle;       // vertex
GLuint planeUvboHandle;      // texture coordinate
GLuint planeNboHandle;       // normal 
GLuint planeIboHandle;       // index

// uniform location
GLuint rockProjMatLoc;
GLuint rockViewMatLoc;
GLuint rockModelMatLoc;
GLuint rockAlbedoLoc;      // texture image
GLuint rockPosLoc;         // slime pos vertex shader
GLuint rockPosCompLoc;     // slime pos compute shader


GLuint planeProjMatLoc;
GLuint planeViewMatLoc;
GLuint planeModelMatLoc;
GLuint planeAlbedoLoc;      // texture image
GLuint planePosLoc;         // slime pos vertex shader
GLuint planePosCompLoc;     // slime pos compute shader







// G buffer
unsigned int gBuffer;
unsigned int gPosition, gNormal, gDiffuse, gSpecular, gView;
unsigned int rboDepth;


// quad
MyShader* deferredShaderPointer;
GLuint quadVAO = 0;
GLuint quadVBO;

// uniform location
GLuint quadPosLoc;
GLuint quadNormalLoc;
GLuint quadAlbedoLoc;
GLuint quadSpecularLoc;

// switch mode
int deferredFlag = 1;
GLuint deferredFlagLoc;
int lastModeT = 4;
bool normalMappingEnabled = false;
bool cascadeEnabled = false;


// normal mapping
std::vector<float> rockTangent;
unsigned char* rockNormalTextureData;
GLuint rockNormalTextureHandle;
GLuint rockTboHandle;
GLuint rockNormalLoc;
GLint normalMappingEnabledLoc;

// shadow mapping
unsigned int depthMapFBO;
unsigned int depthMap;
MyShader* grassShadowShaderPointer;
//GLuint lightLoc;
//glm::mat4 lightSpaceMatrix;
//GLuint quadDepthMapLoc;


MyShader* rockShadowShaderPointer;
MyShader* planeShadowShaderPointer;

// cascaded shadow map
GLuint lightFBO;
GLuint lightDepthMaps;
int depthMapResolution = 4096;
float cameraNearPlane = 1.0f;
float cameraFarPlane = 500.0f;
std::vector<float> shadowCascadeLevels{ 50.0, 200.0 }; // cameraFarPlane = 500.0
unsigned int matricesUBO;
glm::vec3 lightDir = glm::vec3(0.4, 0.5, 0.8);




void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void loadmodel(std::string model_path, int id)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path,
		aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	//std::cout << "Model has " << scene->mNumMeshes << " meshes." << std::endl;

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];
		indexCount[id] = mesh->mNumFaces * 3;
		vertexCount[id] = mesh->mNumVertices;
		std::cout << "Mesh " << m << " has " << mesh->mNumVertices << " vertices." << std::endl;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertexData.push_back(mesh->mVertices[i].x);
			vertexData.push_back(mesh->mVertices[i].y);
			vertexData.push_back(mesh->mVertices[i].z);
			/*std::cout << "Vertex " << i << ": ("
			  << mesh->mVertices[i].x << ", "
			  << mesh->mVertices[i].y << ", "
			  << mesh->mVertices[i].z << ")" << std::endl;*/
			uvData.push_back(mesh->mTextureCoords[0][i].x);
			uvData.push_back(mesh->mTextureCoords[0][i].y);
			uvData.push_back(id);
			/*std::cout << "Texture Coord " << i << ": ("
			 << mesh->mTextureCoords[0][i].x << ", "
			 << mesh->mTextureCoords[0][i].y << ")" << std::endl;*/
			normalData.push_back(mesh->mNormals[i].x);
			normalData.push_back(mesh->mNormals[i].y);
			normalData.push_back(mesh->mNormals[i].z);
			/*std::cout << "normal " << i << ": ("
			 << mesh->mNormals[i].x << ", "
			 << mesh->mNormals[i].y << ", "
			 << mesh->mNormals[i].z << ")" << std::endl;*/
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
				//std::cout << face.mIndices[j] << std::endl;
			}
		}
	}
}

void loadtexture()
{
	// load image
	std::string image_paths[NUM_TEXTURE] = {
	 "assets/bush05.png",
	 "assets/Medieval_Building_LowPoly/Medieval_Building_LowPoly_V1_Albedo_small.png",
	 "assets/Medieval_Building_LowPoly/Medieval_Building_LowPoly_V2_Albedo_small.png",
	 "assets/bush01.png",
	 "assets/grassB_albedo.png",
	};
	// merge the textures to the texture array data
	int width, height, comp;
	for (int i = 0; i < NUM_TEXTURE; ++i) {
		stbi_uc* img = stbi_load(image_paths[i].c_str(), &width, &height, &comp, STBI_rgb_alpha);
		if (img && width == IMG_WIDTH && height == IMG_HEIGHT) {
			memcpy(textureArrayData + i * IMG_WIDTH * IMG_HEIGHT * IMG_CHANNEL, img, IMG_WIDTH * IMG_HEIGHT * IMG_CHANNEL);
			stbi_image_free(img);
		}
		else {
			std::cerr << "Failed to load image or size mismatch for " << image_paths[i] << std::endl;
			if (img) stbi_image_free(img);
		}
	}

	glGenTextures(1, &textureArrayHandle);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayHandle);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 2, GL_RGBA8, IMG_WIDTH, IMG_HEIGHT, NUM_TEXTURE);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, IMG_WIDTH, IMG_HEIGHT, NUM_TEXTURE, GL_RGBA, GL_UNSIGNED_BYTE, textureArrayData);
	///////////////
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// IMPORTANT !! Use mipmap for the foliage rendering
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void loadRockModel()
{
	std::string model_path = "assets/MagicRock/magicRock.obj";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path,
		aiProcess_Triangulate || aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	//cout << "Model has " << scene->mNumMeshes << " meshes." << endl;
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];
		rockIndexCount = mesh->mNumFaces * 3;
		rockVertexCount = mesh->mNumVertices;
		std::cout << "rock Mesh " << m << " has " << mesh->mNumVertices << " vertices." << std::endl;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			rockVertexData.push_back(mesh->mVertices[i].x);
			rockVertexData.push_back(mesh->mVertices[i].y);
			rockVertexData.push_back(mesh->mVertices[i].z);

			/*std::cout << "Vertex " << i << ": ("
			  << mesh->mVertices[i].x << ", "
			  << mesh->mVertices[i].y << ", "
			  << mesh->mVertices[i].z << ")" << std::endl;*/

			rockUvData.push_back(mesh->mTextureCoords[0][i].x);
			rockUvData.push_back(mesh->mTextureCoords[0][i].y);

			/*std::cout << "Texture Coord " << i << ": ("
			 << mesh->mTextureCoords[0][i].x << ", "
			 << mesh->mTextureCoords[0][i].y << ")" << std::endl;*/
			 //float X = mesh->mVertices[i].x;
			 //float Y = mesh->mVertices[i].y;
			 //float Z = mesh->mVertices[i].z;


			rockNormalData.push_back(mesh->mNormals[i].x);
			rockNormalData.push_back(mesh->mNormals[i].y);
			rockNormalData.push_back(mesh->mNormals[i].z);
			//if (i % 3 == 0) {
			//	A.x = mesh->mVertices[i].x;
			//	A.y = mesh->mVertices[i].y;
			//	A.z = mesh->mVertices[i].z;

			//}
			//if (i % 3 == 1) {
			//	B.x = mesh->mVertices[i].x;
			//	B.y = mesh->mVertices[i].y;
			//	B.z = mesh->mVertices[i].z;
			//}
			//if (i % 3 == 2) {
			//	C.x = mesh->mVertices[i].x;
			//	C.y = mesh->mVertices[i].y;
			//	C.z = mesh->mVertices[i].z;
			//	// compute
			//	glm::vec3 U = B - A;
			//	glm::vec3 V = C - A;
			//	U = glm::normalize(U);
			//	V = glm::normalize(V);

			//	glm::vec3 Z = glm::cross(U, V);
			//	Z = glm::normalize(Z);
			//	rockNormalData.push_back(Z.x);
			//	rockNormalData.push_back(Z.y);
			//	rockNormalData.push_back(Z.z);
			//	rockNormalData.push_back(Z.x);
			//	rockNormalData.push_back(Z.y);
			//	rockNormalData.push_back(Z.z);
			//	rockNormalData.push_back(Z.x);
			//	rockNormalData.push_back(Z.y);
			//	rockNormalData.push_back(Z.z);

			//	/*rockTangent.push_back(V.x);
			//	rockTangent.push_back(V.y);
			//	rockTangent.push_back(V.z);
			//	rockTangent.push_back(V.x);
			//	rockTangent.push_back(V.y);
			//	rockTangent.push_back(V.z);
			//	rockTangent.push_back(V.x);
			//	rockTangent.push_back(V.y);
			//	rockTangent.push_back(V.z);*/
			//}

			/*std::cout << "normal " << i << ": ("
			 << mesh->mNormals[i].x << ", "
			 << mesh->mNormals[i].y << ", "
			 << mesh->mNormals[i].z << ")" << std::endl;*/
			rockTangent.push_back(mesh->mTangents[i].x);
			rockTangent.push_back(mesh->mTangents[i].y);
			rockTangent.push_back(mesh->mTangents[i].z);
		}
		//std::cout << mesh->mNumFaces << std::endl;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				//std::cout << i << " mFaces "<< ": " << face.mIndices[j] << std::endl;
				rockIndices.push_back(face.mIndices[j]);
				//std::cout << i << " mFaces "<< ": " << face.mIndices[j] << std::endl;

			}
		}
	}
	std::cout << "num of vertices: " << rockVertexData.size() << std::endl;
	std::cout << "num of normals: " << rockNormalData.size() << std::endl;

}

void loadRockTexture()
{
	glGenTextures(1, &rockTextureHandle);
	glBindTexture(GL_TEXTURE_2D, rockTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	rockTextureData = stbi_load("assets/MagicRock/StylMagicRocks_AlbedoTransparency.png", &width, &height, &nrChannels, 0);
	if (rockTextureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rockTextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(rockTextureData);
}

void loadRockNormalTexture()
{
	glGenTextures(1, &rockNormalTextureHandle);
	glBindTexture(GL_TEXTURE_2D, rockNormalTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	rockNormalTextureData = stbi_load("assets/MagicRock/StylMagicRocks_NormalOpenGL.png", &width, &height, &nrChannels, 0);
	if (rockNormalTextureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rockNormalTextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(rockNormalTextureData);
}

void loadPlaneModel()
{
	std::string model_path = "assets/airplane.obj";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path,
		aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	//cout << "Model has " << scene->mNumMeshes << " meshes." << endl;

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];
		planeIndexCount = mesh->mNumFaces * 3;
		planeVertexCount = mesh->mNumVertices;
		std::cout << "Plane Mesh " << m << " has " << mesh->mNumVertices << " vertices." << std::endl;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			planeVertexData.push_back(mesh->mVertices[i].x);
			planeVertexData.push_back(mesh->mVertices[i].y);
			planeVertexData.push_back(mesh->mVertices[i].z);

			/*std::cout << "Vertex " << i << ": ("
			  << mesh->mVertices[i].x << ", "
			  << mesh->mVertices[i].y << ", "
			  << mesh->mVertices[i].z << ")" << std::endl;*/

			planeUvData.push_back(mesh->mTextureCoords[0][i].x);
			planeUvData.push_back(mesh->mTextureCoords[0][i].y);

			/*std::cout << "Texture Coord " << i << ": ("
			 << mesh->mTextureCoords[0][i].x << ", "
			 << mesh->mTextureCoords[0][i].y << ")" << std::endl;*/
			planeNormalData.push_back(mesh->mNormals[i].x);
			planeNormalData.push_back(mesh->mNormals[i].y);
			planeNormalData.push_back(mesh->mNormals[i].z);

			/*std::cout << "normal " << i << ": ("
			 << mesh->mNormals[i].x << ", "
			 << mesh->mNormals[i].y << ", "
			 << mesh->mNormals[i].z << ")" << std::endl;*/
		}
		//std::cout << mesh->mNumFaces << std::endl;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				//std::cout << i << " mFaces "<< ": " << face.mIndices[j] << std::endl;
				planeIndices.push_back(face.mIndices[j]);
			}
		}
	}
}

void loadPlaneTexture()
{
	glGenTextures(1, &planeTextureHandle);
	glBindTexture(GL_TEXTURE_2D, planeTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	planeTextureData = stbi_load("assets/Airplane_smooth_DefaultMaterial_BaseMap.jpg", &width, &height, &nrChannels, 0);
	if (planeTextureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, planeTextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(planeTextureData);
}

void initSample() {
	MyPoissonSample* sample0 = MyPoissonSample::fromFile("assets/poissonPoints_1010.ppd2");
	MyPoissonSample* sample1 = MyPoissonSample::fromFile("assets/cityLots_sub_0.ppd2");
	MyPoissonSample* sample2 = MyPoissonSample::fromFile("assets/cityLots_sub_1.ppd2");
	MyPoissonSample* sample3 = MyPoissonSample::fromFile("assets/poissonPoints_2797.ppd2");
	MyPoissonSample* sample4 = MyPoissonSample::fromFile("assets/poissonPoints_621043_after.ppd2");

	NUM_SAMPLE0 = sample0->m_numSample;
	NUM_SAMPLE1 = sample1->m_numSample;
	NUM_SAMPLE2 = sample2->m_numSample;
	NUM_SAMPLE3 = sample3->m_numSample;
	NUM_SAMPLE4 = sample4->m_numSample;

	NUM_TOTAL_INSTANCE = NUM_SAMPLE0 + NUM_SAMPLE1 + NUM_SAMPLE2 + NUM_SAMPLE3 + NUM_SAMPLE4;
	rawInsData = new InstanceProperties[NUM_TOTAL_INSTANCE];
	rotCol0 = new Colume[NUM_TOTAL_INSTANCE];
	rotCol1 = new Colume[NUM_TOTAL_INSTANCE];
	rotCol2 = new Colume[NUM_TOTAL_INSTANCE];
	rotCol3 = new Colume[NUM_TOTAL_INSTANCE];


	// query position and rotation
	int k = 0;
	for (int i = 0; i < sample0->m_numSample; i++) {
		glm::vec3 position(sample0->m_positions[i * 3 + 0],
			sample0->m_positions[i * 3 + 1],
			sample0->m_positions[i * 3 + 2]);
		glm::vec3 rad(sample0->m_radians[i * 3 + 0],
			sample0->m_radians[i * 3 + 1],
			sample0->m_radians[i * 3 + 2]);


		// calculate rotation matrix
		glm::quat q = glm::quat(rad);
		glm::mat4 rotationMatrix = glm::toMat4(q);

		// 存點的位置和旋轉
		rawInsData[k].position = glm::vec4(position, 1.0);
		rotCol0[k].vector = rotationMatrix[0];
		rotCol1[k].vector = rotationMatrix[1];
		rotCol2[k].vector = rotationMatrix[2];
		rotCol3[k].vector = rotationMatrix[3];

		k++;
	}
	for (int i = 0; i < sample1->m_numSample; i++) {
		glm::vec3 position(sample1->m_positions[i * 3 + 0],
			sample1->m_positions[i * 3 + 1],
			sample1->m_positions[i * 3 + 2]);
		glm::vec3 rad(sample1->m_radians[i * 3 + 0],
			sample1->m_radians[i * 3 + 1],
			sample1->m_radians[i * 3 + 2]);
		// calculate rotation matrix
		glm::quat q = glm::quat(rad);
		glm::mat4 rotationMatrix = glm::toMat4(q);

		// 存點的位置和旋轉
		rawInsData[k].position = glm::vec4(position, 1.0);
		rotCol0[k].vector = rotationMatrix[0];
		rotCol1[k].vector = rotationMatrix[1];
		rotCol2[k].vector = rotationMatrix[2];
		rotCol3[k].vector = rotationMatrix[3];
		k++;
	}
	for (int i = 0; i < sample2->m_numSample; i++) {
		glm::vec3 position(sample2->m_positions[i * 3 + 0],
			sample2->m_positions[i * 3 + 1],
			sample2->m_positions[i * 3 + 2]);
		glm::vec3 rad(sample2->m_radians[i * 3 + 0],
			sample2->m_radians[i * 3 + 1],
			sample2->m_radians[i * 3 + 2]);

		// calculate rotation matrix
		glm::quat q = glm::quat(rad);
		glm::mat4 rotationMatrix = glm::toMat4(q);

		// 存點的位置和旋轉
		rawInsData[k].position = glm::vec4(position, 1.0);
		rotCol0[k].vector = rotationMatrix[0];
		rotCol1[k].vector = rotationMatrix[1];
		rotCol2[k].vector = rotationMatrix[2];
		rotCol3[k].vector = rotationMatrix[3];
		k++;
	}
	for (int i = 0; i < sample3->m_numSample; i++) {
		glm::vec3 position(sample3->m_positions[i * 3 + 0],
			sample3->m_positions[i * 3 + 1],
			sample3->m_positions[i * 3 + 2]);
		glm::vec3 rad(sample3->m_radians[i * 3 + 0],
			sample3->m_radians[i * 3 + 1],
			sample3->m_radians[i * 3 + 2]);



		// calculate rotation matrix
		glm::quat q = glm::quat(rad);
		glm::mat4 rotationMatrix = glm::toMat4(q);

		// 存點的位置
		rawInsData[k].position = glm::vec4(position, 1.0);
		rotCol0[k].vector = rotationMatrix[0];
		rotCol1[k].vector = rotationMatrix[1];
		rotCol2[k].vector = rotationMatrix[2];
		rotCol3[k].vector = rotationMatrix[3];
		k++;
	}
	for (int i = 0; i < sample4->m_numSample; i++) {
		glm::vec3 position(sample4->m_positions[i * 3 + 0],
			sample4->m_positions[i * 3 + 1],
			sample4->m_positions[i * 3 + 2]);
		glm::vec3 rad(sample4->m_radians[i * 3 + 0],
			sample4->m_radians[i * 3 + 1],
			sample4->m_radians[i * 3 + 2]);

		// calculate rotation matrix

		glm::quat q = glm::quat(rad);
		glm::mat4 rotationMatrix = glm::toMat4(q);

		// 存點的位置
		rawInsData[k].position = glm::vec4(position, 1.0);
		rotCol0[k].vector = rotationMatrix[0];
		rotCol1[k].vector = rotationMatrix[1];
		rotCol2[k].vector = rotationMatrix[2];
		rotCol3[k].vector = rotationMatrix[3];
		k++;
	}
}

void genTexture(int w, int h) {
	// G buffer
	int SCR_WIDTH = w / 2;
	int SCR_HEIGHT = h;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// specular + shininess color buffer
	glGenTextures(1, &gSpecular);
	glBindTexture(GL_TEXTURE_2D, gSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gSpecular, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color color buffer
	glGenTextures(1, &gDiffuse);
	glBindTexture(GL_TEXTURE_2D, gDiffuse);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuse, 0);
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gPosition, 0);
	// view
	/*glGenTextures(1, &gView);
	glBindTexture(GL_TEXTURE_2D, gView);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gView, 0);*/





	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
									GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
	};
	glDrawBuffers(4, attachments);
	// create and attach depth buffer (renderbuffer)

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rockInit() {
	// initialize VAO
	glGenVertexArrays(1, &rockVaoHandle);
	glBindVertexArray(rockVaoHandle);
	// pos
	glGenBuffers(1, &rockVboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, rockVboHandle);
	glBufferData(GL_ARRAY_BUFFER, rockVertexData.size() * sizeof(float), rockVertexData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal
	glGenBuffers(1, &rockNboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, rockNboHandle);
	glBufferData(GL_ARRAY_BUFFER, rockNormalData.size() * sizeof(float), rockNormalData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	// uv
	glGenBuffers(1, &rockUvboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, rockUvboHandle);
	glBufferData(GL_ARRAY_BUFFER, rockUvData.size() * sizeof(float), rockUvData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	// tangent
	glGenBuffers(1, &rockTboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, rockTboHandle);
	glBufferData(GL_ARRAY_BUFFER, rockTangent.size() * sizeof(float), rockTangent.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	// index
	glGenBuffers(1, &rockIboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rockIboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, rockIndices.size() * sizeof(float), rockIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void planeInit() {
	// initialize VAO
	glGenVertexArrays(1, &planeVaoHandle);
	glBindVertexArray(planeVaoHandle);
	// bind the VBO、UVBO、NBO
	glGenBuffers(1, &planeVboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, planeVboHandle);
	glBufferData(GL_ARRAY_BUFFER, planeVertexData.size() * sizeof(float), planeVertexData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &planeUvboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, planeUvboHandle);
	glBufferData(GL_ARRAY_BUFFER, planeUvData.size() * sizeof(float), planeUvData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &planeNboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, planeNboHandle);
	glBufferData(GL_ARRAY_BUFFER, planeNormalData.size() * sizeof(float), planeNormalData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &planeIboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeIndices.size() * sizeof(float), planeIndices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void initialize() {
	// prepare a SSBO for storing raw instance data
	GLuint rawInstanceDataBufferHandle;
	glGenBuffers(1, &rawInstanceDataBufferHandle);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rawInstanceDataBufferHandle);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(InstanceProperties), rawInsData, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, rawInstanceDataBufferHandle);
	// prepare a SSBO for storing VALID instance data
	GLuint validInstanceDataBufferHandle;
	glGenBuffers(1, &validInstanceDataBufferHandle);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, validInstanceDataBufferHandle);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(InstanceProperties), nullptr, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, validInstanceDataBufferHandle);


	// SSBO col0
	GLuint rawRotColHandle0;
	glGenBuffers(1, &rawRotColHandle0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rawRotColHandle0);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), rotCol0, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, rawRotColHandle0);
	GLuint validRotColHandle0;
	glGenBuffers(1, &validRotColHandle0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, validRotColHandle0);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), nullptr, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, validRotColHandle0);
	// SSBO col1
	GLuint rawRotColHandle1;
	glGenBuffers(1, &rawRotColHandle1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rawRotColHandle1);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), rotCol1, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, rawRotColHandle1);
	GLuint validRotColHandle1;
	glGenBuffers(1, &validRotColHandle1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, validRotColHandle1);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), nullptr, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, validRotColHandle1);
	// SSBO col2
	GLuint rawRotColHandle2;
	glGenBuffers(1, &rawRotColHandle2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rawRotColHandle2);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), rotCol2, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, rawRotColHandle2);
	GLuint validRotColHandle2;
	glGenBuffers(1, &validRotColHandle2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, validRotColHandle2);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), nullptr, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, validRotColHandle2);
	// SSBO col3
	GLuint rawRotColHandle3;
	glGenBuffers(1, &rawRotColHandle3);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rawRotColHandle3);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), rotCol3, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, rawRotColHandle3);
	GLuint validRotColHandle3;
	glGenBuffers(1, &validRotColHandle3);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, validRotColHandle3);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, NUM_TOTAL_INSTANCE * sizeof(Colume), nullptr, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, validRotColHandle3);

	drawCommands[0].count = indexCount[0];
	drawCommands[0].instanceCount = 0;
	drawCommands[0].firstIndex = 0;
	drawCommands[0].baseVertex = 0;
	drawCommands[0].baseInstance = 0;

	drawCommands[1].count = indexCount[1];
	drawCommands[1].instanceCount = 0;
	drawCommands[1].firstIndex = indexCount[0];
	drawCommands[1].baseVertex = vertexCount[0];
	drawCommands[1].baseInstance = NUM_SAMPLE0;

	drawCommands[2].count = indexCount[2];
	drawCommands[2].instanceCount = 0;
	drawCommands[2].firstIndex = indexCount[0] + indexCount[1];
	drawCommands[2].baseVertex = vertexCount[0] + vertexCount[1];
	drawCommands[2].baseInstance = NUM_SAMPLE0 + NUM_SAMPLE1;

	drawCommands[3].count = indexCount[3];
	drawCommands[3].instanceCount = 0;
	drawCommands[3].firstIndex = indexCount[0] + indexCount[1] + indexCount[2];
	drawCommands[3].baseVertex = vertexCount[0] + vertexCount[1] + vertexCount[2];
	drawCommands[3].baseInstance = NUM_SAMPLE0 + NUM_SAMPLE1 + NUM_SAMPLE2;

	drawCommands[4].count = indexCount[4];
	drawCommands[4].instanceCount = 0;
	drawCommands[4].firstIndex = indexCount[0] + indexCount[1] + indexCount[2] + indexCount[3];
	drawCommands[4].baseVertex = vertexCount[0] + vertexCount[1] + vertexCount[2] + vertexCount[3];
	drawCommands[4].baseInstance = NUM_SAMPLE0 + NUM_SAMPLE1 + NUM_SAMPLE2 + NUM_SAMPLE3;

	glGenBuffers(1, &cmdBufferHandle);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cmdBufferHandle);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(drawCommands), drawCommands, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, cmdBufferHandle);

	// initialize VAO
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	// bind the VBO、UVBO、NBO
	glGenBuffers(1, &vboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &uvboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, uvboHandle);
	glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(float), uvData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &nboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, nboHandle);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), normalData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &iboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

	// use SSBO as vertex attribute input
	glBindBuffer(GL_ARRAY_BUFFER, validInstanceDataBufferHandle);
	glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(InstanceProperties), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	// col0
	glBindBuffer(GL_ARRAY_BUFFER, validRotColHandle0);
	glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(Colume), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	// col1
	glBindBuffer(GL_ARRAY_BUFFER, validRotColHandle1);
	glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(Colume), (void*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);
	//// col2
	glBindBuffer(GL_ARRAY_BUFFER, validRotColHandle2);
	glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(Colume), (void*)0);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);
	//// col3
	glBindBuffer(GL_ARRAY_BUFFER, validRotColHandle3);
	glVertexAttribPointer(7, 4, GL_FLOAT, false, sizeof(Colume), (void*)0);
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);


	// rock
	rockInit();
	planeInit();

}

void setUniformVariables() {
	viewProjMatLoc = glGetUniformLocation(myCompShaderPointer->ID, "viewProjMat");
	viewMatLoc = glGetUniformLocation(myShaderPointer->ID, "viewMat");
	projMatLoc = glGetUniformLocation(myShaderPointer->ID, "projMat");
	modelMatLoc = glGetUniformLocation(myShaderPointer->ID, "modelMat");
	albedoLoc = glGetUniformLocation(myShaderPointer->ID, "albedoTextureArray");

	rockViewMatLoc = glGetUniformLocation(rockShaderPointer->ID, "viewMat");
	rockProjMatLoc = glGetUniformLocation(rockShaderPointer->ID, "projMat");
	rockModelMatLoc = glGetUniformLocation(rockShaderPointer->ID, "modelMat");
	rockAlbedoLoc = glGetUniformLocation(rockShaderPointer->ID, "albedoTexture");
	rockPosLoc = glGetUniformLocation(rockShaderPointer->ID, "v_worldPosOffset");
	rockNormalLoc = glGetUniformLocation(rockShaderPointer->ID, "NormalTexture");
	normalMappingEnabledLoc = glGetUniformLocation(rockShaderPointer->ID, "u_NormalMappingEnabled");

	planeViewMatLoc = glGetUniformLocation(planeShaderPointer->ID, "viewMat");
	planeProjMatLoc = glGetUniformLocation(planeShaderPointer->ID, "projMat");
	planeModelMatLoc = glGetUniformLocation(planeShaderPointer->ID, "modelMat");
	planeAlbedoLoc = glGetUniformLocation(planeShaderPointer->ID, "albedoTexture");
	planePosLoc = glGetUniformLocation(planeShaderPointer->ID, "v_worldPosOffset");

	quadPosLoc = glGetUniformLocation(deferredShaderPointer->ID, "gPosition");
	quadNormalLoc = glGetUniformLocation(deferredShaderPointer->ID, "gNormal");
	quadAlbedoLoc = glGetUniformLocation(deferredShaderPointer->ID, "gDiffuse");
	deferredFlagLoc = glGetUniformLocation(deferredShaderPointer->ID, "flag");
	quadSpecularLoc = glGetUniformLocation(deferredShaderPointer->ID, "gSpecular");

	//lightLoc = glGetUniformLocation(grassShadowShaderPointer->ID, "lightSpaceMatrix");
	//quadDepthMapLoc = glGetUniformLocation(deferredShaderPointer->ID, "depthMap");

}

void genShadowMaps() {
	glGenFramebuffers(1, &lightFBO);

	glGenTextures(1, &lightDepthMaps);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
	glTexImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,
		GL_DEPTH_COMPONENT32F,
		depthMapResolution,
		depthMapResolution,
		int(shadowCascadeLevels.size()) + 1,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lightDepthMaps, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
		throw 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void configureUBO() {
	// configure UBO
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void myInit() {
	loadmodel("assets/bush05_lod2.obj", 0);
	loadmodel("assets/Medieval_Building_LowPoly/medieval_building_lowpoly_1.obj", 1);
	loadmodel("assets/Medieval_Building_LowPoly/medieval_building_lowpoly_2.obj", 2);
	loadmodel("assets/bush01_lod2.obj", 3);
	loadmodel("assets/grassB.obj", 4);
	loadtexture();
	loadRockModel();
	loadRockTexture();
	loadRockNormalTexture();
	loadPlaneModel();
	loadPlaneTexture();

	myShaderPointer = new MyShader("src/shader/grassVertexShader.glsl", "src/shader/grassFragmentShader.glsl");
	rockShaderPointer = new MyShader("src/shader/rockVertexShader.glsl", "src/shader/rockFragmentShader.glsl");
	planeShaderPointer = new MyShader("src/shader/planeVertexShader.glsl", "src/shader/planeFragmentShader.glsl");
	myCompShaderPointer = new MyComputeShader("src/shader/myComputeShader.glsl");
	myResetCompShaderPointer = new MyComputeShader("src/shader/myResetComputeShader.glsl");
	deferredShaderPointer = new MyShader("src/shader/deferredVertexShader.glsl", "src/shader/deferredFragmentShader.glsl");
	grassShadowShaderPointer = new MyShader("src/shader/grassShadowVertexShader.glsl",
		                                    "src/shader/grassShadowFragmentShader.glsl",
		                                    "src/shader/grassShadowGeometryShader.glsl");
	rockShadowShaderPointer = new MyShader("src/shader/rockShadowVertexShader.glsl",
		                                   "src/shader/rockShadowFragmentShader.glsl",
		                                   "src/shader/rockShadowGeometryShader.glsl");

	planeShadowShaderPointer = new MyShader("src/shader/planeShadowVertexShader.glsl",
		                                    "src/shader/planeShadowFragmentShader.glsl",
		                                    "src/shader/planeShadowGeometryShader.glsl");



	initSample();
	initialize();
	genTexture(1024, 512);
	setUniformVariables();

	// shadow map
	genShadowMaps();
	configureUBO();
}

void myComputeRender(const INANOA::MyCameraManager* m_myCameraManager) {
	glm::mat4 playerProjectionMatrix = m_myCameraManager->playerProjectionMatrix();
	glm::mat4 playerViewMatrix = m_myCameraManager->playerViewMatrix();
	// reset shader
	myResetCompShaderPointer->use();
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// compute shader
	myCompShaderPointer->use();
	glUniform1i(glGetUniformLocation(myCompShaderPointer->ID, "numMaxInstance"), NUM_TOTAL_INSTANCE);
	const glm::mat4  viewProj = playerProjectionMatrix * playerViewMatrix;
	GLuint viewProjMatLoc = glGetUniformLocation(myCompShaderPointer->ID, "viewProjMat");
	glUniformMatrix4fv(viewProjMatLoc, 1, false, glm::value_ptr(viewProj));
	glDispatchCompute((NUM_TOTAL_INSTANCE / 1024) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void myUIinput(INANOA::MyCameraManager* m_myCameraManager, MyImGuiPanel* m_imguiPanel) {
	if (lastModeT != m_imguiPanel->getModeT()) {
		switch (m_imguiPanel->getModeT()) {
		case 1:
			m_myCameraManager->teleport(0);
			lastModeT = 1;
			break;
		case 2:
			m_myCameraManager->teleport(1);
			lastModeT = 2;
			break;
		case 3:
			m_myCameraManager->teleport(2);
			lastModeT = 3;
			break;
		}
	}
	normalMappingEnabled = m_imguiPanel->isNormalMappingEnabled();
	cascadeEnabled = m_imguiPanel->isCascadeEnabled();
	deferredFlag = m_imguiPanel->getMode();

}

void myGodGbufferRender(INANOA::MyCameraManager* m_myCameraManager, MyImGuiPanel* m_imguiPanel) {

	glm::mat4 godProjectionMatrix = m_myCameraManager->godProjectionMatrix();
	glm::mat4 godViewMatrix = m_myCameraManager->godViewMatrix();
	glm::vec4 godViewPort = m_myCameraManager->godViewport();
	glm::vec4 playerViewPort = m_myCameraManager->playerViewport();
	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::vec4 positionVec4 = glm::vec4(25.92, 18.27, 11.75, 1.0);
	const glm::mat4 airplaneModelMat = m_myCameraManager->airplaneModelMatrix();
	glm::vec4 planepositionVec4 = glm::vec4(0.0, 0.0, 0.0, 1.0);

	//// 1. geometry pass: render scene's geometry/color data into gbuffer

	// myShader
	myShaderPointer->use();
	glUniformMatrix4fv(projMatLoc, 1, false, glm::value_ptr(godProjectionMatrix));
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(godViewMatrix));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayHandle);
	glUniform1i(albedoLoc, 0);

	// bind VAO
	glBindVertexArray(vaoHandle);
	// bind draw-indirect-buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cmdBufferHandle);
	// render
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, 5, sizeof(DrawElementsIndirectCommand));

	// rock
	rockShaderPointer->use();
	glUniformMatrix4fv(rockProjMatLoc, 1, GL_FALSE, glm::value_ptr(godProjectionMatrix));
	glUniformMatrix4fv(rockViewMatLoc, 1, GL_FALSE, glm::value_ptr(godViewMatrix));
	glUniformMatrix4fv(rockModelMatLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform4fv(rockPosLoc, 1, glm::value_ptr(positionVec4));
	glUniform1i(normalMappingEnabledLoc, normalMappingEnabled);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, rockTextureHandle);
	glUniform1i(rockAlbedoLoc, 3);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rockNormalTextureHandle);//Normal mapping
	glUniform1i(rockNormalLoc, 5);

	glBindVertexArray(rockVaoHandle);

	glDrawElements(GL_TRIANGLES, rockVertexCount, GL_UNSIGNED_INT, 0);

	//plane
	planeShaderPointer->use();
	glUniformMatrix4fv(planeProjMatLoc, 1, GL_FALSE, glm::value_ptr(godProjectionMatrix));
	glUniformMatrix4fv(planeViewMatLoc, 1, GL_FALSE, glm::value_ptr(godViewMatrix));
	glUniformMatrix4fv(planeModelMatLoc, 1, GL_FALSE, glm::value_ptr(airplaneModelMat));
	glUniform4fv(planePosLoc, 1, glm::value_ptr(planepositionVec4));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planeTextureHandle);
	glUniform1i(planeAlbedoLoc, 4);
	glBindVertexArray(planeVaoHandle);

	glDrawElements(GL_TRIANGLES, planeVertexCount, GL_UNSIGNED_INT, 0);
}

void myPlayerGbufferRender(const INANOA::MyCameraManager* m_myCameraManager, MyImGuiPanel* m_imguiPanel) {

	glm::mat4 playerProjectionMatrix = m_myCameraManager->playerProjectionMatrix();
	glm::mat4 playerViewMatrix = m_myCameraManager->playerViewMatrix();
	glm::vec4 godViewPort = m_myCameraManager->godViewport();
	glm::vec4 playerViewPort = m_myCameraManager->playerViewport();

	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::vec4 positionVec4 = glm::vec4(25.92, 18.27, 11.75, 1.0);
	const glm::mat4 airplaneModelMat = m_myCameraManager->airplaneModelMatrix();
	glm::vec4 planepositionVec4 = glm::vec4(0.0, 0.0, 0.0, 1.0);

	//// 1. geometry pass: render scene's geometry/color data into gbuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// myShader
	myShaderPointer->use();
	glUniformMatrix4fv(projMatLoc, 1, false, glm::value_ptr(playerProjectionMatrix));
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(playerViewMatrix));

	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayHandle);
	glUniform1i(albedoLoc, 0);

	// bind VAO
	glBindVertexArray(vaoHandle);
	// bind draw-indirect-buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cmdBufferHandle);
	// render
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, 5, sizeof(DrawElementsIndirectCommand));

	//rock
	rockShaderPointer->use();
	glUniformMatrix4fv(rockProjMatLoc, 1, GL_FALSE, glm::value_ptr(playerProjectionMatrix));
	glUniformMatrix4fv(rockViewMatLoc, 1, GL_FALSE, glm::value_ptr(playerViewMatrix));
	glUniformMatrix4fv(rockModelMatLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform4fv(rockPosLoc, 1, glm::value_ptr(positionVec4));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, rockTextureHandle);
	glUniform1i(rockAlbedoLoc, 3);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rockNormalTextureHandle);//Normal mapping
	glUniform1i(rockNormalLoc, 5);

	glBindVertexArray(rockVaoHandle);

	glDrawElements(GL_TRIANGLES, rockVertexCount, GL_UNSIGNED_INT, 0);


	//plane
	planeShaderPointer->use();
	glUniformMatrix4fv(planeProjMatLoc, 1, GL_FALSE, glm::value_ptr(playerProjectionMatrix));
	glUniformMatrix4fv(planeViewMatLoc, 1, GL_FALSE, glm::value_ptr(playerViewMatrix));
	glUniformMatrix4fv(planeModelMatLoc, 1, GL_FALSE, glm::value_ptr(airplaneModelMat));
	glUniform4fv(planePosLoc, 1, glm::value_ptr(planepositionVec4));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planeTextureHandle);
	glUniform1i(planeAlbedoLoc, 4);
	glBindVertexArray(planeVaoHandle);

	glDrawElements(GL_TRIANGLES, planeVertexCount, GL_UNSIGNED_INT, 0);
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview)
{
	const auto inv = glm::inverse(projview);

	std::vector<glm::vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	glm::mat4 tmp = proj * view;
	return getFrustumCornersWorldSpace(tmp);
}

glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane, INANOA::MyCameraManager* m_myCameraManager)
{
	float w = m_myCameraManager->getPlayerViewport()[2];
	float h = m_myCameraManager->getPlayerViewport()[3];
	const auto proj = glm::perspective(glm::radians(45.0f), w / h, nearPlane, farPlane);
	glm::mat4 view = m_myCameraManager->playerViewMatrix();


	glm::vec3 lightDir = glm::vec3(0.4, 0.5, 0.8);
	const auto corners = getFrustumCornersWorldSpace(proj, view);
	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	const auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 10.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	return lightProjection * lightView;
}

std::vector<glm::mat4> getLightSpaceMatrices(INANOA::MyCameraManager* m_myCameraManager)
{
	std::vector<glm::mat4> ret;
	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
	{
		if (i == 0)
		{
			ret.push_back(getLightSpaceMatrix(cameraNearPlane, shadowCascadeLevels[i], m_myCameraManager));
		}
		else if (i < shadowCascadeLevels.size())
		{
			ret.push_back(getLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i], m_myCameraManager));
		}
		else
		{
			ret.push_back(getLightSpaceMatrix(shadowCascadeLevels[i - 1], cameraFarPlane, m_myCameraManager));
		}
	}

	return ret;
}

void myShadowRender(INANOA::MyCameraManager* m_myCameraManager, MyImGuiPanel* m_imguiPanel) {

	glm::mat4 playerProjectionMatrix = m_myCameraManager->playerProjectionMatrix();
	glm::mat4 playerViewMatrix = m_myCameraManager->playerViewMatrix();
	glm::vec4 godViewPort = m_myCameraManager->godViewport();
	glm::vec4 playerViewPort = m_myCameraManager->playerViewport();

	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::vec4 positionVec4 = glm::vec4(25.92, 18.27, 11.75, 1.0);
	const glm::mat4 airplaneModelMat = m_myCameraManager->airplaneModelMatrix();
	glm::vec4 planepositionVec4 = glm::vec4(0.0, 0.0, 0.0, 1.0);

	// 0. UBO setup
	const auto lightMatrices = getLightSpaceMatrices(m_myCameraManager);

	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	for (size_t i = 0; i < lightMatrices.size(); ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	// render scene from light's point of view

	grassShadowShaderPointer->use();
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, lightDepthMaps, 0);
	glViewport(0, 0, depthMapResolution, depthMapResolution);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);  // peter panning
	// start render 
	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cmdBufferHandle);
	//glUniformMatrix4fv(lightLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, 4, sizeof(DrawElementsIndirectCommand));
	// end render
	glCullFace(GL_BACK);

	
	rockShadowShaderPointer->use();
	rockShadowShaderPointer->setVec4("v_worldPosOffset", positionVec4);
	//glUniform4fv(glGetUniformLocation(rockShadowShaderPointer->ID, "v_worldPosOffset"), 1, glm::value_ptr(positionVec4));

	////glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, lightDepthMaps, 0);
	////glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	////glViewport(0, 0, depthMapResolution, depthMapResolution);
	////glClear(GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(rockVaoHandle);
	glCullFace(GL_FRONT);
	glDrawElements(GL_TRIANGLES, rockVertexCount, GL_UNSIGNED_INT, 0);
	glCullFace(GL_BACK);

	// plane
	planeShadowShaderPointer->use();
	glBindVertexArray(planeVaoHandle);
	planeShadowShaderPointer->setMat4("modelMat", airplaneModelMat);
	glCullFace(GL_FRONT);
	glDrawElements(GL_TRIANGLES, planeVertexCount, GL_UNSIGNED_INT, 0);
	glCullFace(GL_BACK);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport 應該不用，之後render 前會改
	//glViewport(0, 0, fb_width, fb_height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void myGodRender(INANOA::MyCameraManager* m_myCameraManager, MyImGuiPanel* m_imguiPanel) {
	glm::mat4 godProjectionMatrix = m_myCameraManager->godProjectionMatrix();
	glm::mat4 godViewMatrix = m_myCameraManager->godViewMatrix();
	glm::vec4 godViewPort = m_myCameraManager->godViewport();
	glm::mat4 playerProjectionMatrix = m_myCameraManager->playerProjectionMatrix();
	glm::mat4 playerViewMatrix = m_myCameraManager->playerViewMatrix();
	glm::vec4 playerViewPort = m_myCameraManager->playerViewport();

	// debug start

	// 2. render scene as normal using the generated depth/shadow map  
	glViewport(godViewPort[0], godViewPort[1], godViewPort[2], godViewPort[3]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClear(GL_DEPTH_BUFFER_BIT);
	deferredShaderPointer->use();

	const glm::mat4 projection = playerProjectionMatrix;  // 應該是給 player view 的
	const glm::mat4 view = playerViewMatrix;
	deferredShaderPointer->setMat4("projection", projection);
	deferredShaderPointer->setMat4("view", view);

	// set light uniforms
	deferredShaderPointer->setVec3("viewPos", m_myCameraManager->playerViewOrig());
	deferredShaderPointer->setVec3("lightDir", lightDir);
	deferredShaderPointer->setFloat("farPlane", cameraFarPlane);
	deferredShaderPointer->setInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		deferredShaderPointer->setFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}

	// set visualization corners 


	// 原本的 G buffer
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, gDiffuse);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, gSpecular);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);

	glUniform1i(quadPosLoc, 5);
	glUniform1i(quadNormalLoc, 6);
	glUniform1i(quadAlbedoLoc, 7);
	glUniform1i(quadSpecularLoc, 8);
	deferredShaderPointer->setInt("shadowMap", 10);
	glUniform1i(deferredFlagLoc, deferredFlag);
	deferredShaderPointer->setBool("cascadeFlag", cascadeEnabled);
	renderQuad();
}

void myPlayerRender(const INANOA::MyCameraManager* m_myCameraManager, MyImGuiPanel* m_imguiPanel) {
	glm::mat4 godProjectionMatrix = m_myCameraManager->godProjectionMatrix();
	glm::mat4 godViewMatrix = m_myCameraManager->godViewMatrix();
	glm::vec4 godViewPort = m_myCameraManager->godViewport();
	glm::mat4 playerProjectionMatrix = m_myCameraManager->playerProjectionMatrix();
	glm::mat4 playerViewMatrix = m_myCameraManager->playerViewMatrix();
	glm::vec4 playerViewPort = m_myCameraManager->playerViewport();

	// 2. render scene as normal using the generated depth/shadow map
	glViewport(playerViewPort[0], playerViewPort[1], playerViewPort[2], playerViewPort[3]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClear(GL_DEPTH_BUFFER_BIT);
	deferredShaderPointer->use();

	const glm::mat4 projection = playerProjectionMatrix;  // 應該是給 player view 的
	const glm::mat4 view = playerViewMatrix;
	deferredShaderPointer->setMat4("projection", projection);
	deferredShaderPointer->setMat4("view", view);

	// set light uniforms
	deferredShaderPointer->setVec3("viewPos", m_myCameraManager->playerViewOrig());
	deferredShaderPointer->setVec3("lightDir", lightDir);
	deferredShaderPointer->setFloat("farPlane", cameraFarPlane);
	deferredShaderPointer->setInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		deferredShaderPointer->setFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}

	// 原本的 G buffer
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, gDiffuse);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, gSpecular);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);

	glUniform1i(quadPosLoc, 5);
	glUniform1i(quadNormalLoc, 6);
	glUniform1i(quadAlbedoLoc, 7);
	glUniform1i(quadSpecularLoc, 8);
	deferredShaderPointer->setInt("shadowMap", 10);
	glUniform1i(deferredFlagLoc, deferredFlag);
	//deferredShaderPointer->setBool("cascade", cascadeEnabled);
	renderQuad();

}

