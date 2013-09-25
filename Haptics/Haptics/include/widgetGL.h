#ifndef WIDGETGL_H
#define WIDGETGL_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include "chai3d.h"
#include "cSurfaceMesh.h"
#include "kernel.h"
#include "CShapeMatching3dofPointer.h"

#include <QMouseEvent>
#include <QMetaEnum> // in order to get the value of key pressed

#include <fstream>

enum Camera_Status{IDLING, PANNING, ZOOMING, ROTATING };

class widgetGL : public QGLWidget
{
	Q_OBJECT
public:
	widgetGL(QWidget *parent = 0);
	~widgetGL();

	void close();
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();


public:
	void updateHaptics(void);
	void setHapticDevice(cGenericHapticDevice * pDevice[], int numOfHaptic);
	cSurfaceMesh * getMesh() const { return m_pMesh;}
//	cSurfaceMesh * getSurfaceMesh()const{return m_object;}

	void mousei2d(int x, int y, double *xf, double *yf);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	bool isSelected(double x, double y);
	bool isPicked(double x, double y);

	void screenToFrustum(double &px, double &py, const int &ptx, const int &pty) const;
	void screenToVector(cVector3d &v, const int &ptx, const int &pty) const;

	void init();

	void renderSelectSquare(int x1, int y1, int x2, int y2);
	void beginHaptic();

	void simulateNextStep();

	void setMass(double massValue);

public:
	bool flag_show_grid;
	bool flag_show_mesh;
	bool flag_show_vox;
	bool flag_show_selection;
	//bool flag_initialized;
	bool flag_simulating;
	bool flag_istouched;
	bool flag_show_mass;
	bool flag_show_last_level;

	int current_mouse_button;
	int	last_mouse_button;						// 0 --- left, 1 --- right

	// for selection square
	double upper_left_x, upper_left_y;
	double bottom_right_x, bottom_right_y;

	// Input device info
	int	current_mouse_x, current_mouse_y;		// current mouse windows coordinate
	int	last_mouse_x, last_mouse_y;				// last most windows coordinate

	//// matrix info. (currently only when the rendering is under selection mode will get the matrix)
	//// getting matrix could be slow
	//GLint    currentviewport[4];
	//GLdouble currentmodelview[16];
	//GLdouble currentprojection[16];

	Camera_Status m_camStatus;

	Kernel * m_pKernel;

	
public:
	//haptic world related
	cWorld * m_pworld;
	cCamera * m_pcamera;
	cLight * m_plight;

	//object
	cSurfaceMesh * m_pMesh;
	cGenericObject * rootLabel;
	cLabel * labels[3];
	cGenericObject * wc;
	cGenericObject* graspObject;
	cShapeLine* graspLine[2];

	bool graspActive[2];
	cVector3d globalGraspPos[2];
	cVector3d graspPos[2];

	//haptic handler
	cGenericHapticDevice ** m_phapticDevice;
	int m_numOfHaptic;//num of haptics
	cShapeMatching3dofPointer * m_ptool[2];

	cPrecisionClock m_simClock;

	bool m_simulation;
	bool m_simulationfinished;
	

	double m_proxyRadius;
	cMesh * m_proxyTool;

	cThread* m_hapticsThread;

	cVector3d m_rotVel;

	int m_displayW;
	int m_displayH;

	//openGL related
	float m_bgColorR;
	float m_bgColorG;
	float m_bgColorB;

	//cSurfaceMesh *m_object;

	//rotation matrix
	cMatrix3d objectRot;

	//force related
	Vector3d force_arrow;

	//
public:
	double workspaceScaleFactor;
	double stiffnessMax;
	double deviceForceScale;

	ofstream testoutput;
	bool flag_output;

private slots:
	void timeGL();
//	void slotUpdateBgR(QString value);
//	void slotUpdateBgG(QString value);
//	void slotUpdateBgB(QString value);
};

#endif