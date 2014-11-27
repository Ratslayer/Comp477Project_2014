#version 150

struct Light
{
	bool enabled;
	int type;
	vec3 position;
	vec3 direction;
	vec4 intensity;
	vec4 ambient;
	float aperture;
};

struct Material
{
	vec4 surface;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 metallic;
	vec4 refraction;
	vec4 reflection;
	float shininess;
	float refractionIndex;
	bool useTexture2D;
	bool useTexture3D;
	bool useTextureCube;
};

uniform Light lights[8];
uniform int nbLights;
uniform vec3 cameraPosition;
uniform Material material;
uniform sampler2D texture2D;
uniform sampler3D texture3D;
uniform samplerCube textureCube;
uniform samplerCube environment;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vTexCoord;
in vec4 vColor;

out vec4 fragmentColor;

const float INV_PI = 1.0 / 3.14159265358979323846264;

vec4 BlinnPhongShading()
{
    vec4 ambient  = vec4( 0.0 );
    vec4 diffuse  = vec4( 0.0 );
    vec4 specular = vec4( 0.0 );

	vec3 N = normalize( vNormal );

	for ( int i=0 ; i<nbLights ; i++ )
    {
		if ( lights[i].enabled )
		{
			vec3 L;
			float attenuation;

			if ( lights[i].type == 0 ) // Directional
			{
				L = -lights[i].direction;
				attenuation = 1.0;
			}
			else // Point or spot
			{
				vec3 D = lights[i].position - vPosition;
				float distance = length( D );
				L = normalize( D );
				attenuation = 1.0 / ( distance * distance );
			}

			float nDotL = dot( N, L );

			// Spot light aperture check
			if ( lights[i].type == 2 )
				if ( dot( -lights[i].direction, L ) < cos( lights[i].aperture ) )
					nDotL = 0;

			if ( nDotL > 0.0 )
			{   
				vec3 E = normalize( cameraPosition - vPosition );
				vec3 H = normalize( E + L );
			   
				float pf = pow( max( dot( N, H ), 0.0 ), material.shininess );
				float c = ( material.shininess + 2 ) * 0.5 * INV_PI;

				diffuse += lights[i].intensity * ( attenuation * nDotL * INV_PI );
				specular += lights[i].intensity * ( attenuation * pf * nDotL * c );
			}

			ambient += lights[i].ambient;
		}
    }

	vec4 surface;

	if ( material.useTexture2D )
		surface = texture( texture2D, vTexCoord.xy );
	else if ( material.useTexture3D )
		surface = texture( texture3D, vTexCoord );
	else if ( material.useTextureCube )
		surface = texture( textureCube, vTexCoord );
	else
		surface = material.surface;

	vec4 metallicSurface = material.metallic * surface + ( vec4(1) - material.metallic );

	ambient = ambient * material.ambient * vColor;
	diffuse = diffuse * material.diffuse * vColor;
	specular = specular * material.specular * vColor;

	return ( ambient + diffuse ) * surface + specular * metallicSurface;
}

vec4 RefractionShading() 
{
	float eta = 1 / material.refractionIndex;
	vec3 E = normalize( cameraPosition - vPosition );
	vec3 N = normalize( vNormal );
	float cosTheta1 = dot( E, N );

	if ( cosTheta1 < 0 )
	{
		N = -N;
		cosTheta1 = -cosTheta1;
		eta = 1 / eta;
	}

	float disc = max( 0.0, 1.0 - ( ( eta * eta ) * ( 1.0 - ( cosTheta1 * cosTheta1 ) ) ) );
	
	float cosTheta2 = sqrt( disc );
	vec3 reflectionVector = ( 2.0 * cosTheta1 ) * N - E;
	vec3 refractionVector = ( eta * cosTheta1 - cosTheta2 ) * N - ( eta * E );

	float fresnelRS = max( 0, ( eta * cosTheta1 - cosTheta2 ) / ( eta * cosTheta1 + cosTheta2 ) );
	float fresnelRP = max( 0, ( cosTheta1 - eta * cosTheta2 ) / ( cosTheta1 + eta * cosTheta2 ) );

	float fresnelR = ( fresnelRS * fresnelRS + fresnelRP * fresnelRP ) / 2.0;
	float fresnelT =  ( ( 1.0 - fresnelRS ) * ( 1.0 - fresnelRS ) + ( 1.0 - fresnelRP ) * ( 1.0 - fresnelRP ) ) / 2.0;

	vec4 reflectColor = texture( environment, reflectionVector ) * fresnelR;
	vec4 refractColor = texture( environment, refractionVector ) * fresnelT;

	return ( reflectColor + refractColor ) * material.refraction;
}

vec4 ReflectionShading() 
{
	vec3 E = normalize( cameraPosition - vPosition );
	vec3 N = normalize( vNormal );
	vec3 R = ( 2.0 * dot( E, N ) ) * N - E;

	return texture( environment, R ) * material.reflection;
}

void main()
{
    fragmentColor = BlinnPhongShading();

	if ( ( material.refraction.x != 0.0 ) || ( material.refraction.y != 0.0 ) || ( material.refraction.z != 0.0 ) )
		fragmentColor += RefractionShading();

	if ( ( material.reflection.x != 0.0 ) || ( material.reflection.y != 0.0 ) || ( material.reflection.z != 0.0 ) )
		fragmentColor += ReflectionShading();

    fragmentColor = clamp( fragmentColor, 0.0, 1.0 );
}

