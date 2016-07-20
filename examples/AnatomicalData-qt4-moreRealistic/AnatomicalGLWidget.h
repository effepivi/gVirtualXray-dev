#ifndef ANATOMICAL_GL_WIDGET_H
#define ANATOMICAL_GL_WIDGET_H


/**
********************************************************************************
*
*	@file		AnatomicalGLWidget.h
*
*	@brief      Class to handle an anatomical rendering using OpenGL.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************

#ifndef GVXR_GL_WIDGET_H
#include "GLWidget.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay{

//==============================================================================
/**
*   @class  AnatomicalGLWidget
*   @brief  AnatomicalGLWidget is a class to handle a AnatomicalMesh rendering
*   		in an OpenGL window
*/
//==============================================================================
class AnatomicalGLWidget: public GLWidget
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	AnatomicalGLWidget(QWidget *parent = 0);
	virtual ~AnatomicalGLWidget();

private:
    /*virtual void initializeGL();


	virtual void loadPatient(PatientID aPatientID);


	virtual void render();


	virtual void displayPatient(std::vector<AnatomicalMesh>& apMeshSet,
	                        GLint aShaderID);*/

};
} //namespace gVirtualXRay
#endif //ANATOMICAL_GL_WIDGET_H
