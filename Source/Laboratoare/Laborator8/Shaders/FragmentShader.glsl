#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int mode_location;
uniform int angle;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	

	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 R = reflect(-L, world_normal);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = max(dot(world_normal, L), 0);;

	// TODO: compute specular light component
	float specular_light = max(pow(dot(world_normal, H), material_shininess), 0);


	// TODO: compute light
	
	float light = (material_kd + material_kd * diffuse_light + material_ks * specular_light);
	float d = distance(world_position, light_position);
	float atten = 1 / (0.01 * d * d + 0.02 * d + 1);



	// TODO: write pixel out color
	if(mode_location == 0) {
		out_color = vec4(light * object_color * ambient_light * atten, 1);
	} else {
		float cut_off = radians(angle);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);
 
		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);
		if (spot_light > cos(cut_off)) {	
			// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
			// se calculeaza atenuarea luminii
			out_color = vec4(spot_light * object_color * ambient_light * light_att_factor, 1);
		} else {
			out_color = vec4(0);
		}
	
	}
}