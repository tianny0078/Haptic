#include <QtGui>

#include "widgetNavi.h"

widgetNavi* Global_widgetNavipt;

void warperwidgetNavi()
{
	Global_widgetNavipt->updateHaptics();
}
widgetNavi::widgetNavi(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
	//initializing World, Camera, Light, Haptic handler, Haptic tool
	//and get the num of haptic device, if num is 0, then return;
	m_pworld = new cWorld();
	m_pcamera = new cCamera(m_pworld);
	m_pworld->addChild(m_pcamera);
	m_plight = new cLight(m_pworld);
	m_pcamera->addChild(m_plight);

	m_simulation = false;
	m_simulationfinished = false;

	m_displayW = 0;
	m_displayH = 0;

	m_enableReference = false;
	m_enableLinearVelocity = false;

	m_numOfHaptic = 0;

	m_cursor[0] = NULL;
	m_cursor[1] = NULL;
}
widgetNavi::~widgetNavi()
{
	close();
}

void widgetNavi::initializeGL()
{
	m_pworld->setBackgroundColor(0.0, 0.0, 0.0);
	m_pcamera->set(cVector3d (0.65, 0.0, 0.4),    // camera position (eye)
		cVector3d (0.0, 0.0, 0.0),    // lookat position (target)
		cVector3d (0.0, 0.0, 1.0));
	m_pcamera->setClippingPlanes(0.01, 10.0);
	m_plight->setEnabled(true);
	m_plight->setPos(cVector3d(2.0, 0.5, 1.0));
	m_plight->setDir(cVector3d(-2.0, 0.5, 1.0));
	
	for(int i = 0; i < m_numOfHaptic; i++)
	{
		m_phapticDevice[i]->open();
		m_phapticDevice[i]->initialize();
		info[i] = m_phapticDevice[i]->getSpecifications();
	}
	
	for(int i = 0; i < m_numOfHaptic; i++)
	{
		m_cursor[i] = new cShapeSphere(0.02);
		m_pworld->addChild(m_cursor[i]);

		m_velocity[i] = new cShapeLine(cVector3d(0, 0, 0), cVector3d(0, 0, 0));
		m_pworld->addChild(m_velocity[i]);
	}


	//begin
	m_simulation = true;
	m_hapticsThread = new cThread();
	Global_widgetNavipt = this;
	m_hapticsThread->set(warperwidgetNavi, CHAI_THREAD_PRIORITY_HAPTICS);

}
void widgetNavi::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	m_displayW = w;
	m_displayH = h;
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	// update the size of the viewport

}
void widgetNavi::paintGL()
{
	if (m_enableReference)
	{
		for(int i = 0; i < m_numOfHaptic; i++)
		{
			if (info[i].m_sensedRotation == true)
			{
				m_cursor[i]->setShowFrame(true);
				m_cursor[i]->setFrameSize(0.07, 0.07);
			}
		}
	}
	else
	{
		for(int i = 0; i < m_numOfHaptic; i++)
			m_cursor[i]->setShowFrame(false);
	}

	m_pcamera->renderView(m_displayW, m_displayH);
	glutWireCube(0.4);
}
void widgetNavi::updateHaptics(void)
{
	while (m_simulation)
	{
		for(int i = 0; i < m_numOfHaptic; i++)
		{
			cVector3d newPos;
			m_phapticDevice[i]->getPosition(newPos);

			cMatrix3d newRot;
			m_phapticDevice[i]->getRotation(newRot);
		
			m_cursor[i]->setPos(newPos);
			m_cursor[i]->setRot(newRot);

			cVector3d linearVelocity;
			m_phapticDevice[i]->getLinearVelocity(linearVelocity);

			m_velocity[i]->m_pointA = newPos;

			if (m_enableLinearVelocity)
			{
				m_velocity[i]->m_pointB = cAdd(newPos, linearVelocity);
			}
			else
			{
				m_velocity[i]->m_pointB = newPos;
			}
		}
	}
	m_simulationfinished = true;
}
void widgetNavi::timeGL()
{
	updateGL();
}

void widgetNavi::setHapticDevice(cGenericHapticDevice * pDevice[], int numOfHaptic)
{
	m_phapticDevice = pDevice;
	m_numOfHaptic = numOfHaptic;
}
void widgetNavi::onReferenceON(bool value)
{
	m_enableReference = value;
	updateGL();
}
void widgetNavi::onLinearVelocity(bool value)
{
	m_enableLinearVelocity = value;
	updateGL();
}
void widgetNavi::close()
{
	m_simulation = false;

	while(!m_simulationfinished)
		cSleepMs(100);

	for(int i = 0; i < m_numOfHaptic; i++)
		m_phapticDevice[i]->close();
}