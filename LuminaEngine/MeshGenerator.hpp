#ifndef MESHGENERATOR_HPP
#define MESHGENERATOR_HPP

#include "Config.hpp"
#include "Vertex.hpp"

NS_BEGIN

class MeshGenerator
{
public:
	/// <summary>
	/// Creates the mesh data for a sphere primitive
	/// </summary>
	static void CreateSphere(float radius, UINT numSubdivisions, MeshData& data);

	/// <summary>
	/// Creates the mesh data for a plane primitive
	/// </summary>
	static void CreatePlane(float width, float depth, UINT n, UINT m, MeshData& data);

	/// <summary>
	/// Creates the mesh data for a cube primitive
	/// </summary>
	static void CreateCube(float width, float depth, float height, MeshData& data);

	/// <summary>
	/// Creates the mesh data for a point (literally one Vertex)
	/// </summary>
	static void CreatePoint(MeshData& data);
private:
	/// <summary>
	/// Subdivides the faces of a mesh
	/// </summary>
	static void Subdivide(MeshData& mesh);
};

NS_END

#endif