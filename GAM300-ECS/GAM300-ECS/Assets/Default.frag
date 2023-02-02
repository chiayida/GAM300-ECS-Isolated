/* Start Header**********************************************************************************/
/*
@file    Default.frag
@author  Chia Yi Da		c.yida@digipen.edu
@date    16/01/2023
*/
/* End Header **********************************************************************************/
#version 460 core

uniform sampler2D uTextures[32];

layout (location=0) in vec4 vColor;
layout (location=1) in vec2 vTexture;
layout (location=2) in float vTextureIndex;

layout (location=0) out vec4 fColor;

void main()
{
	int index = int(vTextureIndex);
	vec4 texColor = texture(uTextures[index], vTexture) * vColor;
	fColor = texColor;
}
