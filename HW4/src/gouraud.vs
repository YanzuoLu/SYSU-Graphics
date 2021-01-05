layout(location = 0) in vec3 vertex_model;
layout(location = 1) in vec3 normal_model;

out vec3 color;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat3 normal;
uniform vec3 light_world;

uniform vec3 light_mat;
uniform vec3 ambient_mat;
uniform vec3 diffuse_mat;
uniform vec3 specular_mat;
uniform float specular_power;

void main(){
	// Vertex position in clip space.
	gl_Position =  projection * view * model * vec4(vertex_model, 1);

	// Eye space vector from vertex to camera.
	vec3 vertex_eye = (view * model * vec4(vertex_model, 1)).xyz;
	vec3 eyedir_eye = vec3(0,0,0) - vertex_eye;

	// Eye space vector from vertex to light.
	vec3 light_eye = (view * vec4(light_world, 1)).xyz;
	vec3 lightdir_eye = light_eye + eyedir_eye;
	
	// Eye space normal vector of the vertex.
	vec3 normal_eye = (normal * vec3(normal_model)).xyz;

    // Diffuse angle calculation.
	vec3 n = normalize(normal_eye); // normal
	vec3 l = normalize(lightdir_eye); // to light
    float diffuse_angle = clamp(dot(n, l), 0, 1);
	
    // Specular angle calculation
	vec3 e = normalize(eyedir_eye); // to eye
	vec3 r = reflect(-l, n); // light reflection
    float specular_angle = clamp(dot(e, r), 0, 1);
	
    vec3 ambient = ambient_mat;
    vec3 diffuse = diffuse_mat * light_mat * diffuse_angle;
    vec3 specular = specular_mat * light_mat * pow(specular_angle, specular_power);
    
    color = ambient + diffuse + specular;
}