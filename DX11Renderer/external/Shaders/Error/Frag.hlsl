struct PS_INPUT {
	float4 Position : SV_POSITION;
};

float4 main(PS_INPUT In) : SV_TARGET {
    return float4(0.898, 0.31, 1, 1);
}