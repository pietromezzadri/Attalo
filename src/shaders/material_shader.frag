#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight 
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 1
  
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight[NUM_POINT_LIGHTS];
uniform SpotLight spotLight[NUM_SPOT_LIGHTS];

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{      	 
    // properties
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result;

    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(pointLight[i], normal, FragPos, viewDir);
    }
    for (int i = 0; i < NUM_SPOT_LIGHTS; i++)
    {
        result += calculateSpotLight(spotLight[i], normal, FragPos, viewDir);
    }
    FragColor  = vec4(result, 1.0);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff      = max(dot(normal, lightDir), 0.0);    
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{      
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    float diff      = max(dot(normal, lightDir), 0.0);    
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  

    diffuse  *= intensity;
    specular *= intensity;

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}  