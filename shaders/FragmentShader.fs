#version 330 core

// Interpolated values from the vertex shaders
//in vec2 UV;
//in vec3 Position_worldspace;
//in vec3 Normal_cameraspace;
//in vec3 EyeDirection_cameraspace;
//in vec3 LightDirection_cameraspace;
//in vec4 ShadowCoord;

// Ouput data
//out vec4 color;

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;


// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler; //diffuseTexture
uniform sampler2D shadowMap;

//uniform mat4 MV;
uniform vec3 lightPos;//lightPos
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.005;
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    
    return shadow;
}

void main()
{
    vec3 color = texture(myTextureSampler, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightPower = vec3(1.0);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightPower;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightPower;
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lightPower*lighting, 1.0);
}

//void main(){
//
//	// Light emission properties
//	// You probably want to put them as uniforms
//	vec3 LightColor = vec3(1,1,1);
//	float LightPower = 10.0f;
//
//	// Material properties
//	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
//	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
//	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
//
//	// Distance to the light
//	float distance = length( LightPosition_worldspace - Position_worldspace );
//
//	// Normal of the computed fragment, in camera space
//	vec3 n = normalize( Normal_cameraspace );
//	// Direction of the light (from the fragment to the light)
//	vec3 l = normalize( LightDirection_cameraspace );
//	// Cosine of the angle between the normal and the light direction,
//	// clamped above 0
//	//  - light is at the vertical of the triangle -> 1
//	//  - light is perpendicular to the triangle -> 0
//	//  - light is behind the triangle -> 0
//	float cosTheta = clamp( dot( n,l ), 0,1 );
//
//	// Eye vector (towards the camera)
//	vec3 E = normalize(EyeDirection_cameraspace);
//	// Direction in which the triangle reflects the light
//	vec3 R = reflect(-l,n);
//	// Cosine of the angle between the Eye vector and the Reflect vector,
//	// clamped to 0
//	//  - Looking into the reflection -> 1
//	//  - Looking elsewhere -> < 1
//	float cosAlpha = clamp( dot( E,R ), 0,1 );
//
//    float visibility=1.0;
//    if ( (texture(shadowMap, ShadowCoord.xy)).z  <  ShadowCoord.z){
//        visibility = 0.5;
//    }
////
////    // Fixed bias, or...
////    float bias = 0.005;
//////
//////     ...variable bias
//////     float bias = 0.005*tan(acos(cosTheta));
//////     bias = clamp(bias, 0,0.01);
//////////
////    // Sample the shadow map 4 times
////    for (int i=0;i<4;i++){
////        // use either :
////        //  - Always the same samples.
////        //    Gives a fixed pattern in the shadow, but no noise
////        int index = i;
////        //  - A random sample, based on the pixel's screen location.
////        //    No banding, but the shadow moves with the camera, which looks weird.
////        // int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
////        //  - A random sample, based on the pixel's position in world space.
////        //    The position is rounded to the millimeter to avoid too much aliasing
////        // int index = int(16.0*random(floor(Position_worldspace.xyz*1000.0), i))%16;
////        
////        // being fully in the shadow will eat up 4*0.2 = 0.8
////        // 0.2 potentially remain, which is quite dark.
////        visibility -= 0.2*(1.0-texture( shadowMap, vec3(ShadowCoord.xy + poissonDisk[index]/700.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
////    }
//    
//	color.rgb =
//		// Ambient : simulates indirect lighting
//		MaterialAmbientColor +
//		// Diffuse : "color" of the object
//		visibility * MaterialDiffuseColor * LightColor * LightPower * cosTheta / distance +
//		// Specular : reflective highlight, like a mirror
//		visibility * MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / distance;
//    
//	color.a = 1.0f;
//}
