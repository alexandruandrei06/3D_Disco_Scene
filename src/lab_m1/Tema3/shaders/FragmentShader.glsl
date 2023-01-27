#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties
uniform vec3 floorLight_Pos[64];
uniform vec3 floorLight_Color[64];

uniform vec3 spotLight_Pos[4];
uniform vec3 spotLight_Color[4];
uniform vec3 spotLight_Direction[4];

uniform vec3 eye_position;

uniform float timeT;
uniform int type_of_light;

uniform vec3 discoBall_position;
uniform vec3 discoBallLight_position;

uniform sampler2D texture_1;

uniform float cut_off_angle;

uniform float material_kd;
uniform float material_ke;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int object_type;

// Output
layout(location = 0) out vec4 out_color;

vec3 N, L, V, H, R;
float intensitate_lumina = 1.2;

//Functie care calculeaza culoarea disco ball-ului in functie de textura
vec3 ComputeDiscoBallColour(){
    vec3 light_dir = world_position - discoBall_position;

    vec2 texcoord;
    texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) + timeT / 20;
    texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));

    vec3 colour = texture (texture_1, texcoord).xyz;
    return colour;
}

//Functie care calculeaza lumina provenita de la dico ball
vec3 ComputeDiscoBallLight(){
    vec3 colour = vec3(0, 0, 0);
   
    L = normalize(discoBall_position - world_position);
	H = normalize(L + V);
	R = reflect(-L, world_normal);

    float diffuse_light = material_kd * intensitate_lumina * max(dot(N, L), 0.f);
    float specular_light = 0.f;

    if (diffuse_light > 0.f){
        specular_light = material_ks * intensitate_lumina * pow(max(dot(N, H), 0), material_shininess);
    }

    float d	= distance(discoBall_position, world_position);
	float attenuation_factor = 1.f / max(d * d, 1.f) + 0.6;
	float light = attenuation_factor * (diffuse_light + specular_light);

    vec3 light_dir = world_position - discoBall_position;
    vec2 texcoord;
    texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) + timeT / 20;
    texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));

    colour = texture (texture_1, texcoord).xyz * light;
    return colour;
}

//Functie care calculeaza lumina provenita de la podea
vec3 ComputeWallLight(){
    vec3 colour = vec3(0, 0, 0);
    float att;
    float diffuse_light, specular_light;
    for(int i = 0; i < 64; i++){
        L = normalize(floorLight_Pos[i] - world_position);
	    H = normalize(L + V);
	    R  = reflect(-L, world_normal);
        
        att = 0;
        float dist = distance(world_position, floorLight_Pos[i]);
        if(dist < sqrt(2)){
            att = pow(sqrt(2) - dist, 2);
        }

        diffuse_light =  material_kd * intensitate_lumina * max(dot(N, L), 0.f);
        specular_light = 0;

        if (diffuse_light > 0){
            specular_light = material_ks * intensitate_lumina * pow(max(dot(N, H), 0), material_shininess);
        }

        colour += floorLight_Color[i] * att * (diffuse_light + specular_light);

    }

    return colour;
}

//Functie care calculeaza lumina provenita de la spot-uri
vec3 ComputeSpotLight(){
    vec3 colour = vec3(0, 0, 0);
    float diffuse_light, specular_light;
    for(int i = 0; i < 4; i++){
        L = normalize(spotLight_Pos[i] - world_position);
	    H = normalize(L + V);
	    R = reflect(-L, world_normal);

        float diffuse_light = material_kd * intensitate_lumina * max(dot(N, L), 0.f);
        float specular_light = 0.f;

        if (diffuse_light > 0.f){
            specular_light = material_ks * intensitate_lumina * pow(max(dot(N, H), 0), material_shininess);
        }

        float cut_off_rad		= radians(cut_off_angle);
		float spot_light		= dot(-L, spotLight_Direction[i]);
		float spot_light_limit	= cos(cut_off_rad);
        if (spot_light > spot_light_limit){	 

			float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit) + 0.1;
			float light_att_factor = linear_att * linear_att;
			colour += spotLight_Color[i] * light_att_factor * (diffuse_light + specular_light);
		}
    }

    return colour;
}

void main()
{
    N = world_normal;
	V = normalize(eye_position - world_position);

    float intensitate_lumina = 1;
    vec3 colour;

    int iDiscoBall = 1;
    int iSpot = 1;
    int iPodea = 1;
    
    if(type_of_light == 0){
        iDiscoBall = 0;
        iSpot = 0;
    }else if(type_of_light == 1){
        iDiscoBall = 0;
        iPodea = 0;
    }else if(type_of_light == 2){
        iSpot = 0;
        iPodea = 0;
    }


	if(object_type == 1){
        colour =  iPodea *ComputeWallLight() + iSpot * ComputeSpotLight() + iDiscoBall * ComputeDiscoBallLight();
        out_color = vec4(colour, 1.f);
	}else if(object_type == 2){
        colour = object_color;
        if(iSpot == 0){
            out_color = vec4(colour, 0.f);
        }else{
            out_color = vec4(colour, 0.5f);
        }
        
    }else if(object_type == 3){
        colour = iDiscoBall * ComputeDiscoBallColour() ;
        out_color = vec4(colour, 1f);
    }else{
        colour = iPodea * object_color * material_ke + iSpot * ComputeSpotLight() + iDiscoBall * ComputeDiscoBallLight();
        out_color = vec4(colour, 1.f);
	}
}
