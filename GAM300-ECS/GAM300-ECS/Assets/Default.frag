/* Start Header**********************************************************************************/
/*
@file    Default.frag
@author  Chia Yi da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/
#version 460 core

layout (location=0) in vec4 vColor;

layout (location=0) out vec4 fColor;

void main()
{
	fColor = vColor;
}
