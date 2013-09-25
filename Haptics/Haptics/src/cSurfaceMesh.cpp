#include "csurfacemesh.h"
#include <iostream>
using namespace std;

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>

cSurfaceMesh::cSurfaceMesh(cWorld * a_world):cMesh(a_world)
{
	//initialization
	m = NULL;
	vm = NULL;

	gridDim = 1;

	flag_grid = false;
	flag_vox = false;
	flag_selection = false;
	flag_mesh = false;
	flag_last_level = false;
	flag_particle_coupling = false;

	//vox_mesh_color[0] = 1.0; vox_mesh_color[1] = 174.0/255.0; vox_mesh_color[2] = 201.0/255.0;
	vox_mesh_color[0] = 0.8; vox_mesh_color[1] = 0.1; vox_mesh_color[2] = 0.1;
	anchor_node_color[0] = 1.0; anchor_node_color[1] = 0.5; anchor_node_color[2] = 0.25;

	render_level_color[0][0] = 1.0; render_level_color[0][1] = 0.0; render_level_color[0][2] = 0.0; 
	render_level_color[1][0] = 0.0; render_level_color[1][1] = 1.0; render_level_color[1][2] = 0.0; 
	render_level_color[2][0] = 0.0; render_level_color[2][1] = 0.0; render_level_color[2][2] = 1.0;
	render_level_color[3][0] = 1.0; render_level_color[3][1] = 1.0; render_level_color[3][2] = 0.0;
	render_level_color[4][0] = 1.0; render_level_color[4][1] = 0.0; render_level_color[4][2] = 1.0; 
	render_level_color[5][0] = 0.0; render_level_color[5][1] = 1.0; render_level_color[5][2] = 1.0; 
	render_level_color[6][0] = 1.0; render_level_color[6][1] = 0.5; render_level_color[6][2] = 0.5;
	render_level_color[7][0] = 1.0; render_level_color[7][1] = 1.0; render_level_color[7][2] = 1.0;

	idx_touch = -1;


	// selection square
	//bottom_right_x_Mesh = -1.0;
	//bottom_right_y_Mesh = -1.0;
	//upper_left_x_Mesh   = -1.0;
	//upper_left_y_Mesh   = -1.0;

	//m_displayH_Mesh = 0;
}
cSurfaceMesh::~cSurfaceMesh()
{
	
}
//===========================================================================
/*!
    derived override render method, protected could only be called by other
	member function.
*/
//===========================================================================
void cSurfaceMesh::render(const int a_renderMode)
{	
	// Only render normals on one pass, no matter what the transparency
    // options are...
    if ( (a_renderMode == CHAI_RENDER_MODE_NON_TRANSPARENT_ONLY) ||
         (a_renderMode == CHAI_RENDER_MODE_RENDER_ALL) )
    {
        // render normals
        //if (m_showNormals) renderNormals(m_showNormalsForTriangleVerticesOnly);
    }



    /////////////////////////////////////////////////////////////////////////
    // Conditions for mesh to be rendered
    /////////////////////////////////////////////////////////////////////////

    if(((a_renderMode == CHAI_RENDER_MODE_NON_TRANSPARENT_ONLY) &&
        (m_useTransparency == true)) ||
       ((a_renderMode == CHAI_RENDER_MODE_TRANSPARENT_FRONT_ONLY) &&
        (m_useTransparency == false)) ||
       ((a_renderMode == CHAI_RENDER_MODE_TRANSPARENT_BACK_ONLY) &&
        (m_useTransparency == false)))
    {
        return;
    }
	
    // render triangle mesh
    renderMesh(a_renderMode);


}

void cSurfaceMesh::renderMesh(const int a_renderMode)
{
	
	//get the model view, projection, and viewport matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, currentmodelview);
	glGetDoublev(GL_PROJECTION_MATRIX, currentprojection);
	glGetIntegerv(GL_VIEWPORT, currentviewport);
    //-----------------------------------------------------------------------
    // INITIALIZATION
    //-----------------------------------------------------------------------
    // check if object contains any triangles or vertices
    if ((m_vertices.size() == 0) || (m_triangles.size() == 0))
    {
        return;
    }

    // we are not currently creating a display list
    bool creating_display_list = false;


    //-----------------------------------------------------------------------
    // DISPLAY LIST
    //-----------------------------------------------------------------------
    // Should we render with a display list?
    if (m_useDisplayList)
    {
        // If the display list doesn't exist, create it
        if (m_displayList == -1)
        {
             m_displayList = glGenLists(1);
             if (m_displayList == -1) return;

             // On some machines, GL_COMPILE_AND_EXECUTE totally blows for some reason,
             // so even though it's more complex on the first rendering pass, we use
             // GL_COMPILE (and _repeat_ the first rendering pass)
             glNewList(m_displayList, GL_COMPILE);

             // the list has been created
             creating_display_list = true;

             // Go ahead and render; we'll create this list now...
             // we'll make another (recursive) call to renderMesh()
             // at the end of this function.
        }

        // Otherwise all we have to do is call the display list
        else
        {
            glCallList(m_displayList);

            // All done...
            return;
        }
    }
    //-----------------------------------------------------------------------
    // RENDERING WITH VERTEX ARRAYS OR CLASSIC OPENGL CALLS
    //-----------------------------------------------------------------------

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
    glDisableClientState(GL_EDGE_FLAG_ARRAY);

    if (m_useVertexArrays)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
    }

    /////////////////////////////////////////////////////////////////////////
    // RENDER MATERIAL
    /////////////////////////////////////////////////////////////////////////
    if (m_useMaterialProperty)
    {
        m_material.render();
    }


    /////////////////////////////////////////////////////////////////////////
    // RENDER VERTEX COLORS
    /////////////////////////////////////////////////////////////////////////
    if (m_useVertexColors)
    {
        // Clear the effects of material properties...
        if (!m_useMaterialProperty)
        {
            float fnull[4] = {0,0,0,0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const float *)&fnull);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (const float *)&fnull);
        }

        // enable vertex colors
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        if (m_useVertexArrays)
        {
            glEnableClientState(GL_COLOR_ARRAY);
        }
    }
    else
    {
        glDisable(GL_COLOR_MATERIAL);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    /////////////////////////////////////////////////////////////////////////
    // FOR OBJECTS WITH NO DEFINED COLOR/MATERIAL SETTINGS
    /////////////////////////////////////////////////////////////////////////
    // A default color for objects that don't have vertex colors or
    // material properties (otherwise they're invisible)...
    if ((!m_useVertexColors) && (!m_useMaterialProperty))
    {
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glColor4f(1,1,1,1);
    }

    /////////////////////////////////////////////////////////////////////////
    // TEXTURE
    /////////////////////////////////////////////////////////////////////////
    if ((m_texture != NULL) && (m_useTextureMapping))
    {
        glEnable(GL_TEXTURE_2D);
        if (m_useVertexArrays)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        m_texture->render();
    }


    /////////////////////////////////////////////////////////////////////////
    // RENDER TRIANGLES WITH VERTEX ARRAYS
    /////////////////////////////////////////////////////////////////////////
    if (m_useVertexArrays)
    {
        // Where does our vertex array live?
        vector<cVertex>* vertex_vector = pVertices();
        cVertex* vertex_array = (cVertex*) &((*vertex_vector)[0]);

        // specify pointers to rendering arrays
        glVertexPointer(3, GL_DOUBLE, sizeof(cVertex), &(vertex_array[0].m_localPos));
        glNormalPointer(GL_DOUBLE, sizeof(cVertex), &(vertex_array[0].m_normal));
        glColorPointer(3, GL_FLOAT, sizeof(cVertex), vertex_array[0].m_color.pColor());
        glTexCoordPointer(2, GL_DOUBLE, sizeof(cVertex), &(vertex_array[0].m_texCoord));

        // variables
        unsigned int i;
        unsigned int numItems = m_triangles.size();

        // begin rendering triangles
        glBegin(GL_TRIANGLES);

        // render all active triangles
        for(i=0; i<numItems; i++)
        {
            bool allocated = m_triangles[i].m_allocated;
            if (allocated)
            {
                unsigned int index0 = m_triangles[i].m_indexVertex0;
                unsigned int index1 = m_triangles[i].m_indexVertex1;
                unsigned int index2 = m_triangles[i].m_indexVertex2;
                glArrayElement(index0);
                glArrayElement(index1);
                glArrayElement(index2);
            }
        }

        // finalize rendering list of triangles
        glEnd();
    }

    /////////////////////////////////////////////////////////////////////////
    // RENDER TRIANGLES USING CLASSIC OPENGL COMMANDS
    /////////////////////////////////////////////////////////////////////////
    else
    {
		/////////////////////////////////////////////////////////////////////
		// if rendergrid is true, render a grid which covers the object.
		// by Yuan Tian
		/////////////////////////////////////////////////////////////////////
		if(flag_grid)
		{


			glColor4f(0.0, 1.0, 0.1, 0.05);
			int dim = gridDim;
			int dim2 = dim*dim;
			double v_dim = 1.0 / double(dim);

			// grid line
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
	
			for (int i=0; i<dim+1; ++i)
			{
				float x = float(i)*v_dim - 0.5;
				float y = float(i)*v_dim - 0.5;
				float z = float(i)*v_dim - 0.5;

				glBegin(GL_QUADS);
				glNormal3f(1.0 , 0.0, 0.0);
				glVertex3f(x, -0.5, 0.5);
				glVertex3f(x, -0.5, -0.5);
				glVertex3f(x, 0.5, -0.5);
				glVertex3f(x, 0.5, 0.5);

				glNormal3f(0.0, 1.0, 0.0);
				glVertex3f(-0.5, y, 0.5);
				glVertex3f(0.5, y, 0.5);
				glVertex3f(0.5, y, -0.5);
				glVertex3f(-0.5, y, -0.5);

				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-0.5, -0.5, z);
				glVertex3f(0.5, -0.5, z);
				glVertex3f(0.5, 0.5, z);
				glVertex3f(-0.5, 0.5, z);
				glEnd();
			}

			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
		//grid render finished

        // variables
        unsigned int i;
        unsigned int numItems = m_triangles.size();

        // begin rendering triangles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_TRIANGLES);

        // render all active triangles
        if ((!m_useTextureMapping) && (!m_useVertexColors))
        {
            for(i=0; i<numItems; i++)
            {
                // get pointers to vertices
                cVertex* v0 = m_triangles[i].getVertex(0);
                cVertex* v1 = m_triangles[i].getVertex(1);
                cVertex* v2 = m_triangles[i].getVertex(2);

                // render vertex 0
                glNormal3dv(&v0->m_normal.x);
                glVertex3dv(&v0->m_localPos.x);

                // render vertex 1
                glNormal3dv(&v1->m_normal.x);
                glVertex3dv(&v1->m_localPos.x);

                // render vertex 2
                glNormal3dv(&v2->m_normal.x);
                glVertex3dv(&v2->m_localPos.x);
            }
        }
        else if ((m_useTextureMapping) && (!m_useVertexColors))
        {
            for(i=0; i<numItems; i++)
            {
                // get pointers to vertices
                cVertex* v0 = m_triangles[i].getVertex(0);
                cVertex* v1 = m_triangles[i].getVertex(1);
                cVertex* v2 = m_triangles[i].getVertex(2);

                // render vertex 0
                glNormal3dv(&v0->m_normal.x);
                glTexCoord2dv(&v0->m_texCoord.x);
                glVertex3dv(&v0->m_localPos.x);

                // render vertex 1
                glNormal3dv(&v1->m_normal.x);
                glTexCoord2dv(&v1->m_texCoord.x);
                glVertex3dv(&v1->m_localPos.x);

                // render vertex 2
                glNormal3dv(&v2->m_normal.x);
                glTexCoord2dv(&v2->m_texCoord.x);
                glVertex3dv(&v2->m_localPos.x);
            }
        }
        else if ((!m_useTextureMapping) && (m_useVertexColors))
        {
			/////////////////////////////////////////////////////////
			// combined with Mesh data, and update; 
			// by Yuan Tian
			////////////////////////////////////////////////////////
			


			if(m != NULL && flag_mesh)
			{
				//if (m_children.size() == 0)
				//{
				//	cout << "leaf.." << endl;
				//}
				//else
				//	cout << "not leaf" << endl;

				int countOfMesh = m->number_face;
				int countOfCMesh = getNumTriangles();
				if(countOfMesh == countOfCMesh)
				{
					//glutSolidCube(0.3);
					unsigned int index = 0;
					//revise cMesh data based on Mesh data
					vector<Face>::iterator fi = m->face_list.begin();
					/////////////////////////////////////////
					//QFile file("c://Yuan Tian//out_2.txt");
					//file.open(QIODevice::WriteOnly | QIODevice::Text);
					//QTextStream out(&file);
					//////////////////////////////////////////
					for (; fi != m->face_list.end(); ++fi)
					{
						Vector3d n0, n1, n2, n01, n02, n;

						n0 = fi->node0->coordinate + fi->node0->displacement;
						n1 = fi->node1->coordinate + fi->node1->displacement;
						n2 = fi->node2->coordinate + fi->node2->displacement;

						cVector3d p0 = cVector3d(n0[0], n0[1], n0[2]);
						cVector3d p1 = cVector3d(n1[0], n1[1], n1[2]);
						cVector3d p2 = cVector3d(n2[0], n2[1], n2[2]);
		
						// we get a pointer to the latest created triangle
						cTriangle* triangle = getTriangle(index);

						// we get pointers to its respective vertices.
						cVertex* vertex0 = triangle->getVertex0();
						cVertex* vertex1 = triangle->getVertex1();
						cVertex* vertex2 = triangle->getVertex2();

						vertex0->setPos(p0);
						vertex1->setPos(p1);
						vertex2->setPos(p2);
		
						/////////////////////////////////////////////
						/*
						QString a0 = QString::number(vertex0->getPos().x);
						QString a1 = QString::number(vertex0->getPos().y);
						QString a2 = QString::number(vertex0->getPos().z);
						QString a3 = QString::number(index);

						out << a3 << ":"<< a0 << " " << a1 << " " << a2 << endl;

						
						a0 = QString::number(vertex1->getPos().x);
						a1 = QString::number(vertex1->getPos().y);
						a2 = QString::number(vertex1->getPos().z);
						a3 = QString::number(index);

						out << a3 << ":"<< a0 << " " << a1 << " " << a2 << endl;

						
						a0 = QString::number(vertex2->getPos().x);
						a1 = QString::number(vertex2->getPos().y);
						a2 = QString::number(vertex2->getPos().z);
						a3 = QString::number(index);

						out << a3 << ":"<< a0 << " " << a1 << " " << a2 << endl;
						*/
						/*
						QString a0 = QString::number(n0[0]);
						QString a1 = QString::number(n0[1]);
						QString a2 = QString::number(n0[2]);
						QString a3 = QString::number(index);

						out << a3 << ":"<< a0 << " " << a1 << " " << a2 << endl;

						
						a0 = QString::number(n1[0]);
						a1 = QString::number(n1[1]);
						a2 = QString::number(n1[2]);
						a3 = QString::number(index);

						out << a3 << ":"<< a0 << " " << a1 << " " << a2 << endl;

						
						a0 = QString::number(n2[0]);
						a1 = QString::number(n2[1]);
						a2 = QString::number(n2[2]);
						a3 = QString::number(index);

						out << a3 << ":"<< a0 << " " << a1 << " " << a2 << endl;
						*/
						////////////////////////////////////////////////////////

						vertex0->setColor(0.1, 0.3, 0.6, 1.0);
						vertex1->setColor(0.1, 0.3, 0.6, 1.0);
						vertex2->setColor(0.1, 0.3, 0.6, 1.0);

						//vertex0->setColor(0.1, 0.3, 0.9, 0.5);
						//vertex1->setColor(0.1, 0.3, 0.9, 0.5);
						//vertex2->setColor(0.1, 0.3, 0.9, 0.5);

						//if (idx_touch == index)
						//{
						//	vertex0->setColor(0.5, 0.0, 0.0);
						//	vertex1->setColor(0.1, 0.3, 0.0);
						//	vertex2->setColor(0.1, 0.3, 0.0);
						//}

						setUseVertexColors(true);

						vertex0->m_normal.zero();
						vertex1->m_normal.zero();
						vertex2->m_normal.zero();
		
						n01 = n0 - n1;
						n02 = n0 - n2;

						n = n01.cross(n02);
						n.normalize();

						cVector3d tempNormal = cVector3d(n[0], n[1], n[2]);
						vertex0->m_normal.add(tempNormal);
						vertex1->m_normal.add(tempNormal);
						vertex2->m_normal.add(tempNormal);

						//draw triangles
						if (!flag_particle_coupling)
						{
							// render vertex 0
							glNormal3dv(&vertex0->m_normal.x);
							glColor4fv(vertex0->m_color.pColor());
							glVertex3dv(&vertex0->m_localPos.x);

							// render vertex 1
							glNormal3dv(&vertex1->m_normal.x);
							glColor4fv(vertex1->m_color.pColor());
							glVertex3dv(&vertex1->m_localPos.x);

							// render vertex 2
							glNormal3dv(&vertex2->m_normal.x);
							glColor4fv(vertex2->m_color.pColor());
							glVertex3dv(&vertex2->m_localPos.x);

						}
						index ++;
					}//for
					/////////////////////////////////////////////////////
					// optional, as QFile destructor will already do it:
						//file.close(); 
					/////////////////////////////////////////////////////
				}//if not equal
			}//if m

			//rendering finished
        }
        else if ((m_useTextureMapping) && (m_useVertexColors))
        {
            for(i=0; i<numItems; i++)
            {
                // get pointers to vertices
                cVertex* v0 = m_triangles[i].getVertex(0);
                cVertex* v1 = m_triangles[i].getVertex(1);
                cVertex* v2 = m_triangles[i].getVertex(2);

                // render vertex 0
                glNormal3dv(&v0->m_normal.x);
                glColor4fv(v0->m_color.pColor());
                glTexCoord2dv(&v0->m_texCoord.x);
                glVertex3dv(&v0->m_localPos.x);

                // render vertex 1
                glNormal3dv(&v1->m_normal.x);
                glColor4fv(v1->m_color.pColor());
                glTexCoord2dv(&v1->m_texCoord.x);
                glVertex3dv(&v1->m_localPos.x);

                // render vertex 2
                glNormal3dv(&v2->m_normal.x);
                glColor4fv(v2->m_color.pColor());
                glTexCoord2dv(&v2->m_texCoord.x);
                glVertex3dv(&v2->m_localPos.x);
            }
        }

        // finalize rendering list of triangles
        glEnd();
    }

	/////////////////////////////////////////////////////////////////////
	// if rendervox is true, render vox which covers the object.
	// by Yuan Tian
	////////////////////////////////////////////////////////////////////

	if (flag_vox)
	{
		//renderVox();

		//level detail for the vox mesh
		////current level(0-)
		if (!flag_last_level)
		{
			Level * plevel = p_kernel->level_list[p_kernel->level_display];
			renderLevelVoxMesh(plevel);
		}
		else
		{
			for (int i = 0; i <  p_kernel->level_list.size(); i++)
			{
				//float target_color[3] = {1.0, 174.0/255.0, 201.0/255.0};
				Level * plevel = p_kernel->level_list[i];
				renderLevelVoxMesh4Leaf(plevel, render_level_color[i]);
			}
		}
	}

	if (flag_particle_coupling)
	{
		Level * plevel = p_kernel->level_list[p_kernel->level_display];
		renderParticleCoupling(plevel);
	}


	//vox render finished


    //-----------------------------------------------------------------------
    // FINALIZE
    //-----------------------------------------------------------------------

    // restore OpenGL settings to reasonable defaults
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDisable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Turn off any array variables I might have turned on...
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // If we've gotten this far and we're using a display list for rendering,
    // we must be capturing it right now...
    if ((m_useDisplayList) && (m_displayList != -1) && (creating_display_list))
    {
        // finalize list
        glEndList();

        // Recursively make a call to actually render this object if
        // we didn't use compile_and_execute
        renderMesh(a_renderMode);
    }
}


bool cSurfaceMesh::setMesh(Mesh *value, const bool a_affectChildren)
{
	if(value != NULL)
	{
		m = value;


		
		if(a_affectChildren)
		{
			unsigned int i, numItems;
			numItems = m_children.size();
			for (i=0; i<numItems; i++)
			{
				cGenericObject *nextObject = m_children[i];

				cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
				if (nextMesh)
				{
					nextMesh->setMesh(value, a_affectChildren);
				}
			}
		}
		
		return true;
	}
	else
	{
		cout << "Error! Input Mesh pointer is NULL!" << endl;
		return false;
	}
}

bool cSurfaceMesh::setVoxMesh(VoxMesh *value, const bool a_affectChildren)
{
	if(value != NULL)
	{
		vm = value;
		if(a_affectChildren)
		{
			unsigned int i, numItems;
			numItems = m_children.size();
			for (i=0; i<numItems; i++)
			{
				cGenericObject *nextObject = m_children[i];

				cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
				if (nextMesh)
				{
					nextMesh->setVoxMesh(value, a_affectChildren);
				}
			}
		}

		//createVoxSurMesh();

		return true;
	}
	else
	{
		cout << "Error! Input Mesh pointer is NULL!" << endl;
		return false;
	}
}

bool cSurfaceMesh::setKernel(Kernel *value, const bool a_affectChildren)
{
	if(value != NULL)
	{
		p_kernel = value;
		if(a_affectChildren)
		{
			unsigned int i, numItems;
			numItems = m_children.size();
			for (i=0; i<numItems; i++)
			{
				cGenericObject *nextObject = m_children[i];

				cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
				if (nextMesh)
				{
					nextMesh->setKernel(value, a_affectChildren);
				}
			}
		}

		//createVoxSurMesh();

		return true;
	}
	else
	{
		cout << "Error! Input Mesh pointer is NULL!" << endl;
		return false;
	}
}

void cSurfaceMesh::setIdxTouch(int value, const bool a_affectChildren)
{
	idx_touch = value;
	if(a_affectChildren)
	{
		unsigned int i, numItems;
		numItems = m_children.size();
		for (i=0; i<numItems; i++)
		{
			cGenericObject *nextObject = m_children[i];

			cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
			if (nextMesh)
			{
				nextMesh->setIdxTouch(value, a_affectChildren);
			}
		}
	}
}


void cSurfaceMesh::setGridDensity(int value, const bool a_affectChildren)
{
	if(value >= 1)
		gridDim = value;
	else
		gridDim = 1;

	if(a_affectChildren)
	{
		unsigned int i, numItems;
        numItems = m_children.size();
        for (i=0; i<numItems; i++)
        {
            cGenericObject *nextObject = m_children[i];

            cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
            if (nextMesh)
            {
                nextMesh->setGridDensity(value, a_affectChildren);
            }
        }
	}
}

void cSurfaceMesh::enableGrid(bool value, const bool a_affectChildren)
{
	flag_grid = value;

	if(a_affectChildren)
	{
		unsigned int i, numItems;
        numItems = m_children.size();
        for (i=0; i<numItems; i++)
        {
            cGenericObject *nextObject = m_children[i];

            cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
            if (nextMesh)
            {
                nextMesh->enableGrid(value, a_affectChildren);
            }
        }
	}
}

void cSurfaceMesh::enableMesh(bool value, const bool a_affectChildren)
{
	flag_mesh = value;

	if(a_affectChildren)
	{
		unsigned int i, numItems;
        numItems = m_children.size();
        for (i=0; i<numItems; i++)
        {
            cGenericObject *nextObject = m_children[i];

            cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
            if (nextMesh)
            {
				nextMesh->enableMesh(value, a_affectChildren);
            }
        }
	}
}

void cSurfaceMesh::enableParticleCoupling(bool value, const bool a_affectChildren)
{
	flag_particle_coupling = value;

	if(a_affectChildren)
	{
		unsigned int i, numItems;
		numItems = m_children.size();
		for (i=0; i<numItems; i++)
		{
			cGenericObject *nextObject = m_children[i];

			cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
			if (nextMesh)
			{
				nextMesh->enableParticleCoupling(value, a_affectChildren);
			}
		}
	}
}

void cSurfaceMesh::enableLastLevel(bool value, const bool a_affectChildren)
{
	flag_last_level = value;

	if(a_affectChildren)
	{
		unsigned int i, numItems;
		numItems = m_children.size();
		for (i=0; i<numItems; i++)
		{
			cGenericObject *nextObject = m_children[i];

			cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
			if (nextMesh)
			{
				nextMesh->enableLastLevel(value, a_affectChildren);
			}
		}
	}
}

void cSurfaceMesh::enableVox(bool value, const bool a_affectChildren)
{
	flag_vox = value;

	if(a_affectChildren)
	{
		unsigned int i, numItems;
        numItems = m_children.size();
        for (i=0; i<numItems; i++)
        {
            cGenericObject *nextObject = m_children[i];

            cSurfaceMesh *nextMesh = dynamic_cast<cSurfaceMesh*>(nextObject);
            if (nextMesh)
            {
                nextMesh->enableVox(value, a_affectChildren);
            }
        }
	}
}

void cSurfaceMesh::renderVox()
{
	if(vm == NULL)
	{
		return;
	}
	if(!vm->num_surface_vox)
	{
		return;
	}

	double v_dim = vm->vox_size * 0.8;

	glColor3f(1.0, 1.0, 0.1);
	if (!vm->haptic_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->haptic_node_list.size() << endl;
		for (hi = vm->haptic_node_list.begin(); hi != vm->haptic_node_list.end(); hi++)
		{
			//renderCube(0.01, hi->target_position(0),hi->target_position(1), hi->target_position(2));
			renderCube(0.01, hi->duplicates[0]->target_position(0),hi->duplicates[0]->target_position(1), hi->duplicates[0]->target_position(2));
		}
	}
	if (!vm->ghost_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->ghost_node_list.size() << endl;
		for (hi = vm->ghost_node_list.begin(); hi != vm->ghost_node_list.end(); hi++)
		{
			//renderCube(0.01, hi->target_position(0),hi->target_position(1), hi->target_position(2));
			renderCube(0.01, hi->duplicates[0]->target_position(0),hi->duplicates[0]->target_position(1), hi->duplicates[0]->target_position(2));
		}
	}
	/*
	glColor3f(0.0, 1.0, 0.1);
	if (!vm->haptic_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->haptic_node_list.size() << endl;
		for (hi = vm->haptic_node_list.begin(); hi != vm->haptic_node_list.end(); hi++)
		{
			renderCube(0.01, hi->static_position(0),hi->static_position(1), hi->static_position(2));
		}
	}
	*/
	glColor3f(1.0, 0.0, 0.1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}

	glColor4f(1.0, 0.0, 0.1, 0.2);

	glPolygonMode(GL_FRONT, GL_FILL);
	vox_iter = vm->surface_vox_list.begin();

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		if(idx_touch > -1)
		{
			if (m->face_list[idx_touch].node0->incident_cluster[0]->vox_list[0] == (*vox_iter))
			{
				glPushMatrix();
				glColor3f(0.0, 1.0, 0.1);
				Vector3d center = m->face_list[idx_touch].node0->incident_cluster[0]->current_center;
				renderCube(0.01, center(0), center(1), center(2));
				glPopMatrix();
				glColor4f(0.0, 1.0, 0.1, 0.2);
			}
			else
				glColor4f(1.0, 0.0, 0.1, 0.2);

		}

		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement; 
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}

	double cube_size = v_dim*0.1;
	// need to differentiate selected and none selected (surface) voxel
	//if (flag_selection) 
	//{
	//	glColor3f(1.0, 1.0, 1.0);
	//	vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

	//	for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
	//	{
	//		double wx, wy, wz;
	//		Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;

	//		//gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

	//		//if (isSelected(wx, wy))
	//		//{
	//		//	renderCube(cube_size, p(0), p(1), p(2));
	//		//}
	//	}
	//}

	glColor3f(1.0, 1.0, 1.0);
	vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();
	n_iter = vm->anchor_node_list.begin();
	for(; n_iter!= vm->anchor_node_list.end(); ++n_iter)
	{
		Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;
		renderCube(cube_size, p(0), p(1), p(2));
	}
	for(int i = 0; i < 2; i++)
	{
		if (vm->active_node[i])
		{
			glColor3f(0.0, 0.0, 1.0);
			Vector3d p = vm->active_node[i]->coordinate + vm->active_node[i]->displacement;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
}

void cSurfaceMesh::renderLevelVoxMesh(const Level * plevel)
{
	if(plevel->voxmesh_level == NULL)
		return;

	VoxMesh* vm = plevel->voxmesh_level;
	if (!vm->num_surface_vox)
	{
		return;
	}

	double v_dim = vm->vox_size * 0.5;

	glColor3fv(vox_mesh_color);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();
	//cout << "surface vox number " << vm->surface_vox_list.size() << endl;

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			*/
			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_2->target_position;
			p3 = (*vox_iter)->node_1->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			/*
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			*/

			p0 = (*vox_iter)->node_4->target_position;
			p1 = (*vox_iter)->node_5->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_7->target_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			*/

			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_4->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_3->target_position;


			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			/*
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			*/

			p0 = (*vox_iter)->node_1->target_position;
			p1 = (*vox_iter)->node_2->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_5->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			/*
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			*/
			
			p0 = (*vox_iter)->node_2->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_6->target_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			*/

			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_1->target_position;
			p2 = (*vox_iter)->node_5->target_position;
			p3 = (*vox_iter)->node_4->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
	
	glColor4f(vox_mesh_color[0], vox_mesh_color[1], vox_mesh_color[2], 0.2);

	glPolygonMode(GL_FRONT, GL_FILL);
	vox_iter = vm->surface_vox_list.begin();

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			*/
			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_2->target_position;
			p3 = (*vox_iter)->node_1->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			/*
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			*/

			p0 = (*vox_iter)->node_4->target_position;
			p1 = (*vox_iter)->node_5->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_7->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			*/

			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_4->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_3->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			/*
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			*/
			p0 = (*vox_iter)->node_1->target_position;
			p1 = (*vox_iter)->node_2->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_5->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			/*
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			*/

			p0 = (*vox_iter)->node_2->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_6->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			*/
			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_1->target_position;
			p2 = (*vox_iter)->node_5->target_position;
			p3 = (*vox_iter)->node_4->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
		
	double cube_size = v_dim*0.2;
	// need to differentiate selected and none selected (surface) voxel
	/*
	// render selected nodes for anchor nodes
	glColor3fv(anchor_node_color);
	if (flag_selection) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			
			Vector3d p = (*n_iter)->target_position;
			//Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected4Mesh(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	*/
	
	glColor3f(1.0, 1.0, 1.0);
	// render anchor nodes
	if (!vm->anchor_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->anchor_node_list.begin();
		for(; n_iter!=vm->anchor_node_list.end(); ++n_iter)
		{
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;

			renderCube(cube_size, p(0), p(1), p(2));
		}
	}

	for(int i = 0; i < 2; i++)
	{
		if (vm->active_node[i])
		{
			glColor3f(0.0, 0.0, 1.0);
			Vector3d p = vm->active_node[i]->coordinate + vm->active_node[i]->displacement;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
	/*
	// render selected nodes for constraint nodes
	glColor3fv(active_node_color);
	if (flag_show_constraints) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			
			Vector3d p = (*n_iter)->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	
	// render constraint nodes
	if (!vm->constraint_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->constraint_node_list.begin();
		for(; n_iter!=vm->constraint_node_list.end(); ++n_iter)
		{
			
			Vector3d p = (*n_iter)->target_position;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
	*/
	/*
	// render selected nodes for mass nodes
	glColor3fv(anchor_node_color);
	if (flag_show_mass) 
	{
		vector<Node>::const_iterator n_iter = vm->node_list.begin();

		for (; n_iter != vm->node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			
			Vector3d p = n_iter->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	*/
	

	////render constraint_node
	//if (vm->constraint_node)
	//{
	//	glColor3fv(active_node_color);
	//	/*Vector3d p = vm->constraint_node->coordinate + vm->constraint_node->displacement;*/
	//	Vector3d p = vm->constraint_node->target_position;
	//	renderCube(cube_size, p(0), p(1), p(2));
	//}


}

void cSurfaceMesh::renderLevelVoxMesh4Leaf(const Level * plevel, float * level_color)
{
	if(plevel->voxmesh_level == NULL)
		return;

	VoxMesh* vm = plevel->voxmesh_level;
	if (!vm->num_surface_vox)
	{
		return;
	}

	double v_dim = vm->vox_size * 0.5;

	
	glColor3fv(level_color);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();
	//cout << "surface vox number " << vm->surface_vox_list.size() << endl;

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		if(!plevel->voxmesh_level->cluster_list[(*vox_iter)->clusterid].flag_leafCluster)
			continue;

		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			*/
			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_2->target_position;
			p3 = (*vox_iter)->node_1->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			/*
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			*/

			p0 = (*vox_iter)->node_4->target_position;
			p1 = (*vox_iter)->node_5->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_7->target_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			*/

			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_4->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_3->target_position;


			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			/*
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			*/

			p0 = (*vox_iter)->node_1->target_position;
			p1 = (*vox_iter)->node_2->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_5->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			/*
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			*/
			
			p0 = (*vox_iter)->node_2->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_6->target_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			*/

			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_1->target_position;
			p2 = (*vox_iter)->node_5->target_position;
			p3 = (*vox_iter)->node_4->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
	
	glColor4f(level_color[0], level_color[1], level_color[2], 0.2);

	glPolygonMode(GL_FRONT, GL_FILL);
	vox_iter = vm->surface_vox_list.begin();

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		if(!plevel->voxmesh_level->cluster_list[(*vox_iter)->clusterid].flag_leafCluster)
			continue;

		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			*/
			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_2->target_position;
			p3 = (*vox_iter)->node_1->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			/*
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			*/

			p0 = (*vox_iter)->node_4->target_position;
			p1 = (*vox_iter)->node_5->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_7->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			*/

			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_4->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_3->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			/*
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			*/
			p0 = (*vox_iter)->node_1->target_position;
			p1 = (*vox_iter)->node_2->target_position;
			p2 = (*vox_iter)->node_6->target_position;
			p3 = (*vox_iter)->node_5->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			/*
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			*/

			p0 = (*vox_iter)->node_2->target_position;
			p1 = (*vox_iter)->node_3->target_position;
			p2 = (*vox_iter)->node_7->target_position;
			p3 = (*vox_iter)->node_6->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			*/
			p0 = (*vox_iter)->node_0->target_position;
			p1 = (*vox_iter)->node_1->target_position;
			p2 = (*vox_iter)->node_5->target_position;
			p3 = (*vox_iter)->node_4->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
		
	double cube_size = v_dim*0.2;
	// need to differentiate selected and none selected (surface) voxel
	/*
	// render selected nodes for anchor nodes
	glColor3fv(anchor_node_color);
	if (flag_show_selection) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			Vector3d p = (*n_iter)->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	*/
	glColor3f(1.0, 1.0, 1.0);
	// render anchor nodes
	if (!vm->anchor_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->anchor_node_list.begin();
		for(; n_iter!=vm->anchor_node_list.end(); ++n_iter)
		{
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
	for(int i = 0; i < 2; i++)
	{
		if (vm->active_node[i])
		{
			glColor3f(0.0, 0.0, 1.0);
			Vector3d p = vm->active_node[i]->coordinate + vm->active_node[i]->displacement;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
	/*
	// render selected nodes for constraint nodes
	glColor3fv(active_node_color);
	if (flag_show_constraints) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			
			Vector3d p = (*n_iter)->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	
	// render constraint nodes
	if (!vm->constraint_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->constraint_node_list.begin();
		for(; n_iter!=vm->constraint_node_list.end(); ++n_iter)
		{
			
			Vector3d p = (*n_iter)->target_position;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
	*/
	/*
	// render selected nodes for mass nodes
	glColor3fv(anchor_node_color);
	if (flag_show_mass) 
	{
		vector<Node>::const_iterator n_iter = vm->node_list.begin();

		for (; n_iter != vm->node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			
			Vector3d p = n_iter->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	*/

	/*
	//render constraint_node
	if (vm->constraint_node)
	{
		glColor3fv(active_node_color);
		
		Vector3d p = vm->constraint_node->target_position;
		renderCube(cube_size, p(0), p(1), p(2));
	}
	*/

	//if((!plevel->voxmesh_level->constraint_node_list.empty()) && (!p_kernel->flag_forceNode))
	//{
	//	vector<Node*>::const_iterator n_iter = plevel->voxmesh_level->constraint_node_list.begin();
	//	for(; n_iter!=plevel->voxmesh_level->constraint_node_list.end(); ++n_iter)
	//	{
	//		glColor4f(0.0, 0.0, 1.0, 0.4);
	//		Vector3d p = (*n_iter)->prescribed_position;
	//		renderCube(cube_size, p(0), p(1), p(2));
	//	}
	//}
}

void cSurfaceMesh::renderCube(double v_dim, double vol_center_x, double vol_center_y, double vol_center_z)
{
	// render six quads
	glBegin(GL_QUADS);

	// face up
	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z - v_dim);

	// face down
	glNormal3d(0.0, -1.0, 0.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z + v_dim);

	// face left
	glNormal3d(-1.0, 0.0, 0.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z + v_dim);

	// face right
	glNormal3d(1.0, 0.0, 0.0);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z - v_dim);

	// face front
	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z + v_dim);

	// face back
	glNormal3d(0.0, 0.0, -1.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z - v_dim);

	glEnd();
}

void cSurfaceMesh::createVoxSurMesh()
{
	/* create a cMesh to save the surface vox point, and treated it as the child mesh */
	if ((m_vertices.size() > 0) || (m_triangles.size() > 0))
    {
        return;
    }
	if(vm != NULL)
	{
		if(vm->num_surface_node)
		{
			int num = getNumChildren();
			if( num > 1)
				deleteChild(getChild(num-1));

			cSurfaceMesh * newMesh = createMesh();
			addChild(newMesh);

			// Give him a default color
			setVertexColor(cColorf(1.0,1.0,1.0,1.0),1);
			setUseVertexColors(true, true);
			setUseMaterial(false, true);
			setUseTransparency(true, true);

			double v_dim = vm->vox_size * 0.5;

			vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();

			for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
			{
				Vector3d p0, p1, p2, p3, n1, n2, n;
				int vertices[4]; 

				if ((*vox_iter)->flag_top_face)
				{
					p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
					p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
					p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
					p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

					//add 0, 1, 2, 3 into mesh/////////////////////////
					vertices[0] = newMesh->newVertex(p0[0], p0[1], p0[2]);
					vertices[1] = newMesh->newVertex(p1[0], p1[1], p1[2]);
					vertices[2] = newMesh->newVertex(p2[0], p2[1], p2[2]);
					vertices[3] = newMesh->newVertex(p3[0], p3[1], p3[2]);

					newMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
					newMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
					////////////////////////////////////////////////////////
				}

				if ((*vox_iter)->flag_bottom_face)
				{
					p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
					p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
					p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
					p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

					//add 0, 1, 2, 3 into mesh/////////////////////////
					vertices[0] = newMesh->newVertex(p0[0], p0[1], p0[2]);
					vertices[1] = newMesh->newVertex(p1[0], p1[1], p1[2]);
					vertices[2] = newMesh->newVertex(p2[0], p2[1], p2[2]);
					vertices[3] = newMesh->newVertex(p3[0], p3[1], p3[2]);

					newMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
					newMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
					////////////////////////////////////////////////////////
				}


				if ((*vox_iter)->flag_left_face)
				{
					p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
					p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
					p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
					p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

					//add 0, 1, 2, 3 into mesh/////////////////////////
					vertices[0] = newMesh->newVertex(p0[0], p0[1], p0[2]);
					vertices[1] = newMesh->newVertex(p1[0], p1[1], p1[2]);
					vertices[2] = newMesh->newVertex(p2[0], p2[1], p2[2]);
					vertices[3] = newMesh->newVertex(p3[0], p3[1], p3[2]);

					newMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
					newMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
					////////////////////////////////////////////////////////
				}

				if ((*vox_iter)->flag_right_face)
				{
					p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
					p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
					p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
					p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

					//add 0, 1, 2, 3 into mesh/////////////////////////
					vertices[0] = newMesh->newVertex(p0[0], p0[1], p0[2]);
					vertices[1] = newMesh->newVertex(p1[0], p1[1], p1[2]);
					vertices[2] = newMesh->newVertex(p2[0], p2[1], p2[2]);
					vertices[3] = newMesh->newVertex(p3[0], p3[1], p3[2]);

					newMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
					newMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
					////////////////////////////////////////////////////////
				}

				if ((*vox_iter)->flag_front_face)
				{
					p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
					p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
					p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
					p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

					//add 0, 1, 2, 3 into mesh/////////////////////////
					vertices[0] = newMesh->newVertex(p0[0], p0[1], p0[2]);
					vertices[1] = newMesh->newVertex(p1[0], p1[1], p1[2]);
					vertices[2] = newMesh->newVertex(p2[0], p2[1], p2[2]);
					vertices[3] = newMesh->newVertex(p3[0], p3[1], p3[2]);

					newMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
					newMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
					////////////////////////////////////////////////////////
				}

				if ((*vox_iter)->flag_back_face)
				{
					p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
					p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
					p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
					p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

					//add 0, 1, 2, 3 into mesh/////////////////////////
					vertices[0] = newMesh->newVertex(p0[0], p0[1], p0[2]);
					vertices[1] = newMesh->newVertex(p1[0], p1[1], p1[2]);
					vertices[2] = newMesh->newVertex(p2[0], p2[1], p2[2]);
					vertices[3] = newMesh->newVertex(p3[0], p3[1], p3[2]);

					newMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
					newMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
					////////////////////////////////////////////////////////

				}
			}//for

	
			// compute normals
			newMesh->computeAllNormals();
			newMesh->flag_vox = false;
		}
	}
}

//bool cSurfaceMesh::isSelected4Mesh(double x, double y)
//{
//	y = m_displayH_Mesh - y;
//	//x = x - m_displayW;
//
//	if (x >= upper_left_x_Mesh && x <= bottom_right_x_Mesh && y >= upper_left_y_Mesh && y <= bottom_right_y_Mesh)
//		return true;
//	else 
//		return false;
//}

void cSurfaceMesh::renderParticleCoupling(const Level * plevel)
{
	if(plevel->voxmesh_level == NULL)
		return;

	VoxMesh* vm = plevel->voxmesh_level;
	if (!vm->num_surface_vox)
	{
		return;
	}

	glColor3f(0.0, 1.0, 0.1);
	if (!vm->haptic_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->haptic_node_list.size() << endl;
		for (hi = vm->haptic_node_list.begin(); hi != vm->haptic_node_list.end(); hi++)
		{
			//renderCube(0.01, hi->target_position(0),hi->target_position(1), hi->target_position(2));
			renderCube(0.01, hi->duplicates[0]->target_position(0),hi->duplicates[0]->target_position(1), hi->duplicates[0]->target_position(2));
		}
	}
	if (!vm->ghost_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->ghost_node_list.size() << endl;
		for (hi = vm->ghost_node_list.begin(); hi != vm->ghost_node_list.end(); hi++)
		{
			//renderCube(0.01, hi->target_position(0),hi->target_position(1), hi->target_position(2));
			renderCube(0.01, hi->duplicates[0]->target_position(0),hi->duplicates[0]->target_position(1), hi->duplicates[0]->target_position(2));
		}
	}
	glColor3f(1.0, 0.0, 0.1);
	if (!vm->haptic_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->haptic_node_list.size() << endl;
		for (hi = vm->haptic_node_list.begin(); hi != vm->haptic_node_list.end(); hi++)
		{
			//renderCube(0.01, hi->target_position(0),hi->target_position(1), hi->target_position(2));
			renderCube(0.01, hi->duplicates[0]->static_position(0),hi->duplicates[0]->static_position(1), hi->duplicates[0]->static_position(2));
		}
	}
	if (!vm->ghost_node_list.empty())
	{
		vector<Node>::const_iterator hi;
		//cout << vm->ghost_node_list.size() << endl;
		for (hi = vm->ghost_node_list.begin(); hi != vm->ghost_node_list.end(); hi++)
		{
			//renderCube(0.01, hi->target_position(0),hi->target_position(1), hi->target_position(2));
			renderCube(0.01, hi->duplicates[0]->static_position(0),hi->duplicates[0]->static_position(1), hi->duplicates[0]->static_position(2));
		}
	}
}