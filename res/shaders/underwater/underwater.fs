#version 450

out vec4 o_FragColor;

in vec2  v_FragPosXZ;
in vec2  v_TexCoords;

uniform sampler2D u_underwaterDiffuse;
uniform sampler2D u_bottomReliefDiffuse;
uniform sampler2D u_normalMap;
uniform float     u_normalMapTilingReciprocal;
uniform float     u_mapDimensionReciprocal;
uniform vec3      u_lightDir;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform bool	  u_useDesaturation;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const float MAX_DESATURATING_VALUE = 0.3;

@include desaturationFunc.ifs

void main()
{
    vec4 sampledDiffuse = texture(u_underwaterDiffuse, v_TexCoords);

    @include shadingVariables.ifs

    vec3 sampledNormal = texture(u_normalMap, v_FragPosXZ * u_normalMapTilingReciprocal).xzy;
    sampledNormal.xyz -= vec3(0.5);
    sampledNormal = normalize(NORMAL + sampledNormal);

    float sunPositionAttenuation = clamp(u_lightDir.y * 10, 0.0, 1.0);
    float diffuseComponent = max(dot(sampledNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    //add a bit of red in the night time
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    //make underwater surface the more darker the closer it to the river kernel line (just a visual flavour)
    float reliefAttenuation = 1.0 - texture(u_bottomReliefDiffuse, v_FragPosXZ * u_mapDimensionReciprocal + 0.5).r;
    diffuseColor = sampledDiffuse.rgb * diffuseComponent * reliefAttenuation;
    resultColor = ambientColor + diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);

	if(u_useDesaturation)
	{
		//calculation of this value depends on how much diffuse have influence on a fragment's color
		float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, diffuseComponent);
		ext_desaturate(o_FragColor, desaturatingValue);
	}
}
