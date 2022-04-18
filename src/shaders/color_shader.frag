#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
  
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

vec3 calculateDiff(vec3 light_pos, vec3 light_color, vec3 frag_pos, vec3 normal); 

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 diffuse = calculateDiff(lightPos, lightColor, FragPos, norm);

    // specular
    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = objectColor * (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}

vec3 calculateDiff(vec3 light_pos, vec3 light_color, vec3 frag_pos, vec3 normal) {
    vec3 lightDir = normalize(light_pos - frag_pos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    return diffuse;
}