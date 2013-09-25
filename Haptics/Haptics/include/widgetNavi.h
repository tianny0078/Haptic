#ifndef WIDGETNAVI_H
#define WIDGETNAVI_H

#include <QtGui>
#include <QGLWidget>

#include "chai3d.h"


class widgetNavi : public QGLWidget
{
	Q_OBJECT
public:
	widgetNavi(QWidget *parent = 0);
	~widgetNavi();

	void close();
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private slots:
	void timeGL();
	void onReferenceON(bool value);
	void onLinearVelocity(bool value);
public:
	void updateHaptics(void);
	void setHapticDevice(cGenericHapticDevice * pDevice[], int numOfHaptic);
protected:

private:
	cWorld * m_pworld;
	cCamera * m_pcamera;
	cLight * m_plight;

	cGenericHapticDevice ** m_phapticDevice;
	int m_numOfHaptic;//num of haptics

	cShapeSphere * m_cursor[2];
	cShapeLine * m_velocity[2];
	
	bool m_simulation;
	bool m_simulationfinished;

	cShapeTorus* m_object;

	cThread* m_hapticsThread;

	cVector3d m_rotVel;

	int m_displayW;
	int m_displayH;

	bool m_enableReference;
	bool m_enableLinearVelocity;

	cHapticDeviceInfo info[2];
};

#endif