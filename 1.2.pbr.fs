//"lighting textured fragment shader"
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2DArray texels;
uniform int onOffTest;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
uint bitfieldExtract(uint val, int off, int size) {
    uint mask = uint((1 << size) - 1);
    return uint(val >> off) & mask;
}
void main()
{		
    vec3 albedo     = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao        = texture(aoMap, TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    //FragColor = vec4(color * onOffTest, 1);
	//FragColor = vec4(texture(albedoMap, (gl_FragCoord.xy-vec2(0.5,0.5)) / vec2(1280.0,720.0)).rgb * onOffTest, 1);
	//FragColor = vec4((texture(albedoMap, (gl_FragCoord.xy) / vec2(1280.0,720.0)).rgb * onOffTest) + (color * pow(onOffTest - 1, 2)), 1);
	uint thingInt = uint(texelFetch(texels, ivec3(1, 0, 0),0).r * 256.0);        //use for access with ints
	//uint thingInt = uint(texture(texels, vec3(1.5/10.0, 0, 0)).r * 256.0);     //use for normalized access (floats, [0 to 1])
	//uniform uint closenesses[8];
	//uniform uint stepSize;
	//uint depthBytes[8];
	//uint mid[5];
	//uint guess[5];
	//for(int i = 0; i < 8; i++){
	//	depthBytes[i] = uint(texelFetch(texels, ivec3(x, y, 5 * dept + closenesses[i] ),0).r * 256.0)
	//}
	//for(int i = 0; i < 5; i++){
	// mid[i] = 127 + (dept > 0 ? (bitfieldExtract(depthBytes[0], i, 1) == 1 ? 64 : -64) : 0)
	//			    + (dept > 1 ? (bitfieldExtract(depthBytes[1], i, 1) == 1 ? 32 : -32) : 0)
	//		        + (dept > 2 ? (bitfieldExtract(depthBytes[2], i, 1) == 1 ? 16 : -16) : 0)
	//		        + (dept > 3 ? (bitfieldExtract(depthBytes[3], i, 1) == 1 ? 8 : -8) : 0)
	//		        + (dept > 4 ? (bitfieldExtract(depthBytes[4], i, 1) == 1 ? 4 : -4) : 0)
	//		        + (dept > 5 ? (bitfieldExtract(depthBytes[5], i, 1) == 1 ? 2 : -2) : 0)
	//		        + (dept > 6 ? (bitfieldExtract(depthBytes[6], i, 1) == 1 ? 1 : -1) : 0)
	//}
	//
	//for(int i = 0; i < 5; i++){
	//	guess[i] = mid[i] - stepSize;
	//}
	//
	//
	//depthByte = uint(texelFetch(texels, ivec3(x, y, 5 * depth + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0) ----------------SPECIFIES DEPTH AND CLOSENESS
	//uint thingEx = bitfieldExtract(uint(127), 0, 1);
	//depthBit = bitfieldExtract(depthByte, [R,G,B,M,Ro](starts at 0 index), 1); -----------------SPECIFIES PBR CHANNEL
	// IN THEORY
	// depth1 = uint(texelFetch(texels, ivec3(x, y, closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth2 = uint(texelFetch(texels, ivec3(x, y, 5 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth3 = uint(texelFetch(texels, ivec3(x, y, 10 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth4 = uint(texelFetch(texels, ivec3(x, y, 15 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth5 = uint(texelFetch(texels, ivec3(x, y, 20 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth6 = uint(texelFetch(texels, ivec3(x, y, 25 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth7 = uint(texelFetch(texels, ivec3(x, y, 30 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// depth8 = uint(texelFetch(texels, ivec3(x, y, 35 + closeness[1,2,3,4,5](starts at 0 index) ),0).r * 256.0)
	// Red = 127 + (dept > 0 ? (bitfieldExtract(depth1, 0, 1) == 1 ? 64 : -64)
	//			 + (dept > 1 ? (bitfieldExtract(depth2, 0, 1) == 1 ? 32 : -32) : 0)
	//		     + (dept > 2 ? (bitfieldExtract(depth3, 0, 1) == 1 ? 16 : -16) : 0)
	//		     + (dept > 3 ? (bitfieldExtract(depth4, 0, 1) == 1 ? 8 : -8) : 0)
	//		     + (dept > 4 ? (bitfieldExtract(depth5, 0, 1) == 1 ? 4 : -4) : 0)
	//		     + (dept > 5 ? (bitfieldExtract(depth6, 0, 1) == 1 ? 2 : -2) : 0)
	//		     + (dept > 6 ? (bitfieldExtract(depth7, 0, 1) == 1 ? 1 : -1) : 0)
	uint dept = 7;
	//uint thingEx = (bitfieldExtract(thingInt, 1, 1) * 127) * (dept > 4 ? uint(bitfieldExtract(thingInt, 1, 1)) : 0);
	uint thingEx = 127;
	float thing = float(thingEx) / 255.0;
	//float thing = float(thingInt) / 255.0;
	//float thing = float(uint(127)) / 255.0;
	FragColor = vec4(texture(albedoMap, (gl_FragCoord.xy) / vec2(1280.0,720.0)).rgb * onOffTest + vec3(thing, thing, thing) * pow(onOffTest - 1, 2), 1);

}