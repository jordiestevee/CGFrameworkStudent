varying vec3 v_world_normal;
uniform sampler2D u_texture;
varying vec2 v_uv;

varying vec3 Ip;
void main()
{
    gl_FragColor = vec4(Ip, 1.0);
}
