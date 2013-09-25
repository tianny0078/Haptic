
#include "widgetGL.h"
#include <iostream>
using namespace std;
#include "GL/glut.h"

//typedef   void  ( widgetGL::* func)(void);
//func ptr = &widgetGL::updateHaptics;

widgetGL* Global_widgetGLpt;

void warperwidgetGL()
{
	Global_widgetGLpt->updateHaptics();
}
widgetGL::widgetGL(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
	//initializing World, Camera, Light, Haptic handler, Haptic tool
	//and get the num of haptic device, if num is 0, then return;
	//
	m_pworld = new cWorld();
	m_pcamera = new cCamera(m_pworld);
	m_pworld->addChild(m_pcamera);
	m_plight = new cLight(m_pworld);
	m_pcamera->addChild(m_plight);

	for(int i = 0; i < 2; i++)
	{
		m_ptool[i] = new cShapeMatching3dofPointer(m_pworld);
		m_pworld->addChild(m_ptool[i]);
	}

	rootLabel = new cGenericObject();
	m_pcamera->m_front_2Dscene.addChild(rootLabel);

	wc = new cGenericObject();
	m_pworld->addChild(wc);

	m_pMesh = new cSurfaceMesh(m_pworld);
	m_pworld->addChild(m_pMesh);

	m_simulation = false;
	m_simulationfinished = false;

	m_displayW = 0;
	m_displayH = 0;

	//openGL parameters
	m_bgColorR = 0.8;
	m_bgColorG = 0.8;
	m_bgColorB = 0.8;

	//flags
	flag_show_grid = false;
	flag_show_mesh = false;
	flag_show_vox = false;
	flag_show_selection = false;
	//flag_initialized = false;
	flag_simulating = false;
	flag_istouched = false;
	flag_show_mass = false;
	flag_show_last_level = false;

	// selection square
	bottom_right_x = -1.0;
	bottom_right_y = -1.0;
	upper_left_x   = -1.0;
	upper_left_y   = -1.0;

	m_camStatus = IDLING;
	// accept key input

	objectRot.identity();

	m_numOfHaptic = 0;

	workspaceScaleFactor = 0.0;
	stiffnessMax = 0.0;
	deviceForceScale = 1.0;

	testoutput.open("output.txt", ios::out);
	flag_output = false;

	setFocusPolicy(Qt::StrongFocus);
}
widgetGL::~widgetGL()
{
	testoutput.close();
	close();
}

void widgetGL::init()
{
	m_pworld->setBackgroundColor(m_bgColorR, m_bgColorG, m_bgColorB);
	//m_pworld->setShowFrame(true);
	m_pcamera->set(cVector3d (5.0, 0.0, 0.0),    // camera position (eye)
		cVector3d (0.0, 0.0, 0.0),    // lookat position (target)
		cVector3d (0.0, 0.0, 1.0));
	m_pcamera->setClippingPlanes(0.01, 10.0);

	rootLabel->setPos(10, 750, 0);
	for( int i = 0; i < 3; i++)
	{
		cLabel * newlabel = new cLabel();
		rootLabel->addChild(newlabel);
		newlabel->setPos(0, -20*i, 0);
		newlabel->m_fontColor.set(0.6, 0.6, 0.6);
		labels[i] = newlabel;
	}
	
	wc->setPos(0.1, -2.5, -1.8);
	wc->setShowFrame(true); 
	wc->setFrameSize(0.2, 0.5);
	objectRot.invert();
	wc->setRot(objectRot);

	for(int i = 0; i < m_numOfHaptic; i++)
	{
	// create a small vertical white magnetic line that will be activated when the
    // user deforms the mesh.
		graspLine[i] = new cShapeLine(cVector3d(0,0,0), cVector3d(0,0,0));
		m_pworld->addChild(graspLine[i]);
		graspLine[i]->m_ColorPointA.set(0.6, 0.6, 0.6);
		graspLine[i]->m_ColorPointB.set(0.6, 0.6, 0.6);
		graspLine[i]->setShowEnabled(false);
		// disable haptic feedback for now
		graspLine[i]->setHapticEnabled(false);
		graspActive[i] = false;
	}

	m_pMesh->setPos(0.0, 0.0, 0.0);
	//m_pMesh->setAsGhost(true);
	//m_pMesh->setHapticEnabled(false, true);
	m_pMesh->setFrameSize(1.0, 0.6);
	m_pMesh->setRot(objectRot);
	objectRot.identity();
    // compute a boundary box
    // compute collision detection algorithm
	//m_pMesh->deleteCollisionDetector(true);
    //m_pMesh->createAABBCollisionDetector(1.01 * 1, true, false);
	//m_pMesh->createSphereTreeCollisionDetector(1.01 * 1, true, false);
	//m_pMesh->setWireMode(true);
	


	m_plight->setEnabled(true);
	m_plight->setPos(cVector3d(2.0, 0.5, 1.0));
	m_plight->setDir(cVector3d(-2.0, 0.5, 1.0));
	
	cHapticDeviceInfo info[2];
	for(int i = 0; i < m_numOfHaptic; i++)
	{
		if (m_phapticDevice[i])
		{
			info[i] = m_phapticDevice[i]->getSpecifications();
		}
	
	
		if (m_ptool[i]->getHapticDevice())
		{
			m_ptool[i]->stop();
		}
		m_ptool[i]->setHapticDevice(m_phapticDevice[i]);
		m_ptool[i]->start();
		m_ptool[i]->setWorkspaceRadius(2.0);
		m_ptool[i]->setRadius(0.03);
		m_ptool[i]->m_deviceSphere->setShowEnabled(false);
		

		/* prepare for collision detection */
		//m_ptool[i]->m_proxyPointForceModel->setProxyRadius(0.05);

		m_proxyRadius = 0.03;
		m_ptool[i]->m_proxyPointForceModel->setProxyRadius(m_proxyRadius);
		// informe the finger-proxy force renderer to only check one side of triangles
		m_ptool[i]->m_proxyPointForceModel->m_collisionSettings.m_checkBothSidesOfTriangles = false;
		m_ptool[i]->m_proxyPointForceModel->m_useDynamicProxy = false;
		m_ptool[i]->m_proxyPointForceModel->m_useForceShading = true;


		workspaceScaleFactor = m_ptool[i]->getWorkspaceScaleFactor();
		stiffnessMax = info[i].m_maxForceStiffness / workspaceScaleFactor;
		double force_reference = 10.0;
		deviceForceScale = info[i].m_maxForce / force_reference;
	
		//m_pMesh->setStiffness(0.8*stiffnessMax, true);
		//m_pMesh->setFriction(0.1, 0.2, true);

	}

}

void widgetGL::initializeGL()
{
	
	cout << "Initializing...." << endl;

	init();

	//object = new cShapeTorus(0.23, 0.50);
	//m_pworld->addChild(object);
	//object->setPos(0.0, 0.0, 0.0);
	//object->rotate( cVector3d(0, 1, 0), cDegToRad(90));


	//m_object = new cSurfaceMesh(m_pworld);
//	m_pworld->addChild(m_object);
//	m_object->setPos(0.0, 0.0, 0.0);
//	m_object->setUseCulling(false, true);
//	m_object->setAsGhost(true);

	/*
	int count = 0;
	// here we create a small cloud of triangles
	double ang = 0;
	double angStep = 5;
	while (ang < 360)
	{
		double radius = 0.2;
		double radiusStep = 0.1;
		double tang = ang;
		while (radius < 1.0)
		{
			
			if (count == 3)
			{
				break;
			}
			
			// we create three vectors p0, p1, p2 that describe the position of the vertices
			cVector3d p0 = cVector3d(0.0, radius * cCosDeg(tang), radius * cSinDeg(tang));
			cVector3d p1 = cVector3d(0.0, (radius + radiusStep) * cCosDeg(tang+angStep), (radius + radiusStep) * cSinDeg(tang+angStep));
			cVector3d p2 = cVector3d(0.0, (radius + radiusStep) * cCosDeg(tang-angStep), (radius + radiusStep) * cSinDeg(tang-angStep));

			// we generate a triangles by passing the position of the new vertices
			unsigned int index = m_object->newTriangle(p0, p1, p2);

			// for each triangle we define a color
			cColorf color;
			color.set(ang / 360, 1.0 - ang/360, radius);

			// we get a pointer to the latest created triangle
			cTriangle* triangle = m_object->getTriangle(index);

			// we get pointers to its respective vertices.
			cVertex* vertex0 = triangle->getVertex0();
			cVertex* vertex1 = triangle->getVertex1();
			cVertex* vertex2 = triangle->getVertex2();

			// for each vertex we attribute the new color. In this example
			// the color is the same for all vertices, but it is of course
			// possible to define a specific color for each vertex
			vertex0->setColor(color);
			vertex1->setColor(color);
			vertex2->setColor(color);

			// we increment the different counters
			radiusStep = 1.1 * radiusStep;
			radius = radius + 1.2 * radiusStep;
			tang = tang + 0.5 * angStep;

			//
			count++;
		}
		ang = ang + 3 * angStep;
	}

	m_object->setUseVertexColors(true);
	m_object->computeBoundaryBox(true);
	//m_object->createAABBCollisionDetector(1.01 * m_proxyRadius, true, false);
	m_object->createSphereTreeCollisionDetector(1.5 * m_proxyRadius, true, false);
	//m_object->createSphereTreeCollisionDetector(0, true, false);
	
	//m_object->setShowCollisionTree(true,true);
	//cColorf color(1.0, 0.0, 0.0, 1.0);
	//m_object->setCollisionDetectorProperties(7, color, true);

	m_object->setStiffness(stiffnessMax, true);
	m_object->setFriction(0.1, 0.2, true);
*/
	//begin
	
	
}

void widgetGL::beginHaptic()
{
	m_simulation = true;
	m_hapticsThread = new cThread();
	Global_widgetGLpt = this;
	m_hapticsThread->set(warperwidgetGL, CHAI_THREAD_PRIORITY_HAPTICS);
}

void widgetGL::resizeGL(int w, int h)
{

	glViewport(0, 0, w, h);
	m_displayW = w;
	m_displayH = h;
	//m_pMesh->m_displayH_Mesh = h;
	
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	// update the size of the viewport

}
void widgetGL::paintGL()
{


	//mesh
	if(m_pKernel->flag_mesh_ready)
	{
		m_pMesh->enableMesh(flag_show_mesh);
		
		string str;
		cStr(str, m_pKernel->p_mesh->number_node);
		str = "rendering mesh: " + str + " node, ";
		cStr(str, m_pKernel->p_mesh->number_face);
		str = str + " face";
		labels[0]->m_string = str;

		str = "simulation: ";
		cStr(str, m_pKernel->num_sim_node);
		str = str + " node";
		labels[1]->m_string = str;

		str = "grid: ";
		cStr(str, m_pKernel->grid_density);
		str = str + " X ";
		cStr(str, m_pKernel->grid_density);
		str = str + " X ";
		cStr(str, m_pKernel->grid_density);
		labels[2]->m_string = str;
	}
	else
		m_pMesh->enableMesh(false);
	
	//mesh grid outside
	//m_pMesh->setGridDensity(m_pKernel->grid_density);
	m_pMesh->enableGrid(flag_show_grid);

	//mesh volume inside & outside
	m_pMesh->enableVox(flag_show_vox);
	//enable last level display
	m_pMesh->enableLastLevel(flag_show_last_level);

	m_pcamera->renderView(m_displayW, m_displayH);

	////get the model view, projection, and viewport matrices
	//glGetDoublev(GL_MODELVIEW_MATRIX, currentmodelview);
	//glGetDoublev(GL_PROJECTION_MATRIX, currentprojection);
	//glGetIntegerv(GL_VIEWPORT, currentviewport);

	if(flag_show_vox || flag_show_mass)
		renderSelectSquare( upper_left_x, upper_left_y, bottom_right_x, bottom_right_y);



	//glColor3d(1.0, 1.0, 0.0);
	glutWireCube(3.0);
	//glutSwapBuffers();

	//if(simulation)	
	//{
	//	//cSleepMs(1000);
	//	updateGL();
	//}

	


}

void widgetGL::updateHaptics(void)
{
	m_simClock.reset();
	while (m_simulation)
	{
		/*
		
		cout << m_pMesh->getNumChildren() << endl; 
		if (m_pMesh->getNumChildren() > 1)
			m_pMesh->getChild(0)->setShowEnabled(false, true);
*/
		m_pKernel->time_counter->StartCounter();
		m_pworld->computeGlobalPositions(true);
		
		for(int i = 0; i < m_numOfHaptic; i++)
		{

		////****************************************************
		////m_ptool[i]->setForcesOFF();
		//// update bounding box (can take a little time)
		//if(m_pMesh)
		//	((cMesh *)m_pMesh)->createSphereTreeCollisionDetector(1 * 1, true, true);
		////m_ptool[i]->setForcesON();

		m_ptool[i]->updatePose();
		//m_ptool[i]->computeInteractionForces();
		////*****************************************************

		m_simClock.stop();
		double timeinterval = m_simClock.getCurrentTimeSeconds();
		m_simClock.reset();
		m_simClock.start();
		
		/////////////////////////////////////////////////////////////////////////////////////////
		//grasp line
		/////////////////////////////////////////////////////////////////////////////////////////

		bool button = m_ptool[i]->getUserSwitch(0);
		// check if tool is touching an object
        cGenericObject* contactObject = m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_object;
		
		//find global position of tool
		cVector3d globalToolPos = m_ptool[i]->getProxyGlobalPos();
		cVector3d localToolPos = m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_localPos;
		if(flag_simulating)
		{
		if(button && graspActive[i])
		{
			if (m_pKernel->p_vox_mesh->active_node[i])
			{	
				Vector3d grasp = m_pKernel->p_vox_mesh->active_node[i]->coordinate + m_pKernel->p_vox_mesh->active_node[i]->displacement;
				graspPos[i].set(grasp[0], grasp[1], grasp[2]);
			}

			// retrieve latest position and orientation of grasped ODE object in world coordinates
			cMatrix3d globalGraspObjectRot = graspObject->getGlobalRot();
			cVector3d globalGraspObjectPos = graspObject->getGlobalPos();

			// compute the position of the grasp point on object in global coordinates
			globalGraspPos[i] = globalGraspObjectPos + cMul(globalGraspObjectRot, graspPos[i]);

			globalGraspObjectRot.invert();
			localToolPos = cMul(globalGraspObjectRot, globalToolPos);

			cVector3d offset = localToolPos - graspPos[i];
			cVector3d offset2 = globalToolPos - globalGraspPos[i];

			// model a spring between both points
            double STIFFNESS = 4;
            cVector3d force = STIFFNESS * offset;
			cVector3d force2 = 5 * offset2;
			force2 = force2 * deviceForceScale;

			if (m_pKernel->p_vox_mesh->active_node)
			{
				
				Vector3d stiffForce = m_pKernel->p_vox_mesh->active_node[i]->coordinate;
				stiffForce[0] = force.x;
				stiffForce[1] = force.y;
				stiffForce[2] = force.z;

				m_pKernel->p_vox_mesh->active_node[i]->force = stiffForce;
				for(int j=0; j < m_pKernel->p_vox_mesh->active_node[i]->duplicates.size(); ++j)
				{
					m_pKernel->p_vox_mesh->active_node[i]->duplicates[j]->force = m_pKernel->p_vox_mesh->active_node[i]->force;
				}
			}

			// apply attraction force (grasp) onto object
            //graspObject->addGlobalForceAtGlobalPos(force, globalGraspPos);

            // scale magnitude and apply opposite force to haptic device
            m_ptool[i]->m_lastComputedGlobalForce.copyfrom(cMul(-1.0, force2));

			// update both end points of the line which is used for display purposes only
			graspLine[i]->m_pointA = globalGraspPos[i];
			graspLine[i]->m_pointB = globalToolPos;
		}
		else
		{
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			/*
			// was the user grasping the object at the previous simulation loop
            if (graspActive[i])
            {
                // we disable grasping
                graspActive[i] = false;

                // we hide the virtual line between the tool and the grasp point
                graspLine[i]->setShowEnabled(false);

                // we enable haptics interaction between the tool and the previously grasped object
                if (graspObject != NULL)
                {
                    graspObject->setHapticEnabled(true, true);
					// disable forces
					m_ptool[i]->setForcesOFF();

					// update bounding box (can take a little time)
					((cMesh *)graspObject)->createAABBCollisionDetector(1.01 * 1, true, false);
            
					// enable forces again
					m_ptool[i]->setForcesON();
                }

				if (m_pKernel->p_vox_mesh->active_node)
				{
					m_pKernel->p_vox_mesh->active_node[i]->force.setZero();
					for(int j = 0; j < m_pKernel->p_vox_mesh->active_node[i]->duplicates.size(); ++j)
					{
						m_pKernel->p_vox_mesh->active_node[i]->duplicates[j]->force.setZero();
					}
					m_pKernel->p_vox_mesh->active_node[i] = NULL;
				}
            }
			*/
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//// the user is touching an object
            if (contactObject != NULL)
            {
				
				//***********************************************************************
				/*
				if (!flag_istouched)
				{
					m_pMesh->setIdxTouch(m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_triangle->getIndex());
					cVector3d pos = m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_globalPos;
					cMatrix3d tRot;
					pos.sub(m_pMesh->getGlobalPos());
					m_pMesh->getGlobalRot().transr(tRot);
					tRot.mul(pos);

					Vector3d hapticPos;
					hapticPos[0] = pos[0];
					hapticPos[1] = pos[1];
					hapticPos[2] = pos[2];
					m_pKernel->setHapticNode(m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_triangle->getIndex(), hapticPos);
					//cout << "touch" << endl;
					flag_istouched = true;
				}
				else
				{
					
					cVector3d globalDevicePos = m_ptool[i]->getDeviceGlobalPos();
					cMatrix3d tRot;
					globalDevicePos.sub(m_pMesh->getGlobalPos());
					m_pMesh->getGlobalRot().transr(tRot);
					tRot.mul(globalDevicePos);

					Vector3d goalposition;
					goalposition[0] = globalDevicePos[0];
					goalposition[1] = globalDevicePos[1];
					goalposition[2] = globalDevicePos[2];

					//goalposition[0] = 0;
					//goalposition[1] = 1.5;
					//goalposition[2] = 0;
					
					//cout << "haptic "<< endl;
					//cout << goalposition << endl;
					m_pKernel->setGoalPos4HapticNode(goalposition);
					
					


					//cout << "touch again" << endl;
					//istouched = false;
				}
				*/
				//***************************************************************

				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				/*
                // a new object is being grasped
                graspObject = contactObject;

				vector<Node*>::iterator ni = m_pKernel->p_vox_mesh->surface_node_list.begin();
				double x, y, z;
				Vector3d p;

				for (; ni!=m_pKernel->p_vox_mesh->surface_node_list.end(); ++ni)
				{

					p = (*ni)->coordinate + (*ni)->displacement;
					graspPos[i].set(p[0], p[1], p[2]);
					double d = cDistance(graspPos[i], localToolPos);

					if (d < 0.1)
					{
						m_pKernel->p_vox_mesh->active_node[i] = (*ni);

						// retrieve latest position and orientation of grasped ODE object in world coordinates
						cMatrix3d globalGraspObjectRot = graspObject->getGlobalRot();
						cVector3d globalGraspObjectPos = graspObject->getGlobalPos();

						// compute the position of the grasp point on object in global coordinates
						globalGraspPos[i] = globalGraspObjectPos + cMul(globalGraspObjectRot, graspPos[i]);

						break;
					}
				}
				*/
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 // disable haptic interaction between the tool and the grasped device.

//                 // retrieve the haptic interaction force being applied to the tool
//                 cVector3d force = tool->m_lastComputedGlobalForce;

//                 // apply haptic force to ODE object
//                 cVector3d tmpfrc = cNegate(force);
//                 ODEobject->addGlobalForceAtGlobalPos(tmpfrc, pos);
//             }


            }
			//***************************************************************
			/*
			else
			{
				m_pMesh->idx_touch = -1;
				flag_istouched = false;
				
				vector<Node>::iterator ni = m_pKernel->level_list[0]->voxmesh_level->haptic_node_list.begin();
				for (; ni != m_pKernel->level_list[0]->voxmesh_level->haptic_node_list.end(); ni++)
				{
					ni->incident_cluster[0]->haptic_node_list.clear();
				}
				m_pKernel->level_list[0]->voxmesh_level->haptic_node_list.clear();


				vector<Node>::iterator gi = m_pKernel->level_list[0]->voxmesh_level->ghost_node_list.begin();
				for (; gi != m_pKernel->level_list[0]->voxmesh_level->ghost_node_list.end(); gi++)
				{
					gi->incident_cluster[0]->ghost_node_list.clear();
				}
				m_pKernel->level_list[0]->voxmesh_level->ghost_node_list.clear();
				
			}
			*/
			//******************************************************************
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			/*
			if (button && m_pKernel->p_vox_mesh->active_node[i])
			{

				// grasp in now active!
				graspActive[i] = true;

				// enable small line which display the offset between the tool and the grasp point
				graspLine[i]->setShowEnabled(true);


				//graspObject->setHapticEnabled(false, true);
			}
			*/
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//**********************************************
			//m_pKernel->flag_pause = false;
			//if (m_pKernel->flag_simulator_ready)
			//	m_pKernel->simulateNextStep();
			//***********************************************

			//cVector3d forceTouch = m_ptool[i]->getProxyGlobalPos() - m_ptool[i]->getDeviceGlobalPos();
			//forceTouch = 4 * forceTouch * deviceForceScale;
			//m_ptool[i]->m_lastComputedGlobalForce.copyfrom(forceTouch);


			if (button)
			{
				m_ptool[i]->m_proxyPointForceModel->setProxyGlobalPosition(m_ptool[i]->m_deviceGlobalPos);
				flag_istouched = false;
			}
			
			if(flag_simulating)
			{

				Vector3d localforce;
				cVector3d forceTouch;
				m_pKernel->getForce4HapticNode(localforce);
				//cout << localforce << endl;
				forceTouch[0] = localforce[0];
				forceTouch[1] = localforce[1];
				forceTouch[2] = localforce[2];

				// update global position of tool
				cVector3d tPos;
				m_pMesh->getGlobalRot().mulr(forceTouch, tPos);
				tPos.addr(m_pMesh->getGlobalPos(), forceTouch);

				forceTouch = 2 * forceTouch * deviceForceScale;
				m_ptool[i]->m_lastComputedGlobalForce.copyfrom(forceTouch);
				//cout << forceTouch.length() << endl;
				if (flag_output)
					testoutput << forceTouch.length() << endl;
			}
			
			//cout << forceTouch << endl;

			
			//updateGL();
		}
		} 
		
		m_ptool[i]->applyForces();
		m_pKernel->time_counter->StopCounter();
		//cout << m_pKernel->time_counter->GetElapsedTime() << endl;
		}//for i = 0:1
	}
	m_simulationfinished = true;
}

void widgetGL::simulateNextStep()
{

	m_pworld->computeGlobalPositions(true);
	for(int i = 0; i < m_numOfHaptic; i++)
	{
		//****************************************************
		//m_ptool[i]->setForcesOFF();
		// update bounding box (can take a little time)
		//if(m_pMesh)
			//((cMesh *)m_pMesh)->createSphereTreeCollisionDetector(1 * 1, true, true);
		//m_ptool[i]->setForcesON();

		m_ptool[i]->updatePose();
		//if (!flag_istouched)
		m_ptool[i]->computeInteractionForces();
		// check if tool is touching an object
		cGenericObject* contactObject = m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_object;
		//*****************************************************//// the user is touching an object
        if (contactObject != NULL)
        {
				
			//***********************************************************************
			if (!flag_istouched)
			{
				m_pMesh->setIdxTouch(m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_triangle->getIndex());
				cVector3d pos = m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_globalPos;
				cMatrix3d tRot;
				pos.sub(m_pMesh->getGlobalPos());
				m_pMesh->getGlobalRot().transr(tRot);
				tRot.mul(pos);

				Vector3d hapticPos;
				hapticPos[0] = pos[0];
				hapticPos[1] = pos[1];
				hapticPos[2] = pos[2];
				m_pKernel->setHapticNode(m_ptool[i]->m_proxyPointForceModel->m_contactPoint0->m_triangle->getIndex(), hapticPos);
				//cout << "touch" << endl;
				flag_istouched = true;
			}
			else
			{
					
				cVector3d globalDevicePos = m_ptool[i]->getDeviceGlobalPos();
				cMatrix3d tRot;
				globalDevicePos.sub(m_pMesh->getGlobalPos());
				m_pMesh->getGlobalRot().transr(tRot);
				tRot.mul(globalDevicePos);

				Vector3d goalposition;
				goalposition[0] = globalDevicePos[0];
				goalposition[1] = globalDevicePos[1];
				goalposition[2] = globalDevicePos[2];

				//goalposition[0] = 0;
				//goalposition[1] = 1.5;
				//goalposition[2] = 0;
					
				//cout << "haptic "<< endl;
				//cout << goalposition << endl;
				m_pKernel->setGoalPos4HapticNode(goalposition);
					
					


				//cout << "touch again" << endl;
				//istouched = false;
			}

			//**************************************************************
        }
		//***************************************************************
		else
		{
			//m_pMesh->idx_touch = -1;
			flag_istouched = false;
				
			vector<Node>::iterator ni = m_pKernel->level_list[0]->voxmesh_level->haptic_node_list.begin();
			for (; ni != m_pKernel->level_list[0]->voxmesh_level->haptic_node_list.end(); ni++)
			{
				ni->incident_cluster[0]->haptic_node_list.clear();
			}
			m_pKernel->level_list[0]->voxmesh_level->haptic_node_list.clear();


			vector<Node>::iterator gi = m_pKernel->level_list[0]->voxmesh_level->ghost_node_list.begin();
			for (; gi != m_pKernel->level_list[0]->voxmesh_level->ghost_node_list.end(); gi++)
			{
				gi->incident_cluster[0]->ghost_node_list.clear();
			}
			m_pKernel->level_list[0]->voxmesh_level->ghost_node_list.clear();
				
		}
		//******************************************************************
		
		m_pKernel->flag_pause = false;
		if (m_pKernel->flag_simulator_ready)
			m_pKernel->simulateNextStep();

		/*
		if (flag_istouched)
		{
			Vector3d localpos;
			cVector3d posTouch;
			
			//if(m_pKernel->getPos4HapticNode(localpos))
			//{
			//	//cout << localforce << endl;
			//	posTouch[0] = localpos[0];
			//	posTouch[1] = localpos[1];
			//	posTouch[2] = localpos[2];

			//	// update global position of tool
			//	cVector3d tPos;
			//	m_pMesh->getGlobalRot().mulr(posTouch, tPos);
			//	tPos.addr(m_pMesh->getGlobalPos(), posTouch);

			//	
			//	m_ptool[i]->m_proxyPointForceModel->setProxyGlobalPosition(posTouch);
			//}
			//

			Node * ver = m_pKernel->p_mesh->face_list[m_pMesh->idx_touch].node0;
			Vector3d temp = ver->coordinate + ver->displacement;
			posTouch[0] = temp[0];
			posTouch[1] = temp[1];
			posTouch[2] = temp[2];

			// update global position of tool
			cVector3d tPos;
			m_pMesh->getGlobalRot().mulr(posTouch, tPos);
			tPos.addr(m_pMesh->getGlobalPos(), posTouch);


			m_ptool[i]->m_proxyPointForceModel->setProxyGlobalPosition(posTouch);
		}
		*/
		
		//set proxy position

	}
	updateGL();
}

void widgetGL::close()
{
	m_simulation = false;

	while(!m_simulationfinished)
		cSleepMs(100);
	
	for(int i = 0;  i < m_numOfHaptic; i++)
		m_ptool[i]->stop();
}
void widgetGL::timeGL()
{
	updateGL();
}

void widgetGL::setHapticDevice(cGenericHapticDevice * pDevice[], int numOfHaptic)
{
	m_phapticDevice = pDevice;
	m_numOfHaptic = numOfHaptic;
}
//
//void widgetGL::slotUpdateBgR(QString value)
//{
//	m_bgColorR = value.toFloat() / 255.0;
//	m_pworld->setBackgroundColor(m_bgColorR, m_bgColorG, m_bgColorB);
//	updateGL();
//	
//}
//void widgetGL::slotUpdateBgG(QString value)
//{
//	m_bgColorG = value.toFloat() / 255.0;
//	m_pworld->setBackgroundColor(m_bgColorR, m_bgColorG, m_bgColorB);
//	updateGL();
//
//}
//void widgetGL::slotUpdateBgB(QString value)
//{
//	m_bgColorB = value.toFloat() / 255.0;
//	m_pworld->setBackgroundColor(m_bgColorR, m_bgColorG, m_bgColorB);
//	updateGL();
//
//}

void widgetGL::mousePressEvent(QMouseEvent * e)
{
	current_mouse_x = e->x();
	current_mouse_y = e->y();
	current_mouse_button = e->button();

	if (current_mouse_button == Qt::LeftButton)
	{
		if (flag_show_selection && last_mouse_button == Qt::RightButton)
		{
			vector<Node>::iterator n_iter = m_pKernel->p_vox_mesh->node_list.begin();

			for (; n_iter!=m_pKernel->p_vox_mesh->node_list.end(); ++n_iter)
			{

				double wx, wy, wz;
				Vector3d p = n_iter->coordinate + n_iter->displacement;

				gluProject(p[0], p[1], p[2], m_pMesh->currentmodelview, m_pMesh->currentprojection, m_pMesh->currentviewport, &wx, &wy, &wz);

				//wy = m_displayH - wy;
				
				if (isSelected(wx, wy))
				{
					if (!n_iter->flag_anchor_node)
					{
						n_iter->flag_anchor_node = true;
						m_pKernel->p_vox_mesh->anchor_node_list.push_back(&*n_iter);
					}
				}
			}

			char msg[128];
			sprintf(msg, "%d anchor nodes have been chosen", m_pKernel->p_vox_mesh->anchor_node_list.size());
			QMessageBox::information(NULL, tr("success"), tr(msg));

			bottom_right_x = -1.0;
			bottom_right_y = -1.0;
			upper_left_x   = -1.0;
			upper_left_y   = -1.0;
			//m_pMesh->bottom_right_x_Mesh = -1.0;
			//m_pMesh->bottom_right_y_Mesh = -1.0;
			//m_pMesh->upper_left_x_Mesh   = -1.0;
			//m_pMesh->upper_left_y_Mesh   = -1.0;
		}
	}

	if (current_mouse_button == Qt::RightButton)
	{
		if (flag_show_selection || flag_show_mass)
		{
			bottom_right_x = -1.0;
			bottom_right_y = -1.0;
			upper_left_x = current_mouse_x;
			upper_left_y = current_mouse_y;

			//m_pMesh->bottom_right_x_Mesh = -1.0;
			//m_pMesh->bottom_right_y_Mesh = -1.0;
			//m_pMesh->upper_left_x_Mesh = current_mouse_x;
			//m_pMesh->upper_left_y_Mesh = current_mouse_y;
		}
	}
		
	last_mouse_x = current_mouse_x;
	last_mouse_y = current_mouse_y;
	last_mouse_button = current_mouse_button;

	updateGL();
}

bool widgetGL::isSelected(double x, double y)
{
	y = m_displayH - y;
	//x = x - m_displayW;

	if (x >= upper_left_x && x <= bottom_right_x && y >= upper_left_y && y <= bottom_right_y)
		return true;
	else 
		return false;
}

void widgetGL::mouseDoubleClickEvent(QMouseEvent *e)
{
	current_mouse_button = e->button();
	
	if (current_mouse_button == Qt::LeftButton)
	{
		if (flag_show_selection)
		{
			vector<Node*>::iterator ni = m_pKernel->p_vox_mesh->anchor_node_list.begin();
			for (; ni!=m_pKernel->p_vox_mesh->anchor_node_list.end(); ++ni)
			{
				(*ni)->flag_anchor_node = false;
			}
			m_pKernel->p_vox_mesh->anchor_node_list.clear();
			QMessageBox::information(NULL, "success", "all anchor nodes are removed");
		}

		if (flag_simulating)
		{
			vector<Node*>::iterator ni = m_pKernel->p_vox_mesh->surface_node_list.begin();
			double x, y, z;
			Vector3d p;


			for (; ni!=m_pKernel->p_vox_mesh->surface_node_list.end(); ++ni)
			{

				p = (*ni)->coordinate + (*ni)->displacement;
				gluProject(p[0], p[1], p[2], m_pMesh->currentmodelview, m_pMesh->currentprojection, m_pMesh->currentviewport, &x, &y, &z);



				if (isPicked(x, y))
				{
					m_pKernel->p_vox_mesh->active_node[0] = (*ni);
					break;
				}
			}
		}
	}

	if (current_mouse_button == Qt::RightButton)
	{

	}
		
	last_mouse_button = current_mouse_button;
	
	updateGL();
}

bool widgetGL::isPicked(double x, double y)
{
	y = m_displayH - y;
	//x = m_displayW - x;
	if ((current_mouse_x-x)*(current_mouse_x-x) + (current_mouse_y-y)*(current_mouse_y-y) <= 10.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void widgetGL::mouseMoveEvent(QMouseEvent * e)
{
	// mouseMoveEvent is not able to catch what mouse button is down
	double delta_x, delta_y;

	current_mouse_x = e->x();
	current_mouse_y = e->y();

	// truncate if over win boundary 
	current_mouse_x = current_mouse_x > int(m_displayW) ? int(m_displayW) : current_mouse_x;
	current_mouse_x = current_mouse_x < 1 ? 1 : current_mouse_x;

	current_mouse_y = current_mouse_y > int(m_displayH) ? int(m_displayH) : current_mouse_y;
	current_mouse_y = current_mouse_y < 1 ? 1 : current_mouse_y;	
	
	if (last_mouse_button == Qt::LeftButton)
	{
		switch (m_camStatus)
		{
		case PANNING:
			{
					double current_mouse_xd, current_mouse_yd, last_mouse_xd, last_mouse_yd;
					
					screenToFrustum(current_mouse_xd, current_mouse_yd, current_mouse_x, current_mouse_y );
					screenToFrustum(last_mouse_xd, last_mouse_yd, last_mouse_x, last_mouse_y );

					delta_x = last_mouse_xd - current_mouse_xd;
					delta_y = last_mouse_yd - current_mouse_yd;

					m_pcamera->translate(0, delta_x, delta_y);
			}
			break;

		case ROTATING:
			{
				cVector3d vL, vC;

				screenToVector(vC, current_mouse_x, current_mouse_y);
				screenToVector(vL, last_mouse_x, last_mouse_y);

				//  calculate angle prop to length mouse movement
				double dX = vC.x - vL.x;
				double dY = vC.y - vL.y;
				double dZ = vC.z - vL.z;
				//float ang = 90.0 * sqrt(dX*dX + dY*dY + dZ*dZ);
				double ang = (-1)*sqrt(dX*dX + dY*dY + dZ*dZ);
				
				vC.cross(vL);
				dX = vC.x;
				dZ = vC.z;
				vC.z = vC.y;
				vC.x = dZ;
				vC.y = dX;

				vC.normalize();

				m_pMesh->rotate(vC, ang);
				wc->rotate(vC, ang);

				cMatrix3d temp = m_pMesh->getGlobalRot();
				objectRot.mul(temp);
			}
			break;

		case ZOOMING:
			{
				double current_mouse_xd, current_mouse_yd, last_mouse_xd, last_mouse_yd;
				
				screenToFrustum(current_mouse_xd, current_mouse_yd, current_mouse_x, current_mouse_y );
				screenToFrustum(last_mouse_xd, last_mouse_yd, last_mouse_x, last_mouse_y );

				delta_y = last_mouse_yd - current_mouse_yd;

				m_pcamera->translate(-delta_y, 0, 0 );
			}
			break;

		case IDLING:
			break;

		default:
			break;
		}
	}
		
	if (last_mouse_button == Qt::RightButton)
	{
		// defining a selection square with right button movement
		if (flag_show_selection || flag_show_mass)
		{
			if (last_mouse_button == Qt::RightButton)
			{
				bottom_right_x = current_mouse_x;
				bottom_right_y = current_mouse_y;
				//m_pMesh->bottom_right_x_Mesh = current_mouse_x;
				//m_pMesh->bottom_right_y_Mesh = current_mouse_y;
			}

			//if (flag_show_mass)
			//{

			//}
		}

		if (flag_simulating)
		{
			switch (m_pKernel->used_simulator)
			{
			case Kernel::SHAPE_MATCHING:
			case Kernel::SINGLE_GRID:
			case Kernel::HIERARCHY_SHAPE_MATCHING:
			case Kernel::NEW_HIERARCHY_SHAPE_MATCHING:
			case Kernel::ADAPTIVE_HIERARCHY_SHAPE_MATCHING:
			case Kernel::ADAPTIVE_SHAPE_MATCHING:
				if (m_pKernel->p_vox_mesh->active_node[0])
				{
					Vector3d force_arrow_begin = m_pKernel->p_vox_mesh->active_node[0]->coordinate + m_pKernel->p_vox_mesh->active_node[0]->displacement;

					double wx, wy, wz;

					gluProject(force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2], 
						m_pMesh->currentmodelview, m_pMesh->currentprojection, m_pMesh->currentviewport, &wx, &wy, &wz);

					wx = current_mouse_x;
					wy = m_displayH - current_mouse_y;

					gluUnProject(wx, wy, wz, m_pMesh->currentmodelview, m_pMesh->currentprojection, m_pMesh->currentviewport, 
						&force_arrow[0], &force_arrow[1], &force_arrow[2]);

					m_pKernel->p_vox_mesh->active_node[0]->force = 5.0*(force_arrow - force_arrow_begin);
					for(int j = 0; j < m_pKernel->p_vox_mesh->active_node[0]->duplicates.size(); ++j)
					{
						m_pKernel->p_vox_mesh->active_node[0]->duplicates[j]->force = m_pKernel->p_vox_mesh->active_node[0]->force;
					}
				}
			}
		}
	}
	

	last_mouse_x = current_mouse_x;
	last_mouse_y = current_mouse_y;
	if (!flag_simulating)
	{
		updateGL();
	}
}

void widgetGL::mouseReleaseEvent(QMouseEvent *e)
{

	current_mouse_button = e->button();
	
	if (current_mouse_button == Qt::RightButton)
	{
		if(flag_simulating)
		{
			if (m_pKernel->p_vox_mesh->active_node[0])
			{
				m_pKernel->p_vox_mesh->active_node[0]->force.setZero();
				for(int j = 0; j < m_pKernel->p_vox_mesh->active_node[0]->duplicates.size(); ++j)
				{
					m_pKernel->p_vox_mesh->active_node[0]->duplicates[j]->force.setZero();
				}
				m_pKernel->p_vox_mesh->active_node[0] = NULL;
				force_arrow.setZero();
			}
		}
	}
	
	last_mouse_button = current_mouse_button;
	updateGL();
}

void widgetGL::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{

	case Qt::Key_Control: 
		m_camStatus = PANNING;
		break;

	case Qt::Key_Shift:
		m_camStatus = ZOOMING;
		break;

	case Qt::Key_Alt:
		m_camStatus = ROTATING;
		break;

	case Qt::Key_Escape:
		//m_pMesh->setWireMode(false);
		m_pMesh->enableParticleCoupling(false);
		updateGL();
		break;

	case Qt::Key_Backspace:
		//m_pMesh->setWireMode(true);
		m_pMesh->enableParticleCoupling(true);
		updateGL();
		break;
	case Qt::Key_A:
		flag_output = !flag_output;
		break;

	default:
		m_camStatus = IDLING;
		break;

	}
	updateGL();
	return;
}

void widgetGL::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{

	case Qt::Key_Control: 
		m_camStatus = IDLING;
		break;

	case Qt::Key_Shift:
		m_camStatus = IDLING;
		break;

	case Qt::Key_Alt:
		m_camStatus = IDLING;
		break;

	case Qt::Key_C:
				
		break;

	default:

		//theCamera.status = Camera::IDLING;
		break;

	}
	updateGL();
	return;
}


void widgetGL::screenToFrustum(double &px, double &py, const int &ptx, const int &pty) const{
  int w = width();
  int h = height();
  float r = (float)h/w;

  float wFact = 1.0f, hFact = 1.0f;
  if (r > 1){ 
    wFact = 1;
    hFact = r;
    }
  else{
    wFact = 1/r;
    hFact = 1;
    }

  //  map to -wFact..wFact in x
  px = 2.0*wFact*(ptx - w/2)/(float)w;

  //  map to -hFact..hFact in y
  py = -2.0*hFact*(pty - h/2)/(float)h;
}

void widgetGL::screenToVector(cVector3d &v, const int &ptx, const int &pty) const{
  int w = width();
  int h = height();
  float r = (float)h/w;

  float wFact = 1.0f, hFact = 1.0f;
  if (r > 1){
    wFact = 1;
    hFact = r;
    }
  else{
    wFact = 1/r;
    hFact = 1;
    }

  //  map to -wFact..wFact in x
  v[0] = 2.0*wFact*(ptx - w/2)/(float)w;

  //  map to -hFact..hFact in y
  v[1] = -2.0*r*(pty - h/2)/(float)h;

  //  find z
  v[2] = 0;
  double d = v.length();
  v[2] = cos((M_PI/2.0) * (d < 1.0? d:1.0));

  //  normalise
  v.normalize();
}

void widgetGL::renderSelectSquare(int x1, int y1, int x2, int y2)
{
	/***
		Need to re specify both model view and projection matrix
	***/

	if (x1 <0 || x2 < 0 || y1 < 0 || y2 <0)
	return;

	glDisable(GL_LIGHTING);
	
	glColor4f(0.1, 0.4, 0.2, 0.1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_displayW, 0, m_displayH, 0, 1);

	glDisable(GL_DEPTH_TEST);

	y1 = m_displayH-y1;
	y2 = m_displayH-y2;

	glDisable(GL_BLEND);
	glColor3f(0.1, 0.9, 0.2);

	glBegin(GL_LINE_LOOP);
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
		
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
}

void widgetGL::setMass(double massValue)
{
	if (flag_show_mass && last_mouse_button == Qt::RightButton)
	{
		char msg[1024];
		int cx = 0;
		for (int i = 0; i < m_pKernel->level_list.size(); i++)
		{
			vector<Node>::iterator n_iter = m_pKernel->level_list[i]->voxmesh_level->node_list.begin();
			int count = 0;

			for (; n_iter!=m_pKernel->level_list[i]->voxmesh_level->node_list.end(); ++n_iter)
			{
				double wx, wy, wz;
				Vector3d p = n_iter->coordinate + n_iter->displacement;

				gluProject(p[0], p[1], p[2], m_pMesh->currentmodelview, m_pMesh->currentprojection, m_pMesh->currentviewport, &wx, &wy, &wz);

				if (isSelected(wx, wy))
				{
					n_iter->mass = massValue;
					vector<DuplicatedNode *>::iterator d_iter = n_iter->duplicates.begin();
					for(; d_iter != n_iter->duplicates.end(); ++d_iter)
					{
						(*d_iter)->mass = massValue;
					}
					count ++;
				}
			}

			vector<Cluster>::iterator c_iter = m_pKernel->level_list[i]->voxmesh_level->cluster_list.begin();
			for (; c_iter!=m_pKernel->level_list[i]->voxmesh_level->cluster_list.end(); ++c_iter)
			{
				c_iter->computeRestMassCentroid();
				c_iter->computeAQQ();
			}

			cx += sprintf(msg+cx, "Level %d:  %d nodes mass have been set\n", i, count);
			if(count > 0)
				m_pKernel->initializeSimulator();
		}
		bottom_right_x = -1.0;
		bottom_right_y = -1.0;
		upper_left_x   = -1.0;
		upper_left_y   = -1.0;

		QMessageBox::information(NULL, tr("success"), tr(msg));
	}
}