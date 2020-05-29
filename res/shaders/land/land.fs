#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;

uniform sampler2D u_landDiffuse[2];
uniform sampler2D u_diffuseMixMap;
uniform sampler2D u_normalMap;
uniform vec3      u_lightDir;
uniform float     u_mapDimensionReciprocal;
uniform float     u_normalMapTilingReciprocal;
uniform bool      u_shadowEnable;
uniform float     u_ambientDay;
uniform float     u_ambientNight;

const vec3  NORMAL = vec3( 0.0, 1.0, 0.0 );
const float MAX_DESATURATING_VALUE = 0.5;
//VERTEX_NORMAL_INFLUENCE - define impact vertex normal has on the resulting normal
const float VERTEX_NORMAL_INFLUENCE = 1.0 / 6.0;
const float ONE_MINUS_VERTEX_NORMAL_INFLUENCE = 1.0 - VERTEX_NORMAL_INFLUENCE;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void main()
{
    float diffuseTextureMix = texture( u_diffuseMixMap, v_FragPos.xz * u_mapDimensionReciprocal + 0.5 ).r;
    vec4 sampledDiffuse = mix( texture( u_landDiffuse[0], v_TexCoords ),
                               texture( u_landDiffuse[1], v_TexCoords ),
                               diffuseTextureMix );

    @include shadingVariables.ifs

    //we need this to be swizzled (watch v_TNB comment in hills vertex shader)
    vec3 shadingNormal = texture( u_normalMap, v_FragPos.xz * u_normalMapTilingReciprocal ).xzy;
    shadingNormal.xyz -= vec3(0.5);
    shadingNormal = normalize( VERTEX_NORMAL_INFLUENCE * NORMAL +
                               ONE_MINUS_VERTEX_NORMAL_INFLUENCE * shadingNormal );

    float sunPositionAttenuation = clamp( u_lightDir.y * 10, 0.0, 1.0 );
    float diffuseComponent = max( dot( shadingNormal, u_lightDir ), 0.0 ) * sunPositionAttenuation;

    //no specular lighting for land

    ambientColor = mix( ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation );
    //add a bit of red in the night time
    ambientColor += nightAmbientColor * ( 1.0 - sunPositionAttenuation );

    if(u_shadowEnable)
    {
        int shadowMapIndex;
        vec3 projectedCoords;
        float luminosity;
        ext_calculateShadowMapIndexAndProjectedCoords( shadowMapIndex, projectedCoords );
        if( shadowMapIndex == 0 ) //use more precise algorithm for nearby fragments
		{
            luminosity = ext_calculateLuminosity5( shadowMapIndex, projectedCoords, u_bias );
		}
        else
		{
            luminosity = ext_calculateLuminosity3Lowp( shadowMapIndex, projectedCoords, u_bias );
		}

        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4( resultColor, sampledDiffuse.a );

        //apply no desaturation if there is no sun lit surface, or little if diffuse component is low enough
        float desaturatingValue = mix( 0.0,
                                       MAX_DESATURATING_VALUE,
                                       min( luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE ) );
        ext_desaturate( o_FragColor, desaturatingValue );
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4( resultColor, sampledDiffuse.a );
        //no desaturation here as it requires luminosity calculation
    }
}
