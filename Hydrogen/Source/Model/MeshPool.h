#pragma once

#include "Common.h"
#include "Core/ResourcePool.h"
#include "Geometry/Mesh.h"

namespace Hydrogen
{

	class MeshPool
	{
	public:


		HYD  MeshPool() = default;
		HYD ~MeshPool() = default;


		//Init & Destruct

		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		HYD Id     CreateMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives);
		HYD Id     PushMesh(Mesh** pMesh);
		HYD uint32 PopMesh(Id* pId);

		HYD uint32 BakeTransform(const Transformation& pTransform);
		HYD uint32 RenderMeshes(Transformation& pTransform= Transformation());

		HYD Mesh& GetMesh(const std::string& pName) noexcept;
		HYD Mesh& GetMesh(const Id pId) noexcept;

		HYD Mesh& operator[](const std::string& pName);
		HYD Mesh& operator[](const Id pId);

		HYD const std::unordered_map<Id, int32>::const_iterator begin() const;
		HYD const std::unordered_map<Id, int32>::const_iterator end() const;

		HYD MeshPool(MeshPool&& pOther);
		HYD MeshPool(const MeshPool& pOther) = delete;

		HYD MeshPool& operator=(MeshPool&& pOther);
		HYD MeshPool& operator=(const MeshPool& pOther) = delete;



	private:
		ResourcePool<Mesh>					m_MeshePool;
		std::unordered_map<std::string, Id> m_MeshTable;

		friend class GLTFLoader;
	};

};