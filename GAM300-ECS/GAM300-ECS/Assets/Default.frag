/* Start Header**********************************************************************************/
/*
@file    Default.frag
@author  Chia Yi da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/
#version 460 core

uniform vec4 uColor;

// Output data
out vec4 fColor;

void main()
{
	fColor = uColor;
}
