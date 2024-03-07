
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
uniform vec3 Ia;
uniform vec3 lightPosition;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 cameraPosition;
uniform vec3 flag;
uniform mat4 u_model;

varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform sampler2D u_texture;
uniform sampler2D normalMap;

void main()
{
    vec3 Ip;
    vec3 ka_f = Ka;
    vec3 ks_f = Ks;
    vec3 kd_f = Kd;

    vec3 P = v_world_position;
    vec3 L = normalize(lightPosition - P);
    vec3 V = normalize(cameraPosition - P);
    vec3 N = normalize(v_world_normal);
    vec3 R = reflect(-L, N);    

    vec4 colorTexture = texture2D(u_texture, v_uv);
    vec4 normal = texture2D(normalMap, v_uv);

    if (flag.x == 1.0){
        kd_f = colorTexture.rgb;
        ka_f *= colorTexture.rgb; 
    }

    if (flag.y == 1.0){
        ks_f = vec3(colorTexture.a);
    }

    if (flag.z == 1.0){
        normal = (2.0*normal - vec4(1.0));
        vec3 world_normal = normalize((u_model * vec4(normal.xyz, 0.0)).xyz);
        float mix_factor = 0.8;
        N = normalize(mix(v_world_normal, world_normal, mix_factor));
    }

    vec3 ambient = ka_f* Ia;
    vec3 diffuse = kd_f*clamp(dot(N,L), 0.0, 1.0)*Id;
    vec3 specular = ks_f*pow(clamp(dot(R,V), 0.0, 1.0), shininess)*Is;

    Ip = ambient + diffuse + specular; 

    gl_FragColor = vec4(Ip, 1.0); //output of the vertex shader
}
    