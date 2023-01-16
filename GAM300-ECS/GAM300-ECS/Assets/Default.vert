/* Start Header**********************************************************************************/
/*
@file    Default.vert
@author  Chia Yi da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/
#version 460 core

uniform mat4 uTransformMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

layout(location = 0) in vec3 vPosition;

void main()
{
    gl_Position = uProjectionMatrix * uViewMatrix * uTransformMatrix * vec4(vPosition, 1.0f);
}