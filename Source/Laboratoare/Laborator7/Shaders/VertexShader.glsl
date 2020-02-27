#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	

	vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 N = normalize( mat3(Model) * v_normal );
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 R = reflect(-L, N);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = 0.25;


	// TODO: compute diffuse light component
	float diffuse_light = max(dot(N, L), 0);

	// TODO: compute specular light component
	float specular_light = pow(dot(R, V), material_shininess);
	


	if (diffuse_light > 0)
	{
		specular_light = max(pow(dot(N, H), material_shininess), 0);
	}

	// TODO: compute light

	float light = (material_kd + material_kd * diffuse_light + material_ks * specular_light);
	float d = distance(world_pos, light_position);
	float atten = 1 / (0.01 * d * d + 0.02 * d + 1);

	// TODO: send color light output to fragment shader
	color = light * object_color * ambient_light * atten;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
