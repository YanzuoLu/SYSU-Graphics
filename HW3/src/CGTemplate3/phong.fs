in vec3 normal_eye;
in vec3 eyedir_eye;
in vec3 lightdir_eye;

out vec3 color;

uniform vec3 light_mat;
uniform vec3 ambient_mat;
uniform vec3 diffuse_mat;
uniform vec3 specular_mat;
uniform float specular_power;

void main(){
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