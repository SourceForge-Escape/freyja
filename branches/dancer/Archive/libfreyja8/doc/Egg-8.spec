
 -------------------
 Egg v8 spec - 001

 Author: Mongoose
 Date  : 20010101
 -------------------

 1. Base Types
 1.1. Texel
 1.2. Vertex
 1.3. Polygon
 1.4. Metadata

 2. Compound Types
 2.1. Vertex Grouping
 2.2. Mesh
 2.3. Tag
 2.4. BoneFrame

 3. File format
 3.1. Layout


 ---------------------------------------------------------------

 1. Base Types

 1.1. Texel

  1.1.1 texel[n](s, t)

        Texture coordinates, used to map texture to polygons

 ---------------------------------------------------------------

 1.2. Vertex

  1.2.1 vertex[n](x, y, z)

        3 space coordinates, used to make polygons in 3 space

 ---------------------------------------------------------------

 1.3. Polygon

  1.3.1 polygon[n](vertex_list, texel_list, shader_index)

        Polygons are the basic building blocks of the model

 ---------------------------------------------------------------

 1.4. Metadata

  1.4.1 metadata[n](type, metadata_index, x, y, z)

        Metadata markers are used to add lights and other
        special objects that aren't normally handled by
        a poly modeler.

 ---------------------------------------------------------------

 2. Compound Types

 2.1. Vertex Grouping

  2.1.1 group[n](vertex_list, metadata_list, center, bounding_box)

        Groups are used to make editing easier, and allow for
        maximum felxiblity with prefabs, intermesh polygons,
        and quickly produce modular partial meshes.

        * Group selection by a node placed at center.

 ---------------------------------------------------------------

 2.2. Mesh

  2.2.1 mesh[n](vertex_grouping_list, polygon_list)

        The vertex grouping list would be for vertex morphing 
        frames.  The polygon list is the polygons that compose
        the mesh per frame.

 ---------------------------------------------------------------

 2.3. Tag

  2.3.1 tag[n](mesh_list, master_index, tag_slave_list, 
               bounding_box, center, scale[3], rotation[3])

        Tags are used to make modular models with interchangable
        parts.  
 
 ---------------------------------------------------------------

 2.4. BoneFrame

  2.4.1 boneframe[n](tag_list)

        BoneFrames are skeletal animation frames.

 ---------------------------------------------------------------

 3. File format

  3.1. Layout

       Strings are stored like pascal strings with length
       before data.  Lenght is stored as an unsigned byte.
       To read a string stored like this, read the first byte
       to get string lenght.  Then read lenght bytes as the
       string data.

   3.1.1 Header

         8 byte Egg file version id
         ? byte Modeler version id string
         4 byte Number of Texels
         4 byte Number of Vertices
         4 byte Number of Polygons
         4 byte Number of Metatags
         4 byte Number of Groups
         4 byte Number of Meshes
         4 byte Number of Tags
         4 byte Number of BoneFrames

         [Egg 8.01][22][GooseEgg 0.1.4-20010101]
         [4][4][2][2][1][1][1][1]

   3.1.2 Data

         Data follows the header, and *could be read as 
         chunks after the base types are read.  However, 
         it's intented to be ordered just like the header 
         order.  The file basicly has 3 sections: header,
         base type data, and complex type data.  The data
         is written unpacked in little endian byte order.

            1. [header]
            2. [texels][vertices][polygons][metatags]
            3. [groups][meshes][tags][boneframes]
