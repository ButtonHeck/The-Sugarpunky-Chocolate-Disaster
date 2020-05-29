#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;

uniform sampler2D   u_specularTexture;
uniform sampler2D   u_normalMap;
uniform sampler2D   u_dudvMap;
uniform sampler2D   u_reflectionMap;
uniform sampler2D   u_refractionMap;
uniform sampler2D   u_refractionDepthMap;
uniform float       u_normalMapTilingReciprocal;
uniform vec3        u_lightDir;
uniform vec3        u_viewPosition;
//u_debugRenderMode - project temporal flag to bypass all other shading
uniform bool        u_debugRenderMode;
uniform float       u_ambientDay;
uniform float       u_ambientNight;
uniform float       u_dudvMoveOffset;
uniform float       u_screenWidth;
uniform float       u_screenHeight;
uniform float       u_near;
uniform float       u_far;

const vec3  NORMAL = vec3( 0.0, 1.0, 0.0 );
const float MAX_DESATURATING_VALUE = 0.5;
const float SPECULAR_SHININESS = 16.0;
const vec3  KISSEL_COLOR = ( vec3( 133.75, 18.75, 6.25 ) / 255.0 ) ;
const float KISSEL_ALPHA_MIN = 0.6;
const float REFLECTION_MIX_DAY = 0.2;
const float REFLECTION_MIX_NIGHT = 0.4;
const float DUDV_INFLUENCE = 0.008;
const float TEXTURE_TILING = 1.0 / 8.0;
const vec2  SCREEN_SPACE_DIVISOR = vec2( u_screenWidth, u_screenHeight );

@include shadowSampling.ifs
@include desaturationFunc.ifs

/*
this function takes current fragment depth (which is not linear)
and returns a linear value representation of it
do not divide by u_far when using this function as it has already done here
*/
float linearizeDepth( float depth )
{
    float z = depth * 2.0 - 1.0; //map back to NDC
    return ( 2.0 * u_near ) / ( u_far + u_near - z * ( u_far - u_near ) ); // * u_far
}

vec3 calculateShadingNormal( vec2 texCoords )
{
	//we need this to be swizzled (watch v_TNB comment in hills vertex shader) + use DUDV texCoords offset
    vec3 shadingNormal = texture( u_normalMap, texCoords ).xzy + NORMAL;
    shadingNormal.xz *= 2.0;
    shadingNormal.xz -= 1.0;
    return normalize(shadingNormal);
}

void main()
{
    if(u_debugRenderMode)
	{
        o_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	}
    else
    {
		vec3 sampledDiffuse = KISSEL_COLOR;
        @include shadingVariables.ifs

        vec2 fragPosTexCoords = v_FragPos.xz * TEXTURE_TILING;
        vec2 dudvTextureOffset = ( texture( u_dudvMap, fragPosTexCoords + vec2(u_dudvMoveOffset) ).rg * 2.0 - 1.0 )
                                  * DUDV_INFLUENCE;

        vec3 shadingNormal = calculateShadingNormal( fragPosTexCoords + dudvTextureOffset );

        float sunPositionAttenuation = clamp( ( u_lightDir.y - 0.02 ) * 10, 0.0, 1.0 );

        //diffuse
        float diffuseComponent = max( dot( shadingNormal, u_lightDir ), 0.0 ) * sunPositionAttenuation;

        ambientColor = mix( ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation );
        //add a bit of red in the night time
        ambientColor += nightAmbientColor * ( 1.0 - sunPositionAttenuation );

        //specular
        vec3 lightDirectionReflected = reflect( -u_lightDir, shadingNormal );
		vec3 viewDirection = normalize( u_viewPosition - v_FragPos );
        float specularComponent = pow( max( dot( lightDirectionReflected, viewDirection ), 0.0 ), SPECULAR_SHININESS );
        vec3 sampledSpecular = texture( u_specularTexture, fragPosTexCoords.yx + dudvTextureOffset ).rgb * sunPositionAttenuation;

		//world reflection/refraction texture coordinates
		vec2 screenSpaceTexCoordsReflection = gl_FragCoord.xy / SCREEN_SPACE_DIVISOR;
        vec2 screenSpaceTexCoordsRefraction = screenSpaceTexCoordsReflection;
        //turn these coordinates upside down to make correct reflection texturing
        screenSpaceTexCoordsReflection.y = 1.0 - screenSpaceTexCoordsReflection.y;
		vec2 sampledWorldTexCoordsOffset = dudvTextureOffset + vec2( shadingNormal.xz ) * u_normalMapTilingReciprocal;
		//world reflection
        vec4 sampledWorldReflection = vec4( texture( u_reflectionMap, screenSpaceTexCoordsReflection + sampledWorldTexCoordsOffset ).rgb, 1.0 );
        //world refraction
        vec4 sampledWorldRefraction = vec4( texture( u_refractionMap, screenSpaceTexCoordsRefraction + sampledWorldTexCoordsOffset ).rgb, 1.0 );

		//detect kissel "deepness" at a fragment's position
        float refractionDepth = linearizeDepth( texture( u_refractionDepthMap, screenSpaceTexCoordsRefraction ).r );
        float currentFragDepth = linearizeDepth( gl_FragCoord.z );
        //emphasize this for smooth transition with shore, higher == sharper transition
        float kisselShoreDepth = min( ( refractionDepth - currentFragDepth ) * 192, 1.0 );

        //shadowing
        int shadowMapIndex;
        vec3 projectedCoords;
        ext_calculateShadowMapIndexAndProjectedCoords( shadowMapIndex, projectedCoords );
        //apply DUDV offset here to make shadows on kissel surface more natural
        projectedCoords.xy += dudvTextureOffset * kisselShoreDepth * 0.3;
        //no LOD here
        float luminosity = ext_calculateLuminosity3( shadowMapIndex, projectedCoords, u_bias );

        diffuseColor = luminosity * sampledDiffuse * diffuseComponent;
        //make sure that shadowed fragment do not have any specular reflection
        float specularLuminosityInfluence = 1.0 - ( ( 1.0 - luminosity ) * SHADOW_INFLUENCE_RECIPROCAL );
        specularColor = specularLuminosityInfluence * specularComponent * sampledSpecular * 0.3;
        resultColor = ambientColor + diffuseColor + specularColor;

		//fresnel (!ain't sure whether it is done correct!)
        float perpendicularity = max( dot( viewDirection, shadingNormal ), 0.0 );
        //the more perpendicular a fragment's normal to the camera - the less it should be opaque
        float opacity = max( 1.0 - perpendicularity, KISSEL_ALPHA_MIN );
        /*
        the more opaque a fragment is - the more reflection(limited) prevails over refraction
        also make reflection brighter at night time than at day time
        */
        float worldReflectionMix = opacity * mix( REFLECTION_MIX_NIGHT, REFLECTION_MIX_DAY, sunPositionAttenuation );
        vec3 rgbCombined = resultColor + mix( sampledWorldRefraction.rgb, sampledWorldReflection.rgb, worldReflectionMix ) * 0.25;
        o_FragColor = vec4( rgbCombined, mix( kisselShoreDepth, 1.0, kisselShoreDepth ) );

        //apply no desaturation if there is no sun lit surface, or little if diffuse component is low enough
        float desaturatingValue = mix( 0.0,
                                       MAX_DESATURATING_VALUE,
                                       min( luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE ) );
        ext_desaturate( o_FragColor, desaturatingValue );
    }
}
