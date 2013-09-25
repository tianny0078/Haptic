#include "haptics.h"
using namespace std;

Haptics::Haptics(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	
	//load haptic
	m_phandler = new cHapticDeviceHandler();
	m_numOfHaptic = m_phandler->getNumDevices();
	//haptic device pointer initialized as NULL.
	m_phapticDevice[0] = NULL;
	m_phapticDevice[1] = NULL;
	if(m_numOfHaptic != 0)
	{
		for(int i = 0; i < m_numOfHaptic; i++)
			m_phandler->getDevice(m_phapticDevice[i], i);
	}
	
	ui.widget->setHapticDevice(m_phapticDevice, m_numOfHaptic);
	ui.widget_haptic->setHapticDevice(m_phapticDevice, m_numOfHaptic);

	//
	//if(!simulation_timer.isActive())
	//{
	//	connect(&simulation_timer, SIGNAL(timeout()), this, SLOT(simulate()));
	//	simulation_timer.start(0);
	//}
	
	//kernel
	m_pKernel = new Kernel;
	ui.widget->m_pKernel = m_pKernel;
	flag_kernelRunning = false;
	//ui.widget->m_pKernel = m_pKernel;


	flag_captureScreen = false;
	flag_captureSubScreen = false;
	flag_exportObj = false;

	//disable parts of UI
	ui.comboBox_level->setDisabled(true);
	ui.pushButton_addLevel->setDisabled(true);
	ui.radioButton_point->setDisabled(true);
	ui.radioButton_orientation->setDisabled(true);
	//ui.actionSetConstraint->setDisabled(true);

	//ui.radioButton_Server->setDisabled(true);
	//ui.radioButton_Client->setDisabled(true);
	//ui.pushButton_setNumOfClients->setDisabled(true);
	//ui.pushButton_configNetwork->setDisabled(true);
}
Haptics::~Haptics()
{
	ui.widget_haptic->close();
	ui.widget->close();
}

void Haptics::simulate()
{
	ui.widget_haptic->updateGL();
	
	double start = GetTickCount();
	/*
	if (!m_pKernel->simulateNextStep())
	{
		pauseSimulation();
	}

	if(m_pKernel->pReceiver != NULL)
	{
		if(m_pKernel->pReceiver->isMeshReady)
			m_pKernel->flag_mesh_ready = true;
		else
			m_pKernel->flag_mesh_ready = false;
	}
	ui.widget->updateGL();

	*/


	ui.widget->simulateNextStep();
	ui.widget->updateGL();
	if (flag_captureScreen)
	{
		//QPixmap Screenshot = ui.RenderWidget->renderPixmap();
		QPixmap Screenshot = QPixmap::grabWindow(QApplication::desktop()->winId(), 
			this->pos().x(), this->pos().y(), width()+15, height()+35);
		char filenames[32];
		sprintf(filenames, "..\\ScreenShots\\SIMG%0004d.bmp", m_pKernel->time_step_index);
		Screenshot.save(filenames, "bmp", -1);
	}

	if (flag_captureSubScreen)
	{
		//ui.renderWidget->fini();
		QImage Screenshot = ui.widget->grabFrameBuffer(false);
		char filenames[32];
		sprintf(filenames, "..\\ScreenShots\\IMG%0004d.png", m_pKernel->time_step_index);
		Screenshot.save(filenames, "png", -1);
	}

	if (flag_exportObj)
	{
		char filenames[32];
		sprintf(filenames, "..\\OBJs\\OBJ%0004d.obj", m_pKernel->time_step_index);
		m_pKernel->exportToOBJ(filenames);
	}	

	double end = GetTickCount() - start;
	if(end != 0)
		m_pKernel->fps = 1000/end;
	//cout << m_pKernel->fps << endl;
	//cout << end << endl;
}

void Haptics::loadMesh()
{
	QString path = QFileDialog::getOpenFileName(this, tr("load mesh"), ".", tr("obj file(*.obj)"));
	if(path.length())
	{
		m_pKernel->p_mesh->read(path.toLatin1());
		m_pKernel->flag_mesh_ready = true;
		ui.widget->flag_show_mesh = true;
		ui.actionShowMesh->setChecked(true);

		//set cMesh as the data
		if(!m_pKernel->p_mesh)
			cout << "Error! Mesh is not loaded correctly!" << endl;
		else
		{
			ui.widget->getMesh()->clearAllChildren();
			ui.widget->getMesh()->clear();

			ui.widget->getMesh()->loadFromFile(path.toUtf8().constData());
			ui.widget->getMesh()->setMesh(m_pKernel->p_mesh);
			//ui.widget->getMesh()->setWireMode(false);
			ui.widget->getMesh()->setUseTransparency(true);

			ui.widget->getMesh()->setStiffness(0.8*ui.widget->stiffnessMax, true);
			ui.widget->getMesh()->setFriction(0.1, 0.2, true);


			ui.widget->getMesh()->computeBoundaryBox(true);
			ui.widget->getMesh()->createSphereTreeCollisionDetector(1.0 * 1, true, true);
			
			//ui.widget->getMesh()->setShowCollisionTree(true, true);
			//ui.widget->getMesh()->setCollisionDetectorProperties(3, cColorf(0.0, 1.0, 0.0), true);
			
			ui.widget->getMesh()->setWireMode(false);
			ui.widget->getMesh()->setTransparencyRenderMode(false);
			ui.widget->getMesh()->setTransparencyLevel(0.5);
			ui.widget->m_ptool[0]->m_proxyPointForceModel->m_useDynamicProxy = true;
			//ui.widget->getMesh()->scale(0.1);
			ui.widget->updateGL();
		}
		

		if(m_pKernel->p_mesh->flag_normalized)
		{
			m_pKernel->mark_preprocess4Voxel(m_pKernel->p_mesh, m_pKernel->p_voxel, m_pKernel->grid_density);
			ui.actionScaleMesh->setChecked(true);
		}
		else
		{
			//scale the mesh
			scaleMesh();
		}
		//set greiddensity
		setGridDensity();
		
		if(!ui.widget->getMesh()->setVoxMesh(m_pKernel->p_vox_mesh))
			cout << "Error! VoxMesh is not generated correctly!" << endl;
		else
		{
			//ui
			ui.actionShowVox->setChecked(true);
			ui.widget->flag_show_vox = true;
		}

		ui.widget->getMesh()->setKernel(m_pKernel);

		char msg[128];
		sprintf(msg, "read %d nodes and %d faces", m_pKernel->p_mesh->number_node, m_pKernel->p_mesh->number_face);
		cout << msg << endl;
		QMessageBox::information(NULL, "Done", msg);
	}
}

void Haptics::scaleMesh()
{
	if (!m_pKernel->flag_mesh_ready)
	{
		QMessageBox::warning(NULL, "warning", "load a mesh first");
		return;
	}

	if (!m_pKernel->p_mesh->flag_normalized)
	{
		m_pKernel->p_mesh->scale();
		m_pKernel->mark_preprocess4Voxel(m_pKernel->p_mesh, m_pKernel->p_voxel, m_pKernel->grid_density);
	}
	ui.actionScaleMesh->setChecked(true);
	ui.widget->updateGL();
}

void Haptics::showGrid()
{
	ui.widget->flag_show_grid = ui.actionShowGrid->isChecked();
	ui.actionShowGrid->setChecked(ui.widget->flag_show_grid);
	ui.widget->updateGL();
}

void Haptics::showMesh(bool value)
{
	ui.widget->flag_show_mesh = value;
	ui.widget->updateGL();
}

void Haptics::showVox(bool value)
{
	ui.widget->flag_show_vox = value;
	ui.widget->updateGL();
}

void Haptics::showSelection(bool value)
{
	if(!m_pKernel->flag_vox_ready)
	{
		QMessageBox::warning(NULL, "warning", "generate voxel mesh first");
		ui.actionShowSelection->setChecked(false);
		return;
	}

	ui.widget->flag_show_selection = value;
	ui.widget->updateGL();
}

void Haptics::setGridDensity()
{
	if (!m_pKernel->flag_mesh_ready)
	{
		QMessageBox::warning(NULL, "warning", "load a mesh first");
		return;
	}

	else if (!m_pKernel->p_mesh->flag_normalized)
	{
		QMessageBox::warning(NULL, "warning", "scale the mesh first");
		return;
	}

	int d = ui.spinBox_gridDensity->value();
	
	int level = 0;
	if(ui.comboBox_level->isEnabled())
		level = ui.comboBox_level->currentIndex();

	if(level > 0)
	{
		int parent_index = level - 1;
		if(m_pKernel->level_list[parent_index]->voxmesh_level == NULL)
		{
			QString message = "Error! The grid density of Level " + QString::number(parent_index) + " has not been set yet!" ;
			QMessageBox::warning(NULL, "warning", message);
			return;
		}
	}
	//if this level has a child level, then regenerate volmesh
	m_pKernel->generateVoxMesh4Level(level++, d);
	while(level < m_pKernel->level_list.size())
	{
		if(m_pKernel->level_list[level]->voxmesh_level == NULL)
			break;
		d = m_pKernel->level_list[level]->gridDensity;
		m_pKernel->generateVoxMesh4Level(level, d);
		level ++;
	}
	ui.widget->updateGL();

}

void Haptics::startSimulation()
{
	if (!m_pKernel->flag_simulator_ready)
	{
		QMessageBox::warning(NULL, "warning", "initialize simulator first");
		return;
	}

	if (!simulation_timer.isActive())
	{
		//test to disable
		ui.widget->beginHaptic();
		connect(&simulation_timer, SIGNAL(timeout()), this, SLOT(simulate()));
		ui.actionShowSelection->setChecked(false);
		ui.widget->flag_show_selection = false;
		ui.widget->flag_simulating = true;
		simulation_timer.start(0);

		//ui.actionSetConstraint->setEnabled(true);
		ui.radioButton_point->setEnabled(true);
		ui.radioButton_point->setChecked(true);
		ui.radioButton_orientation->setEnabled(true);
	}
}

void Haptics::pauseSimulation()
{
	//flag_kernelRunning = ! flag_kernelRunning;
	if (!simulation_timer.isActive())
	{
		simulation_timer.start(0);
	}
	else
	{
		simulation_timer.stop();
	}
}

void Haptics::initializeSimulation()
{
	if (m_pKernel->used_simulator == Kernel::UNDEFINED)
	{
		QMessageBox::warning(NULL, "warning", "choose simulator first");
		return;
	}

	m_pKernel->initializeSimulator();
	QMessageBox::information(NULL, "success", "simulator initialized");
}

void Haptics::resetCamera()
{
	ui.widget->init();
	ui.widget->updateGL();
}

void Haptics::chooseSimulator(int t)
{
	//disable some parts of UI.
	ui.comboBox_level->setDisabled(true);
	ui.pushButton_addLevel->setDisabled(true);
	switch (t)
	{
	case 0:
		m_pKernel->used_simulator = Kernel::UNDEFINED;
		break;

	case 1:
		m_pKernel->used_simulator = Kernel::SHAPE_MATCHING;
		break;
	case 2:
		//m_pKernel->used_simulator = Kernel::HIERARCHY;
		m_pKernel->used_simulator = Kernel::HIERARCHY_SHAPE_MATCHING;
		// keep Level 0, dicard other Levels
		m_pKernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->setDisabled(true);
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 3:
		//m_pKernel->used_simulator = Kernel::HIERARCHY;
		m_pKernel->used_simulator = Kernel::NEW_HIERARCHY_SHAPE_MATCHING;
		// keep Level 0, dicard other Levels
		m_pKernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->setDisabled(true);
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 4:
		//m_pKernel->used_simulator = Kernel::HIERARCHY;
		m_pKernel->used_simulator = Kernel::ADAPTIVE_HIERARCHY_SHAPE_MATCHING;
		// keep Level 0, dicard other Levels
		m_pKernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->setDisabled(true);
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 5:
		//m_pKernel->used_simulator = Kernel::HIERARCHY;
		m_pKernel->used_simulator = Kernel::ADAPTIVE_SHAPE_MATCHING;
		// keep Level 0, dicard other Levels
		m_pKernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->setDisabled(true);
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 6:
		m_pKernel->used_simulator = Kernel::VELOCITY_MATCHING;
		break;
	case 7:
		m_pKernel->used_simulator = Kernel::SINGLE_GRID;
		break;
	case 8:
		m_pKernel->used_simulator = Kernel::MULTIPLE_VELOCITY_MATCHING;
		// keep Level 0, dicard other Levels
		m_pKernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 9:
		m_pKernel->used_simulator = Kernel::SIMULATION_NETWORKING;
		ui.widget->flag_show_mesh = true;
		ui.actionShowMesh->setChecked(true);
		ui.actionScaleMesh->setChecked(true);
		//ui.comboBox_selectTreeDepth->setEnabled(true);
		ui.widget->updateGL();
		break;
	case 10:
		m_pKernel->used_simulator = Kernel::SIMULATION_MOBILE;
		ui.widget->flag_show_mesh = true;
		ui.actionShowMesh->setChecked(true);
		ui.actionScaleMesh->setChecked(true);
		//ui.comboBox_selectTreeDepth->setEnabled(true);
		ui.widget->updateGL();
		m_pKernel->myMobile.open("output_2.txt");
		if(m_pKernel->myMobile.fail())
			return;
	default:
		break;
	}

}

void Haptics::addLevel()
{
	m_pKernel->addLevel();
	//ui
	int index = ui.comboBox_level->count();
	QString	item = "Level " + QString::number(index);
	ui.comboBox_level->addItem(item);
	ui.comboBox_level->setCurrentIndex(index);

}

void Haptics::clearLevel()
{
	m_pKernel->clearAllLevel();
	//ui
	ui.comboBox_level->setCurrentIndex(0);
	while(ui.comboBox_level->count() > 1)
		ui.comboBox_level->removeItem(1);
}

void Haptics::chooseLevelDisplay(int value)
{
	m_pKernel->level_display = value;
	if (m_pKernel->level_display != -1)
		ui.widget->updateGL();
}

void Haptics::setMultigrid(bool a)
{
	m_pKernel->flag_multigrid = a;
}

void Haptics::setDynamics(bool a)
{
	m_pKernel->flag_dynamics = a;
}

void Haptics::setGravity(bool a)
{
	m_pKernel->flag_gravity = a;

}

void Haptics::setGravityMagnitude()
{
	m_pKernel->gravity_magnitude = (-1) * ui.lineEdit_gravityMagnitude->text().toDouble();
}

void Haptics::setForceScalar()
{
	m_pKernel->force_scalar = ui.lineEdit_forceScalar->text().toDouble();
}

void Haptics::setEnableMass(bool value)
{
	if (!m_pKernel->flag_vox_ready)
	{
		QMessageBox::warning(NULL, "warning", "generate voxel mesh first");
		ui.actionSetMass->setChecked(false);
		return;
	}

	ui.widget->flag_show_mass = value;
	if(value && ui.widget->flag_show_selection)
	{
		ui.widget->flag_show_selection = false;
		ui.actionShowSelection->setChecked(false);
	}
	ui.widget->updateGL();
}

void Haptics::setShowLastLevel(bool a)
{
	ui.widget->flag_show_last_level = a;
	ui.widget->updateGL();
}

void Haptics::setEnergyThreshold()
{
	m_pKernel->threshold_energy = ui.lineEdit_thresholdEnergy->text().toDouble();
}

void Haptics::setMass()
{
	ui.widget->setMass(ui.lineEdit_mass->text().toDouble());
}

void Haptics::resetMass()
{
	m_pKernel->resetMass4Level();
}

void Haptics::setShowParticleCoupling(bool value)
{
	//ui.widget->getMesh()->setWireMode(value);
	ui.widget->getMesh()->enableParticleCoupling(value);
	ui.widget->updateGL();
}

void Haptics::setCaptureScreen(bool a)
{
	flag_captureScreen = a;
}

void Haptics::setCaptureSubScreen(bool a)
{
	flag_captureSubScreen = a;
}