#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTexCoord;
in vec4 aColor;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vTexCoord;
out vec4 vColor;

void main()
{
    vPosition = vec3( modelMatrix * vec4( aPosition, 1.0 ) );
    vNormal = normalize( normalMatrix * aNormal );
	vTexCoord = aTexCoord;
	vColor = aColor;
    gl_Position = modelViewProjectionMatrix * vec4( aPosition, 1.0 );
}
