//===========================================================================
/*

*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CShapeMatching3dofPointerH
#define CShapeMatching3dofPointerH
//---------------------------------------------------------------------------
#include "tools/CGenericTool.h"
#include "graphics/CColor.h"
#include "scenegraph/CShapeSphere.h"
#include "scenegraph/CMesh.h"
#include "cShapeMatchingForceAlgo.h"
#include "forces/CPotentialFieldForceAlgo.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       cShapeMatching3dofPointer.h
    
    \brief  
    <b> Haptic Tools </b> \n 
    Single Point Contact Tool.
*/
//===========================================================================

//===========================================================================
/*!
    \class      cShapeMatching3dofPointer
    \ingroup    tools  
    
    \brief      
    cShapeMatching3dofPointer represents a haptic tool that can apply forces in 
    three degrees of freedom and maintains three or six degrees of 
    device pose. \n

    This class provides i/o with haptic devices and a basic graphical 
    representation of a tool.
*/
//===========================================================================
class cShapeMatching3dofPointer : public cGenericTool
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of cShapeMatching3dofPointer.
    cShapeMatching3dofPointer(cWorld* a_world);

    //! Destructor of cShapeMatching3dofPointer.
    virtual ~cShapeMatching3dofPointer();


    //-----------------------------------------------------------------------
    // METHODS - TOOL COMMANDS
    //-----------------------------------------------------------------------

    //! Start communication with the device connected to the tool (0 indicates success).
    virtual int start();

    //! Stop communication with the device connected to the tool (0 indicates success).
    virtual int stop();

    //! Initialize the device connected to the tool (0 indicates success).
    virtual int initialize(const bool a_resetEncoders=false);

    //! Toggle forces \b ON.
    virtual int setForcesON();

    //! Toggle forces \b OFF.
    virtual int setForcesOFF();

    //! Update position and orientation of the device.
    virtual void updatePose();

    //! Compute interaction forces with environment.
    virtual void computeInteractionForces();

    //! Apply latest computed forces to device.
    virtual void applyForces();

    //! Get a pointer to the proxy force algorithm.
    virtual cShapeMatchingForceAlgo* getProxy() { return m_proxyPointForceModel; }

    //! Check if the tool is touching a particular object.
    virtual bool isInContact(cGenericObject* a_object);

    //! Read position of proxy in global world coordinates.
    virtual cVector3d getProxyGlobalPos() { return (m_proxyPointForceModel->getProxyGlobalPosition()); }

    //! Read orientation of proxy in global world coordinates.
    virtual cMatrix3d getProxyGlobalRot() { return (m_deviceGlobalRot); }

    //! Read position of haptic device in global world coordinates.
    virtual cVector3d getDeviceGlobalPos() { return (m_deviceGlobalPos); }

    //! Read orientation of haptic device in global world coordinates.
    virtual cMatrix3d getDeviceGlobalRot() { return (m_deviceGlobalRot); }

    //! Read position of haptic device in local coordinates.
    virtual cVector3d getDeviceLocalPos() { return (m_deviceLocalPos); }

    //! Read orientation of haptic device in local coordinates.
    virtual cMatrix3d getDeviceLocalRot() { return (m_deviceLocalRot); }

	


    //-----------------------------------------------------------------------
    // METHODS - WORKSPACE SETTINGS
    //-----------------------------------------------------------------------

    //! Set radius of pointer.
    virtual void setRadius(const double& a_radius);

    //! Set virtual workspace dimensions in which tool will be working.
    virtual void setWorkspaceRadius(const double& a_workspaceRadius);

    //! Read the radius of the workspace of the tool.
    double getWorkspaceRadius() { return(m_workspaceRadius); }

    //! Set the scale factor between the workspace of the tool and one of the haptic device.
    void setWorkspaceScaleFactor(const double& a_workspaceScaleFactor);

    //! Read the scale factor between the workspace of the tool and one of the haptic device.
    double getWorkspaceScaleFactor() { return (m_workspaceScaleFactor); }


    //-----------------------------------------------------------------------
    // METHODS - GRAPHICS
    //-----------------------------------------------------------------------

    //! Render the object in OpenGL.
    virtual void render(const int a_renderMode=0);


    //-----------------------------------------------------------------------
    // MEMBERS - GRAPHICS
    //-----------------------------------------------------------------------

    //! Sphere representing the device.
    cShapeSphere* m_deviceSphere;

    //! Sphere representing the proxy.
    cShapeSphere* m_proxySphere;

    //! Mesh representing the device.
    cMesh* m_deviceMesh;

    //! Mesh representing the proxy.
    cMesh* m_proxyMesh;

    //! Color of line connecting proxy and device position together.
    cColorf m_colorLine;

    //! Material properties of proxy.
    cMaterial m_materialProxy;

    //! Material properties of proxy when button is pressed.
    cMaterial m_materialProxyButtonPressed;


    //-----------------------------------------------------------------------
    // MEMBERS - FORCE MODELS
    //-----------------------------------------------------------------------

    //! Finger-proxy algorithm model to handle interactions with mesh objects.
    cShapeMatchingForceAlgo* m_proxyPointForceModel;

    //! Potential fields model.
    cPotentialFieldForceAlgo* m_potentialFieldsForceModel;

    /*!
        The last force computed for application to this tool, in the world coordinate
        system.  [N] \n

        If you want to manually send forces to a device, you can modify this
        value before calling 'applyForces'.
    */
    cVector3d m_lastComputedGlobalForce;

    /*!
        The last force computed for application to this tool, in the device coordinate.
        system.  [N]
    */
    cVector3d m_lastComputedLocalForce;


    //-----------------------------------------------------------------------
    // MEMBERS - TOOL STATUS AND WORKSPACE SETTINGS
    //-----------------------------------------------------------------------

    //! Radius of the workspace which can be accessed by the tool.
    double m_workspaceRadius;

    //! Scale factor between the sizes of the tool workspace and the haptic device workspace.
    double m_workspaceScaleFactor;

    //! Position of device in device local coordinate system.
    cVector3d m_deviceLocalPos;

    //! Position of device in world global coordinate system.
    cVector3d m_deviceGlobalPos;

    //! Velocity of device in device local coordinate system.
    cVector3d m_deviceLocalVel;

    //! Velocity of device in world global coordinate system.
    cVector3d m_deviceGlobalVel;

    //! Orientation of wrist in local coordinates of device.
    cMatrix3d m_deviceLocalRot;

    //! Orientation of wrist in global coordinates of device.
    cMatrix3d m_deviceGlobalRot;


  protected:

    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    //! World in which tool is interacting.
    cWorld* m_world;

    //! Radius of sphere representing position of pointer.
    double m_displayRadius;

    //! Last status of user switch 0. This value is used by the graphical rendering function.
    bool m_userSwitch0;

    //! This flag records whether the user has enabled forces.
    bool m_forceON;

    //! Flag to avoid initial bumps in force (has the user sent a _small_ force yet?).
    bool m_forceStarted;

    /*!
        Normally this class waits for a very small force before initializing forces
        to avoid initial "jerks" (a safety feature); you can bypass that requirement
        with this variable.
    */
    bool m_waitForSmallForce;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

