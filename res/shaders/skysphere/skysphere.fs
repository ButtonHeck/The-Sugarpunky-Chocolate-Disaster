#version 450

out vec4 o_FragColor;

uniform sampler2D u_theSunAmbientLightingDiffuse;
uniform sampler2D u_starsDiffuse;
uniform sampler2D u_cloudsDiffuse;
uniform vec3      u_lightDir;
//u_type - defines what kind of skysphere is being rendered
uniform int       u_type;

const int   SKYSPHERE_TYPE_AMBIENT_LIGHTING = 0;
const int   SKYSPHERE_TYPE_STARS = 1;
const int	SKYSPHERE_TYPE_CLOUDS = 2;
const float STARS_RGBA_DAMP_FACTOR = 1.7;
const float CLOUDS_RGBA_DAMP_FACTOR = 1.3;
/*
STARS_TEXTURE_Y_DAMP_FACTOR - defines offset from lower and upper texture Y coordinate
from which no mix with black color should occure
*/
const float STARS_TEXTURE_Y_DAMP_FACTOR = 0.05;

in vec2 v_TexCoords;
in vec3 v_Normal;

void main()
{
    vec3 normal = normalize(v_Normal);
    float sunPositionAttenuation = clamp((u_lightDir.y + 0.05) * 8, 0.0, 1.0);

    if (u_type == SKYSPHERE_TYPE_AMBIENT_LIGHTING)
    {
        //mimic more realistic ambient lighting transition during dusk/dawn
        float normalInfluence = mix(pow((1.0 - normal.y), 1.2),
                                    max(dot(u_lightDir, normal), 0.0),
                                    sunPositionAttenuation);
        o_FragColor = texture(u_theSunAmbientLightingDiffuse, v_TexCoords) * normalInfluence * sunPositionAttenuation;
    }
    else if (u_type == SKYSPHERE_TYPE_STARS)
    {
        /*
        the closer texture Y coordinate to 1.0 or 0.0 the more black it should be
        to hide texturing artefacts at sphere polars
        */
        float textureMix = 1.0; //no mix with black color by default
        if (v_TexCoords.y < STARS_TEXTURE_Y_DAMP_FACTOR)
            textureMix = v_TexCoords.y / STARS_TEXTURE_Y_DAMP_FACTOR;
        else if (v_TexCoords.y > (1.0 - STARS_TEXTURE_Y_DAMP_FACTOR))
            textureMix = (1.0 - v_TexCoords.y) / STARS_TEXTURE_Y_DAMP_FACTOR;

        /*
        apply some preprocessing to diminish entire brightness of the texture a bit and emphasize contrast,
        secondly make sure no stars are visible when the sun is visible
        */
        o_FragColor = pow(mix(vec4(0.0), texture(u_starsDiffuse, v_TexCoords), textureMix), vec4(STARS_RGBA_DAMP_FACTOR))
                      * (1.0 - sunPositionAttenuation);
    }
	else
	{
		float sunPositionAmplification = 1.0 - clamp((u_lightDir.y + 0.2) * 4, 0.0, 1.0);
		vec4 sampledClouds = texture(u_cloudsDiffuse, v_TexCoords);
        //make clouds black during day time
        o_FragColor = pow(mix(vec4(0.0, 0.0, 0.0, sampledClouds.a * 2), sampledClouds, sunPositionAmplification), vec4(CLOUDS_RGBA_DAMP_FACTOR));
	}
}
