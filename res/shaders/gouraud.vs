uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shinninnes;
uniform vec3 Ia;
uniform vec3 lightPosition;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 cameraPosition;

varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 Ip;

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;

	// Convert local normal to world space
	vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal;

    //3.3

    vec3 P = v_world_position;
    vec3 L = normalize(lightPosition - P);
    vec3 V = normalize(cameraPosition - P);
    vec3 N = normalize(v_world_normal);
    vec3 R = reflect(-L, N);    

    vec3 ambient = Ka* Ia;
    vec3 diffuse = Kd*clamp(dot(N,L), 0.0, 1.0)*Id;
    vec3 specular = Ks*pow(clamp(dot(R,V), 0.0, 1.0), shinninnes)*Is;

    Ip = ambient + diffuse + specular; 

    gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}    