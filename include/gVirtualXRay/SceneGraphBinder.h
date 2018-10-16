#ifndef __SceneGraphBinder_h
#define __SceneGraphBinder_h

#include <map>
#include <string>

// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

// For MATRIX4
#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class SceneGraphBinder;


class SceneGraphNode
{
public:
    SceneGraphNode(const std::string& aLabel = "", const SceneGraphNode* apParent = 0);
    SceneGraphNode(const std::string& aLabel, const PolygonMesh& aPolygonMesh, const VEC3& aPosition, const SceneGraphNode* apParent);

    const SceneGraphNode* getParent() const;
    unsigned int getNumberOfChildren() const;
    SceneGraphNode& getChild(unsigned int i);
    const SceneGraphNode& getChild(unsigned int i) const;

    const std::string& getLabel() const;
    const PolygonMesh& getPolygonMesh() const;
    MATRIX4 getLocalTransformationMatrix() const;
    MATRIX4 getWorldTransformationMatrix() const;
    void applyLocalTransformation(const MATRIX4& aMatrix);

    void addChild(const std::string& aLabel, const PolygonMesh& aPolygonMesh, const VEC3& aPosition);

    friend class SceneGraphBinder;

protected:
    std::string m_label;
    PolygonMesh  m_geometry;
    MATRIX4     m_partial_local_transformation;
    VEC3     m_position;

    const SceneGraphNode* m_p_parent;
    std::vector<SceneGraphNode> m_p_children;
};


class SceneGraphBinder
{
public:
	SceneGraphBinder();
	SceneGraphBinder(const SceneGraphBinder& aSceneGraph);
	SceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength);
	SceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);
	virtual ~SceneGraphBinder();

	SceneGraphBinder& operator=(const SceneGraphBinder& aSceneGraph);


	virtual void loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);


	virtual void loadSceneGraph(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength);

	void clear();


	const std::map<std::string, PolygonMesh>& getPolygonMeshSet() const;


	std::map<std::string, PolygonMesh>& getPolygonMeshSet();


	//const PolygonMesh& getPolygonMesh(const std::string& aLabel) const;
	//const PolygonMesh& getPolygonMesh(const char* aLabel) const;

	//PolygonMesh& getPolygonMesh(const std::string& aLabel);
	//PolygonMesh& getPolygonMesh(const char* aLabel);


	virtual void updateMatrices();


	void display(GLint aShaderID);

	void scale(const std::string& aNodeLabel,
			const VEC3& aScalingFactorSet);

	virtual void scale(const char* aNodeLabel,
			const VEC3& aScalingFactorSet);

	void rotate(const std::string& aNodeLabel,
			float aRotationAngleInDegrees,
			const VEC3& aRotationAxis);


	virtual void rotate(const char* aNodeLabel,
			float aRotationAngleInDegrees,
			const VEC3& aRotationAxis);



	void translate(const std::string& aNodeLabel,
			const VEC3& aTranslationVector);

	virtual void translate(const char* aNodeLabel,
			const VEC3& aTranslationVector);


	MATRIX4 getTransformationMatrix(const std::string& aNodeLabel) const;


	virtual MATRIX4 getTransformationMatrix(const char* aNodeLabel) const;


	virtual void moveToCentre();
	virtual void moveToCenter();

	virtual MATRIX4 getRootNodeTransformationMatrix() const;
	virtual MATRIX4 getNodeTransformationMatrix(const char* aNodeLabel) const;

	virtual void setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix);
	virtual void setNodeTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix);

	std::pair<VEC3, VEC3> getBoundingBox() const;


    void addPolygonMesh(const std::string& aLabel, const PolygonMesh& aPolygonMesh, const VEC3& aPosition, const std::string& aParent = "root");


protected:
    SceneGraphNode* getNode(const std::string& aLabel);
    const SceneGraphNode* getNode(const std::string& aLabel) const;

	SceneGraphNode m_root_node;

	std::map<std::string, PolygonMesh> m_p_polygon_mesh_set;
	RATIONAL_NUMBER m_unit_of_length;
};


} // namespace gVirtualXRay


#endif // __SceneGraphBinder_h
