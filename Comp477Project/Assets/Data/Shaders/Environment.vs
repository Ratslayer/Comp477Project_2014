#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTexCoord;
in vec3 aColor;

out vec3 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;
    gl_Position = modelViewProjectionMatrix * vec4( aPosition, 1.0 );
	
	// No camera translation
	// vec4 modelPos = modelMatrix * vec4( aPosition, 1.0 );
	// vec4 viewPos = viewMatrix * vec4( modelPos.xyz, 0.0 );
	// gl_Position = projectionMatrix * vec4( viewPos.xyz, 1.0 );
}
