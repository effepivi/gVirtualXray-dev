/**
********************************************************************************
*
*   @file       SceneGraphBinder.cxx
*
*   @brief      Class to handle scenegraphes using The Open-Asset-Importer-Lib
*   			(ASSIMP), see http://www.assimp.org/
*
*   @version    1.0
*
*   @date       16/07/2018
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jul 2018, 2018, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <limits>
#include <fstream>
#include <cstdio>

// ASSIMP to load Collada files
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


#ifndef __AssimpSceneGraphBinder_h
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

// For exceptions
#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __FileDoesNotExistException_h
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;



//-----------------------------------------------
AssimpSceneGraphBinder::AssimpSceneGraphBinder():
//-----------------------------------------------
#ifdef HAS_ASSIMP
		SceneGraphBinder(),
		m_p_scene(0)
#else
		SceneGraphBinder()
#endif
//-----------------------------------------------
{
#ifndef HAS_ASSIMP
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ASSIMP is not supported in your implementation. To enable it, install ASSIMP and re-compile gVirtualXRay to support it.");
#endif
}


//----------------------------------------------------------------------------------------
AssimpSceneGraphBinder::AssimpSceneGraphBinder(const AssimpSceneGraphBinder& aSceneGraph):
//----------------------------------------------------------------------------------------
		SceneGraphBinder(aSceneGraph)
//----------------------------------------------------------------------------------------
{
#ifdef HAS_ASSIMP
	setSceneGraph(aSceneGraph.m_p_scene, m_unit_of_length);
#else
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ASSIMP is not supported in your implementation. To enable it, install ASSIMP and re-compile gVirtualXRay to support it.");
#endif
}


AssimpSceneGraphBinder::AssimpSceneGraphBinder(const char* aFileName,
	    									   RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName, aUnitOfLength);
}


AssimpSceneGraphBinder::AssimpSceneGraphBinder(const std::string& aFileName,
	    									   RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


//-----------------------------------------------
AssimpSceneGraphBinder::~AssimpSceneGraphBinder()
//-----------------------------------------------
{}


//--------------------------------------------------------------------------------------------------
AssimpSceneGraphBinder& AssimpSceneGraphBinder::operator=(const AssimpSceneGraphBinder& aSceneGraph)
//--------------------------------------------------------------------------------------------------
{
	SceneGraphBinder::operator=(aSceneGraph);

#ifdef HAS_ASSIMP
	setSceneGraph(aSceneGraph.m_p_scene, aSceneGraph.m_unit_of_length);
#endif

	return (*this);
}


#ifdef HAS_ASSIMP
//-----------------------------------------------------------------------
void AssimpSceneGraphBinder::setSceneGraph(const aiScene* apSceneGraph,
										   RATIONAL_NUMBER aUnitOfLength)
//-----------------------------------------------------------------------
{
	// Empty the scenegraph
	m_p_polygon_mesh_set.clear();

	if (m_p_scene != apSceneGraph)
	{
		m_p_scene = const_cast<aiScene*>(apSceneGraph);
	}

	m_unit_of_length = aUnitOfLength;

	aiMatrix4x4 scaling_to_unit = aiMatrix4x4::Scaling(aiVector3D(m_unit_of_length, m_unit_of_length, m_unit_of_length), scaling_to_unit);

	recursivePolygonMeshBuild(m_p_scene->mRootNode, scaling_to_unit);
}
#endif


//------------------------------------------------------------------------
void AssimpSceneGraphBinder::loadSceneGraph(const char* aFileName,
										    RATIONAL_NUMBER aUnitOfLength)
//------------------------------------------------------------------------
{
	// Check if file exists
	std::ifstream file_stream(aFileName);

	// The file exist
	if(!file_stream.fail())
	{
		file_stream.close();
	}
	// The file doest not exist
	else
	{
		// Throw an error
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
	}

#ifdef HAS_ASSIMP
	// Load the file
	const aiScene* p_scene = m_importer.ReadFile( aFileName, aiProcessPreset_TargetRealtime_Quality);

	// The import failed
	if (!p_scene)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, m_importer.GetErrorString());
	}

	// Load and set the scenegraph
	setSceneGraph(p_scene, aUnitOfLength);
#endif
}


//-------------------------------------------
void AssimpSceneGraphBinder::updateMatrices()
//-------------------------------------------
{
#ifdef HAS_ASSIMP
	aiMatrix4x4 scaling_to_unit = aiMatrix4x4::Scaling(aiVector3D(m_unit_of_length, m_unit_of_length, m_unit_of_length), scaling_to_unit);

	recursiveUpdateMatrices(m_p_scene->mRootNode, scaling_to_unit);
#endif
}


//-----------------------------------------------------------------------------
void AssimpSceneGraphBinder::scale(const char* aNodeLabel,
                                   const VEC3& aScalingFactorSet)
//-----------------------------------------------------------------------------
{
#ifdef HAS_ASSIMP
	// Find the node
	aiNode* p_node = findNode(aNodeLabel);

	// The node has been found
	if (p_node)
	{
		// Create the transformation matrix
		aiMatrix4x4 scaling_matrix;

		scaling_matrix = aiMatrix4x4::Scaling(
				aiVector3t<float>(aScalingFactorSet[0], aScalingFactorSet[1], aScalingFactorSet[2]),
				scaling_matrix);

		// Update the node's transformation matrix
		p_node->mTransformation = scaling_matrix * p_node->mTransformation;
	}
	// The node has not been found
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}
#endif
}


//--------------------------------------------------------------------------
void AssimpSceneGraphBinder::rotate(const char* aNodeLabel,
								    float aRotationAngleInDegrees,
							        const VEC3& aRotationAxis)
//--------------------------------------------------------------------------
{
#ifdef HAS_ASSIMP
	// Find the node
	aiNode* p_node = findNode(aNodeLabel);

	// The node has been found
	if (p_node)
	{
		// Create the rotation matrix
		aiMatrix4x4 rotation_matrix;

		rotation_matrix = aiMatrix4x4::Rotation(
				M_PI * aRotationAngleInDegrees /180.0,
				aiVector3t<float>(aRotationAxis[0], aRotationAxis[1], aRotationAxis[2]),
				rotation_matrix);

		// Update the node's transformation matrix
		p_node->mTransformation = p_node->mTransformation * rotation_matrix;
	}
	// The node has not been found
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}
#endif
}


//--------------------------------------------------------------------------
void AssimpSceneGraphBinder::translate(const char* aNodeLabel,
							        const VEC3& aTranslationVector)
//--------------------------------------------------------------------------
{
#ifdef HAS_ASSIMP
	// Find the node
	aiNode* p_node = findNode(aNodeLabel);

	// The node has been found
	if (p_node)
	{
		// Create the rotation matrix
		aiMatrix4x4 translation_matrix;

		translation_matrix = aiMatrix4x4::Translation(
				aiVector3t<float>(aTranslationVector[0], aTranslationVector[1], aTranslationVector[2]),
				translation_matrix);

		// Update the node's transformation matrix
		p_node->mTransformation = p_node->mTransformation * translation_matrix;
	}
	// The node has not been found
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}
#endif
}


//-------------------------------------------------------------------------------------------------
MATRIX4 AssimpSceneGraphBinder::getTransformationMatrix(const char* aNodeLabel) const
//-------------------------------------------------------------------------------------------------
{
	return (getNodeTransformationMatrix(aNodeLabel));
}


MATRIX4 AssimpSceneGraphBinder::getRootNodeTransformationMatrix() const
{
#ifdef HAS_ASSIMP
	aiMatrix4x4 temp_matrix = m_p_scene->mRootNode->mTransformation;
	temp_matrix.Transpose();

	return MATRIX4(
			temp_matrix.a1, temp_matrix.a2, temp_matrix.a3, temp_matrix.a4,
			temp_matrix.b1, temp_matrix.b2, temp_matrix.b3, temp_matrix.b4,
			temp_matrix.c1, temp_matrix.c2, temp_matrix.c3, temp_matrix.c4,
			temp_matrix.d1, temp_matrix.d2, temp_matrix.d3, temp_matrix.d4);
#else
    return MATRIX4();
#endif
}


MATRIX4 AssimpSceneGraphBinder::getNodeTransformationMatrix(const char* aNodeLabel) const
{
#ifdef HAS_ASSIMP
	aiMatrix4x4 temp_matrix = recursiveFindNode(m_p_scene->mRootNode, aNodeLabel)->mTransformation;
	temp_matrix.Transpose();

	return MATRIX4(
			temp_matrix.a1, temp_matrix.a2, temp_matrix.a3, temp_matrix.a4,
			temp_matrix.b1, temp_matrix.b2, temp_matrix.b3, temp_matrix.b4,
			temp_matrix.c1, temp_matrix.c2, temp_matrix.c3, temp_matrix.c4,
			temp_matrix.d1, temp_matrix.d2, temp_matrix.d3, temp_matrix.d4);
#else
    return MATRIX4();
#endif
}


void AssimpSceneGraphBinder::setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix)
{
#ifdef HAS_ASSIMP       
	if (m_p_scene)
	{
		if (m_p_scene->mRootNode)
		{
			m_p_scene->mRootNode->mTransformation.a1 = aTransformationMatrix.get4x4()[ 0];
			m_p_scene->mRootNode->mTransformation.a2 = aTransformationMatrix.get4x4()[ 4];
			m_p_scene->mRootNode->mTransformation.a3 = aTransformationMatrix.get4x4()[ 8];
			m_p_scene->mRootNode->mTransformation.a4 = aTransformationMatrix.get4x4()[12];

			m_p_scene->mRootNode->mTransformation.b1 = aTransformationMatrix.get4x4()[ 1];
			m_p_scene->mRootNode->mTransformation.b2 = aTransformationMatrix.get4x4()[ 5];
			m_p_scene->mRootNode->mTransformation.b3 = aTransformationMatrix.get4x4()[ 9];
			m_p_scene->mRootNode->mTransformation.b4 = aTransformationMatrix.get4x4()[13];

			m_p_scene->mRootNode->mTransformation.c1 = aTransformationMatrix.get4x4()[ 2];
			m_p_scene->mRootNode->mTransformation.c2 = aTransformationMatrix.get4x4()[ 6];
			m_p_scene->mRootNode->mTransformation.c3 = aTransformationMatrix.get4x4()[10];
			m_p_scene->mRootNode->mTransformation.c4 = aTransformationMatrix.get4x4()[14];

			m_p_scene->mRootNode->mTransformation.d1 = aTransformationMatrix.get4x4()[ 3];
			m_p_scene->mRootNode->mTransformation.d2 = aTransformationMatrix.get4x4()[ 7];
			m_p_scene->mRootNode->mTransformation.d3 = aTransformationMatrix.get4x4()[11];
			m_p_scene->mRootNode->mTransformation.d4 = aTransformationMatrix.get4x4()[15];
		}
	}
#endif
}


void AssimpSceneGraphBinder::setNodeTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix)
{
#ifdef HAS_ASSIMP       
	if (m_p_scene)
	{
		aiNode* p_node = recursiveFindNode(m_p_scene->mRootNode, aNodeLabel);

		if (p_node)
		{
			p_node->mTransformation.a1 = aTransformationMatrix.get4x4()[ 0];
			p_node->mTransformation.a2 = aTransformationMatrix.get4x4()[ 4];
			p_node->mTransformation.a3 = aTransformationMatrix.get4x4()[ 8];
			p_node->mTransformation.a4 = aTransformationMatrix.get4x4()[12];

			p_node->mTransformation.b1 = aTransformationMatrix.get4x4()[ 1];
			p_node->mTransformation.b2 = aTransformationMatrix.get4x4()[ 5];
			p_node->mTransformation.b3 = aTransformationMatrix.get4x4()[ 9];
			p_node->mTransformation.b4 = aTransformationMatrix.get4x4()[13];

			p_node->mTransformation.c1 = aTransformationMatrix.get4x4()[ 2];
			p_node->mTransformation.c2 = aTransformationMatrix.get4x4()[ 6];
			p_node->mTransformation.c3 = aTransformationMatrix.get4x4()[10];
			p_node->mTransformation.c4 = aTransformationMatrix.get4x4()[14];

			p_node->mTransformation.d1 = aTransformationMatrix.get4x4()[ 3];
			p_node->mTransformation.d2 = aTransformationMatrix.get4x4()[ 7];
			p_node->mTransformation.d3 = aTransformationMatrix.get4x4()[11];
			p_node->mTransformation.d4 = aTransformationMatrix.get4x4()[15];
		}
	}
#endif
}


void AssimpSceneGraphBinder::moveToCentre()
{
#ifdef HAS_ASSIMP       
	aiMatrix4x4 scaling_to_unit = aiMatrix4x4::Scaling(aiVector3D(m_unit_of_length, m_unit_of_length, m_unit_of_length), scaling_to_unit);

	VEC3 min_corner(std::numeric_limits<RATIONAL_NUMBER>::max(),
			std::numeric_limits<RATIONAL_NUMBER>::max(),
			std::numeric_limits<RATIONAL_NUMBER>::max());
	VEC3 max_corner(-std::numeric_limits<RATIONAL_NUMBER>::max(),
			-std::numeric_limits<RATIONAL_NUMBER>::max(),
			-std::numeric_limits<RATIONAL_NUMBER>::max());

	recursiveBoundingBoxBuild(m_p_scene->mRootNode, scaling_to_unit, min_corner, max_corner);

	VEC3 translation_vector = -min_corner - (max_corner - min_corner) / 2.0;

	aiMatrix4x4 translation_matrix;

	translation_matrix = aiMatrix4x4::Translation(
			aiVector3t<float>(translation_vector[0], translation_vector[1], translation_vector[2]),
			translation_matrix);

	m_p_scene->mRootNode->mTransformation = translation_matrix * m_p_scene->mRootNode->mTransformation;
#endif
}


void AssimpSceneGraphBinder::moveToCenter()
{
	moveToCentre();
}


#ifdef HAS_ASSIMP
//--------------------------------------------------------------------------
const aiNode* AssimpSceneGraphBinder::findNode(const char* aNodeLabel) const
//--------------------------------------------------------------------------
{
	return (recursiveFindNode(m_p_scene->mRootNode, aNodeLabel));
}


//---------------------------------------------------------------------------------
const aiNode* AssimpSceneGraphBinder::findNode(const std::string& aNodeLabel) const
//---------------------------------------------------------------------------------
{
	return (findNode(aNodeLabel.data()));
}


//--------------------------------------------------------------
aiNode* AssimpSceneGraphBinder::findNode(const char* aNodeLabel)
//--------------------------------------------------------------
{
	return (recursiveFindNode(m_p_scene->mRootNode, aNodeLabel));
}


//---------------------------------------------------------------------
aiNode* AssimpSceneGraphBinder::findNode(const std::string& aNodeLabel)
//---------------------------------------------------------------------
{
	return (findNode(aNodeLabel.data()));
}


void AssimpSceneGraphBinder::recursivePolygonMeshBuild(const aiNode* nd, aiMatrix4x4 tStack)
{
	std::string node_label = nd->mName.C_Str();
	std::cout << nd->mName.C_Str() << "\t" << nd->mNumChildren << "\t" << nd->mNumMeshes << std::endl;

	aiMatrix4x4 m =  nd->mTransformation ;
	tStack = tStack * m;

	aiMatrix4x4 temp_matrix = tStack;
	temp_matrix.Transpose();

	MATRIX4 modelling_matrix(
			temp_matrix.a1, temp_matrix.a2, temp_matrix.a3, temp_matrix.a4,
			temp_matrix.b1, temp_matrix.b2, temp_matrix.b3, temp_matrix.b4,
			temp_matrix.c1, temp_matrix.c2, temp_matrix.c3, temp_matrix.c4,
			temp_matrix.d1, temp_matrix.d2, temp_matrix.d3, temp_matrix.d4);

	std::vector<float> p_vertex_set;
	std::vector<unsigned int> p_index_array;

	unsigned int index_offset = 0;

	for (unsigned int n=0; n < nd->mNumMeshes; ++n)
	{
		const aiMesh* mesh = m_p_scene->mMeshes[nd->mMeshes[n]];

		if (mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE)
		{
			index_offset += p_vertex_set.size() / 3;

			for (int t = 0; t < mesh->mNumFaces; ++t)
			{
				const aiFace* face = &mesh->mFaces[t];
				GLenum face_mode;

				for(int i = 0; i < face->mNumIndices; i++)
				{
					p_index_array.push_back(face->mIndices[i] + index_offset);
				}
			}

			for (int t = 0; t < mesh->mNumVertices; ++t)
			{
				aiVector3D vertex(mesh->mVertices[t]);

				p_vertex_set.push_back( (vertex.x));
				p_vertex_set.push_back( (vertex.y));
				p_vertex_set.push_back( (vertex.z));
			}
		}
	}

	if (p_vertex_set.size() && p_index_array.size())
	{
		m_p_polygon_mesh_set[node_label] = (PolygonMesh());

		m_p_polygon_mesh_set[node_label].setInternalData(GL_TRIANGLES,
			&p_vertex_set,
			&p_index_array,
			true,
			GL_STATIC_DRAW);

		m_p_polygon_mesh_set[node_label].computeNormalVectors();
		m_p_polygon_mesh_set[node_label].setTransformationMatrix(modelling_matrix);
	}

	// all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n)
	{
		recursivePolygonMeshBuild(nd->mChildren[n], tStack);
	}
}


void AssimpSceneGraphBinder::recursiveUpdateMatrices(const aiNode* nd, aiMatrix4x4 tStack)
{
	std::string node_label = nd->mName.C_Str();
	//std::cout << nd->mName.C_Str() << "\t" << nd->mNumChildren << "\t" << nd->mNumMeshes << std::endl;

	aiMatrix4x4 m =  nd->mTransformation ;
	tStack = tStack * m;

	aiMatrix4x4 temp_matrix = tStack;
	temp_matrix.Transpose();

	MATRIX4 modelling_matrix(
			temp_matrix.a1, temp_matrix.a2, temp_matrix.a3, temp_matrix.a4,
			temp_matrix.b1, temp_matrix.b2, temp_matrix.b3, temp_matrix.b4,
			temp_matrix.c1, temp_matrix.c2, temp_matrix.c3, temp_matrix.c4,
			temp_matrix.d1, temp_matrix.d2, temp_matrix.d3, temp_matrix.d4);

	std::vector<float> p_vertex_set;

	for (unsigned int n=0; n < nd->mNumMeshes; ++n)
	{
		if (nd->mMeshes)
		{
			m_p_polygon_mesh_set[node_label].setTransformationMatrix(modelling_matrix);
		}
	}

	// all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n)
	{
		recursiveUpdateMatrices(nd->mChildren[n], tStack);
	}
}


//-----------------------------------------------------------------------------
const aiNode* AssimpSceneGraphBinder::recursiveFindNode(const aiNode* nd,
														const char* aNodeLabel) const
//-----------------------------------------------------------------------------
{
	const aiNode* p_node = 0;


	//std::cout << nd->mName.C_Str() << "\t" <<  std::endl;

	// The node has been found
	if (!strcmp(nd->mName.C_Str(), aNodeLabel))
	{
		p_node = nd;
	}
	// The node has not been found
	else
	{
		// all children
		for (unsigned int n=0; n < nd->mNumChildren; ++n)
		{
			if (!p_node)
			{
				p_node = recursiveFindNode(nd->mChildren[n], aNodeLabel);
			}
		}
	}

	return (p_node);
}


//-----------------------------------------------------------------------
aiNode* AssimpSceneGraphBinder::recursiveFindNode(aiNode* nd,
												  const char* aNodeLabel)
//-----------------------------------------------------------------------
{
	aiNode* p_node = 0;


	//std::cout << nd->mName.C_Str() << "\t" <<  std::endl;

	// The node has been found
	if (!strcmp(nd->mName.C_Str(), aNodeLabel))
	{
		p_node = nd;
	}
	// The node has not been found
	else
	{
		// all children
		for (unsigned int n=0; n < nd->mNumChildren; ++n)
		{
			if (!p_node)
			{
				p_node = recursiveFindNode(nd->mChildren[n], aNodeLabel);
			}
		}
	}

	return (p_node);
}


void AssimpSceneGraphBinder::recursiveBoundingBoxBuild(const aiNode* nd, aiMatrix4x4 tStack, VEC3& aMinCorner, VEC3& aMaxCorner)
{
	std::string node_label = nd->mName.C_Str();
	//std::cout << nd->mName.C_Str() << "\t" << nd->mNumChildren << "\t" << nd->mNumMeshes << std::endl;

	aiMatrix4x4 m =  nd->mTransformation ;
	tStack = tStack * m;

	for (unsigned int n=0; n < nd->mNumMeshes; ++n)
	{
		const aiMesh* mesh = m_p_scene->mMeshes[nd->mMeshes[n]];

		for (int t = 0; t < mesh->mNumVertices; ++t)
		{
			aiVector3D vertex(tStack * mesh->mVertices[t]);

			if (aMinCorner[0] > vertex.x) aMinCorner[0] = vertex.x;
			if (aMinCorner[1] > vertex.y) aMinCorner[1] = vertex.y;
			if (aMinCorner[2] > vertex.z) aMinCorner[2] = vertex.z;

			if (aMaxCorner[0] < vertex.x) aMaxCorner[0] = vertex.x;
			if (aMaxCorner[1] < vertex.y) aMaxCorner[1] = vertex.y;
			if (aMaxCorner[2] < vertex.z) aMaxCorner[2] = vertex.z;
		}
	}


	// all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n)
	{
		recursiveBoundingBoxBuild(nd->mChildren[n], tStack, aMinCorner, aMaxCorner);
	}
}

#endif

