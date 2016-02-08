#include "psinput.hlsl"
#include "camera.hlsl"
#include "material.hlsl"
#include "lights.hlsl"

float3 computePointLight(float3 materialColor, Light light, PSInput input)
{
    float3  normal = normalize(input.normal);
    float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

    // Compute ambient color
    float3  ambient = light.ambient * materialColor; // TO-DO: Use ka

    // Compute diffuse color
    float3  lightDirection = normalize(light.position - position);
    float   diff = max(dot(normal, lightDirection), 0.0f);
    float3  diffuse = light.diffuse * diff * materialColor; // TO-DO: Use kd

    // Compute specular color
    float3  viewDir = normalize(camera.position - position);
    float3  reflectionDirection = reflect(-lightDirection, normal);
    float   spec = pow(max(dot(viewDir, reflectionDirection), 0.0), 32.0f/* TO-DO: Replace by shininess */);
    float3  specular = light.specular * spec * materialColor; // TO-DO: Use ks

      // TO-DO: Compute attenuation
	float3	lightToPosition = position - light.position;
	float	distance = sqrtf(dot(lightToPosition, lightToPosition));
	float	attenuationFactor = 1 / (light.constantAttenuation + light.linearAttenuation * distance + light.quadraticAttenuation * pow(distance, 2.0));
    return (ambient + attenuationFactor * ( diffuse + specular)); // TO-DO add Ke
}


float smoothStep(float min, float max, float x)
{
	if (x < min)
	{
		return (0.0f);
	}
	else if (x >= max)
	{
		return (1.0f);
	}
	else
	{
		return (-2.0f * pow((x - min) / (max - min), 3.0) + 3.0f * pow((x - min) / (max - min), 2));
	}

}

float3 computeSpotLigth(float3 materialColor, Light light, PSInput input)
{
	float3  position = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);

	float3  lightDirection = normalize(light.position - position);

	float cos = dot(lightDirection, light.direction);

	float smoothstep = smoothStep(cosf(light.outerCutOff), cosf(light.cutOff), cos);
	return computePointLight(materialColor, light, input) * smoothstep;
}



float3 computeLight(float3 materialColor, Light light, PSInput input)
{
	// TO-DO: Manage others types
	if (light.type == 0) // POINT
		return (computePointLight(materialColor, light, input));
	else if (light.type == 2)
		return (computeSpotLigth(materialColor, light, input));
	else
		return (float3(0.0f, 0.0f, 0.0f));
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3  materialColor = getMaterialColor(input.normal);
    float3  color;

    for (uint i = 0; i < camera.nb_lights; i++)
        color += computeLight(materialColor, lights[i], input);

    return (float4(color, 1.0f));
}
