/**
********************************************************************************
*
*   @file       OpenGL3VBO.inl
*
*   @brief      Class to handle vertex buffer objects (VBOs) in OpenGL 3.x or 4.x.
*
*   @version    1.0
*
*   @date       07/08/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------------
inline OpenGL3VBO::~OpenGL3VBO()
//------------------------------
{
	destroy();
}


//-------------------------------------------------------
inline VBO& OpenGL3VBO::operator=(const OpenGL3VBO& aVBO)
//-------------------------------------------------------
{
    this->VBO::operator=(aVBO);
    m_vao_id = aVBO.m_vao_id;

	return (*this);
}


} // namespace gVirtualXRay
