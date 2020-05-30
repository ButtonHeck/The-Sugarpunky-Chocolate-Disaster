#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec2  v_TexCoords;
in float v_ShoreLandMix;
in float v_ShoreUnderwaterMix;
in vec3  v_Normal;

uniform sampler2D u_landDiffuse[2];
uniform sampler2D u_shoreDiffuse;
uniform sampler2D u_diffuseMixMap;
uniform sampler2D u_underwaterDiffuse;
uniform sampler2D u_bottomReliefDiffuse;
uniform float     u_normalMapTilingReciprocal;
uniform float     u_mapDimensionReciprocal;
uniform sampler2D u_normalMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
//u_debugRenderMode - project temporal flag to bypass all other shading
uniform bool      u_debugRenderMode;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform vec3      u_viewPosition;

const vec3  NORMAL = vec3( 0.0, 1.0, 0.0 );
const float MAX_DESATURATING_VALUE_LAND = 0.5;
const float MAX_DESATURATING_VALUE_SHORE = 0.7;
const float MAX_DESATURATING_VALUE_UNDERWATER = 0.3;
//VERTEX_NORMAL_INFLUENCE_SHORE(LAND) - define impact vertex normal has on the resulting normal
const float VERTEX_NORMAL_INFLUENCE_SHORE = 2.0 / 3.0;
const float ONE_MINUS_VERTEX_NORMAL_INFLUENCE_SHORE = 1.0 - VERTEX_NORMAL_INFLUENCE_SHORE;
const float VERTEX_NORMAL_INFLUENCE_LAND = 1.0 / 6.0;
const float ONE_MINUS_VERTEX_NORMAL_INFLUENCE_LAND = 1.0 - VERTEX_NORMAL_INFLUENCE_LAND;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void main()
{
    if(u_debugRenderMode)
	{
        o_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	}
    else
    {
		vec2 mapSpaceTextureCoords = v_FragPos.xz * u_mapDimensionReciprocal + 0.5;
        float diffuseTextureMix = texture( u_diffuseMixMap, mapSpaceTextureCoords ).r;
        /*
        clamping this value in fragment shader gives more visually pleasing result
        mix range stands for: 0.0 == fully shore texture, 1.0 == fully land texture
        */
        float terrainTypeMixClamped = clamp( v_ShoreLandMix, 0.0, 1.0 );

		vec4 sampledDiffuse = texture( u_shoreDiffuse, v_TexCoords );
		sampledDiffuse = mix( sampledDiffuse, sampledDiffuse * vec4( 0.94, 0.88, 0.8, 1.0 ), diffuseTextureMix );
		if( terrainTypeMixClamped >= 0.01 )
		{
			sampledDiffuse = mix( sampledDiffuse,
			                      mix( texture( u_landDiffuse[0], v_TexCoords ),
								 	   texture( u_landDiffuse[1], v_TexCoords ),
								 	   diffuseTextureMix ),
								  terrainTypeMixClamped );
		}
		if( v_ShoreUnderwaterMix >= 0.01 )
		{
			sampledDiffuse = mix( sampledDiffuse, 
								  texture( u_underwaterDiffuse, v_TexCoords ), 
								  v_ShoreUnderwaterMix );
		}

        //no specular lighting for shore and land

        @include shadingVariables.ifs

        //we need this to be swizzled (watch v_TNB comment in hills vertex shader)
        vec3 sampledNormal = texture( u_normalMap, v_FragPos.xz * u_normalMapTilingReciprocal ).xzy;
        sampledNormal.xyz -= vec3(0.5);
		vec3 sampledNormalUnderwater = sampledNormal;
        sampledNormal = normalize(sampledNormal);
		sampledNormalUnderwater = normalize( NORMAL + sampledNormalUnderwater );

        vec3 shadingNormalLand = normalize( VERTEX_NORMAL_INFLUENCE_LAND * NORMAL +
                                            ONE_MINUS_VERTEX_NORMAL_INFLUENCE_LAND * sampledNormal );
        vec3 shadingNormalShore = normalize( VERTEX_NORMAL_INFLUENCE_SHORE * v_Normal +
                                             ONE_MINUS_VERTEX_NORMAL_INFLUENCE_SHORE * sampledNormal );

		//luminosity calculation
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
            luminosity = ext_calculateLuminosity3( shadowMapIndex, projectedCoords, u_bias * ( shadowMapIndex + 1 ) );
		}

        float diffuseComponentShore = clamp( dot( shadingNormalShore, u_lightDir ), 0.0, luminosity );
        float diffuseComponentLand = clamp( dot( shadingNormalLand, u_lightDir ), 0.0, luminosity );
        float sunPositionAttenuation = clamp( u_lightDir.y * 10, 0.0, 1.0 );

		//underwater stuff
		float diffuseComponentUnderwater = max( dot( sampledNormalUnderwater, u_lightDir ), 0.0 );
		float reliefAttenuationUnderwater = 1.0 - texture( u_bottomReliefDiffuse, mapSpaceTextureCoords ).r;

        float diffuseComponent = mix( diffuseComponentShore, diffuseComponentLand, terrainTypeMixClamped );
		diffuseComponent = mix( diffuseComponent, diffuseComponentUnderwater * reliefAttenuationUnderwater, v_ShoreUnderwaterMix )
						   * sunPositionAttenuation;

        ambientColor = mix( ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation );
        //add a bit of red in the night time
        ambientColor += nightAmbientColor * ( 1.0 - sunPositionAttenuation );

        if(u_shadowEnable)
        {
            diffuseColor = mix( luminosity, 1.0, v_ShoreUnderwaterMix ) * sampledDiffuse.rgb * diffuseComponent;
            resultColor = ambientColor + diffuseColor;
            o_FragColor = vec4( resultColor, sampledDiffuse.a );

            //apply no desaturation if there is no sun lit surface, or little if diffuse component is low enough
            float desaturatingValue = mix( 0.0,
                                           mix( MAX_DESATURATING_VALUE_SHORE, MAX_DESATURATING_VALUE_LAND, terrainTypeMixClamped ),
                                           min( luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE ) );
			float desaturatingValueUnderwater = mix( 0.0, MAX_DESATURATING_VALUE_UNDERWATER, diffuseComponentUnderwater );
			desaturatingValue = mix( desaturatingValue, desaturatingValueUnderwater, v_ShoreUnderwaterMix );
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
}
