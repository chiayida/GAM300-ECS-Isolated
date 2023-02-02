/* Start Header**********************************************************************************/
/*
@file    Default.vert
@author  Chia Yi Da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/
#version 460 core

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;
layout (location=2) in vec2 aVertexTexture;
layout (location=3) in float aTextureIndex;
layout (location=4) in vec3 aTPosition;
layout (location=5) in vec3 aTScale;
layout (location=6) in vec3 aTRotation;

layout (location=0) out vec4 vColor;
layout (location=1) out vec2 vTexture;
layout (location=2) out float vTextureIndex;

void main()
{
	// X
	mat4 rotationX = mat4(vec4(1.0, 0.0, 0.0, 0.0),
                        vec4(0.0, cos(aTRotation.x), sin(aTRotation.x), 0.0),
                        vec4(0.0, -sin(aTRotation.x), cos(aTRotation.x), 0.0),
                        vec4(0.0, 0.0, 0.0, 1.0));

	// Y
	mat4 rotationY = mat4(vec4(cos(aTRotation.y), 0.0, -sin(aTRotation.y), 0.0),
                        vec4(0.0, 1.0, 0.0, 0.0),
                        vec4(sin(aTRotation.y), 0.0, cos(aTRotation.y), 0.0),
                        vec4(0.0, 0.0, 0.0, 1.0));

	// Z 
	mat4 rotationZ = mat4(vec4(cos(aTRotation.z), sin(aTRotation.z), 0.0, 0.0),
                        vec4(-sin(aTRotation.z), cos(aTRotation.z), 0.0, 0.0),
                        vec4(0.0, 0.0, 1.0, 0.0),
                        vec4(0.0, 0.0, 0.0, 1.0));

	// Might not be the right matrix multiplication order. 
	// In my case it doesnt matter as its only for particles
	mat4 rotationMatrix = rotationZ * rotationX * rotationY;

	// Compute uModel_to_NDC matrix
	mat4 uModel_to_NDC = mat4(
		// Translate
		mat4(vec4(1.f, 0.f, 0.f, 0.f),
			 vec4(0.f, 1.f, 0.f, 0.f),
			 vec4(0.f, 0.f, 1.f, 0.f),
			 vec4(aTPosition.x, aTPosition.y, aTPosition.z , 1.f))
		*
		// Rotate
		rotationMatrix

		*
		// Scale
		mat4(vec4(aTScale.x, 0.f, 0.f, 0.f),
			 vec4(0.f, aTScale.y, 0.f, 0.f),
			 vec4(0.f, 0.f, aTScale.z, 0.f),
			 vec4(0.f, 0.f, 0.f, 1.f))
		);

    gl_Position = uProjectionMatrix * uViewMatrix * uModel_to_NDC * vec4(aVertexPosition, 1.0f);

	vColor = aVertexColor;
	vTexture = aVertexTexture;
	vTextureIndex = aTextureIndex;
}