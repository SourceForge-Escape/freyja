
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>


class NiTriShape
{
	public:
		int len;

		bool readChunk(mstl::SystemIO::FileReader &r)
		{
			long i;

			len = r.ReadLong();
			mesh_name = new char[len];
			r.ReadString(len, mesh_name);
			unknown = r.ReadLongU();
			associated_skin_controller_id = r.ReadLongU();
			unknown1 = r.ReadInt16();

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

			r.ReadLongU(); // ?!

			num_properties = r.ReadLongU();

			propertyIDs = new long[num_properties];
			 
			for (i = 0; i < num_properties; ++i)
				propertyIDs[i] = r.ReadLong();
			 
			unknown3 = r.ReadLongU();//=0;
			associated_trishape_data_id = r.ReadLongU();

			r.ReadLongU(); // -1 ?!

			return (!r.IsEndOfFile());
		}

		char *mesh_name; //char[len];
		int unknown; //=0;
		unsigned int associated_skin_controller_id; // Set to zero if there is none
		short unknown1;//=4;
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
		float unknown2[3];//={1,0,0}; // Like a spare vector: maybe an artifact from a matrix
		int num_properties;
		long *propertyIDs;//word[num_properties];
		int unknown3;//=0;
		unsigned int associated_trishape_data_id; // The mesh
	};
