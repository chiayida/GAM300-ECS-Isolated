/* Start Header**********************************************************************************/
/*
@file    Default.vert
@author  Chia Yi da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/
#version 460 core

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uTransformMatrix;

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;
layout (location=2) in vec3 aTPosition;
layout (location=3) in vec3 aTScale;
layout (location=4) in float aTRotation;

layout (location=0) out vec4 vColor;

void main()
{
	// Compute uModel_to_NDC matrix
	mat4 uModel_to_NDC = mat4(
		// Translate
		mat4(vec4(1.f, 0.f, 0.f, 0.f),
			 vec4(0.f, 1.f, 0.f, 0.f),
			 vec4(0.f, 0.f, 1.f, 0.f),
			 vec4(aTPosition.x, aTPosition.y, aTPosition.z , 1.f))
		*
		// Rotate
		mat4(vec4(1.f, 0.f, 0.f, 0.f),
			 vec4(0.f, cos(aTRotation), sin(aTRotation), 0.f),
			 vec4(0.f, -sin(aTRotation), cos(aTRotation), 0.f),
			 vec4(0.f, 0.f, 0.f, 1.f))
		*
		// Scale
		mat4(vec4(aTScale.x, 0.f, 0.f, 0.f),
			 vec4(0.f, aTScale.y, 0.f, 0.f),
			 vec4(0.f, 0.f, aTScale.z, 0.f),
			 vec4(0.f, 0.f, 0.f, 1.f))
		);


    gl_Position = vec4(vec3(uProjectionMatrix * uViewMatrix * uTransformMatrix * vec4(aVertexPosition, 1.0f)), 1.0);
	vColor = aVertexColor;
}