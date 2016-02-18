#include "Physx.h"

/*  Author by JH */

void getMeshInfo(const Ogre::MeshPtr mesh,
	size_t &vertex_count,
	Ogre::Vector3* &vertices,
	size_t &index_count,
	unsigned long* &indices,
	const Ogre::Vector3 &position = Vector3::ZERO,
	const Ogre::Quaternion &orient = Quaternion::IDENTITY,
	const Ogre::Vector3 &scale = Vector3::UNIT_SCALE)
{
	bool   added_shared   = false;
	size_t current_offset = 0;
	size_t shared_offset  = 0;
	size_t next_offset    = 0;
	size_t index_offset   = 0;
	vertex_count = index_count = 0;
	// Calculate how many vertices and indices we're going to need
	for(unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// We noly need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if(!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}
		// Add the indices
		index_count += submesh->indexData->indexCount;
	}
	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices  = new unsigned long[index_count];
	added_shared = false;
	// Run through the submeshes again, adding the data into arrays
	for(unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh*    submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		if( (!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared) )
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}
			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			float* pReal;
			for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}
			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}
		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
		unsigned long*  pLong  = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
		size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;
		if(use32bitindexes)
		{
			for(size_t k = 0; k < numTris * 3; ++k)
			{
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for(size_t k = 0; k < numTris * 3; ++k)
			{
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}
		ibuf->unlock();
		current_offset = next_offset;
	}
}


Physx::Physx(void)
{
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

	//创建Foundation
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if(!mFoundation)
		MessageBoxA(0,"PxCreateFoundation failed!","Message",0);

	bool recordMemoryAllocations = true;
	PxProfileZoneManager * mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
	if(!mProfileZoneManager)
		MessageBoxA(0,"PxProfileZoneManager::createProfileZoneManager failed!","Message",0);

	//创建Physx
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager );
	if(!mPhysics)
		MessageBoxA(0,"PxCreatePhysics failed!","Message",0);

	//Cooking和Extensions
	mCooking= PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams());
	if (!mCooking)
		MessageBoxA(0,"PxCreateCooking failed!","Message",0);
	if (!PxInitExtensions(*mPhysics))
		MessageBoxA(0,"PxInitExtensions failed!","Message",0);


	/// 创建场景		
	//Scene的属性描述结构体
	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f); // 设置重力
	if (!sceneDesc.filterShader)
	{
		sceneDesc.filterShader = gDefaultFilterShader;
	}

	//创建CPU分发器，PxDefaultCpuDispatcherCreate的参数是线程数
	pxtask::CpuDispatcher* mCpuDispatcher;

	if (!sceneDesc.cpuDispatcher)
	{
		mCpuDispatcher = PxDefaultCpuDispatcherCreate(4);
		if (!mCpuDispatcher){
			MessageBoxA(0,"PxDefaultCpuDispatcherCreate failed!","Message",0);
		}
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

#pragma region CUDA


	//检测并初始化CUDA模块，如果你有支持CUDA的显卡，这里就可以用到了。
	pxtask::CudaContextManagerDesc cudaContextManagerDesc;
	pxtask::CudaContextManager* mCudaContextManager = pxtask::createCudaContextManager(*mFoundation, cudaContextManagerDesc, mProfileZoneManager);
	if (mCudaContextManager){
		if (!mCudaContextManager->contextIsValid()){
			mCudaContextManager->release();
			mCudaContextManager = NULL;
		}
	}

	//尝试使用GPU分发器
	if (!sceneDesc.gpuDispatcher && mCudaContextManager)
	{
		sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
	}
#pragma endregion CUDA

	//创建Scene
	mScene = mPhysics->createScene(sceneDesc);
	if (!mScene){
		MessageBoxA(0,"createscene failed!","Message",0);
	}

	//创建Material
	mMaterial = mPhysics->createMaterial(0.1f, 0.1f, 0.1f);    //static friction, dynamic friction, restitution
	if(!mMaterial)
		MessageBoxA(0,"createMaterial failed!","Message",0);


	manager = PxCreateControllerManager(*mFoundation);

	
}


/*
	entity ----- ogre实体
	radians ----- 物体要旋转的角度（有可能物体导入ogre时朝向有问题，所以需要调整角度，如果不需要调整，传0）
	position ----- 物体在物理世界当中的初始位置，一般跟ogre物体的位置一致
	actor ----- 静态刚体（physx的变量）
	shape ---- 网格形状，即包围模型的网格，用来做碰撞检测等（physx的变量）
*/
void Physx::createStaticRigid(Entity * entity,float radians,Vector3 position,PxRigidStatic *& actor,PxShape *& shape)
{
	/// Physx 创建场景
	size_t vertex_count, index_count;  
	Vector3 * vertices;  
	unsigned long * indices;  
	getMeshInfo(entity->getMesh(), vertex_count, vertices, index_count, indices); 

	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = vertex_count;  
	meshDesc.points.stride = 3*4;			
	meshDesc.triangles.count = index_count/3;	
	meshDesc.points.data = vertices;		
	meshDesc.triangles.stride = 3*4;
	meshDesc.triangles.data = indices;	
	PxDefaultMemoryOutputStream writeBuffer;

	bool result = mCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!result)
	{
		MessageBoxA(0,"cookTriangleMesh failed!","1",0);
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh * scene_triangle_mesh = mPhysics->createTriangleMesh(readBuffer);
	actor = mPhysics->createRigidStatic(PxTransform::createIdentity());
	shape = actor->createShape(PxTriangleMeshGeometry(scene_triangle_mesh), *mMaterial);
	if (!actor)
	{
		MessageBoxA(0,"create sceneActor failed!","1",0);
	}

	PxVec3 physx_position;
	physx_position.x = position.x;
	physx_position.y = position.y;
	physx_position.z = position.z;
	actor->setGlobalPose(PxTransform(physx_position,PxQuat(radians,PxVec3(0,1,0))));
	mScene->addActor(*actor);
	
}


/*
	entity ----- ogre实体
	scale ----- 物体要放大的倍数，一般跟ogre物体的放大倍数一致（有可能物体导入ogre时模型过小，所以需要调整模型，如果不需要调整，传1）
	position ----- 物体在物理世界当中的初始位置，一般跟ogre物体的位置一致
	actor ----- 静态刚体（physx的变量）
	shape ---- 网格形状，即包围模型的网格，用来做碰撞检测等（physx的变量）
*/
void Physx::createDynamicRigid(Entity * entity,int scale,Vector3 position,PxRigidDynamic *& actor,PxShape *& shape)
{
	/// Physx 创建英雄
	const AxisAlignedBox & box = entity->getBoundingBox();
	Vector3 size = box.getSize();	
	PxReal density = 0.5f;
	PxVec3 physx_size(scale*size.x/2, scale*size.y/2, scale*size.z/2);
	PxVec3 physx_position;
	physx_position.x = position.x;
	physx_position.y = position.y;
	physx_position.z = position.z;
	actor = mPhysics->createRigidDynamic(PxTransform(physx_position));
	if (!actor)
	{
		MessageBoxA(0,"createActor failed!","1",0);
	}
	shape = actor->createShape(PxBoxGeometry(physx_size), *mMaterial);
	PxRigidBodyExt::updateMassAndInertia(*actor, density);
	//mScene->addActor(*actor);
}



void Physx::start_simulate(float elapsedTime)
{
	mScene->simulate(elapsedTime);
	mScene->fetchResults(true);
}



void Physx::createCapsuleController(Entity * entity,float radius_scale,float height_scale,Vector3 position,float step_offset,PxController *& mController)
{
	const AxisAlignedBox & box = entity->getBoundingBox();
	Vector3 size = box.getSize();	
	PxCapsuleControllerDesc desc;
	desc.density = 10.00f;
	desc.height = height_scale*size.y;
	desc.radius = radius_scale*size.x/2.0;
	desc.position = PxExtendedVec3(position.x,position.y,position.z);
	desc.material = mMaterial;
	desc.stepOffset = step_offset;
	desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	desc.invisibleWallHeight = 0;
	desc.maxJumpHeight = 0;

	mController = manager->createController(*mPhysics, mScene, desc);	

}

void Physx::releaseController(PxController *& mController)
{
	mController->release();
	mController = NULL;
}


void Physx::createBoxController(Entity * entity,float scale,Vector3 position,PxController *& mController)
{
	manager = PxCreateControllerManager(*(mFoundation));
	const AxisAlignedBox & box = entity->getBoundingBox();
	Vector3 size = box.getSize();	
	PxBoxControllerDesc desc;

	desc.density=1.0f;
	desc.halfForwardExtent=scale*size.z/2.0;  
	desc.halfHeight=scale*size.y/2.0;;  
	desc.halfSideExtent=scale*size.x/2.0;
	//desc.invisibleWallHeight=6;    
	desc.material=mMaterial;
	desc.maxJumpHeight=1;
	desc.position=PxExtendedVec3(position.x,position.y,position.z);

	mController = manager->createController(*mPhysics, mScene, desc);
	//actor = mController->getActor();
	//PxShape** buffer = new PxShape*[1];
	//actor->getShapes(buffer,1);
	//shape = buffer[0];
}

Physx::~Physx(void)
{
}
