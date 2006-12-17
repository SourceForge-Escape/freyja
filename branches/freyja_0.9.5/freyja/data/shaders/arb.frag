!!ARBfp1.0

TEMP temp;   #temporary
ATTRIB tex0 = fragment.texcoord[0];
ATTRIB col0 = fragment.color;
PARAM pink = { 1.0, 0.4, 0.4, 1.0};
OUTPUT out = result.color;
TEX temp, tex0, texture[0], 2D;  #Fetch texture
MOV out, temp; #replace
#MUL out, col0, temp; #modulate
#MUL out, temp, pink; #modulate with constant color
END
