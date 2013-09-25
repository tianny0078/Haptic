#ifndef CSURFACEMESH
#define CSURFACEMESH


#include "chai3d.h"
#include "mesh.h"
#include "kernel.h"

//===========================================================================
/*!
    This class is for deformable mesh, which could includes a Mesh pointer related
	to deformable algorithm of Kernel. This class mainly implement graphical rendering.

	by Yuan Tian
*/
//===========================================================================

class cSurfaceMesh : public cMesh
{
public:
	cSurfaceMesh (cWorld * a_world);
	~cSurfaceMesh ();

	bool setMesh(Mesh * value, const bool a_affectChildren = true);
	bool setVoxMesh(VoxMesh * value, const bool affectChildren = true);
	bool setKernel(Kernel * value, const bool affectChildren = true);

protected:
	virtual void render(const int a_renderMode = CHAI_RENDER_MODE_RENDER_ALL);

public:
	inline virtual cSurfaceMesh* createMesh() const { return new cSurfaceMesh(m_parentWorld); }

	 //! Render triangles, material and texture properties.
    virtual void renderMesh(const int a_renderMode=0);

	void setGridDensity(int value, const bool a_affectChildren = true);

	void enableGrid(bool value, const bool a_affectChildren = true);

	void enableVox(bool value, const bool a_affectChildren = true);

	void enableMesh(bool value, const bool a_affectChildren = true);
	void enableParticleCoupling(bool value, const bool a_affectChildren = true);
	void enableLastLevel(bool value, const bool a_affectChildren = true);

	void renderVox();
	void createVoxSurMesh();

	void renderCube(double v_dim, double vol_center_x, double vol_center_y, double vol_center_z);

	void setIdxTouch(int value, const bool a_affectChildren = true);


	//level rendering
	void renderLevelVoxMesh(const Level * plevel);
	void renderLevelVoxMesh4Leaf(const Level * plevel, float * level_color);
	void renderParticleCoupling(const Level * plevel);

	//UI
	//bool isSelected4Mesh(double x, double y);

private:
	Kernel * p_kernel;
	Mesh *m;
	VoxMesh * vm;
	int gridDim;//Dim of surrounding dim
	bool flag_grid;
	bool flag_vox;
	bool flag_selection;
	bool flag_mesh;
	bool flag_last_level;
	bool flag_particle_coupling;
	float vox_mesh_color[3];
	float anchor_node_color[3];
	float render_level_color[8][3];
public:
		// matrix info. (currently only when the rendering is under selection mode will get the matrix)
	// getting matrix could be slow
	GLint    currentviewport[4];
	GLdouble currentmodelview[16];
	GLdouble currentprojection[16];

	//// for selection square
	//double upper_left_x_Mesh, upper_left_y_Mesh;
	//double bottom_right_x_Mesh, bottom_right_y_Mesh;
	//int m_displayH_Mesh;

	int idx_touch;
};

#endif