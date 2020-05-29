#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;
in mat3  v_TNB;
in vec2  v_TexCoords;
in float v_TerrainTypeMix;
in float v_SpecularComponent;

uniform sampler2D u_landDiffuse[2];
uniform sampler2D u_hillsDiffuse;
uniform sampler2D u_hillsSpecular;
uniform sampler2D u_diffuseMixMap;
uniform sampler2D u_normalMap;
uniform int       u_textureTilingDimension;
uniform float     u_normalMapTilingReciprocal;
uniform float     u_mapDimensionReciprocal;
//u_maxHillHeight - used in createCanyonRings function
uniform float     u_maxHillHeight;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
//u_debugRenderMode - project temporal flag to bypass all other shading
uniform bool      u_debugRenderMode;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform vec3      u_viewPosition;

const vec3  NORMAL = vec3( 0.0, 1.0, 0.0 );
//VERTEX_NORMAL_INFLUENCE_HILL(LAND) - define impact vertex normal has on the resulting normal
const float VERTEX_NORMAL_INFLUENCE_HILL = 1.0 / 3.0;
const float ONE_MINUS_VERTEX_NORMAL_INFLUENCE_HILL = 1.0 - VERTEX_NORMAL_INFLUENCE_HILL;
const float VERTEX_NORMAL_INFLUENCE_LAND = 1.0 / 6.0;
const float ONE_MINUS_VERTEX_NORMAL_INFLUENCE_LAND = 1.0 - VERTEX_NORMAL_INFLUENCE_LAND;
const float MAX_DESATURATING_VALUE_HILL = 0.5;
const float MAX_DESATURATING_VALUE_LAND = 0.5;
const float MIN_CANYON_CIRCLE_HEIGHT = 2.5;
const int   NUM_CANYON_CIRCLES = 12;

@include shadowSampling.ifs
@include desaturationFunc.ifs

/*
adds a bright color rings, which resembles canyon rings (different rock material layers)
i.e. like in the Grand Canyon. Adds a bit of visual diversity to the hills
*/
void createCanyonRings( inout vec4 fragColor,
                        vec3 circleColor,
                        float minHeight,
                        float maxHeight,
                        int numCircles,
                        int randomMultiplier,
                        float normalY )
{
    //intentionally left in range[0;1]
    vec2 distribution = texture( u_diffuseMixMap, v_FragPos.xz * u_mapDimensionReciprocal + 0.5 ).rg;
    float[2] canyonCircleOffset = { distribution.x, distribution.y };
    float circlesStep = ( maxHeight - minHeight ) / numCircles;
    //the more vertical oriented surface is - the more color intense "ring" we get
    vec3 canyonColor = circleColor * ( 1.0 - normalY );
    float influenceStep = 0.1 / ( numCircles - 1 );
    for( int i = 0; i < numCircles; i++ )
    {
        //potentionally higher circles should also be more color intense
        float influence = 1.0 + i * influenceStep;
        //denser the circle == farther from others (higher value == more visually thin)
        float density = 3.0 + influence;
        //the farther fragment Y coord to a circle Y coord - the more it being diminished
        float localityDamping = v_FragPos.y - ( minHeight + i * circlesStep );
        float offset = canyonCircleOffset[i%2] * randomMultiplier;
        float attenuation = clamp( abs( density * localityDamping + offset ), 0.0, influence );
        fragColor.rgb += canyonColor * ( influence - attenuation );
    }
}

void main()
{
    if(u_debugRenderMode)
	{
        o_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	}
    else
    {
        float diffuseTextureMix = texture( u_diffuseMixMap, v_FragPos.xz * u_mapDimensionReciprocal + 0.5 ).r;
        /*
        clamping in vertex shader gives smoother overall result, but makes land/hill
        seams visible, which is not desired
        mix range stands for: 0.0 == fully land texture, 1.0 == fully hill texture
        */
        float terrainTypeMixClamped = clamp( v_TerrainTypeMix, 0.0, 1.0 );

		//diffuse color calculation
		vec4 sampledDiffuse = texture( u_hillsDiffuse, v_TexCoords );
		sampledDiffuse = mix( sampledDiffuse, sampledDiffuse * vec4( 0.85, 0.83, 0.82, 1.0 ), diffuseTextureMix );
		if( terrainTypeMixClamped <= 0.99 )
		{
			vec2 landTexCoords = v_TexCoords * u_textureTilingDimension;
			sampledDiffuse = mix( mix( texture( u_landDiffuse[0], landTexCoords ),
			                           texture( u_landDiffuse[1], landTexCoords ),
			                           diffuseTextureMix ),
			                      sampledDiffuse,
			                      terrainTypeMixClamped );
		}

        //no specular lighting for land
        vec4 sampledSpecular = mix( vec4(0.0), texture( u_hillsSpecular, v_TexCoords ), terrainTypeMixClamped );

        @include shadingVariables.ifs

        //we need this to be swizzled (watch v_TNB comment in vertex shader)
        vec3 sampledNormal = texture( u_normalMap, v_FragPos.xz * u_normalMapTilingReciprocal ).xzy;
        sampledNormal -= vec3(0.5);
        sampledNormal = normalize(sampledNormal);

        vec3 shadingNormalLand = normalize( VERTEX_NORMAL_INFLUENCE_LAND * NORMAL +
                                            ONE_MINUS_VERTEX_NORMAL_INFLUENCE_LAND * sampledNormal );
        vec3 shadingNormalHill = normalize( VERTEX_NORMAL_INFLUENCE_HILL * v_Normal +
                                            ONE_MINUS_VERTEX_NORMAL_INFLUENCE_HILL * ( v_TNB * sampledNormal ) );

        float diffuseComponentHill = max( dot( shadingNormalHill, u_lightDir ), 0.0 );
        float diffuseComponentLand = max( dot( shadingNormalLand, u_lightDir ), 0.0 );
        float sunPositionAttenuation = clamp( u_lightDir.y * 10, 0.0, 1.0 );
        float diffuseComponent = mix( diffuseComponentLand, diffuseComponentHill, terrainTypeMixClamped ) * sunPositionAttenuation;

        ambientColor = mix( ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation );
        //add a bit of red in the night time
        ambientColor += nightAmbientColor * ( 1.0 - sunPositionAttenuation );

        if(u_shadowEnable)
        {
            int shadowMapIndex;
            vec3 projectedCoords;
            float luminosity;
            ext_calculateShadowMapIndexAndProjectedCoords( shadowMapIndex, projectedCoords );
            //tweak bias to adjust it with light direction
            float bias = u_bias * ( 2.0 - 1.0 * dot( u_lightDir, shadingNormalHill ) );

            if ( shadowMapIndex == 0 ) //use more precise algorithm for nearby fragments
			{
                luminosity = ext_calculateLuminosity5( shadowMapIndex, projectedCoords, bias );
			}
            else
			{
                luminosity = ext_calculateLuminosity3Lowp( shadowMapIndex, projectedCoords, bias );
			}

            diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
            //make sure that shadowed fragment do not have any specular reflection
            float specularLuminosityInfluence = 1.0 - ( ( 1.0 - luminosity ) * SHADOW_INFLUENCE_RECIPROCAL );
            specularColor = specularLuminosityInfluence * v_SpecularComponent * sampledSpecular.rgb * sunPositionAttenuation;
            resultColor = ambientColor + diffuseColor + specularColor;
            o_FragColor = vec4( resultColor, sampledDiffuse.a );

            //apply no desaturation if there is no sun lit surface, or little if diffuse component is low enough
            float desaturatingValue = mix( 0.0,
                                           mix( MAX_DESATURATING_VALUE_LAND, MAX_DESATURATING_VALUE_HILL, terrainTypeMixClamped ),
                                           min( luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE ) );
            ext_desaturate( o_FragColor, desaturatingValue );
        }
        else
        {
            diffuseColor = sampledDiffuse.rgb * diffuseComponent;
            specularColor = v_SpecularComponent * sampledSpecular.rgb * sunPositionAttenuation;
            resultColor = ambientColor + diffuseColor + specularColor;
            o_FragColor = vec4( resultColor, sampledDiffuse.a );
            //no desaturation here as it requires luminosity calculation
        }

        createCanyonRings( o_FragColor,
                           ambientColor + diffuseColor,
                           MIN_CANYON_CIRCLE_HEIGHT,
                           u_maxHillHeight,
                           NUM_CANYON_CIRCLES,
                           4,
                           shadingNormalHill.y );
    }
}
