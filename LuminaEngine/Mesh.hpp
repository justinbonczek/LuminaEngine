#ifndef MESH_H
#define MESH_H

#include "Config.hpp"
#include "Vertex.hpp"
#include "GraphicsDevice.hpp"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

NS_BEGIN

class Mesh
{
public:
	Mesh::Mesh(std::string filepath, ID3D11Device* dev);
	Mesh(MeshData& mesh, GraphicsDevice* graphicsDevice);

	~Mesh();

	/// <summary>
	/// Returns number of vertices in the mesh
	/// </summary>
	UINT GetNumVertices();

	/// <summary>
	/// Returns number of indices in the mesh
	/// </summary>
	UINT GetNumIndices();

	/// <summary>
	/// Returns a pointer to the vertex buffer of the mesh
	/// </summary>
	ID3D11Buffer* VBuffer();

	/// <summary>
	/// Returns a pointer to the index buffer of the mesh
	/// </summary>
	ID3D11Buffer* IBuffer();
private:
	/// <summary>
	/// Uses the Open Asset Import Library to load a scene from a file and process the data
	/// </summary>
	void ProcessScene(aiNode* node, const aiScene* scene);

	/// <summary>
	/// Used in ProcessScene(..,..) to process mesh data
	/// </summary>
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	std::vector<Vertex> _vertices;
	std::vector<UINT>  _indices;

	UINT numVertices;
	UINT numIndices;
};
NS_END
#endif