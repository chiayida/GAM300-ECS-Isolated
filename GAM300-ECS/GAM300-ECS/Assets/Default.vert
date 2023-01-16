/* Start Header**********************************************************************************/
/*
@file    Default.vert
@author  Chia Yi da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/

#version 450 core
 
uniform mat3 uCamMatrix;

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec4 aVertexColor;
layout (location=2) in vec2 aVertexTexture;
layout (location=3) in float aTextureIndex;
layout (location=4) in vec2 aTPosition;
layout (location=5) in vec2 aTScale;
layout (location=6) in float aTRotation;
layout (location=7) in float aIsLight;

layout (location=0) out vec4 vColor;
layout (location=1) out vec2 vTexture;
layout (location=2) out float vTextureIndex;
layout (location=3) out float vIsLight;


void main() 
{
	// Compute uModel_to_NDC matrix
	mat3 uModel_to_NDC = mat3(
		// Translate
		mat3(vec3(1.f, 0, 0),
			 vec3(0, 1.f, 0),
			 vec3(aTPosition.x, aTPosition.y, 1.f))
		*
		// Rotate
		mat3(vec3(cos(aTRotation), sin(aTRotation), 0.f),
			 vec3(-sin(aTRotation), cos(aTRotation), 0.f),
			 vec3(0.f, 0.f, 1.f))
		*
		// Scale
		mat3(vec3(aTScale.x, 0, 0),
			 vec3(0, aTScale.y, 0),
			 vec3(0, 0, 1.f))
		);

	// object position
	gl_Position = vec4(vec2(uCamMatrix * uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);

	vColor = aVertexColor;
	vTexture = aVertexTexture;
	vTextureIndex = aTextureIndex;
	vIsLight = aIsLight;
}