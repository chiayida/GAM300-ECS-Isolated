/* Start Header**********************************************************************************/
/*
@file    Default.frag
@author  Chia Yi da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/

#version 450 core

uniform sampler2D uTextures[32];

layout (location=0) in vec4 vColor;
layout (location=1) in vec2 vTexture;
layout (location=2) in float vTextureIndex;
layout (location=3) in float vIsLight;

layout (location=0) out vec4 fColor;


void main () 
{
	// Draw call is drawing light, use light colour
	if (vIsLight > 0.5f) {
		vec4 texColor = texture(uTextures[0], vTexture) * vColor;

		fColor = texColor;
	}
	// Draw call is drawing texture object, use texture colour
	else {
		int index = int(vTextureIndex);
		vec4 texColor = texture(uTextures[index], vTexture) * vColor;

		fColor = texColor;
	}
}