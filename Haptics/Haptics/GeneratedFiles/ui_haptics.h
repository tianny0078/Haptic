/********************************************************************************
** Form generated from reading UI file 'haptics.ui'
**
** Created: Fri Nov 22 22:14:52 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HAPTICS_H
#define UI_HAPTICS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "widgetgl.h"
#include "widgetnavi.h"

QT_BEGIN_NAMESPACE

class Ui_HapticsClass
{
public:
    QAction *actionLoadMech;
    QAction *actionScaleMesh;
    QAction *actionShowGrid;
    QAction *actionShowMesh;
    QAction *actionShowVox;
    QAction *actionShowSelection;
    QAction *actionReset;
    QAction *actionSetMass;
    QWidget *centralWidget;
    widgetGL *widget;
    QGroupBox *groupBox;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    widgetNavi *widget_haptic;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox_gridDensity;
    QPushButton *pushButton_ok;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QComboBox *comboBox_level;
    QPushButton *pushButton_addLevel;
    QPushButton *pushButton_deleteLevel;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBox_multigrid;
    QCheckBox *checkBox_dynamics;
    QWidget *horizontalLayoutWidget_10;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *checkBox_gravity;
    QLabel *label_18;
    QLineEdit *lineEdit_gravityMagnitude;
    QPushButton *pushButton_setGravityMagnitude;
    QWidget *horizontalLayoutWidget_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_17;
    QLineEdit *lineEdit_forceScalar;
    QPushButton *pushButton_setForceScalar;
    QGroupBox *groupBox_4;
    QWidget *horizontalLayoutWidget_23;
    QHBoxLayout *horizontalLayout_23;
    QLabel *label_21;
    QLineEdit *lineEdit_thresholdEnergy;
    QPushButton *pushButton_setThresholdEnergy;
    QWidget *horizontalLayoutWidget_24;
    QHBoxLayout *horizontalLayout_24;
    QCheckBox *checkBox_setShowLastLevel;
    QCheckBox *checkBox_showParticle;
    QWidget *horizontalLayoutWidget_20;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_20;
    QLineEdit *lineEdit_mass;
    QPushButton *pushButton_setMass;
    QPushButton *pushButton_resetMass;
    QWidget *tab_2;
    QGroupBox *groupBox_2;
    QRadioButton *radioButton_point;
    QRadioButton *radioButton_orientation;
    QWidget *horizontalLayoutWidget_11;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *checkBox_exportObj;
    QCheckBox *checkBox_captureScreen;
    QCheckBox *checkBox_captureSubScreen;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *lineEdit_damping;
    QPushButton *pushButton_setDamping;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_initSimulator;
    QPushButton *pushButton_startSimulation;
    QPushButton *pushButton_nextStep;
    QPushButton *pushButton__pauseSimulation;
    QPushButton *pushButton_resetSimulation;
    QComboBox *comboBox_chooseSimulator;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *HapticsClass)
    {
        if (HapticsClass->objectName().isEmpty())
            HapticsClass->setObjectName(QString::fromUtf8("HapticsClass"));
        HapticsClass->resize(1400, 850);
        HapticsClass->setMinimumSize(QSize(1400, 850));
        HapticsClass->setMaximumSize(QSize(1400, 850));
        actionLoadMech = new QAction(HapticsClass);
        actionLoadMech->setObjectName(QString::fromUtf8("actionLoadMech"));
        actionLoadMech->setCheckable(false);
        actionLoadMech->setChecked(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Haptics/Resources/folder_drag_accept.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadMech->setIcon(icon);
        actionScaleMesh = new QAction(HapticsClass);
        actionScaleMesh->setObjectName(QString::fromUtf8("actionScaleMesh"));
        actionScaleMesh->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Haptics/Resources/scale_ruler.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScaleMesh->setIcon(icon1);
        actionShowGrid = new QAction(HapticsClass);
        actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
        actionShowGrid->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Haptics/Resources/Matrix.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowGrid->setIcon(icon2);
        actionShowMesh = new QAction(HapticsClass);
        actionShowMesh->setObjectName(QString::fromUtf8("actionShowMesh"));
        actionShowMesh->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Haptics/Resources/stock_3d_material.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowMesh->setIcon(icon3);
        actionShowVox = new QAction(HapticsClass);
        actionShowVox->setObjectName(QString::fromUtf8("actionShowVox"));
        actionShowVox->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Haptics/Resources/CMS.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowVox->setIcon(icon4);
        actionShowSelection = new QAction(HapticsClass);
        actionShowSelection->setObjectName(QString::fromUtf8("actionShowSelection"));
        actionShowSelection->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Haptics/Resources/Selection.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowSelection->setIcon(icon5);
        actionReset = new QAction(HapticsClass);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Haptics/Resources/undocamera.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon6);
        actionSetMass = new QAction(HapticsClass);
        actionSetMass->setObjectName(QString::fromUtf8("actionSetMass"));
        actionSetMass->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Haptics/Resources/TwoCubes.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetMass->setIcon(icon7);
        centralWidget = new QWidget(HapticsClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widget = new widgetGL(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 1024, 768));
        widget->setMinimumSize(QSize(1024, 768));
        widget->setMaximumSize(QSize(1024, 768));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(1040, 470, 351, 311));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(40, 20, 111, 17));
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(190, 20, 91, 17));
        widget_haptic = new widgetNavi(groupBox);
        widget_haptic->setObjectName(QString::fromUtf8("widget_haptic"));
        widget_haptic->setGeometry(QRect(10, 40, 331, 251));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(1040, 40, 351, 351));
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setTabPosition(QTabWidget::East);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayoutWidget = new QWidget(tab);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 40, 311, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spinBox_gridDensity = new QSpinBox(horizontalLayoutWidget);
        spinBox_gridDensity->setObjectName(QString::fromUtf8("spinBox_gridDensity"));
        spinBox_gridDensity->setValue(3);

        horizontalLayout->addWidget(spinBox_gridDensity);

        pushButton_ok = new QPushButton(horizontalLayoutWidget);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

        horizontalLayout->addWidget(pushButton_ok);

        horizontalLayoutWidget_4 = new QWidget(tab);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 0, 311, 31));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        comboBox_level = new QComboBox(horizontalLayoutWidget_4);
        comboBox_level->setObjectName(QString::fromUtf8("comboBox_level"));

        horizontalLayout_4->addWidget(comboBox_level);

        pushButton_addLevel = new QPushButton(horizontalLayoutWidget_4);
        pushButton_addLevel->setObjectName(QString::fromUtf8("pushButton_addLevel"));

        horizontalLayout_4->addWidget(pushButton_addLevel);

        pushButton_deleteLevel = new QPushButton(horizontalLayoutWidget_4);
        pushButton_deleteLevel->setObjectName(QString::fromUtf8("pushButton_deleteLevel"));

        horizontalLayout_4->addWidget(pushButton_deleteLevel);

        horizontalLayoutWidget_7 = new QWidget(tab);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(10, 80, 291, 31));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        checkBox_multigrid = new QCheckBox(horizontalLayoutWidget_7);
        checkBox_multigrid->setObjectName(QString::fromUtf8("checkBox_multigrid"));
        checkBox_multigrid->setChecked(true);

        horizontalLayout_7->addWidget(checkBox_multigrid);

        checkBox_dynamics = new QCheckBox(horizontalLayoutWidget_7);
        checkBox_dynamics->setObjectName(QString::fromUtf8("checkBox_dynamics"));
        checkBox_dynamics->setChecked(true);

        horizontalLayout_7->addWidget(checkBox_dynamics);

        horizontalLayoutWidget_10 = new QWidget(tab);
        horizontalLayoutWidget_10->setObjectName(QString::fromUtf8("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(10, 120, 291, 31));
        horizontalLayout_10 = new QHBoxLayout(horizontalLayoutWidget_10);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        checkBox_gravity = new QCheckBox(horizontalLayoutWidget_10);
        checkBox_gravity->setObjectName(QString::fromUtf8("checkBox_gravity"));
        checkBox_gravity->setChecked(false);

        horizontalLayout_10->addWidget(checkBox_gravity);

        label_18 = new QLabel(horizontalLayoutWidget_10);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_10->addWidget(label_18);

        lineEdit_gravityMagnitude = new QLineEdit(horizontalLayoutWidget_10);
        lineEdit_gravityMagnitude->setObjectName(QString::fromUtf8("lineEdit_gravityMagnitude"));

        horizontalLayout_10->addWidget(lineEdit_gravityMagnitude);

        pushButton_setGravityMagnitude = new QPushButton(horizontalLayoutWidget_10);
        pushButton_setGravityMagnitude->setObjectName(QString::fromUtf8("pushButton_setGravityMagnitude"));

        horizontalLayout_10->addWidget(pushButton_setGravityMagnitude);

        horizontalLayoutWidget_9 = new QWidget(tab);
        horizontalLayoutWidget_9->setObjectName(QString::fromUtf8("horizontalLayoutWidget_9"));
        horizontalLayoutWidget_9->setGeometry(QRect(10, 160, 291, 31));
        horizontalLayout_9 = new QHBoxLayout(horizontalLayoutWidget_9);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_17 = new QLabel(horizontalLayoutWidget_9);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_9->addWidget(label_17);

        lineEdit_forceScalar = new QLineEdit(horizontalLayoutWidget_9);
        lineEdit_forceScalar->setObjectName(QString::fromUtf8("lineEdit_forceScalar"));

        horizontalLayout_9->addWidget(lineEdit_forceScalar);

        pushButton_setForceScalar = new QPushButton(horizontalLayoutWidget_9);
        pushButton_setForceScalar->setObjectName(QString::fromUtf8("pushButton_setForceScalar"));

        horizontalLayout_9->addWidget(pushButton_setForceScalar);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 200, 291, 111));
        horizontalLayoutWidget_23 = new QWidget(groupBox_4);
        horizontalLayoutWidget_23->setObjectName(QString::fromUtf8("horizontalLayoutWidget_23"));
        horizontalLayoutWidget_23->setGeometry(QRect(0, 30, 281, 31));
        horizontalLayout_23 = new QHBoxLayout(horizontalLayoutWidget_23);
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        horizontalLayout_23->setContentsMargins(0, 0, 0, 0);
        label_21 = new QLabel(horizontalLayoutWidget_23);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        horizontalLayout_23->addWidget(label_21);

        lineEdit_thresholdEnergy = new QLineEdit(horizontalLayoutWidget_23);
        lineEdit_thresholdEnergy->setObjectName(QString::fromUtf8("lineEdit_thresholdEnergy"));

        horizontalLayout_23->addWidget(lineEdit_thresholdEnergy);

        pushButton_setThresholdEnergy = new QPushButton(horizontalLayoutWidget_23);
        pushButton_setThresholdEnergy->setObjectName(QString::fromUtf8("pushButton_setThresholdEnergy"));

        horizontalLayout_23->addWidget(pushButton_setThresholdEnergy);

        horizontalLayoutWidget_24 = new QWidget(groupBox_4);
        horizontalLayoutWidget_24->setObjectName(QString::fromUtf8("horizontalLayoutWidget_24"));
        horizontalLayoutWidget_24->setGeometry(QRect(0, 70, 281, 31));
        horizontalLayout_24 = new QHBoxLayout(horizontalLayoutWidget_24);
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        horizontalLayout_24->setContentsMargins(0, 0, 0, 0);
        checkBox_setShowLastLevel = new QCheckBox(horizontalLayoutWidget_24);
        checkBox_setShowLastLevel->setObjectName(QString::fromUtf8("checkBox_setShowLastLevel"));

        horizontalLayout_24->addWidget(checkBox_setShowLastLevel);

        checkBox_showParticle = new QCheckBox(horizontalLayoutWidget_24);
        checkBox_showParticle->setObjectName(QString::fromUtf8("checkBox_showParticle"));

        horizontalLayout_24->addWidget(checkBox_showParticle);

        horizontalLayoutWidget_20 = new QWidget(tab);
        horizontalLayoutWidget_20->setObjectName(QString::fromUtf8("horizontalLayoutWidget_20"));
        horizontalLayoutWidget_20->setGeometry(QRect(10, 310, 301, 31));
        horizontalLayout_20 = new QHBoxLayout(horizontalLayoutWidget_20);
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        horizontalLayout_20->setContentsMargins(0, 0, 0, 0);
        label_20 = new QLabel(horizontalLayoutWidget_20);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        horizontalLayout_20->addWidget(label_20);

        lineEdit_mass = new QLineEdit(horizontalLayoutWidget_20);
        lineEdit_mass->setObjectName(QString::fromUtf8("lineEdit_mass"));

        horizontalLayout_20->addWidget(lineEdit_mass);

        pushButton_setMass = new QPushButton(horizontalLayoutWidget_20);
        pushButton_setMass->setObjectName(QString::fromUtf8("pushButton_setMass"));

        horizontalLayout_20->addWidget(pushButton_setMass);

        pushButton_resetMass = new QPushButton(horizontalLayoutWidget_20);
        pushButton_resetMass->setObjectName(QString::fromUtf8("pushButton_resetMass"));

        horizontalLayout_20->addWidget(pushButton_resetMass);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 300, 311, 41));
        radioButton_point = new QRadioButton(groupBox_2);
        radioButton_point->setObjectName(QString::fromUtf8("radioButton_point"));
        radioButton_point->setGeometry(QRect(30, 20, 111, 17));
        radioButton_orientation = new QRadioButton(groupBox_2);
        radioButton_orientation->setObjectName(QString::fromUtf8("radioButton_orientation"));
        radioButton_orientation->setGeometry(QRect(160, 20, 131, 17));
        horizontalLayoutWidget_11 = new QWidget(tab_2);
        horizontalLayoutWidget_11->setObjectName(QString::fromUtf8("horizontalLayoutWidget_11"));
        horizontalLayoutWidget_11->setGeometry(QRect(10, 10, 296, 31));
        horizontalLayout_11 = new QHBoxLayout(horizontalLayoutWidget_11);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        checkBox_exportObj = new QCheckBox(horizontalLayoutWidget_11);
        checkBox_exportObj->setObjectName(QString::fromUtf8("checkBox_exportObj"));
        checkBox_exportObj->setChecked(false);

        horizontalLayout_11->addWidget(checkBox_exportObj);

        checkBox_captureScreen = new QCheckBox(horizontalLayoutWidget_11);
        checkBox_captureScreen->setObjectName(QString::fromUtf8("checkBox_captureScreen"));
        checkBox_captureScreen->setChecked(false);

        horizontalLayout_11->addWidget(checkBox_captureScreen);

        checkBox_captureSubScreen = new QCheckBox(horizontalLayoutWidget_11);
        checkBox_captureSubScreen->setObjectName(QString::fromUtf8("checkBox_captureSubScreen"));
        checkBox_captureSubScreen->setChecked(false);

        horizontalLayout_11->addWidget(checkBox_captureSubScreen);

        horizontalLayoutWidget_5 = new QWidget(tab_2);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(10, 50, 291, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(150, 25));
        label_3->setMaximumSize(QSize(150, 25));

        horizontalLayout_5->addWidget(label_3);

        lineEdit_damping = new QLineEdit(horizontalLayoutWidget_5);
        lineEdit_damping->setObjectName(QString::fromUtf8("lineEdit_damping"));

        horizontalLayout_5->addWidget(lineEdit_damping);

        pushButton_setDamping = new QPushButton(horizontalLayoutWidget_5);
        pushButton_setDamping->setObjectName(QString::fromUtf8("pushButton_setDamping"));
        pushButton_setDamping->setMinimumSize(QSize(70, 25));
        pushButton_setDamping->setMaximumSize(QSize(70, 25));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/joint_deformation/Resources/dialog_apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_setDamping->setIcon(icon8);

        horizontalLayout_5->addWidget(pushButton_setDamping);

        tabWidget->addTab(tab_2, QString());
        groupBox_2->raise();
        horizontalLayoutWidget_11->raise();
        horizontalLayoutWidget_5->raise();
        pushButton_setDamping->raise();
        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(1040, 400, 351, 61));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_initSimulator = new QPushButton(horizontalLayoutWidget_2);
        pushButton_initSimulator->setObjectName(QString::fromUtf8("pushButton_initSimulator"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Haptics/Resources/gnome_run.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_initSimulator->setIcon(icon9);

        horizontalLayout_2->addWidget(pushButton_initSimulator);

        pushButton_startSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_startSimulation->setObjectName(QString::fromUtf8("pushButton_startSimulation"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/Haptics/Resources/gtk_media_play_ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_startSimulation->setIcon(icon10);

        horizontalLayout_2->addWidget(pushButton_startSimulation);

        pushButton_nextStep = new QPushButton(horizontalLayoutWidget_2);
        pushButton_nextStep->setObjectName(QString::fromUtf8("pushButton_nextStep"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Haptics/Resources/gtk_media_next_ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_nextStep->setIcon(icon11);

        horizontalLayout_2->addWidget(pushButton_nextStep);

        pushButton__pauseSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton__pauseSimulation->setObjectName(QString::fromUtf8("pushButton__pauseSimulation"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/Haptics/Resources/gtk_media_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton__pauseSimulation->setIcon(icon12);

        horizontalLayout_2->addWidget(pushButton__pauseSimulation);

        pushButton_resetSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_resetSimulation->setObjectName(QString::fromUtf8("pushButton_resetSimulation"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/Haptics/Resources/gtk_media_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_resetSimulation->setIcon(icon13);

        horizontalLayout_2->addWidget(pushButton_resetSimulation);

        comboBox_chooseSimulator = new QComboBox(centralWidget);
        comboBox_chooseSimulator->setObjectName(QString::fromUtf8("comboBox_chooseSimulator"));
        comboBox_chooseSimulator->setGeometry(QRect(1040, 10, 351, 22));
        HapticsClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(HapticsClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1400, 21));
        HapticsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(HapticsClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        HapticsClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(HapticsClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        HapticsClass->setStatusBar(statusBar);

        mainToolBar->addAction(actionLoadMech);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionScaleMesh);
        mainToolBar->addAction(actionShowGrid);
        mainToolBar->addAction(actionShowMesh);
        mainToolBar->addAction(actionShowVox);
        mainToolBar->addAction(actionShowSelection);
        mainToolBar->addAction(actionReset);
        mainToolBar->addAction(actionSetMass);

        retranslateUi(HapticsClass);
        QObject::connect(checkBox, SIGNAL(clicked(bool)), widget_haptic, SLOT(onReferenceON(bool)));
        QObject::connect(checkBox_2, SIGNAL(clicked(bool)), widget_haptic, SLOT(onLinearVelocity(bool)));
        QObject::connect(actionLoadMech, SIGNAL(triggered()), HapticsClass, SLOT(loadMesh()));
        QObject::connect(actionScaleMesh, SIGNAL(triggered()), HapticsClass, SLOT(scaleMesh()));
        QObject::connect(actionShowGrid, SIGNAL(triggered()), HapticsClass, SLOT(showGrid()));
        QObject::connect(actionShowMesh, SIGNAL(triggered(bool)), HapticsClass, SLOT(showMesh(bool)));
        QObject::connect(actionShowSelection, SIGNAL(triggered(bool)), HapticsClass, SLOT(showSelection(bool)));
        QObject::connect(actionShowVox, SIGNAL(triggered(bool)), HapticsClass, SLOT(showVox(bool)));
        QObject::connect(pushButton_ok, SIGNAL(clicked()), HapticsClass, SLOT(setGridDensity()));
        QObject::connect(pushButton_initSimulator, SIGNAL(clicked()), HapticsClass, SLOT(initializeSimulation()));
        QObject::connect(pushButton_startSimulation, SIGNAL(clicked()), HapticsClass, SLOT(startSimulation()));
        QObject::connect(pushButton__pauseSimulation, SIGNAL(clicked()), HapticsClass, SLOT(pauseSimulation()));
        QObject::connect(actionReset, SIGNAL(triggered()), HapticsClass, SLOT(resetCamera()));
        QObject::connect(comboBox_chooseSimulator, SIGNAL(currentIndexChanged(int)), HapticsClass, SLOT(chooseSimulator(int)));
        QObject::connect(comboBox_level, SIGNAL(currentIndexChanged(int)), HapticsClass, SLOT(chooseLevelDisplay(int)));
        QObject::connect(pushButton_addLevel, SIGNAL(clicked()), HapticsClass, SLOT(addLevel()));
        QObject::connect(pushButton_deleteLevel, SIGNAL(clicked()), HapticsClass, SLOT(clearLevel()));
        QObject::connect(checkBox_multigrid, SIGNAL(clicked(bool)), HapticsClass, SLOT(setMultigrid(bool)));
        QObject::connect(checkBox_dynamics, SIGNAL(clicked(bool)), HapticsClass, SLOT(setDynamics(bool)));
        QObject::connect(checkBox_gravity, SIGNAL(clicked(bool)), HapticsClass, SLOT(setGravity(bool)));
        QObject::connect(pushButton_setGravityMagnitude, SIGNAL(clicked()), HapticsClass, SLOT(setGravityMagnitude()));
        QObject::connect(pushButton_setForceScalar, SIGNAL(clicked()), HapticsClass, SLOT(setGravityMagnitude()));
        QObject::connect(actionSetMass, SIGNAL(triggered(bool)), HapticsClass, SLOT(setEnableMass(bool)));
        QObject::connect(checkBox_setShowLastLevel, SIGNAL(clicked(bool)), HapticsClass, SLOT(setShowLastLevel(bool)));
        QObject::connect(pushButton_setThresholdEnergy, SIGNAL(clicked()), HapticsClass, SLOT(setEnergyThreshold()));
        QObject::connect(pushButton_setMass, SIGNAL(clicked()), HapticsClass, SLOT(setMass()));
        QObject::connect(pushButton_resetMass, SIGNAL(clicked()), HapticsClass, SLOT(resetMass()));
        QObject::connect(checkBox_showParticle, SIGNAL(clicked(bool)), HapticsClass, SLOT(setShowParticleCoupling(bool)));
        QObject::connect(checkBox_captureScreen, SIGNAL(clicked(bool)), HapticsClass, SLOT(setCaptureScreen(bool)));
        QObject::connect(checkBox_captureSubScreen, SIGNAL(clicked(bool)), HapticsClass, SLOT(setCaptureSubScreen(bool)));
        QObject::connect(pushButton_setDamping, SIGNAL(clicked()), HapticsClass, SLOT(setDampParameter()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(HapticsClass);
    } // setupUi

    void retranslateUi(QMainWindow *HapticsClass)
    {
        HapticsClass->setWindowTitle(QApplication::translate("HapticsClass", "Haptics", 0, QApplication::UnicodeUTF8));
        actionLoadMech->setText(QApplication::translate("HapticsClass", "loadMech", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLoadMech->setToolTip(QApplication::translate("HapticsClass", "loadMesh", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLoadMech->setShortcut(QApplication::translate("HapticsClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionScaleMesh->setText(QApplication::translate("HapticsClass", "scaleMesh", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionScaleMesh->setToolTip(QApplication::translate("HapticsClass", "scaleMesh", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionScaleMesh->setShortcut(QApplication::translate("HapticsClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionShowGrid->setText(QApplication::translate("HapticsClass", "showGrid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowGrid->setToolTip(QApplication::translate("HapticsClass", "showGrid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowGrid->setShortcut(QApplication::translate("HapticsClass", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        actionShowMesh->setText(QApplication::translate("HapticsClass", "showMesh", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowMesh->setToolTip(QApplication::translate("HapticsClass", "showMesh", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowMesh->setShortcut(QApplication::translate("HapticsClass", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        actionShowVox->setText(QApplication::translate("HapticsClass", "showVox", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowVox->setToolTip(QApplication::translate("HapticsClass", "showVox", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowVox->setShortcut(QApplication::translate("HapticsClass", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        actionShowSelection->setText(QApplication::translate("HapticsClass", "showSelection", 0, QApplication::UnicodeUTF8));
        actionShowSelection->setShortcut(QApplication::translate("HapticsClass", "Ctrl+4", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("HapticsClass", "Reset", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionReset->setToolTip(QApplication::translate("HapticsClass", "actionReset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReset->setShortcut(QApplication::translate("HapticsClass", "Ctrl+5", 0, QApplication::UnicodeUTF8));
        actionSetMass->setText(QApplication::translate("HapticsClass", "setMass", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("HapticsClass", "Haptic Navigation", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("HapticsClass", "Reference Frame", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("HapticsClass", "Linear Velocity", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HapticsClass", "Grid Density", 0, QApplication::UnicodeUTF8));
        pushButton_ok->setText(QApplication::translate("HapticsClass", "OK", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("HapticsClass", "Level", 0, QApplication::UnicodeUTF8));
        pushButton_addLevel->setText(QApplication::translate("HapticsClass", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_deleteLevel->setText(QApplication::translate("HapticsClass", "Delete", 0, QApplication::UnicodeUTF8));
        checkBox_multigrid->setText(QApplication::translate("HapticsClass", "Multigrid", 0, QApplication::UnicodeUTF8));
        checkBox_dynamics->setText(QApplication::translate("HapticsClass", "Dynamics", 0, QApplication::UnicodeUTF8));
        checkBox_gravity->setText(QApplication::translate("HapticsClass", "Gravity", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("HapticsClass", "Gravity Magnitude", 0, QApplication::UnicodeUTF8));
        lineEdit_gravityMagnitude->setText(QApplication::translate("HapticsClass", "0.01", 0, QApplication::UnicodeUTF8));
        pushButton_setGravityMagnitude->setText(QApplication::translate("HapticsClass", "SetMagnitude", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("HapticsClass", "Force Scalar", 0, QApplication::UnicodeUTF8));
        lineEdit_forceScalar->setText(QApplication::translate("HapticsClass", "10.0", 0, QApplication::UnicodeUTF8));
        pushButton_setForceScalar->setText(QApplication::translate("HapticsClass", "Set", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("HapticsClass", "Adapative Option", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("HapticsClass", "Threshold", 0, QApplication::UnicodeUTF8));
        lineEdit_thresholdEnergy->setText(QApplication::translate("HapticsClass", "0.25", 0, QApplication::UnicodeUTF8));
        pushButton_setThresholdEnergy->setText(QApplication::translate("HapticsClass", "SetEnergyThreshold", 0, QApplication::UnicodeUTF8));
        checkBox_setShowLastLevel->setText(QApplication::translate("HapticsClass", "Show Last Level Only", 0, QApplication::UnicodeUTF8));
        checkBox_showParticle->setText(QApplication::translate("HapticsClass", "Show Particle Coupling", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("HapticsClass", "Mass", 0, QApplication::UnicodeUTF8));
        lineEdit_mass->setText(QApplication::translate("HapticsClass", "1.0", 0, QApplication::UnicodeUTF8));
        pushButton_setMass->setText(QApplication::translate("HapticsClass", "SetMass", 0, QApplication::UnicodeUTF8));
        pushButton_resetMass->setText(QApplication::translate("HapticsClass", "ResetMass", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("HapticsClass", "Grid", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("HapticsClass", "Constraints", 0, QApplication::UnicodeUTF8));
        radioButton_point->setText(QApplication::translate("HapticsClass", "Position Constraint", 0, QApplication::UnicodeUTF8));
        radioButton_orientation->setText(QApplication::translate("HapticsClass", "Orientation Constraint", 0, QApplication::UnicodeUTF8));
        checkBox_exportObj->setText(QApplication::translate("HapticsClass", "ExportOBJ", 0, QApplication::UnicodeUTF8));
        checkBox_captureScreen->setText(QApplication::translate("HapticsClass", "CaptureScreen", 0, QApplication::UnicodeUTF8));
        checkBox_captureSubScreen->setText(QApplication::translate("HapticsClass", "CaptureSubScreen", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("HapticsClass", "Damping", 0, QApplication::UnicodeUTF8));
        lineEdit_damping->setText(QApplication::translate("HapticsClass", "0.01", 0, QApplication::UnicodeUTF8));
        pushButton_setDamping->setText(QApplication::translate("HapticsClass", "SetDamp", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("HapticsClass", "ShapeMatching", 0, QApplication::UnicodeUTF8));
        pushButton_initSimulator->setText(QString());
        pushButton_startSimulation->setText(QString());
        pushButton_nextStep->setText(QString());
        pushButton__pauseSimulation->setText(QString());
        pushButton_resetSimulation->setText(QString());
        comboBox_chooseSimulator->clear();
        comboBox_chooseSimulator->insertItems(0, QStringList()
         << QApplication::translate("HapticsClass", "NONE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Native Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Hierarchical Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "NewHierarchical Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Adaptive Hierarchy Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Adaptive Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Velocity Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Single Grid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Pair Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Multiple Velocity Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Simulation through Network", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("HapticsClass", "Simulation for Mobile", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class HapticsClass: public Ui_HapticsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HAPTICS_H
