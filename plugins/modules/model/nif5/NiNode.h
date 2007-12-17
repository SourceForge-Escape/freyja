
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>


class NiNode
{
public:

	// subnodes: int32 sz, char name[sz], 

	bool readChunk(mstl::SystemIO::FileReader &r)
	{
		 len = r.ReadInt32();
		 node_name = new char[len];
		 r.ReadString(len, node_name);
		 associated_keyframe_controller_id = r.ReadInt32U();
		 associated_lookat_controller_id = r.ReadInt32U();
		 unknown = r.ReadInt16();
		 x = r.ReadFloat32();
		 y = r.ReadFloat32();
		 z = r.ReadFloat32();
		 vector_x_axis_x = r.ReadFloat32();
		 vector_x_axis_y = r.ReadFloat32();
		 vector_x_axis_z = r.ReadFloat32();
		 vector_y_axis_x = r.ReadFloat32();
		 vector_y_axis_y = r.ReadFloat32();
		 vector_y_axis_z = r.ReadFloat32();
		 vector_z_axis_x = r.ReadFloat32();
		 vector_z_axis_y = r.ReadFloat32();
		 vector_z_axis_z = r.ReadFloat32();
		 unknown2[0] = r.ReadFloat32();
		 unknown2[1] = r.ReadFloat32();
		 unknown2[2] = r.ReadFloat32();
		 unknown3 = r.ReadInt32();

		 num_properties = r.ReadInt32();
		 propertyIDs = new unsigned int[num_properties];
		 
		 for (unsigned int i = 0, n = num_properties; i < n; ++i)
			 propertyIDs[i]  = r.ReadInt32U();

		 unkn2 = r.ReadInt32U();
		 
		 num_children = r.ReadInt32();
		 childrenIDs = new unsigned int[num_children];
		 
		 for (unsigned int i = 0, n = num_children; i < n; ++i)
			 childrenIDs[i]  = r.ReadInt32U();
		 
		 num_blocks = r.ReadInt32();
		 blockIDs = new unsigned int[num_blocks];

		 for (unsigned int i = 0, n = num_blocks; i < n; ++i)
			 blockIDs[i]  = r.ReadInt32U();

		 return (!r.IsEndOfFile());
	}

	int len;
	char *node_name; // char[len];
	unsigned int associated_keyframe_controller_id;//zeroifnot linked to one
	unsigned int associated_lookat_controller_id;//zero if not linked to one
	short unknown; //(12 once)
	float x;
	float y;
	float z;
	float vector_x_axis_x;
	float vector_x_axis_y;
	float vector_x_axis_z;
	float vector_y_axis_x;
	float vector_y_axis_y;
	float vector_y_axis_z;
	float vector_z_axis_x;
	float vector_z_axis_y;
	float vector_z_axis_z;
	float unknown2[3]; //={1,0,0}; matrix col or aug vec?
	int unknown3; //=0;
	int num_properties;
	unsigned int *propertyIDs;//[num_properties];
	int unkn2;//=0;
	int num_children;
	unsigned int *childrenIDs;//[num_children];
	int num_blocks;
	unsigned int *blockIDs;//[num_blocks];
};


