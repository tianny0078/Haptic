#ifndef HAPTICS_H
#define HAPTICS_H

#include <QtGui/QMainWindow>
#include "ui_haptics.h"

#include "kernel.h"
#include "cSurfaceMesh.h"

class Haptics : public QMainWindow
{
	Q_OBJECT

public:
	Haptics(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Haptics();


private:
	Ui::HapticsClass ui;

	
	//haptie related
	cHapticDeviceHandler * m_phandler;//handler
	cGenericHapticDevice * m_phapticDevice[2];//device pointer
	int m_numOfHaptic;//num of haptics

	//timer for simulation
	QTimer simulation_timer;

	//Mesh
	Kernel * m_pKernel;

	//if kernel is running
	bool flag_kernelRunning;
	//if caputre screen
	bool flag_captureScreen;
	bool flag_captureSubScreen;
	//if export obj per frame
	bool flag_exportObj;

public slots:
	//timer callback
	void simulate();
	void initializeSimulation();

	void loadMesh();
	void scaleMesh();
	void showGrid();
	void showMesh(bool);
	void showVox(bool);
	void showSelection(bool);
	void setGridDensity();
	void startSimulation();
	void pauseSimulation();

	void resetCamera();


	void chooseSimulator(int);

	void addLevel();
	void clearLevel();

	void chooseLevelDisplay(int);
	void setMultigrid(bool);
	void setDynamics(bool);
	void setGravity(bool);
	void setGravityMagnitude();
	void setForceScalar();
	void setEnableMass(bool);
	void setShowLastLevel(bool);
	void setEnergyThreshold();
	void setMass();
	void resetMass();
	void setShowParticleCoupling(bool);
	void setCaptureScreen(bool);
	void setCaptureSubScreen(bool);

	void setDampParameter();
public:

};

#endif // HAPTICS_H
