#version 450

out vec4 o_FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_skyboxColor[2];
uniform samplerCube u_skyboxNormals[2];
//u_type - defines what kind of skybox is being rendered
uniform int         u_type;
uniform vec3        u_lightDir;
uniform float       u_ambientDay;
uniform float       u_ambientNight;

const int   SKYBOX_TYPE_HILLS_NEAR = 0;
const int   SKYBOX_TYPE_HILLS_FAR =  1;
const float MAX_DESATURATING_VALUE = 0.5;

@include desaturationFunc.ifs

void main()
{
	//hills skyboxes texcoords higher than 0.2 on Y are transparent, in that case just bypass any calculations
	if ( v_TexCoords.y > 0.2 )
	{
		o_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
		return;
	}

    vec4 sampledDiffuse = texture(u_skyboxColor[u_type], v_TexCoords);
    vec3 sampledNormal = texture(u_skyboxNormals[u_type], v_TexCoords).rgb * 2.0 - 1.0;
    sampledNormal = normalize(sampledNormal);

    @include shadingVariables.ifs

    float sunPositionAttenuation = clamp(u_lightDir.y * 10, 0.0, 1.0);
    float diffuseComponent = max(dot(sampledNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    //add a bit of red in the night time
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    diffuseColor = sampledDiffuse.rgb * diffuseComponent;
    resultColor = ambientColor + diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);

    //apply no desaturation if sun's Y coord is close to 0, or little if diffuse component is low enough
    float desaturatingValue = mix(0.0,
                                  MAX_DESATURATING_VALUE,
                                  min(1.0 * sunPositionAttenuation, diffuseComponent + 0.5));
    ext_desaturate(o_FragColor, desaturatingValue);
}
