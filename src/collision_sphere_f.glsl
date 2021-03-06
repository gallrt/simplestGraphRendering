#version 130

uniform sampler2D priority_data_tx2D;

uniform float time;
uniform int mode;
uniform uint highest_priority;
uniform int show_elimination_idx;

flat in ivec2 data_idx;
flat in int instance_id;
in float collision_time;
in float distance_to_center;
in float current_radius;

out vec4 frag_colour;

void main()
{
    float min_priority = texelFetch(priority_data_tx2D,data_idx,0).y; 
    float priority = texelFetch(priority_data_tx2D,data_idx,0).x;
    float v = (float(highest_priority)-min_priority > 0.0) ? (priority-min_priority) /(float(highest_priority)-min_priority) : 0.0;

    float r = v;
    float g = 0.0;
    float b = 1.0 - v;
    
    frag_colour = vec4(r,g,b,1.0);

    if(mode == 0)
    {
        if( (distance_to_center < (time/collision_time)) && (instance_id < show_elimination_idx) )
            frag_colour = vec4(0.0,0.0,0.0,1.0);
    }
    else
    {
        if( (distance_to_center < (time/collision_time)*current_radius) && (instance_id < show_elimination_idx) )
            frag_colour = vec4(0.0,0.0,0.0,1.0);
    }
}