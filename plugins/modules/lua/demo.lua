--======================================================================
-- 
-- Project : libfreyja
-- Author  : Terry 'Mongoose' Hendrix II
-- Website : http://icculus.org/~mongoose
-- Email   : mongooseichiban@gmail.com
-- Object  : 
-- Comments: This is a demo script for the libfreyja lua binding.
-- 
---- History ---------------------------------------------------------- 
--
-- 2007.06.21:
-- Mongoose, Created
--======================================================================

-- Create a cube mesh, remember indices start at 1 kids!
cube0 = freyjaMeshCreate()

size = 8

vertices = {
	freyjaMeshVertexCreate3fv(cube0, 0.0, size, 0.0),
	freyjaMeshVertexCreate3fv(cube0, size, size, 0.0),
	freyjaMeshVertexCreate3fv(cube0, size, 0.0, 0.0),
	freyjaMeshVertexCreate3fv(cube0, 0.0, 0.0, 0.0),
	freyjaMeshVertexCreate3fv(cube0, 0.0, size, size),
	freyjaMeshVertexCreate3fv(cube0, size, size, size),
	freyjaMeshVertexCreate3fv(cube0, size, 0.0, size),
	freyjaMeshVertexCreate3fv(cube0, 0.0, 0.0, size)
}


texcoords = {
	
	freyjaMeshTexCoordCreate2f(cube0, 0.0, 0.0),
	freyjaMeshTexCoordCreate2f(cube0, 0.0, 1.0),
	freyjaMeshTexCoordCreate2f(cube0, 1.0, 1.0),
	freyjaMeshTexCoordCreate2f(cube0, 1.0, 0.0)
}


face = freyjaMeshPolygonCreate(cube0)
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[1])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[1])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[2])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[2])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[6])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[3])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[5])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[4])
freyjaMeshPolygonMaterial(cube0, face, 0)

face = freyjaMeshPolygonCreate(cube0)
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[4])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[1])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[8])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[2])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[7])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[3])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[3])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[4])
freyjaMeshPolygonMaterial(cube0, face, 0)

face = freyjaMeshPolygonCreate(cube0)
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[5])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[1])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[1])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[2])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[4])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[3])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[8])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[4])
freyjaMeshPolygonMaterial(cube0, face, 0)

face = freyjaMeshPolygonCreate(cube0)
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[1])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[1])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[2])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[2])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[3])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[3])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[4])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[4])
freyjaMeshPolygonMaterial(cube0, face, 0)

face = freyjaMeshPolygonCreate(cube0)
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[2])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[1])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[3])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[2])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[7])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[3])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[6])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[4])
freyjaMeshPolygonMaterial(cube0, face, 0)

face = freyjaMeshPolygonCreate(cube0)
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[5])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[1])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[6])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[2])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[7])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[3])
freyjaMeshPolygonAddVertex1i(cube0, face, vertices[8])
freyjaMeshPolygonAddTexCoord1i(cube0, face, texcoords[4])
freyjaMeshPolygonMaterial(cube0, face, 0)


-- Create a cube mesh much faster  ;)
cube1 = freyjaMeshCreateCube(0.0, 8.0, 0.0, 8.0)

-- Create a cube given user input.
size = freyjaQueryInteger("size")
cube2 = freyjaMeshCreateCube(0.0, 16.0, 0.0, size)

-- Create a cube given user input.
sizeR = freyjaQueryFloat("size")
cube2 = freyjaMeshCreateCube(0.0, 16.0, 0.0, sizeR)

s = freyjaQueryString("test")
freyjaPrintMessage(s)



