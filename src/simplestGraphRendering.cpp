#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <cmath>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>

#include <string>
#include <locale>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <memory>
#include <map>
#include <list>
#include <chrono>

typedef unsigned int uint;

namespace Math
{
	#define PI 3.141592653589793238462643383279502884197169399375105820f

	struct Vec2
	{
		Vec2() : x(0.0), y(0.0) {}
		Vec2(float x, float y) : x(x), y(y){}

		float x, y;

		Vec2 operator* (const float a)
		{
			return Vec2(a*x,a*y);
		}

		Vec2 operator+ (const Vec2& v)
		{
			return Vec2(x+v.x,y+v.y);
		}

		Vec2 operator- (const Vec2& v)
		{
			return Vec2(x-v.x,y-v.y);
		}

		float length() const
		{
			return std::sqrt(x*x+y*y);
		}
	};

	struct Vec3
	{
		Vec3() : x(0.0), y(0.0), z(0.0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		float x, y, z;

		Vec3 operator* (const float a)
		{
			return Vec3(a*x,a*y,a*z);
		}

		Vec3 operator+ (const Vec3& v)
		{
			return Vec3(x+v.x,y+v.y,z+v.z);
		}

		Vec3 operator- (const Vec3& v)
		{
			return Vec3(x-v.x,y-v.y,z-v.z);
		}

		float length() const
		{
			return std::sqrt(x*x+y*y+z*z);
		}
	};
	
	struct Mat4x4
	{
		Mat4x4() : data() {}
		Mat4x4(const std::array<float,16> data) : data(data) {}
	
		std::array<float,16> data;
	
		float& operator[] (int index)
		{
			return data[index];
		}
	
		Mat4x4 operator* (const Mat4x4& m)
		{
			Mat4x4 b(m);

			return Mat4x4(
				std::array<float,16>({	// first row
					data[0]*b[0] + data[4]*b[1] + data[8]*b[2] + data[12]*b[3],
					data[1]*b[0] + data[5]*b[1] + data[9]*b[2] + data[13]*b[3],
					data[2]*b[0] + data[6]*b[1] + data[10]*b[2] + data[14]*b[3],
					data[3]*b[0] + data[7]*b[1] + data[11]*b[2] + data[15]*b[3],
					// second row
					data[0]*b[4] + data[4]*b[5] + data[8]*b[6] + data[12]*b[7],
					data[1]*b[4] + data[5]*b[5] + data[9]*b[6] + data[13]*b[7],
					data[2]*b[4] + data[6]*b[5] + data[10]*b[6] + data[14]*b[7],
					data[3]*b[4] + data[7]*b[5] + data[11]*b[6] + data[15]*b[7],
					// third row
					data[0]*b[8] + data[4]*b[9] + data[8]*b[10] + data[12]*b[11],
					data[1]*b[8] + data[5]*b[9] + data[9]*b[10] + data[13]*b[11],
					data[2]*b[8] + data[6]*b[9] + data[10]*b[10] + data[14]*b[11],
					data[3]*b[8] + data[7]*b[9] + data[11]*b[10] + data[15]*b[11],
					// fourth row
					data[0]*b[12] + data[4]*b[13] + data[8]*b[14] + data[12]*b[15],
					data[1]*b[12] + data[5]*b[13] + data[9]*b[14] + data[13]*b[15],
					data[2]*b[12] + data[6]*b[13] + data[10]*b[14] + data[14]*b[15],
					data[3]*b[12] + data[7]*b[13] + data[11]*b[14] + data[15]*b[15],
				})
			);
		}
	
		Mat4x4 inverse()
		{
			float a00 = data[0], a01 = data[1], a02 = data[2], a03 = data[3],
		    a10 = data[4], a11 = data[5], a12 = data[6], a13 = data[7],
		    a20 = data[8], a21 = data[9], a22 = data[10], a23 = data[11],
		    a30 = data[12], a31 = data[13], a32 = data[14], a33 = data[15],

		    b00 = a00 * a11 - a01 * a10,
		    b01 = a00 * a12 - a02 * a10,
		    b02 = a00 * a13 - a03 * a10,
		    b03 = a01 * a12 - a02 * a11,
		    b04 = a01 * a13 - a03 * a11,
		    b05 = a02 * a13 - a03 * a12,
		    b06 = a20 * a31 - a21 * a30,
		    b07 = a20 * a32 - a22 * a30,
		    b08 = a20 * a33 - a23 * a30,
		    b09 = a21 * a32 - a22 * a31,
		    b10 = a21 * a33 - a23 * a31,
		    b11 = a22 * a33 - a23 * a32,

		    // Calculate the determinant
		    det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

		    //if (!det) {
		    //    return null;
		    //}
			// The inverse is used as multiplication factor
		    det = 1.0f / det;

		    return Mat4x4(std::array<float,16>({	(a11 * b11 - a12 * b10 + a13 * b09) * det,
						(a02 * b10 - a01 * b11 - a03 * b09) * det,
						(a31 * b05 - a32 * b04 + a33 * b03) * det,
						(a22 * b04 - a21 * b05 - a23 * b03) * det,
						(a12 * b08 - a10 * b11 - a13 * b07) * det,
						(a00 * b11 - a02 * b08 + a03 * b07) * det,
						(a32 * b02 - a30 * b05 - a33 * b01) * det,
						(a20 * b05 - a22 * b02 + a23 * b01) * det,
						(a10 * b10 - a11 * b08 + a13 * b06) * det,
						(a01 * b08 - a00 * b10 - a03 * b06) * det,
						(a30 * b04 - a31 * b02 + a33 * b00) * det,
						(a21 * b02 - a20 * b04 - a23 * b00) * det,
						(a11 * b07 - a10 * b09 - a12 * b06) * det,
						(a00 * b09 - a01 * b07 + a02 * b06) * det,
						(a31 * b01 - a30 * b03 - a32 * b00) * det,
						(a20 * b03 - a21 * b01 + a22 * b00) * det}));
		}
	};

	struct Mat2x2
	{
		Mat2x2() : data() {}
		Mat2x2(const std::array<float,4> data) : data(data) {}

		std::array<float,4> data;

		float& operator[] (int index)
		{
			return data[index];
		}

		Vec2 operator* (const Vec2& v)
		{
			return Vec2( data[0]*v.x+data[2]*v.y, data[1]*v.x+data[3]*v.y );
		}
	};

	Vec3 operator* (const float a, const Vec3& v)
	{
		return Vec3(a*v.x,a*v.y,a*v.z);
	}

	Vec3 operator/ (const Vec3& v,const float a)
	{
		return Vec3(v.x/a,v.y/a,v.z/a);
	}

	float dot(const Vec3& u, const Vec3& v)
	{
		return (u.x*v.x + u.y*v.y + u.z*v.z);
	}

	float dot(const Vec2& u, const Vec2& v)
	{
		return (u.x*v.x + u.y*v.y);
	}

	double dot64(const Vec3& u, const Vec3& v)
	{
		double ux = (double)u.x;
		double uy = (double)u.y;
		double uz = (double)u.z;
		double vx = (double)v.x;
		double vy = (double)v.y;
		double vz = (double)v.z;

		return (ux*vx+uy*vy+uz*vz);
	}

	Vec3 cross(const Vec3& u, const Vec3& v)
	{
		return Vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
	}

	Vec3 normalize(const Vec3& v)
	{
		float l = v.length();

		return ( v/l );
	}
}

/**
 * Node struct for parsing preprocessed osm graph from file
 */
struct Node
{
	Node() : lat(0), lon(0) {}
	Node(double la, double lo) : lat(la), lon(lo) {}

	double lat;
	double lon;
};

/**
 * Edge struct for parsing preprocessed osm graph from file
 */
struct Edge
{
	Edge() : source(0), target(0), width(0), color(0) {}
	Edge(uint s, uint t, uint w, uint c) : source(s), target(t), width(w), color(c) {}

	uint source;
	uint target;
	uint width;
	int color;
};

/**
 * Each vertex contains the geo coordinates of a node and the color properties of adjacent edges.
 * Thus, for each node with adjacent egdes of different color multiple vertices are constructed.
 */
struct Vertex
{
	Vertex() : longitude(0.0), latitude(0.0), color(-1) {}
	Vertex(float lon, float lat) : longitude(lon), latitude(lat), color(-1) {}

	float longitude;
	float latitude;
	float color;
};

struct Node_RGB
{
	Node_RGB() : lat(0), lon(0), r(0), g(0), b(0), a((char)255) {}
	Node_RGB(double la, double lo, char r, char g, char b, char a = (char)255) : lat(la), lon(lo), r(r), g(g), b(b), a(a) {}

	double lat;
	double lon;

	char r;
	char g;
	char b;
	char a;
};

struct Edge_RGB
{
	Edge_RGB() : source(0), target(0), r(0), g(0), b(0), a((char)255) {}
	Edge_RGB(uint s, uint t, char r, char g, char b, char a = (char)255)
		: source(s), target(t), r(r), g(g), b(b), a(a) {}

	uint source;
	uint target;
	char r;
	char g;
	char b;
	char a;
};

struct Vertex_RGB
{
	Vertex_RGB() : longitude(0.0), latitude(0.0), r(0), g(0), b(0), a((char)255) {}
	Vertex_RGB(float lon, float lat, char r, char g, char b, char a = (char)255)
		: longitude(lon), latitude(lat), r(r), g(g), b(b), a(a) {}

	float longitude;
	float latitude;
	char r;
	char g;
	char b;
	char a;
};

struct Vertex_ID_RGB
{
	Vertex_ID_RGB() : longitude(0.0), latitude(0.0), id(-1), r(0), g(0), b(0), a((char)255) {}
	Vertex_ID_RGB(float lon, float lat, char r, char g, char b, int id, char a = (char)255)
		: longitude(lon), latitude(lat), id(id), r(r), g(g), b(b), a(a) {}

	float longitude;
	float latitude;
	int id;
	char r;
	char g;
	char b;
	char a;
};

struct Vertex_XYZ
{
	Vertex_XYZ() : x(0.0f), y(0.0f), z(0.0f) {}
	Vertex_XYZ(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x;
	float y;
	float z;
};

struct Triangle_RGB
{
	Triangle_RGB() : v1(0), v2(0), v3(0), r(0), g(0), b(0), a((char)255) {}
	Triangle_RGB(uint v1, uint v2, uint v3, char r, char g, char b, char a = (char)255)
		: v1(v1), v2(v2), v3(v3), r(r), g(g), b(b), a(a) {}

	uint v1;
	uint v2;
	uint v3;
	char r;
	char g;
	char b;
	char a;
};

struct CollisionSphere
{
	CollisionSphere() : lat(0), lon(0) {}
	CollisionSphere(uint id, double la, double lo, double rad, uint prio, double ct, uint cp_id)
		: id(id), lat(la), lon(lo), radius(rad), priority(prio), collision_time(ct), collision_partner_id(cp_id) {}

	uint id;

	double lat;
	double lon;

	double radius;
	uint priority;
	
	double collision_time;

	uint collision_partner_id;
};

/**
 * A goe coordinate bounding box
 */
struct GeoBoundingBox
{
	float min_longitude;
	float min_latitude;

	float max_longitude;
	float max_latitude;
};

namespace Controls {
	int collisionSphere_mode = 0;
}

/**
 * Function to simply read the string of a shader source file from disk
 */
const std::string readShaderFile(const char* const path)
{
	std::ifstream inFile( path, std::ios::in );

	std::ostringstream source;
	while( inFile.good() ) {
		int c = inFile.get();
		if( ! inFile.eof() ) source << (char) c;
	}
	inFile.close();

	return source.str();
}

std::vector<uint16_t> toUnicodePoints(const std::string & str)
{
	std::vector<uint16_t> result;
	for(std::string::const_iterator it(str.begin()), end(str.end()); it != end; ++it)
	{
		if (static_cast<uint8_t>(*it) < 0x80)
		{
			result.emplace_back(*it);
		}
		else if ((static_cast<uint8_t>(*it) >> 5) == 0x6)
		{ //codepoints is encoded by 2
			uint32_t code_point = *it;
			++it;
			assert(it != end);
			code_point = ((code_point << 6) & 0x7ff) + ((*it) & 0x3f);
			result.emplace_back(code_point);
		}
		else if ((static_cast<uint8_t>(*it) >> 3) == 0x1e)
		{
			uint32_t code_point = static_cast<uint8_t>(*it);
			++it;
			assert(it != end);

			code_point = ((code_point << 12) & 0xffff) + ((static_cast<uint8_t>(*it) << 6) & 0xfff);
			++it;
			assert(it != end);

			code_point += (*it) & 0x3f;
			if (code_point > std::numeric_limits<uint16_t>::max()) {
				throw std::range_error("Cannot encode unicode codepoint in 2 Bytes");
			}
			result.emplace_back(code_point);
		}
		else
		{
			throw std::runtime_error("utf8 codepoints with more than 2 Bytes are not supported");
		}
	}
	return result;
}

/**
 * Function for compiling shader source code. Returns the handle of the compiled shader
 */
GLuint compileShader(const std::string * const source, GLenum shaderType)
{
	/* Check if the source is empty */
	if (source->empty())
	{
		//TODO exit program?
	}

	/* Create shader object */
	const GLchar* c_source = source->c_str();
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &c_source, NULL);

	/* Compile shader */
	glCompileShader(shader);

	/* Check for errors */
	std::string shaderlog;
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);

	GLint logLen = 0;
	shaderlog = "";
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
	if(logLen > 0)
	{
		char *log = new char[logLen];
		GLsizei written;
		glGetShaderInfoLog(shader, logLen, &written, log);
		shaderlog = log;
		delete [] log;

		std::cout<<shaderlog;
	}

	if(compile_ok == GL_FALSE)
	{
		glDeleteShader(shader);
		return -1;
	}

	return shader;
}

/**
 * Load a shader program
 * \attribute vs_path Path to vertex shader source file
 * \attribute fs_path Path to fragement shader source file
 * \attribute attributes Vertex shader input attributes (i.e. vertex layout)
 * \return Returns the handle of the created GLSL program
 */
GLuint createShaderProgram(const char* vs_path, const char* fs_path, std::vector<const char*> attributes)
{
	/* Create a shader program object */
	GLuint handle;
	handle = glCreateProgram();

	/* Set the location (i.e. index) of the attribute (basically the input variable) in the vertex shader.
	 * The vertices intended to be used with this program will have to match that index in their
	 * attribute decription, so that a connection between the vertex data and the shader input can be made.
	 */
	for(GLuint i=0; i<attributes.size(); i++)
		glBindAttribLocation(handle, i, attributes[i]);

	/* Read the shader source files */
	std::string vs_source = readShaderFile(vs_path);

	GLuint vertex_shader = compileShader(&vs_source, GL_VERTEX_SHADER);

	/* Attach shader to program */
	glAttachShader(handle, vertex_shader);

	/* Flag shader program for deletion.
	 * It will only be actually deleted after the program is deleted. (See destructor for program deletion.
	 */
	glDeleteShader(vertex_shader);


	/* Load, compile and attach fragment shader */
	std::string fs_source = readShaderFile(fs_path);

	GLuint fragment_shader = compileShader(&fs_source,GL_FRAGMENT_SHADER);

	/* Attach shader to program */
	glAttachShader(handle, fragment_shader);

	/* Flag shader program for deletion.
	 * It will only be actually deleted after the program is deleted. (See destructor for program deletion.
	 */
	glDeleteShader(fragment_shader);


	/* Link program */
	glLinkProgram(handle);

	/* Check if linking was successful */
	std::string shader_log;
	GLint status = GL_FALSE;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);

	GLint logLen = 0;
	shader_log = "";
	glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLen);
	if(logLen > 0)
	{
		char *log = new char[logLen];
		GLsizei written;
		glGetProgramInfoLog(handle, logLen, &written, log);
		shader_log = log;
		delete [] log;
	}

	std::cout<<shader_log<<std::endl;

	if(status == GL_FALSE)
		return -1;

	return handle;

}

/**
 * Collection of functions for loading graphic resources
 */
namespace ResourceLoader
{

	/**
	 * \brief Read a the header of a ppm image file. Courtesy to the computer vision lecture I attended.
	 * \param filename Location of the image file
	 * \param headerEndPos Out parameter, marks the point where the header of the ppm file ends
	 * \param imgDimX Out parameter, containing the dimension of the image in X direction in pixels
	 * \param imgDimY Out parameter, containing the dimension of the image in Y direction in pixels
	 * \return Returns true if the ppm header was succesfully read, false otherwise
	 */
	bool readPpmHeader(const char* filename, unsigned long& headerEndPos, int& imgDimX, int& imgDimY)
	{
		int currentComponent = 0;
		bool firstline = false;
		std::string::iterator itr1;
		std::string::iterator itr2;
		std::string buffer;
		std::string compBuffer;
		std::ifstream file (filename,std::ios::in | std::ios::binary);
	
		/*
		/	Check if the file could be opened.
		*/
		if(!( file.is_open() ))return false;
	
		/*
		/	Go to the beginning of the file and read the first line.
		*/
		file.seekg(0, file.beg);
		std::getline(file,buffer,'\n');
		itr1 = buffer.begin();
		for(itr2 = buffer.begin(); itr2 != buffer.end(); itr2++)
		{
			/*
			/	Check if the first line contains more than just ppm's magic number.
			/	If it does, it should look like this:
			/	"magic_number image_dimension_x image_dimension_y maximum_value"
			/	Therefore we scan the string for a space character and start parsing it.
			*/
			if(*itr2 == ' ')
			{
				if(currentComponent == 0)
				{
					/*	The first component is the magic number. We don't need it.	*/
					currentComponent++;
					firstline = true;
					itr1 = (itr2 + 1);
				}
				else if(currentComponent == 1)
				{
					/*	Get the image dimension in x.	*/
					compBuffer.assign(itr1, itr2);
					imgDimX = atoi(compBuffer.c_str());
					currentComponent++;
					itr1 = (itr2 + 1);
				}
				else if(currentComponent == 2)
				{
					/*	Get the image dimension in y.	*/
					compBuffer.assign(itr1, itr2);
					imgDimY = atoi(compBuffer.c_str());
					currentComponent++;
					itr1 = (itr2 + 1);
				}
			}
		}
	
		/*
		/	If the information we were looking for was inside the first line, we are done here.
		/	Note the position where we left off and exit with return true after closing the file.
		*/
		if(firstline)
		{
			headerEndPos = static_cast<long>(file.tellg());
			file.close();
			return true;
		}
	
		/*
		/	If the information wasn't inside the first line we have to keep reading lines.
		/	Skip all comment lines (first character = '#').
		*/
		std::getline(file,buffer,'\n');
		while( buffer[0]=='#' || (buffer.size() < 1) )
		{
			std::getline(file,buffer,'\n');
		}
	
		/*
		/	Now we should have a string containing the image dimensions and can extract them.
		*/
		itr1 = buffer.begin();
		for(itr2 = buffer.begin(); itr2 != buffer.end(); itr2++)
		{
			/*	Get the image dimension in x.	*/
			if(*itr2 == ' ')
			{
				compBuffer.assign(itr1, itr2);
				imgDimX = atoi(compBuffer.c_str());
				currentComponent++;
				itr1 = (itr2 + 1);
			}
		}
	
		/*
		/	The last component of a line can't be parsed within the loop since it isn't followed by
		/	a space character, but an end-of-line.
		/
		/	Get the image dimension in x.
		*/
		compBuffer.assign(itr1, itr2);
		imgDimY = atoi(compBuffer.c_str());
	
		/*
		/	Read one more line. This should contain the maximum value of the image, but we don't need
		/	that.
		/	Note down the position after this line and exit with return true after closing the file.
		*/
		std::getline(file,buffer,'\n');
		headerEndPos = static_cast<unsigned long>(file.tellg());
		file.close();
		return true;
	}
	
	/**
	 * \brief Read a the data of a ppm image file. Courtesy to the computer vision lecture I attended.
	 * \param filename Location of the image file
	 * \param imageData Pointer to the data buffer, that the image data will be written to
	 * \param dataBegin Marks the location within the ppm file, where the data block begins
	 * \param imgDimX Dimension of the image in X direction in pixels
	 * \param imgDimY Dimension of the image in Y direction in pixels
	 * \return Returns true if the ppm header was succesfully read, false otherwise
	 */
	bool readPpmData(const char* filename, char* imageData, unsigned long dataBegin, int imgDimX, int imgDimY)
	{
		std::ifstream file (filename,std::ios::in | std::ios::binary);
	
		/*
		/	Check if the file could be opened.
		*/
		if(!( file.is_open() ))return false;
	
		/*
		/	Determine the length from the beginning of the image data to the end of the file.
		*/
		file.seekg(0, file.end);
		unsigned long length = static_cast<unsigned long>(file.tellg());
		length = length - dataBegin;
		char* buffer = new char[length];
	
		file.seekg(dataBegin,std::ios::beg);
		file.read(buffer,length);
	
		/*
		/	Rearrange the image information so that the data begins with the lower left corner.
		*/
		int k = 0;
		for(int i=0; i < imgDimY; i++)
		{
			int dataLoc = (imgDimY-1-i)*imgDimX*3;
			for(int j=0; j < imgDimX; j++)
			{
				imageData[k]=buffer[dataLoc+(j*3)];
				k++;
				imageData[k]=buffer[dataLoc+(j*3)+1];
				k++;
				imageData[k]=buffer[dataLoc+(j*3)+2];
				k++;
			}
		}
	
		file.close();
		delete[] buffer;
		return true;
	}

}

/*
 * Collection of functions for parsing files. Taken from lasagne and modified to meet expected graph input format
 */
namespace Parser
{
	/**
	 * Creates a new graph node
	 * @param input_string Input string containing node data
	 * @param n Nodes vector to put the new node into.
	 */
	void createNode(const std::string& input_string, std::vector<Node>& n)
	{
		std::string lat, lon;

		std::stringstream ss(input_string);
		ss >> lat >> lon;

		n.emplace_back(std::stof(lat), std::stof(lon));
	}

	/**
	 * createEdge - create a new edge
	 * @param input_string Input string containing edge data
	 * @param e Edges vector to put the new edge into.
	 */
	void createEdge(const std::string& input_string, std::vector<Edge>& e)
	{
		std::string source, target, width, color;

		std::stringstream ss(input_string);
		ss >> source >> target >> width >> color;

		e.emplace_back(std::stoul(source), std::stoul(target), std::stoul(width), std::stoi(color));
	}

	/**
	 * Parse node and egde from input file
	 * @param graphfile Path to the graphfile
	 * @param n Vector for storing the parsed nodes
	 * @param e Vector for storing the parsed edges
	 */
	bool parseTxtGraphFile(const std::string& graphfile, std::vector<Node>& n, std::vector<Edge>& e)
	{
		std::string buffer;
		std::ifstream file;

		file.open(graphfile.c_str(), std::ios::in);

		if(file.is_open())
		{
			file.seekg(0, std::ios::beg);

			getline(file,buffer,'\n');
			uint node_count = std::stoul(buffer);
			getline(file,buffer,'\n');
			uint edge_count = std::stoul(buffer);

			n.reserve(node_count);
			for(uint i=0; i<node_count; i++)
			{
				getline(file,buffer,'\n');
				createNode(buffer, n);
			}

			e.reserve(edge_count);
			for(uint j=0; j<edge_count; j++)
			{
				getline(file,buffer,'\n');
				createEdge(buffer, e);
			}
			file.close();

			return true;
		}

		return false;
	}


	void createNodeRGB(const std::string& input_string, std::vector<Node_RGB>& n)
	{
		std::string lat, lon, r, g, b, a;

		std::stringstream ss(input_string);
		ss >> lat >> lon >> r >> g >> b >> a;
		
		int av;
		try {
			av = std::stoi(a);
		}
		catch (const std::invalid_argument & e) {
			av = 255;
		};

		n.emplace_back(std::stof(lat), std::stof(lon), std::stoi(r), std::stoi(g), std::stoi(b), av);
	}

	void createEdgeRGB(const std::string& input_string, std::vector<Edge_RGB>& e)
	{
		std::string source, target, r, g, b, a;

		std::stringstream ss(input_string);
		ss >> source >> target >> r >> g >> b >> a;
		
		int av;
		try {
			av = std::stoi(a);
		}
		catch (const std::invalid_argument & e) {
			av = 255;
		};

		e.emplace_back(std::stoul(source), std::stoul(target), std::stoi(r), std::stoi(g), std::stoi(b), av);
	}

	void createTriangleRGB(const std::string& input_string, std::vector<Triangle_RGB>& t)
	{
		std::string v1, v2, v3, r, g, b, a;

		std::stringstream ss(input_string);
		ss >> v1 >> v2 >> v3 >> r >> g >> b >> a;
		
		int av;
		try {
			av = std::stoi(a);
		}
		catch (const std::invalid_argument & e) {
			av = 255;
		};

		t.emplace_back(std::stoul(v1), std::stoul(v2), std::stoul(v3), std::stoi(r), std::stoi(g), std::stoi(b), av);
	}

	bool parseTxtTriangleGraphFile(const std::string& graphfile, std::vector<Node_RGB>& n, std::vector<Edge_RGB>& e, std::vector<Triangle_RGB>& t)
	{
		std::string buffer;
		std::ifstream file;

		file.open(graphfile.c_str(), std::ios::in);

		if( file.is_open())
		{
			file.seekg(0, std::ios::beg);

			getline(file,buffer,'\n');
			uint node_count = std::stoul(buffer);
			getline(file,buffer,'\n');
			uint edge_count = std::stoul(buffer);
			getline(file,buffer,'\n');
			uint triangle_count = std::stoul(buffer);

			n.reserve(node_count);
			for(uint i=0; i<node_count; i++)
			{
				getline(file,buffer,'\n');
				createNodeRGB(buffer, n);
			}

			e.reserve(edge_count);
			for(uint j=0; j<edge_count; j++)
			{
				getline(file,buffer,'\n');
				createEdgeRGB(buffer, e);
			}

			t.reserve(triangle_count);
			for(uint k=0; k<triangle_count; k++)
			{
				getline(file,buffer,'\n');
				createTriangleRGB(buffer, t);
			}

			file.close();

			return true;
		}

		return false;
	}


	void createCollisionSphere(const std::string& input_string, std::vector<CollisionSphere>& s, std::map<uint,uint>& id_map)
	{
		std::string id, lat, lon, rad, prio, ct, cpId;

		std::stringstream ss(input_string);
		ss >> id >> lat >> lon >> rad >> prio >> ct >> cpId;

		id_map.insert(std::pair<uint,uint>(std::stoul(id),s.size()));
		s.emplace_back(std::stoul(id), std::stod(lat), std::stod(lon), std::stod(rad), std::stoul(prio), std::stod(ct), std::stoul(cpId));
	}

	bool parseTxtCollisionSpheresFile(const std::string& filename, std::vector<CollisionSphere>& s, std::map<uint,uint>& id_map)
	{
		std::string buffer;
		std::ifstream file;

		file.open(filename.c_str(), std::ios::in);

		if( file.is_open())
		{
			file.seekg(0, std::ios::beg);

			getline(file,buffer,'\n');
			uint sphere_count = std::stoul(buffer);

			s.reserve(sphere_count);
			for(uint i=0; i<sphere_count; i++)
			{
				getline(file,buffer,'\n');
				createCollisionSphere(buffer, s, id_map);
			}

			file.close();

			return true;
		}

		return false;
	}


	void parseTextureList(const std::string& texture_list_path, std::vector<std::string>& texture_paths)
	{
		std::string buffer;
		std::ifstream file;

		file.open(texture_list_path.c_str(), std::ios::in);

		if( file.is_open())
		{
			file.seekg(0, std::ios::beg);

			while(!file.eof())
			{
				getline(file,buffer,'\n');

				if( !buffer.empty() && (buffer.compare(0,2,"//") != 0) )
					texture_paths.push_back(buffer);
			}
		}
	}
}

/*
 * Camera (for OpenGL) orbiting a sphere that is centered on the origin
 */
struct OrbitalCamera
{
	OrbitalCamera() : longitude(0.0f), latitude(0.0f), orbit(5.0f), near(0.01f), far(10.0f), fovy(1.0), aspect_ratio(1.0) {}
	~OrbitalCamera() {}

	float longitude;
	float latitude;
	float orbit;

	float near;
	float far;
	float fovy;
	float aspect_ratio;

	void moveInOrbit(float delta_lat, float delta_lon, float delta_height)
	{
		latitude += delta_lat;
		longitude += delta_lon;

		if(longitude < -180.0f)
			longitude = 360.0f + longitude;

		if(longitude > 180.0f)
			longitude = -180.0f + (longitude-180.0f);

		//m_orbit += delta_height;

		orbit =  std::max(1.0f+0.000025f,orbit + delta_height);

		updateViewMatrix();
	}

	Math::Mat4x4 view_matrix;
	Math::Mat4x4 projection_matrix;

	void updateProjectionMatrix()
	{
		float f = 1.0f / std::tan(fovy / 2.0f);
        float nf = 1.0f / (near - far);
		projection_matrix[0] = f / aspect_ratio;
		projection_matrix[1] = 0.0f;
		projection_matrix[2] = 0.0f;
		projection_matrix[3] = 0.0f;
		projection_matrix[4] = 0.0f;
		projection_matrix[5] = f;
		projection_matrix[6] = 0.0f;
		projection_matrix[7] = 0.0f;
		projection_matrix[8] = 0.0f;
		projection_matrix[9] = 0.0f;
		projection_matrix[10] = (far + near) * nf;
		projection_matrix[11] = -1.0f;
		projection_matrix[12] = 0.0f;
		projection_matrix[13] = 0.0f;
		projection_matrix[14] = (2.0f * far * near) * nf;
		projection_matrix[15] = 0.0f;
	}

	void updateViewMatrix()
	{
		float lat_sin = sin( (PI/180.0f) * latitude);
		float lon_sin = sin( (PI/180.0f) * longitude);

		float lat_cos = cos( (PI/180.0f) * latitude);
		float lon_cos = cos( (PI/180.0f) * longitude);

		float camera_position[3];
		camera_position[0] = (lon_sin * lat_cos * orbit);
		camera_position[1] = (lat_sin * orbit);
		camera_position[2] = (lat_cos * lon_cos * orbit);

		Math::Mat4x4 lat_rotation(std::array<float,16>({1.0, 0.0, 0.0, 0.0,
								0.0, lat_cos, -lat_sin, 0.0,
								0.0, lat_sin, lat_cos, 0.0,
								0.0, 0.0, 0.0, 1.0}));

		Math::Mat4x4 lon_rotation(std::array<float,16>({lon_cos, 0.0, -lon_sin, 0.0,
								0.0, 1.0, 0.0, 0.0,
								lon_sin, 0.0, lon_cos, 0.0,
								0.0, 0.0, 0.0 , 1.0}));

		Math::Mat4x4 rotation_matrix = lon_rotation * lat_rotation;

		rotation_matrix = rotation_matrix.inverse();

		Math::Mat4x4 translation_matrix(std::array<float,16>({1.0, 0.0, 0.0, 0.0,
										0.0, 1.0, 0.0, 0.0,
										0.0, 0.0, 1.0, 0.0,
										-camera_position[0], -camera_position[1], -camera_position[2], 1.0}));

		view_matrix = rotation_matrix * translation_matrix;
	}

	GeoBoundingBox computeVisibleArea()
	{
		GeoBoundingBox bbox;

		// computer euklid. position of camera
		float lat_sin = sin( (PI/180.0f) * latitude);
		float lon_sin = sin( (PI/180.0f) * longitude);
	
		float lat_cos = cos( (PI/180.0f) * latitude);
		float lon_cos = cos( (PI/180.0f) * longitude);
	
		float r = orbit;
	
		Math::Vec3 world_position( lon_sin * lat_cos * r,
									lat_sin * r,
									lat_cos * lon_cos * r );

		// Build camera vector space
		Math::Vec3 front_vec =  normalize(-1.0 * world_position);
		Math::Vec3 right_vec = normalize(cross(front_vec, Math::Vec3(0.0f,1.0f,0.0f) ));
		Math::Vec3 up_vec = normalize(cross(right_vec,front_vec));

		// generate rays from the camera position along the right and upper frustrum plane
		Math::Vec3 upper_ray = normalize(front_vec + (std::tan(fovy*0.5f) * up_vec));
		Math::Vec3 right_ray = normalize(front_vec + (std::tan(fovy*0.5f)*aspect_ratio * right_vec));

		// intersect with unit sphere

		Math::Vec3 upper_intersection;
		float a = dot(upper_ray, upper_ray);
		float b = 2.0f * dot(upper_ray, world_position);
		float c = dot(world_position, world_position) - 1.0f * 1.0f;
		float discr = b * b - 4.0f * a * c;
		if (discr < 0.0f) {
			// no intersection, use tangent point from camera to sphere in up direction
			upper_intersection = up_vec;
		}
		else
		{
			float lambda = (-b - sqrt(discr)) / (2.0f * a);
			upper_intersection = world_position + (lambda * upper_ray); 
		}

		Math::Vec3 right_intersection;
		a = dot(right_ray, right_ray);
		b = 2.0f * dot(right_ray, world_position);
		c = dot(world_position, world_position) - 1.0f * 1.0f;
		discr = b * b - 4.0f * a * c;
		if (discr < 0.0f) {
			// no intersection, use tangent point from camera to sphere in right direction
			right_intersection = right_vec;
		}
		else
		{
			float lambda = (-b - sqrt(discr)) / (2.0f * a);
			right_intersection = world_position + (lambda * right_ray);
		}

		// using dot product suffers from nasty precision problems...
		// compute angle between Vec(SphereCenter,Camera) and Vec(SphereCenter,RightIntersection)
		//double longitude_angle = acos(dot64(normalize(world_position),normalize(right_intersection)));

		// compute angle between Vec(SphereCenter,Camera) and Vec(SphereCenter,UpperIntersection)
		//double latitude_angle = acos(dot64(normalize(world_position),normalize(upper_intersection)));


		// convert from euklidean to geo coordinates
		float upper_intersection_lat = (180.0f * std::asin(upper_intersection.y))/PI;

		float right_intersection_lat_cos = std::cos(std::asin(right_intersection.y));
		float right_intersection_lon = (180.0f * std::asin(right_intersection.x/right_intersection_lat_cos))/PI;

		if(dot(right_intersection,Math::Vec3(0.0,0.0,-1.0)) > 0.0)
		{
			if(dot(right_intersection,Math::Vec3(1.0,0.0,0.0)) > 0.0)
				right_intersection_lon = 180.0f - right_intersection_lon;
			else
				right_intersection_lon = -180.0f - right_intersection_lon;
		}

		bbox.min_longitude = longitude - abs(right_intersection_lon - longitude);
		bbox.min_longitude = (bbox.min_longitude < -180.0f) ? 360.0f + bbox.min_longitude : bbox.min_longitude;

		bbox.max_longitude = right_intersection_lon;

		bbox.min_latitude = latitude - abs(upper_intersection_lat -latitude);
		bbox.max_latitude = upper_intersection_lat;

		return bbox;
	}
};

struct IcoSphere
{
	IcoSphere()
	{
		//TODO
	};

	IcoSphere(uint subdivision)
	{
		GLfloat x = 0.525731112119133606f;
		GLfloat z = 0.850650808352039932f;

		std::vector<Vertex_XYZ> sphere_vertices({Vertex_XYZ(-x,0.0f,z),
												Vertex_XYZ(x,0.0f,z),
												Vertex_XYZ(-x,0.0f,-z),
												Vertex_XYZ(x,0.0f,-z),
												Vertex_XYZ(0.0f,z,x),
												Vertex_XYZ(0.0f,z,-x),
												Vertex_XYZ(0.0f,-z,x),
												Vertex_XYZ(0.0f,-z,-x),
												Vertex_XYZ(z,x,0.0f),
												Vertex_XYZ(-z,x,0.0f),
												Vertex_XYZ(z,-x,0.0f),
												Vertex_XYZ(-z,-x,0.0f) });
		std::vector<unsigned int> sphere_indices({	0,4,1,	0,9,4,	9,5,4,	4,5,8,	4,8,1,
													8,10,1,	8,3,10,	5,3,8,	5,2,3,	2,7,3,
													7,10,3,	7,6,10,	7,11,6,	11,0,6,	0,1,6,
													6,1,10,	9,0,11,	9,11,2,	9,2,5,	7,2,11 });

		// Subdivide icosahedron
		for(uint subdivs=0; subdivs<subdivision; subdivs++)
		{
			std::vector<unsigned int> refined_indices;
			refined_indices.reserve( sphere_indices.size() * 3);

			for(int i=0; i<(int)sphere_indices.size(); i=i+3)
			{
				unsigned int idx1 = sphere_indices[i];
				unsigned int idx2 = sphere_indices[i+1];
				unsigned int idx3 = sphere_indices[i+2];

				Math::Vec3 newVtx1( (sphere_vertices[idx1].x + sphere_vertices[idx2].x),
								(sphere_vertices[idx1].y + sphere_vertices[idx2].y),
								(sphere_vertices[idx1].z + sphere_vertices[idx2].z) );
				newVtx1 = Math::normalize(newVtx1);

				Math::Vec3 newVtx2( (sphere_vertices[idx2].x + sphere_vertices[idx3].x),
								(sphere_vertices[idx2].y + sphere_vertices[idx3].y),
								(sphere_vertices[idx2].z + sphere_vertices[idx3].z));
				newVtx2 = Math::normalize(newVtx2);

				Math::Vec3 newVtx3( (sphere_vertices[idx3].x + sphere_vertices[idx1].x),
								(sphere_vertices[idx3].y + sphere_vertices[idx1].y),
								(sphere_vertices[idx3].z + sphere_vertices[idx1].z));
				newVtx3 = Math::normalize(newVtx3);

				unsigned int newIdx1 = sphere_vertices.size();
				sphere_vertices.push_back( Vertex_XYZ(newVtx1.x,newVtx1.y,newVtx1.z) );

				unsigned int newIdx2 = newIdx1 +1;
				sphere_vertices.push_back( Vertex_XYZ(newVtx2.x,newVtx2.y,newVtx2.z) );

				unsigned int newIdx3 = newIdx2 +1;
				sphere_vertices.push_back( Vertex_XYZ(newVtx3.x,newVtx3.y,newVtx3.z) );

				refined_indices.push_back(idx1);
				refined_indices.push_back(newIdx1);
				refined_indices.push_back(newIdx3);
				
				refined_indices.push_back(newIdx1);
				refined_indices.push_back(idx2);
				refined_indices.push_back(newIdx2);

				refined_indices.push_back(newIdx3);
				refined_indices.push_back(newIdx1);
				refined_indices.push_back(newIdx2);

				refined_indices.push_back(newIdx3);
				refined_indices.push_back(newIdx2);
				refined_indices.push_back(idx3);
			}

			sphere_indices.clear();

			sphere_indices = refined_indices;
		}
		
		index_cnt = sphere_indices.size();

		auto va_size = sizeof(Vertex_XYZ) * sphere_vertices.size();
		auto vi_size = sizeof(uint) * sphere_indices.size();

		glGenVertexArrays(1, &va_handle);
		glGenBuffers(1, &vbo_handle);
		glGenBuffers(1, &ibo_handle);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, sphere_vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, sphere_indices.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex_XYZ), 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	};
	~IcoSphere()
	{
		// delete mesh resources
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ibo_handle);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDeleteBuffers(1, &vbo_handle);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &va_handle);
	}

	GLuint va_handle;
	GLuint vbo_handle;
	GLuint ibo_handle;

	uint index_cnt;

	void draw(uint instance_cnt)
	{
		glBindVertexArray(va_handle);
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)index_cnt, GL_UNSIGNED_INT, nullptr, instance_cnt);
	}
};

/**
 * This struct essentially holds a renderable representation of a subgraph as a mesh, which is made up from
 * a set of vertices and a set of indices (the latter describing the mesh connectivity).
 * In this case the mesh uses line primitives, i.e. two succesive indices describe a single line segment.
 *
 * From a programming point of view, it makes sense to keep the three OpenGL handles required for a mesh obejct
 * organised together in a struct as most high level operations like "send the mesh data to the GPU" require
 * several OpenGL function calls and all of these handles.
 */
struct Subgraph
{
	Subgraph() : va_handle(0), vbo_handle(0), ibo_handle(0), isVisible(true), index_offsets(), line_widths() {}
	Subgraph(const Subgraph&) = delete;
	~Subgraph()
	{
		if( va_handle != 0 )
		{
			// delete mesh resources
			glBindVertexArray(va_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &ibo_handle);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glDeleteBuffers(1, &vbo_handle);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &va_handle);
		}
	}

	/* Handle for the vertex array object */
	GLuint va_handle;

	/* Handle for the vertex buffer object (allows access to vertex data in GPU memory) */
	GLuint vbo_handle;

	/* Handle for the index buffer objects (allows access to index data in GPU memory) */
	GLuint ibo_handle;

	bool isVisible;

	/* To draw lines of different type, i.e of different width seperatly but still store them
	 * in the same index buffer object, offsets into the buffer are used to only draw a subset of the index buffer
	 * in each draw call.
	 */
	std::vector<uint> index_offsets;
	/* Stores the width of each subset of lines */
	std::vector<float> line_widths;


	void loadGraphData(std::vector<Node>& nodes, std::vector<Edge>& edges)
	{
		index_offsets.clear();
		line_widths.clear();

		std::vector<Vertex> vertices;
		std::vector<uint> indices;

		// At least as many vertices as there are nodes are required
		vertices.reserve(nodes.size());
	
		// Each edge contributes two indices
		indices.reserve(edges.size()*2);
	
		// Copy geo coordinates from input nodes to vertices
		for(auto& node : nodes)
		{
			vertices.push_back(Vertex((float)node.lon,(float)node.lat));
		}
	
		std::sort(edges.begin(),edges.end(), [](Edge u, Edge v) { return u.width < v.width; } );
	
		// Copy indices from edge array to index array
		std::vector<bool> has_next(nodes.size(), false);
		std::vector<uint> next(nodes.size(),0);
		uint width = 0;
		uint counter = 0;
		for(auto& edge : edges)
		{
			uint src_id = edge.source;
			uint tgt_id = edge.target;
	
			while(has_next[src_id] && (vertices[src_id].color != edge.color))
			{
				src_id = next[src_id];
			}
	
			if(vertices[src_id].color == -1)
			{
				vertices[src_id].color = (float)edge.color;
			}
	
			if(vertices[src_id].color != edge.color)
			{
				uint next_id = (uint)vertices.size();
				vertices.push_back(Vertex(vertices[src_id].longitude,vertices[src_id].latitude));
				vertices[next_id].color = (float)edge.color;
				has_next.push_back(false);
				next.push_back(0);
	
				next[src_id] = next_id;
				has_next[src_id] = true;
				src_id = next_id;
			}
	
			while(has_next[tgt_id] && (vertices[tgt_id].color != edge.color))
			{
				tgt_id = next[tgt_id];
			}
	
			if(vertices[tgt_id].color == -1)
			{
				vertices[tgt_id].color = (float)edge.color;
			}
	
			if(vertices[tgt_id].color != edge.color)
			{
				uint next_id = (uint)vertices.size();
				vertices.push_back(Vertex(vertices[tgt_id].longitude,vertices[tgt_id].latitude));
				vertices[next_id].color = (float)edge.color;
				has_next.push_back(false);
				next.push_back(0);
	
				next[tgt_id] = next_id;
				has_next[tgt_id] = true;
				tgt_id = next_id;
			}
	
			//std::cout<<"Edge color: "<<edge.color<<std::endl;
			//std::cout<<"Source color: "<<vertices[src_id].color<<std::endl;
			//std::cout<<"Target color: "<<vertices[tgt_id].color<<std::endl;
	
			if(width != edge.width)
			{
				index_offsets.push_back(counter);
				line_widths.push_back((float)edge.width);
				width = edge.width;
			}
	
			indices.push_back(src_id);
			indices.push_back(tgt_id);
	
			counter += 2;
		}
		index_offsets.push_back( (uint)indices.size() );


		// Allocate GPU memory and send data
		if(vertices.size() < 1 || indices.size() < 1)
			return;

		auto va_size = sizeof(Vertex) * vertices.size();
		auto vi_size = sizeof(uint) * indices.size();

		if(va_handle == 0 || vbo_handle == 0 || ibo_handle == 0)
		{
			glGenVertexArrays(1, &va_handle);
			glGenBuffers(1, &vbo_handle);
			glGenBuffers(1, &ibo_handle);
		}

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, indices.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (sizeof(GL_FLOAT)*2));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//std::cout<<"GfxGraph consisting of "<<vertices.size()<<" vertices and "<<indices.size()<<" indices"<<std::endl;
	}

	void draw(float scale)
	{
		//glBindVertexArray(va_handle);
		//glDrawElements(GL_LINES, indices.size(),  GL_UNSIGNED_INT,  0 );

		glBindVertexArray(va_handle);

		for(size_t i=0; i< index_offsets.size()-1; i++)
		{
			glLineWidth(std::max(1.0f,line_widths[i] * scale));
			//glLineWidth(line_widths[i]);

			glDrawElements(GL_LINES,  index_offsets[i+1]-index_offsets[i],  GL_UNSIGNED_INT,  (void*)(index_offsets[i] * sizeof(GLuint)) );
		}
	}
};

/**
 * A graph made up from subgraphs, that can be arranged on multiple layers. Limited to rendering edges.
 * This struct primarily holds a set of subgraphs and offers the neccessary functionality to add and change subgraphs.
 */
struct Graph
{
	Graph()
	{
		prgm_handle =  createShaderProgram("../src/edge_v.glsl","../src/edge_f.glsl",{"v_geoCoords","v_color"});
	}
	~Graph()
	{
		// delete shader program
		glDeleteProgram(prgm_handle);
	}

	/**
	 * Add a new subgraph. Defaults to layer 0.
	 * \param nodes Set of nodes of the new subgraph.
	 * \param edges Set of edges of the new subgraph.
	 */
	void addSubgraph(std::vector<Node>& nodes, std::vector<Edge>& edges)
	{
		std::unique_ptr<Subgraph> subgraph(new Subgraph);
		subgraphs.push_back(std::move(subgraph));

		subgraphs.back()->loadGraphData(nodes,edges);

		auto itr = layers.insert(std::pair<uint,std::list<uint>>(0,std::list<uint>()));
		itr.first->second.push_back(subgraphs.size()-1);
	}

	/**
	 * Add a new subgraph on a given layer. If the layer index doesn't exist, a new layer is created.
	 * \param nodes Set of nodes of the new subgraph.
	 * \param edges Set of edges of the new subgraph.
	 * \param layer Layer to place the new subgraph on. If layer doesn't exist yet, it is automatically created.
	 */
	void addSubgraph(std::vector<Node>& nodes, std::vector<Edge>& edges, uint layer)
	{
		std::unique_ptr<Subgraph> subgraph(new Subgraph);
		subgraphs.push_back(std::move(subgraph));

		subgraphs.back()->loadGraphData(nodes,edges);

		auto itr = layers.insert(std::pair<uint,std::list<uint>>(layer,std::list<uint>()));
		itr.first->second.push_back(subgraphs.size()-1);
	}

	/**
	 * Set visibily of a given subgraph.
	 * \param index Target subgraph index
	 * \param visibility If set to false, subgraph will not be rendered.
	 */
	void setVisibilty(uint index, bool visibility)
	{
		if(index < subgraphs.size())
			subgraphs[index]->isVisible = visibility;
	}

	/**
	 * Set layer of a given subgraph.
	 * \param index Target subgraph index
	 * \param layer Layer that the subgraph is assigned to.
	 */
	void setLayer(uint index, uint layer)
	{
		//TODO find cleaner solution
		for(auto& layer : layers)
			for(auto itr = layer.second.begin(); itr != layer.second.end(); itr++)
				if(*itr == index)
					layer.second.erase(itr);

		auto itr = layers.insert(std::pair<uint,std::list<uint>>(layer,std::list<uint>()));
		itr.first->second.push_back(index);
	}

	/**
	 * Draw all subgraphs, that are set to visible.
	 * The draw order depends on layers and -within each layer- on edge type.
	 * \param scale Additonal scale factor for edge widths.
	 */
	void draw(OrbitalCamera& camera,float scale)
	{
		glUseProgram(prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		for(auto& layer : layers)
		{
			for(auto& subgraph_idx : layer.second)
			{
				if(subgraphs[subgraph_idx]->isVisible)
					subgraphs[subgraph_idx]->draw(scale);
			}
		}
	}

private:
	/**
	 * OpenGL handle to graph rendering shader program
	 */
	GLuint prgm_handle;

	/**
	 * Actual (Linear) storage of all subgraphs.
	 */
	std::vector<std::unique_ptr<Subgraph>> subgraphs;

	/**
	 * List of subgraphs (given by index) per layer.
	 */
	std::map<uint,std::list<uint>> layers;
};

/**
 * A graph specifically made to display nodes, edges and triangles of a triangulation of a sphere surface.
 */
struct TriangleGraph
{
	TriangleGraph()
		: triangle_prgm_handle(0), nodeEdge_prgm_handle(0), num_nodes(0),
		num_edges(0), num_triangles(0), node_va_handle(0), node_vbo_handle(0), node_ibo_handle(0),
		edge_va_handle(0), edge_vbo_handle(0), edge_ibo_handle(0),
		triangle_va_handle(0), triangle_vbo_handle(0), triangle_ibo_handle(0), show_sphere(false),
		num_sphere_indices(0), sphere_world_position(1.0f,0.0f,0.0f), sphere_scale(1.0f), sphere_target_scale(1.0f),
		sphere(4)
	{
		// Create shader programs
		triangle_prgm_handle = createShaderProgram("../src/triangleGraph_triangle_v.glsl","../src/triangleGraph_triangle_f.glsl",{"v_geoCoords","v_id","v_colour"});
		nodeEdge_prgm_handle = createShaderProgram("../src/triangleGraph_nodeEdge_v.glsl","../src/triangleGraph_nodeEdge_f.glsl",{"v_geoCoords","v_colour"});
		picking_prgm_handle = createShaderProgram("../src/triangleGraph_picking_v.glsl","../src/triangleGraph_picking_f.glsl",{"v_geoCoords","v_id","v_colour"});
		sphere_prgm_handle = createShaderProgram("../src/triangleGraph_sphere_v.glsl","../src/triangleGraph_sphere_f.glsl",{"v_position"});

		// Create framebuffer object for picking pass
		glGenFramebuffers(1, &picking_fbo_handle);
		glGenTextures(1, &picking_color_attachment_handle);

		glBindTexture(GL_TEXTURE_2D, picking_color_attachment_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, 1600, 900, 0, GL_RED_INTEGER, GL_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D,0);

		glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo_handle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, picking_color_attachment_handle, 0);

		glGenRenderbuffers(1, &picking_depth_buffer_handle);
		glBindRenderbuffer(GL_RENDERBUFFER, picking_depth_buffer_handle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1600, 900);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, picking_depth_buffer_handle);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	TriangleGraph(const TriangleGraph&) = delete;
	~TriangleGraph()
	{
		if( node_va_handle != 0 )
		{
			// delete mesh resources
			glBindVertexArray(node_va_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &node_ibo_handle);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glDeleteBuffers(1, &node_vbo_handle);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &node_va_handle);
		}

		if( edge_va_handle != 0 )
		{
			// delete mesh resources
			glBindVertexArray(edge_va_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &edge_ibo_handle);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glDeleteBuffers(1, &edge_vbo_handle);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &edge_va_handle);
		}

		if( triangle_va_handle != 0 )
		{
			// delete mesh resources
			glBindVertexArray(triangle_va_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &triangle_ibo_handle);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glDeleteBuffers(1, &triangle_vbo_handle);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &triangle_va_handle);
		}

		// delete shader program
		glDeleteProgram(triangle_prgm_handle);
		glDeleteProgram(nodeEdge_prgm_handle);
		glDeleteProgram(picking_prgm_handle);
		glDeleteProgram(sphere_prgm_handle);
	}

	GLuint triangle_prgm_handle;
	GLuint nodeEdge_prgm_handle;
	GLuint picking_prgm_handle;
	GLuint sphere_prgm_handle;

	size_t num_nodes;
	size_t num_edges;
	size_t num_triangles;

	GLuint node_va_handle;
	GLuint node_vbo_handle;
	GLuint node_ibo_handle;

	GLuint edge_va_handle;
	GLuint edge_vbo_handle;
	GLuint edge_ibo_handle;

	GLuint triangle_va_handle;
	GLuint triangle_vbo_handle;
	GLuint triangle_ibo_handle;

	GLuint picking_fbo_handle;
	GLuint picking_color_attachment_handle;
	GLuint picking_depth_buffer_handle;

	bool show_sphere;
	uint num_sphere_indices;
	Math::Vec3 sphere_world_position;
	float sphere_scale;
	float sphere_target_scale;
	std::chrono::time_point<std::chrono::high_resolution_clock> t_0;
	std::chrono::time_point<std::chrono::high_resolution_clock> t_1;

	IcoSphere sphere;

	// Need a copy of graph data for sphere creation
	std::vector<Node_RGB> nodes;
	std::vector<Triangle_RGB> triangles;

	void loadGraphData(std::vector<Node_RGB>& nodes, std::vector<Edge_RGB>& edges, std::vector<Triangle_RGB>& triangles)
	{
		num_nodes = nodes.size();
		num_edges = edges.size();
		num_triangles = triangles.size();

		this->nodes = nodes;
		this->triangles = triangles;

		//////////
		// Nodes
		//////////

		std::vector<Vertex_RGB> node_vertices;
		std::vector<uint> node_indices;

		// At least as many vertices as there are nodes are required
		node_vertices.reserve(nodes.size());
	
		// Each edge contributes two indices
		node_indices.reserve(nodes.size());

		// Copy geo coordinates from input nodes to vertices
		uint index_counter = 0;
		for(auto& node : nodes)
		{
			node_vertices.push_back(Vertex_RGB((float)node.lon,(float)node.lat, node.r, node.g, node.b, node.a));

			node_indices.push_back(index_counter++);
		}

		// Allocate GPU memory and send data
		if(node_vertices.size() < 1 || node_indices.size() < 1)
			return;

		auto va_size = sizeof(Vertex_RGB) * node_vertices.size();
		auto vi_size = sizeof(uint) * node_indices.size();

		if(node_va_handle == 0 || node_vbo_handle == 0 || node_ibo_handle == 0)
		{
			glGenVertexArrays(1, &node_va_handle);
			glGenBuffers(1, &node_vbo_handle);
			glGenBuffers(1, &node_ibo_handle);
		}

		glBindVertexArray(node_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, node_vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, node_vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node_ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, node_indices.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(node_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, node_vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex_RGB), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex_RGB), (GLvoid*) (sizeof(GL_FLOAT)*2));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//////////
		// Edges
		//////////

		std::vector<Vertex_RGB> edge_vertices;
		std::vector<uint> edge_indices;

		// At least as many vertices as there are nodes are required
		edge_vertices.reserve(nodes.size());
	
		// Each edge contributes two indices
		edge_indices.reserve(edges.size()*2);

		index_counter = 0;
		for(auto& edge : edges)
		{
			edge_vertices.push_back(Vertex_RGB(nodes[edge.source].lon,nodes[edge.source].lat,edge.r,edge.g,edge.b, edge.a));
			edge_vertices.push_back(Vertex_RGB(nodes[edge.target].lon,nodes[edge.target].lat,edge.r,edge.g,edge.b, edge.a));

			edge_indices.push_back(index_counter++);
			edge_indices.push_back(index_counter++);

			//TODO could be possible to optimize by finding vertices of the same color
		}

		// Allocate GPU memory and send data
		if(edge_vertices.size() < 1 || edge_indices.size() < 1)
			return;

		va_size = sizeof(Vertex_RGB) * edge_vertices.size();
		vi_size = sizeof(uint) * edge_indices.size();

		if(edge_va_handle == 0 || edge_vbo_handle == 0 || edge_ibo_handle == 0)
		{
			glGenVertexArrays(1, &edge_va_handle);
			glGenBuffers(1, &edge_vbo_handle);
			glGenBuffers(1, &edge_ibo_handle);
		}

		glBindVertexArray(edge_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, edge_vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, edge_vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, edge_indices.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(edge_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, edge_vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex_RGB), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex_RGB), (GLvoid*) (sizeof(GL_FLOAT)*2));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//////////////
		// Triangles
		//////////////

		std::vector<Vertex_ID_RGB> triangle_vertices;
		std::vector<uint> triangle_indices;

		// At least as many vertices as there are nodes are required
		triangle_vertices.reserve(nodes.size());
	
		// Each edge contributes two indices
		triangle_indices.reserve(triangles.size()*3);

		index_counter = 0;
		int triangle_counter = 0;
		for(auto& triangle : triangles)
		{
			triangle_vertices.push_back(Vertex_ID_RGB(nodes[triangle.v1].lon,nodes[triangle.v1].lat,triangle.r,triangle.g,triangle.b,triangle_counter, triangle.a));
			triangle_vertices.push_back(Vertex_ID_RGB(nodes[triangle.v2].lon,nodes[triangle.v2].lat,triangle.r,triangle.g,triangle.b,triangle_counter, triangle.a));
			triangle_vertices.push_back(Vertex_ID_RGB(nodes[triangle.v3].lon,nodes[triangle.v3].lat,triangle.r,triangle.g,triangle.b,triangle_counter, triangle.a));

			triangle_indices.push_back(index_counter++);
			triangle_indices.push_back(index_counter++);
			triangle_indices.push_back(index_counter++);

			triangle_counter++;
			//TODO could be possible to optimize by finding vertices of the same color
		}

		// Allocate GPU memory and send data
		if(triangle_vertices.size() < 1 || triangle_indices.size() < 1)
			return;

		va_size = sizeof(Vertex_ID_RGB) * triangle_vertices.size();
		vi_size = sizeof(uint) * triangle_indices.size();

		if(triangle_va_handle == 0 || triangle_vbo_handle == 0 || triangle_ibo_handle == 0)
		{
			glGenVertexArrays(1, &triangle_va_handle);
			glGenBuffers(1, &triangle_vbo_handle);
			glGenBuffers(1, &triangle_ibo_handle);
		}

		glBindVertexArray(triangle_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, triangle_vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, triangle_indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(triangle_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex_ID_RGB), 0);
		//	glEnableVertexAttribArray(1);
		//	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(Vertex_ID_RGB), (GLvoid*) (sizeof(GL_FLOAT)*2));
		//	glEnableVertexAttribArray(2);
		//	glVertexAttribPointer(2, 1, GL_INT, false, sizeof(Vertex_ID_RGB), (GLvoid*) ((sizeof(GL_FLOAT)*2) + (sizeof(GL_UNSIGNED_BYTE)*3)) );

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_INT, false, sizeof(Vertex_ID_RGB), (GLvoid*) (sizeof(GL_FLOAT)*2));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex_ID_RGB), (GLvoid*) ((sizeof(GL_FLOAT)*2) + sizeof(GL_INT)) );

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void draw(OrbitalCamera& camera,float triangle_transparency)
	{
		glCullFace(GL_FRONT);

		glUseProgram(triangle_prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(triangle_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(triangle_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());
		glUniform1fv(glGetUniformLocation(triangle_prgm_handle, "transparency"),1, &triangle_transparency);

		glBindVertexArray(triangle_va_handle);
		glDrawElements(GL_TRIANGLES, (GLsizei)num_triangles*3, GL_UNSIGNED_INT, nullptr );

		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);

		glUseProgram(nodeEdge_prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(nodeEdge_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(nodeEdge_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		//glLineWidth(std::max(1.0f,20.0f * scale));
		glBindVertexArray(edge_va_handle);
		glDrawElements(GL_LINES,  (GLsizei)num_edges * 2,  GL_UNSIGNED_INT, nullptr );

		//glPointSize(std::max(2.0f,15.0f * scale));
		glPointSize(5.0);
		glBindVertexArray(node_va_handle);
		glDrawElements(GL_POINTS,  (GLsizei)num_nodes,  GL_UNSIGNED_INT, nullptr );

		t_1 = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::duration<double>>(t_1-t_0).count();
		t_0 = std::chrono::high_resolution_clock::now();


		// Draw triangle front faces
		glUseProgram(triangle_prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(triangle_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(triangle_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());
		glUniform1fv(glGetUniformLocation(triangle_prgm_handle, "transparency"),1, &triangle_transparency);

		glBindVertexArray(triangle_va_handle);
		glDrawElements(GL_TRIANGLES, (GLsizei)num_triangles*3, GL_UNSIGNED_INT, nullptr );


		if(show_sphere)
		{
			glUseProgram(sphere_prgm_handle);

			
			sphere_scale = (sphere_scale < sphere_target_scale) ? (sphere_scale + sphere_target_scale * dt) : sphere_target_scale;

			Math::Mat4x4 model_matrix;
			model_matrix[0] = sphere_scale;
			model_matrix[5] = sphere_scale;
			model_matrix[10] = sphere_scale;
			model_matrix[12] = sphere_world_position.x;
			model_matrix[13] = sphere_world_position.y;
			model_matrix[14] = sphere_world_position.z;
			model_matrix[15] = 1.0f;

			glUniformMatrix4fv(glGetUniformLocation(sphere_prgm_handle, "model_matrix"), 1, GL_FALSE, model_matrix.data.data());
			glUniformMatrix4fv(glGetUniformLocation(sphere_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
			glUniformMatrix4fv(glGetUniformLocation(sphere_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());
			
			glCullFace(GL_FRONT);
			//glBindVertexArray(sphere_va_handle);
			//glDrawElements(GL_TRIANGLES, (GLsizei)num_sphere_indices, GL_UNSIGNED_INT, nullptr );

			sphere.draw(1);
		}

		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
	}

	Math::Vec3 geoToCartesian(float lon, float lat)
	{
		float lat_sin = sin( (PI/180.0) * lat);
		float lon_sin = sin( (PI/180.0) * lon);
		
		float lat_cos = cos( (PI/180.0) * lat);
		float lon_cos = cos( (PI/180.0) *lon);
		
		float r = 1.0; //6378137.0;
		
		return Math::Vec3( lon_sin * lat_cos * r,
							lat_sin * r,
							lat_cos * lon_cos * r );
	}

	void pickingPass(OrbitalCamera& camera)
	{
		glEnable(GL_DEPTH);

		glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo_handle);
		glClearColor(-1,-1,-1,-1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1600, 900);

		GLenum bufs[1];
		bufs[0] = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1,bufs);

		glUseProgram(picking_prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(picking_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(picking_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		glBindVertexArray(triangle_va_handle);
		glDrawElements(GL_TRIANGLES, (GLsizei)num_triangles*3, GL_UNSIGNED_INT, nullptr );

		glDisable(GL_DEPTH);
	}

	void placeSphere(int triangle_id)
	{
		if(triangle_id < 0)
		{
			show_sphere = false;
			return;
		}

		Math::Vec3 v1 = geoToCartesian(nodes[triangles[triangle_id].v1].lon,nodes[triangles[triangle_id].v1].lat);
		Math::Vec3 v2 = geoToCartesian(nodes[triangles[triangle_id].v2].lon,nodes[triangles[triangle_id].v2].lat);
		Math::Vec3 v3 = geoToCartesian(nodes[triangles[triangle_id].v3].lon,nodes[triangles[triangle_id].v3].lat);

		show_sphere = true;
		// http://www.ics.uci.edu/~eppstein/junkyard/circumcenter.html
		sphere_world_position = v1 + (( pow((v3-v1).length(),2.0f) * Math::cross(Math::cross((v2-v1),(v3-v1)),(v2-v1))
									+ pow((v2-v1).length(),2.0f) * Math::cross((v3-v1),Math::cross((v2-v1),(v3-v1))) )
									/ (2.0f * pow( Math::cross((v2-v1),(v3-v1)).length() ,2.0f)));
		sphere_target_scale = (sphere_world_position - v1).length();
		sphere_scale = 0.0;
	}
};

struct CollisionSpheres
{
	CollisionSpheres() : 
		sphere_cnt(0),
		grow_factor(0.0),
		highest_priority(0),
		show_eliminations_factor(0.0),
		current_timestep(0),
		time(0.0),
		ss_prgm_handle(0),
		cs_prgm_handle(0),
		data_texture_handle(0),
		priority_texture_handle(0),
		ss_mesh(6),
		cs_mesh(3)
	{
		// Create shader progams
		ss_prgm_handle = createShaderProgram("../src/surface_sphere_v.glsl","../src/surface_sphere_f.glsl",{"v_position"});
		cs_prgm_handle = createShaderProgram("../src/collision_sphere_v.glsl","../src/collision_sphere_f.glsl",{"v_position"});
	};

	~CollisionSpheres() {};

	std::vector<CollisionSphere> collision_sphere_data;
	uint sphere_cnt;
	float grow_factor;
	uint highest_priority;
	float show_eliminations_factor;

	uint current_timestep;
	float time;

	GLuint ss_prgm_handle;			///< Surface sphere program handle
	GLuint cs_prgm_handle;			///< Collision spheres program handle
	GLuint data_texture_handle;		///< Collision spheres data texture handle
	GLuint priority_texture_handle; ///< Additional collision spheres data texture handle

	IcoSphere ss_mesh;				///< Surface sphere mesh
	IcoSphere cs_mesh;				///< Collision spheres mesh

	
	Math::Vec3 geoToCartesian(float lon, float lat)
	{
		float lat_sin = sin( (PI/180.0) * lat);
		float lon_sin = sin( (PI/180.0) * lon);
		
		float lat_cos = cos( (PI/180.0) * lat);
		float lon_cos = cos( (PI/180.0) *lon);
		
		float r = 1.0; //6378137.0;
		
		return Math::Vec3( lon_sin * lat_cos * r,
							lat_sin * r,
							lat_cos * lon_cos * r );
	}

	void moveTimestep(int direction)
	{

		current_timestep = (static_cast<int>(current_timestep)+direction < 0) ? 0u : std::min(current_timestep+direction,sphere_cnt);

		time = collision_sphere_data[current_timestep].collision_time;
	}
	
	static constexpr double DEG_TO_RAD = 0.017453292519943295769236907684886;
	static constexpr double EARTH_RADIUS_IN_CENTIMETERS = 637279756.0856;
	
	double haversine(const CollisionSphere & from, const CollisionSphere & to) {
		double lat_arc = (from.lat- to.lat) * DEG_TO_RAD;
		double lon_arc = (from.lon - to.lon) * DEG_TO_RAD;
		double lat_h = sin(lat_arc * 0.5);
		lat_h *= lat_h;
		double lon_h = sin(lon_arc * 0.5);
		lon_h *= lon_h;
		double tmp = cos(from.lat*DEG_TO_RAD) * cos(to.lat*DEG_TO_RAD);
		return 2.0 * asin(sqrt(lat_h + tmp*lon_h));
	}

	void loadData(std::vector<CollisionSphere>& spheres, std::map<uint,uint>& id_map)
	{
		// Copy data
		collision_sphere_data = spheres;

		sphere_cnt = spheres.size() - 4;

		current_timestep = (sphere_cnt > 1000) ? sphere_cnt - 1000 : 0;
		time = spheres[current_timestep].collision_time;

		// Extract information from collision sphere and store in texture for rendering
		uint tx_height = std::ceil( static_cast<float>(sphere_cnt) / 8096.0f );
		std::vector<float> tx_data(8096*tx_height*4,0.0f);
		std::vector<float> prio_tx_data(8096*tx_height*2,0.0f);

		float min_priority = static_cast<float>(spheres[sphere_cnt-1].priority);

		for(int i = (sphere_cnt-1); i >= 0; i=i-1)
		{
			tx_data[i*4 + 0] = (spheres[i].lat);
			tx_data[i*4 + 1] = (spheres[i].lon);

			// approximate radius in 3d space
			auto search = id_map.find(spheres[i].collision_partner_id);
			assert(search != id_map.end());
			uint q_idx = search->second;
			assert(q_idx < spheres.size());

			Math::Vec3 p = geoToCartesian(spheres[i].lon,spheres[i].lat);
			Math::Vec3 q = geoToCartesian(spheres[q_idx].lon,spheres[q_idx].lat);
			double rp = spheres[i].radius;
			double rq = spheres[q_idx].radius;

// 			float d = (p-q).length();
			double d = haversine(spheres[i], spheres[q_idx]);
			

			double bl = d * (rp/(rp+rq));
			double radius = 2.0 * ::sin(bl/2.0);

			tx_data[i*4 + 2] = (radius);
			tx_data[i*4 + 3] = (spheres[i].collision_time);

			min_priority = std::min(min_priority,static_cast<float>(spheres[i].priority));
			prio_tx_data[i*2 +0] = static_cast<float>(spheres[i].priority);
			prio_tx_data[i*2 +1] = min_priority;

			highest_priority = std::max(highest_priority,spheres[i].priority);
		}

		if(data_texture_handle == 0)
			glGenTextures(1, &data_texture_handle);
		glBindTexture(GL_TEXTURE_2D, data_texture_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 8096, tx_height, 0, GL_RGBA, GL_FLOAT, tx_data.data());
		glBindTexture(GL_TEXTURE_2D,0);

		if(priority_texture_handle == 0)
			glGenTextures(1, &priority_texture_handle);
		glBindTexture(GL_TEXTURE_2D, priority_texture_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 8096, tx_height, 0, GL_RG, GL_FLOAT, prio_tx_data.data());
		glBindTexture(GL_TEXTURE_2D,0);
		
	}

	void draw(OrbitalCamera& camera)
	{
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);

		// draw planetary surface sphere
		glUseProgram(ss_prgm_handle);
		glUniformMatrix4fv(glGetUniformLocation(ss_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(ss_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());
		ss_mesh.draw(1);

		// draw collision spheres
		glUseProgram(cs_prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(cs_prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(cs_prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, data_texture_handle);
		int zero = 0;
		glUniform1iv(glGetUniformLocation(cs_prgm_handle, "data_tx2D"),1,&zero);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, priority_texture_handle);
		int one = 1;
		glUniform1iv(glGetUniformLocation(cs_prgm_handle, "priority_data_tx2D"),1,&one);

		glUniform1fv(glGetUniformLocation(cs_prgm_handle, "time"),1,  &time);
		float zpo = 0.0001;
		glUniform1fv(glGetUniformLocation(cs_prgm_handle, "grow_factor"),1,  &zpo);
		glUniform1iv(glGetUniformLocation(cs_prgm_handle, "mode"),1, &Controls::collisionSphere_mode);
		glUniform1uiv(glGetUniformLocation(cs_prgm_handle, "id_offset"),1,&current_timestep);
		glUniform1uiv(glGetUniformLocation(cs_prgm_handle, "highest_priority"),1,&highest_priority);

		uint remaining_sphere_cnt = sphere_cnt-current_timestep;
		int show_elimination_idx = remaining_sphere_cnt*show_eliminations_factor;
		glUniform1iv(glGetUniformLocation(cs_prgm_handle, "show_elimination_idx"),1,  &show_elimination_idx);

		cs_mesh.draw(remaining_sphere_cnt);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
	}
};

/**
 * Collection of text labels on the map.
 */
struct TextLabels
{
	std::array<float,255> u;
	std::array<float,255> v;

	TextLabels()
	{
		//	std::vector<std::basic_string<wchar_t>> atlas_rows;
		//	
		//	atlas_rows.push_back( L"ABCDEFGHIJKLMN");
		//	atlas_rows.push_back( L"OPQRSTUVWXYZab");
		//	atlas_rows.push_back( L"cdefghijklmnop");
		//	atlas_rows.push_back( L"qrstuvwxyz1234");
		//	atlas_rows.push_back( L"567890&@.,?!'\"");
		//	atlas_rows.push_back( L"\"()*-_ßöäü");

		// Using u16string, hoping to gain support for umlauts
		std::vector< std::vector<uint16_t> > u16_atlas_rows;

		u16_atlas_rows.push_back(toUnicodePoints( "ABCDEFGHIJKLMN" ));
		u16_atlas_rows.push_back(toUnicodePoints( "OPQRSTUVWXYZab" ));
		u16_atlas_rows.push_back(toUnicodePoints( "cdefghijklmnop" ));
		u16_atlas_rows.push_back(toUnicodePoints( "qrstuvwxyz1234" ));
		u16_atlas_rows.push_back(toUnicodePoints( "567890&@.,?!'\"" ));
		u16_atlas_rows.push_back(toUnicodePoints( "\"()*-_ßöäüÖÄÜ" ));

		float u_value = 1.0f/16.0f;
		float v_value = 5.0f/6.0f;

		for(auto& s : u16_atlas_rows)
		{
			for(auto c : s)
			{
				u[c] = u_value;
				u_value += 1.0f/16.0f;
				v[c] = v_value;

				//std::cout<<"Char: "<< (char16_t) c<<" Code: "<<(int)c<<" u: "<<u[c]<<" v: "<<v[c]<<std::endl;
			}

			u_value = 1.0f/16.0f;
			v_value -= 1.0f/6.0f;
		}

		// Generate vertex and index buffer array and allocate enough space for 10000 labels
		glGenVertexArrays(1, &va_handle);
		glGenBuffers(1, &vbo_handle);
		glGenBuffers(1, &ibo_handle);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 16 * 10000, NULL, GL_STATIC_DRAW); //each label uses vertex data worth 16 floats
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* 6 * 10000, NULL, GL_STATIC_DRAW); //each label uses 6 indices
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(GL_FLOAT)*4, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GL_FLOAT)*4, (GLvoid*) (sizeof(GL_FLOAT)*2));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Load text label shader program
		prgm_handle = createShaderProgram("../src/textLabel_v.glsl","../src/textLabel_f.glsl",{"v_position","v_uv"});

		// Load font atlas
		unsigned long begin_pos;
		int x_dim, y_dim;
		char* img_data;
		ResourceLoader::readPpmHeader("../resources/font_atlas.ppm", begin_pos, x_dim, y_dim);
		img_data = new char[x_dim * y_dim * 3];
		ResourceLoader::readPpmData("../resources/font_atlas.ppm", img_data, begin_pos, x_dim, y_dim);

		glGenTextures(1, &font_atlas_handle);
		//assert(font_atlas_handle > 0);
		glBindTexture(GL_TEXTURE_2D, font_atlas_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x_dim, y_dim, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,3);
		glBindTexture(GL_TEXTURE_2D,0);

		delete[] img_data;

	}
	/* Structs and classes that free OpenGL resources within their destructor
	 * should be non-copyable to prevent bad stuff from happening
	 */
	TextLabels(const TextLabels&) = delete;
	~TextLabels()
	{
		// delete mesh resources
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ibo_handle);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDeleteBuffers(1, &vbo_handle);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &va_handle);

		// delete GLSL program
		glDeleteProgram(prgm_handle);

		// delete font atlas
		glDeleteTextures(1,&font_atlas_handle);

		for(GLuint tx_handle : text_texture_handles)
			glDeleteTextures(1,&tx_handle);
	}

	GLuint va_handle;

	GLuint vbo_handle;

	GLuint ibo_handle;

	GLuint prgm_handle;

	GLuint font_atlas_handle;

	/** Total number of labels */
	uint num_labels = 0;

	/** Geo-Cooridnates of each label */
	std::vector<float> geoCoordinates;
	/** String of each label encoded as look-up-texture for font altas */
	std::vector<GLuint> text_texture_handles;
	/** Number of characters of each label */
	std::vector<unsigned int> lengths;
	/** Relative scale of each label */
	std::vector<float> scales;
	/** Visibility of each label i.e. rendered or not */
	std::vector<bool> visibility;
	/** Index array offset to beginning of label */
	std::vector<size_t> offsets;

	void addLabel(std::string label_text, float latitude, float longitude, float scale)
	{
		if(num_labels < 10000)
		{

		num_labels++;

		// Create basic quad mesh for rendering characters
		float x_min = (-0.03f * label_text.length());
		float x_max = (0.03f * label_text.length());
		std::array< float, 16 > vertex_array = {{ x_min,-0.1f,0.0f,0.0f,
												x_min,0.1f,0.0f,1.0f,
												x_max,0.1f,1.0f,1.0f,
												x_max,-0.1f,1.0f,0.0f }};
		uint offset = num_labels * 4;
		std::array< GLuint, 6 > index_array = {{ offset+0,offset+2,offset+1,offset+2,offset+0,offset+3 }};

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferSubData(GL_ARRAY_BUFFER, num_labels * sizeof(vertex_array), sizeof(vertex_array), vertex_array.data()); //each label uses vertex data worth 16 floats
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, num_labels * sizeof(index_array), sizeof(index_array), index_array.data()); //each label uses 6 indices
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		offsets.push_back(num_labels * sizeof(index_array));


		geoCoordinates.push_back(longitude);
		geoCoordinates.push_back(latitude);

		// convert string to text texture (i.e. character to uv position in texture atlas)
		float* data = new float[label_text.length()*2];
		//	for(size_t i=0; i<label_text.length(); i++)
		//	{
		//		std::cout<<"Char: "<<label_text[i]<<" Code: "<<(uint)label_text[i]<<" u: "<<u[(int)label_text[i]]<<" v: "<<v[(int)label_text[i]]<<std::endl;
		//	
		//		data[i*2] = u[label_text[i]];
		//		data[i*2 + 1] = v[label_text[i]];
		//	}

		std::vector<uint16_t> u16_label_text = toUnicodePoints(label_text);

		uint i = 0;
		for(auto& c : u16_label_text)
		{
			//std::cout<<"Char: "<<c<<" Code: "<<(int)c<<" u: "<<u[c]<<" v: "<<v[c]<<std::endl;

			data[i++] = u[c];
			data[i++] = v[c];
		}

		GLuint texture_handle = 0;
		glGenTextures(1, &texture_handle);
		//assert(font_atlas_handle > 0);
		glBindTexture(GL_TEXTURE_2D, texture_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, label_text.length(), 1, 0, GL_RG, GL_FLOAT, data);
		glBindTexture(GL_TEXTURE_2D,0);

		text_texture_handles.push_back(texture_handle);

		lengths.push_back(label_text.length());
		scales.push_back(scale);
		visibility.push_back(true);

		}
	}

	void draw(OrbitalCamera& camera)
	{
		glUseProgram(prgm_handle);

		// bind font atlas texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,font_atlas_handle);
		int zero = 0;
		glUniform1iv(glGetUniformLocation(prgm_handle,"fontAtlas_tx2D"),1,&zero);

		// set label independent uniforms
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		glBindVertexArray(va_handle);

		for(size_t i=0; i<visibility.size(); i++)
		{
			if(visibility[i])
			{
				glUniform2fv(glGetUniformLocation(prgm_handle,"label_geoCoords"),1,&geoCoordinates[i*2]);
				float charCount = (float)lengths[i];
				glUniform1fv(glGetUniformLocation(prgm_handle,"label_charCount"),1,&charCount);
				glUniform1fv(glGetUniformLocation(prgm_handle,"label_scale"),1,&scales[i]);

				// bind content texture
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D,text_texture_handles[i]);
				int one = 1;
				glUniform1iv(glGetUniformLocation(prgm_handle,"label_text_tx2D"),1,&one);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) offsets[i] );
			}
		}
	}
};

struct Icons
{
	enum Icon
	{
		ATM2,
		BUS,
		INFORMATION,
		PARKINGGARAGE,
		RESTAURANT,
		SIGHT2,
		TAXI,
		TELEPHONE,
		TOILETS,
		TOOLS,
		TRASH,
		WIFI,
		HOSPITAL,
		CINEMA,
		THEATER
	};

	std::array<float,255> u;
	std::array<float,255> v;

	GLuint va_handle;
	GLuint vbo_handle;
	GLuint ibo_handle;

	GLuint prgm_handle;

	GLuint icon_atlas_handle;

	/** Total number of labels */
	uint icon_cnt = 0;

	/** Geo-Cooridnates of each label */
	std::vector<float> geoCoordinates;
	/** Relative scale of each icon */
	std::vector<float> scales;
	/** Visibility of each icon i.e. rendered or not */
	std::vector<bool> visibility;
	/** */
	std::vector<float> atlas_uv;

	Icons()
	{
		// Load text label shader program
		prgm_handle = createShaderProgram("../src/icon_v.glsl","../src/icon_f.glsl",{"v_position","v_uv"});

		// Load icon atlas
		unsigned long begin_pos;
		int x_dim, y_dim;
		char* img_data;
		ResourceLoader::readPpmHeader("../resources/icon_atlas.ppm", begin_pos, x_dim, y_dim);
		img_data = new char[x_dim * y_dim * 3];
		ResourceLoader::readPpmData("../resources/icon_atlas.ppm", img_data, begin_pos, x_dim, y_dim);

		glGenTextures(1, &icon_atlas_handle);
		//assert(font_atlas_handle > 0);
		glBindTexture(GL_TEXTURE_2D, icon_atlas_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x_dim, y_dim, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glBindTexture(GL_TEXTURE_2D,0);

		// Create proxy geometry for icons
		float x_min = -0.086f;
		float x_max = 0.086f;
		std::array< float, 16 > vertex_array = {{ x_min,-0.1f,0.0f,0.0f,
												x_min,0.1f,0.0f,1.0f,
												x_max,0.1f,1.0f,1.0f,
												x_max,-0.1f,1.0f,0.0f }};
		std::array< GLuint, 6 > index_array = {{ 0,2,1,2,0,3 }};

		glGenVertexArrays(1, &va_handle);
		glGenBuffers(1, &vbo_handle);
		glGenBuffers(1, &ibo_handle);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 16, vertex_array.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* 6, index_array.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(GL_FLOAT)*4, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GL_FLOAT)*4, (GLvoid*) (sizeof(GL_FLOAT)*2));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~Icons()
	{
	}

	void addIcon(Icon icon, float latitude, float longitude, float scale)
	{
		icon_cnt++;

		geoCoordinates.push_back(longitude);
		geoCoordinates.push_back(latitude);

		float uv_x = (1.0f/9.0f) + 2.0f * (1.0f/9.0f) * (icon % 4);
		float uv_y = (1.0f/9.0f) + 2.0f * (1.0f/9.0f) * std::floor(icon / 4.0f);

		atlas_uv.push_back(uv_x);
		atlas_uv.push_back(uv_y);

		scales.push_back(scale);

		visibility.push_back(true);
	}

	void draw(OrbitalCamera& camera)
	{
		glUseProgram(prgm_handle);

		// bind font atlas texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,icon_atlas_handle);
		int zero = 0;
		glUniform1iv(glGetUniformLocation(prgm_handle,"iconAtlas_tx2D"),1,&zero);

		// set label independent uniforms
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		glBindVertexArray(va_handle);

		for(size_t i=0; i<visibility.size(); i++)
		{
			if(visibility[i])
			{
				glUniform2fv(glGetUniformLocation(prgm_handle,"icon_geoCoords"),1,&geoCoordinates[i*2]);
				glUniform1fv(glGetUniformLocation(prgm_handle,"icon_scale"),1,&scales[i]);
				glUniform2fv(glGetUniformLocation(prgm_handle,"icon_atlasUV"),1,&atlas_uv[i*2]);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
			}
		}
	}
};

/**
 * Collection of polygons on the map.
 */
struct Polygons
{
	Polygons()
	{
		// Load polygon shader program
		prgm_handle = createShaderProgram("../src/polygon_v.glsl","../src/polygon_f.glsl",{"v_geoCoords"});

		// Load fallback background texture
		unsigned long begin_pos;
		int x_dim, y_dim;
		char* img_data;
		ResourceLoader::readPpmHeader("../resources/notx.ppm", begin_pos, x_dim, y_dim);
		img_data = new char[x_dim * y_dim * 3];
		ResourceLoader::readPpmData("../resources/notx.ppm", img_data, begin_pos, x_dim, y_dim);

		glGenTextures(1, &notx_texture_handle);
		glBindTexture(GL_TEXTURE_2D, notx_texture_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x_dim, y_dim, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
		glBindTexture(GL_TEXTURE_2D,0);

		delete[] img_data;

		// Load background textures
		std::vector<std::string> texture_paths;
		Parser::parseTextureList("../resources/texture_list",texture_paths);

		for(auto& texture_path : texture_paths)
		{
			unsigned long begin_pos;
			int x_dim, y_dim;
			char* img_data;
			ResourceLoader::readPpmHeader(("../resources/" + texture_path).c_str(), begin_pos, x_dim, y_dim);
			img_data = new char[x_dim * y_dim * 3];
			ResourceLoader::readPpmData(("../resources/" + texture_path).c_str(), img_data, begin_pos, x_dim, y_dim);

			texture_handles.push_back(0);

			glGenTextures(1, &texture_handles.back());
			glBindTexture(GL_TEXTURE_2D, texture_handles.back());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x_dim, y_dim, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
			glBindTexture(GL_TEXTURE_2D,0);

			delete[] img_data;
		}

		index_offsets.push_back(0);
	}

	Polygons(const Polygons& cpy) = delete;

	~Polygons()
	{
		// delete mesh resources
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ibo_handle);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDeleteBuffers(1, &vbo_handle);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &va_handle);

		// delete GLSL program
		glDeleteProgram(prgm_handle);

		// delete textures
		for(GLuint tx_handle : texture_handles)
			glDeleteTextures(1,&tx_handle);
	}

	/**
	 * Adds a new polygon on the map froma set of nodes defining the boundary of the polygon.
	 * Nodes have to be given in counter-clockwise order! Furthermore, the polygon musn't cross the +180°/-180° longitude border.
	 * Furthermore, the polygon is automatically closed using the first and last node.
	 * \param poly_border A set of nodes in geo coordinates defining the polygon boundary (counter-clockwise order required).
	 * \param texture_index Index of the texture that should be used for the polygon
	 */
	void addPolygon(const std::vector<Node>& poly_border, int texture_index)
	{
		std::vector<Math::Vec2> triangulation_vertices;
		std::vector<uint> triangulation_indices;
		uint index_offset = static_cast<uint>(vertices.size()/2.0f);

		for(auto& node : poly_border)
		{
			vertices.push_back(node.lon);
			vertices.push_back(node.lat);
			triangulation_vertices.push_back(Math::Vec2(node.lon,node.lat));
		}

		triangulation_indices = computeTriangulation(triangulation_vertices);


		for(auto& index : triangulation_indices)
		{
			indices.push_back(index + index_offset);
		}

		index_offsets.push_back(static_cast<uint>(triangulation_indices.size())+index_offsets.back());


		polygon_texture_idx.push_back(texture_index);


		if(vertices.size() < 1 || indices.size() < 1)
			return;

		auto va_size = sizeof(float) * vertices.size();
		auto vi_size = sizeof(uint) * indices.size();

		if(va_handle == 0 || vbo_handle == 0 || ibo_handle == 0)
		{
			glGenVertexArrays(1, &va_handle);
			glGenBuffers(1, &vbo_handle);
			glGenBuffers(1, &ibo_handle);
		}

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(GL_FLOAT)*2, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/**
	 * Draw all polygons.
	 */
	void draw(OrbitalCamera& camera)
	{
		glUseProgram(prgm_handle);

		glActiveTexture(GL_TEXTURE0);
		int zero = 0;
		glUniform1iv(glGetUniformLocation(prgm_handle,"background_tx2D"),1,&zero);

		// set label independent uniforms
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		glBindVertexArray(va_handle);

		for(size_t i=0; i< index_offsets.size()-1; i++)
		{
			int tx_idx = polygon_texture_idx[i];
			if( tx_idx >= (int)texture_handles.size() || tx_idx < 0 )
				glBindTexture(GL_TEXTURE_2D, notx_texture_handle);
			else
				glBindTexture(GL_TEXTURE_2D, texture_handles[ polygon_texture_idx[i] ]);

			glDrawElements(GL_TRIANGLES,  index_offsets[i+1]-index_offsets[i],  GL_UNSIGNED_INT,  (void*)(index_offsets[i] * sizeof(GLuint)) );
		}
	}

private:
	// Vertices for all polygons
	std::vector<float> vertices;

	// Indices for alle polygons
	std::vector<uint> indices;

	std::vector<uint> index_offsets;

	GLuint va_handle;

	GLuint vbo_handle;

	GLuint ibo_handle;

	GLuint prgm_handle;

	GLuint notx_texture_handle;

	std::vector<int> polygon_texture_idx;

	std::vector<GLuint> texture_handles;

	static bool PointsOnSameLineSide(Math::Vec2 p1, Math::Vec2 p2, Math::Vec2 a, Math::Vec2 b)
	{
		Math::Mat2x2 lrot({0.0f,1.0f,-1.0f,0.0f});
		double ab1 = Math::dot( lrot*(b-a), p1-a);
		double ab2 = Math::dot( lrot*(b-a), p2-a);
		return ab1*ab2 >= 0.0;
	}
	static bool vec2InTriangle(Math::Vec2 v, Math::Vec2 pre, Math::Vec2 cur, Math::Vec2 nex)
	{
		return
			PointsOnSameLineSide(v, pre, cur, nex) &&
			PointsOnSameLineSide(v, cur, nex, pre) &&
			PointsOnSameLineSide(v, nex, pre, cur);
	}
	static bool vec2LeftOfLine(Math::Vec2 v, Math::Vec2 a, Math::Vec2 b)
	{
		Math::Mat2x2 lrot({0,1,-1,0});
		return Math::dot( lrot*(b-a), v-a) > 0.0;
	}
	std::vector<uint> computeTriangulation(std::vector<Math::Vec2>& polyvertices)
	{
		if(polyvertices.size() < 3)
			throw(std::runtime_error(
				std::string("can not triangulate polygon with less than 3 vertices")));
	
		std::vector<uint> triangles;
		std::list<Math::Vec3> vertices;
		for(uint i=0; i<polyvertices.size(); i++)
			vertices.push_back(Math::Vec3(polyvertices[i].x, polyvertices[i].y, float(i)));
	
		while(vertices.size() >= 3)
		{
			Math::Vec3 pre = vertices.front();
			Math::Vec2 pre2(pre.x, pre.y);
			vertices.pop_front();
			Math::Vec3 cur = vertices.front();
			Math::Vec2 cur2(cur.x, cur.y);
			vertices.pop_front();
			Math::Vec3 nex = vertices.front();
			Math::Vec2 nex2(nex.x, nex.y);
			vertices.pop_front();
			/* check if triangle is an ear */
			if( vec2LeftOfLine(nex2, pre2, cur2) )
			{
				/* check if triangle pre->cur->nex->pre can be cut */
				bool vint = false;
				for(auto& v : vertices)
					if( vec2InTriangle(Math::Vec2(v.x,v.y), pre2, cur2, nex2) )
						vint = true;
				if(vint)
				{
					/* there is a vertex in the triangle, move to next triangle */
					vertices.push_back(pre);
					vertices.push_front(nex);
					vertices.push_front(cur);
				}
				else
				{ /* no vertex in triangle */
					vertices.push_front(nex);
					vertices.push_front(pre);
					triangles.push_back(uint(pre.z));
					triangles.push_back(uint(cur.z));
					triangles.push_back(uint(nex.z));
				}
			}
			else
			{
				vertices.push_back(pre);
				vertices.push_front(nex);
				vertices.push_front(cur);
			}
		}
		return triangles;
	}
};


struct DebugSphere
{
	DebugSphere()
	{
		// Load debug sphere shader program
		prgm_handle = createShaderProgram("../src/debug_v.glsl","../src/debug_f.glsl",{"v_position"});

		// Create debug sphere geometry
		std::vector<float> vertices;
		std::vector<uint> indices;

		float latitude = -90.0;
		float longitude = -180.0;
		for(int i=0; i<50; i=i+1)
		{
			for(int j=0; j<100; j=j+1)
			{
				float lat_sin = sin( (PI/180.0f) * latitude);
				float lon_sin = sin( (PI/180.0f) * longitude);

				float lat_cos = cos( (PI/180.0f) * latitude);
				float lon_cos = cos( (PI/180.0f) * longitude);

				float r = 1.0f; //6378137.0;

				vertices.push_back(lon_sin * lat_cos * r);
				vertices.push_back(lat_sin * r);
				vertices.push_back(lat_cos * lon_cos * r);

				longitude += (1.0f/100.0f) * 360.0f;
			}
			latitude += (1/50.0f) * 180.0f;
		}

		for(int i=0; i<50*100; i=i+1)
		{
			indices.push_back((uint)i);
		}

		if(vertices.size() < 1 || indices.size() < 1)
			return;

		auto va_size = sizeof(float) * vertices.size();
		auto vi_size = sizeof(uint) * indices.size();

		//if(va_handle == 0 || vbo_handle == 0 || ibo_handle == 0)
		//{
			glGenVertexArrays(1, &va_handle);
			glGenBuffers(1, &vbo_handle);
			glGenBuffers(1, &ibo_handle);
		//}

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GL_FLOAT)*3, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	DebugSphere(const DebugSphere&) = delete;
	~DebugSphere()
	{
		// delete shader program
		glDeleteProgram(prgm_handle);

		// delete mesh resources
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ibo_handle);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDeleteBuffers(1, &vbo_handle);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &va_handle);
	}

	/** Handle for the vertex array object */
	GLuint va_handle;

	/** Handle for the vertex buffer object (allows access to vertex data in GPU memory) */
	GLuint vbo_handle;

	/** Handle for the index buffer objects (allows access to index data in GPU memory) */
	GLuint ibo_handle;

	/** Handle for the OpenGL program object */
	GLuint prgm_handle;

	void draw(OrbitalCamera& camera)
	{
		glUseProgram(prgm_handle);

		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "view_matrix"), 1, GL_FALSE, camera.view_matrix.data.data());
		glUniformMatrix4fv(glGetUniformLocation(prgm_handle, "projection_matrix"), 1, GL_FALSE, camera.projection_matrix.data.data());

		glPointSize(2.0);

		glBindVertexArray(va_handle);
		glDrawElements(GL_POINTS,  5000,  GL_UNSIGNED_INT,  NULL );
	}
};

void windowSizeCallback(GLFWwindow *window, int width, int height)
{
	OrbitalCamera* active_camera = reinterpret_cast<OrbitalCamera*>(glfwGetWindowUserPointer(window));

	active_camera->aspect_ratio =  ((float)width/(float)height);
	active_camera->updateProjectionMatrix();
}

namespace Controls {

	namespace
	{
		std::array<float,2> latest_cursor_position = {{0.0,0.0}};

		TriangleGraph* active_triangleGraph;

		CollisionSpheres* active_collisionSpheres = nullptr;
	}

	void mouseScrollFeedback(GLFWwindow *window, double x_offset, double y_offset)
	{
		if (glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			//if(active_collisionSpheres != nullptr)
				//active_collisionSpheres->moveTimestep( 1-2*std::signbit(y_offset) );
				//active_collisionSpheres->moveTimestep(int((active_collisionSpheres->sphere_cnt)*0.01)*(1-2*std::signbit(y_offset)) );

			if(active_collisionSpheres != nullptr)
			{
				int dist_up=active_collisionSpheres->sphere_cnt-active_collisionSpheres->current_timestep;
				active_collisionSpheres->moveTimestep(int((dist_up)*0.02+1)*(1-2*std::signbit(y_offset)) );
			}

			std::cout<<"Time: "<<active_collisionSpheres->time<<std::endl;
		}
		else
		{
			OrbitalCamera* active_camera = reinterpret_cast<OrbitalCamera*>(glfwGetWindowUserPointer(window));

			float camera_height_inertia = std::pow( (active_camera->orbit - 1.0f )*0.1f, 1.0f);

			active_camera->moveInOrbit(0.0f,0.0f,-camera_height_inertia * (float)y_offset);
		}
	}

	void mouseButtonFeedback(GLFWwindow* window, int button, int action, int mods)
	{
		if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			double pos_x, pos_y;
			glfwGetCursorPos(window, &pos_x, &pos_y);

			double pos_x_normalized = (pos_x/(double)width);
			double pos_y_normalized = (pos_y/(double)height);

			glBindFramebuffer(GL_FRAMEBUFFER, active_triangleGraph->picking_fbo_handle);
			GLfloat* data = new GLfloat[1]; data[0] = -1;
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glReadPixels((GLint)(pos_x_normalized*1600.0), (GLint)(900.0 - pos_y_normalized*900.0), 1, 1, GL_RED_INTEGER, GL_INT, data);

			//std::cout<<"Id: "<<(int)*data<<std::endl;

			active_triangleGraph->placeSphere((int)*data);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void keyFeedback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window,true);
				break;
			case GLFW_KEY_C:
				if(action == GLFW_PRESS)
					collisionSphere_mode = (collisionSphere_mode==0) ? 1 : 0;
			default:
				break;
			}
	}

	void updateOrbitalCamera(GLFWwindow *window)
	{
		OrbitalCamera* active_camera = reinterpret_cast<OrbitalCamera*>(glfwGetWindowUserPointer(window));

		/*	Get current cursor position on the screen */
		double pos_x, pos_y;
		glfwGetCursorPos(window, &pos_x, &pos_y);
		std::array<float,2> currentCursorPosition = {{(float) pos_x, (float) pos_y}};
		std::array<float,2> cursor_movement = {{latest_cursor_position[0] - currentCursorPosition[0],latest_cursor_position[1] - currentCursorPosition[1]}};

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			//camera_lon += cursor_movement.x;
			//camera_lat -= cursor_movement.y;

			GLfloat camera_vertical_inertia = std::pow( (active_camera->orbit - 1.0f)*0.1f, 1.2f);

			active_camera->moveInOrbit(-cursor_movement[1]*camera_vertical_inertia,cursor_movement[0]*camera_vertical_inertia,0.0);
		}

		latest_cursor_position = currentCursorPosition;
	}

	void setActiveTriangleGraph(TriangleGraph* g)
	{
		active_triangleGraph = g;
	}

	void setActiveCollisionSpheres(CollisionSpheres* cs)
	{
		active_collisionSpheres = cs;
	}
}

void help(std::ostream & out) {
	out << "simple OPTIONS:\n"
		"\t-gf <graph.gl|graph.sg>\tfile suffix selects the type of the graph\n"
		"\t-f format\tformat=[sg, gl, raw] overrides file suffix\n"
		"\t-t float\ttriangle transparency\n"
		"\t-x float\tshow elimination factor\n"
		"\t-bg float float float float\tset the background color\n"
		"\t-opengl3\tuse opengl 3 instead of 4\n"
		"\t--debug\tenable some debugging output\n"
		"\t--no-bg-sphere\tdisable the background sphere\n"
		"\t--no-angle-labels\tdisable angle labels\n"
	<< std::flush;
}

typedef enum {GFF_INVALID, GFF_SG, GFF_GL, GFF_RAW} GraphFileFormat;

int main(int argc, char*argv[])
{
	/////////////////////////////////////
	// overly simple command line parsing
	/////////////////////////////////////

	std::string filepath;
	float bgColor[4] = {0.2f, 0.2f, 0.2f, 1.0f};
	float trianlge_transparency = 1.0f;
	float etd = 0.0f;
	bool debugMode = false;
	bool disableBgSphere = false;
	bool angleLabels = true;
	GraphFileFormat gff = GFF_INVALID;
	
	int glMajorVersion = 4;

	int i=1;
    if (argc < 3) {
		help(std::cout);
		return 0;
	}
	while(i<argc)
	{
		if(argv[i] == (std::string) "-gf")
		{
			i++;
			if(i<argc && argv[i][0] != '-') { filepath = argv[i]; i++; }
			else { std::cerr<<"Missing parameter for -gf"<<std::endl; return -1; }
		}
		else if(argv[i] == (std::string) "-f")
		{
			i++;
			if(i>argc) {
				std::cerr << "Missing parameter for -f" << std::endl;
				return -1;
			}
			std::string token(argv[i]);
			if (token == "sg") { gff = GFF_SG; }
			else if (token == "gl") { gff = GFF_GL; }
			else if (token == "raw") { gff = GFF_RAW; }
			else {
				std::cerr << "Unkown graph file format: " << token << std::endl;
				return -1;
			}
		}
		else if(argv[i] == (std::string) "-t")
		{
			i++;
			if(i<argc && argv[i][0] != '-') { trianlge_transparency = std::stof(argv[i]); i++; }
			else { std::cerr<<"Missing parameter for -t"<<std::endl; return -1; }
		}
		else if(argv[i] == (std::string) "-x")
		{
			i++;
			if(i<argc && argv[i][0] != '-') { etd = std::stof(argv[i]); i++; }
			else { std::cerr<<"Missing parameter for -x"<<std::endl; return -1; }
		}
		else if(argv[i] == (std::string) "-bg") {
			++i;
			if(i+4 <= argc) {
				bgColor[0] = std::stof(argv[i]);
				bgColor[1] = std::stof(argv[i+1]);
				bgColor[2] = std::stof(argv[i+2]);
				bgColor[3] = std::stof(argv[i+3]);
				i += 4;
			}
			else {
				std::cerr << "Not enough parameters for -bg" << std::endl; 
				return -1;
			}
		}
		else if(argv[i] == (std::string) "-opengl3")
		{
			++i;
			glMajorVersion = 3;
		}
		else if(argv[i] == (std::string) "--debug")
		{
			i++;
			debugMode = true;
		}
		else if(argv[i] == (std::string) "--no-bg-sphere")
		{
			i++;
			disableBgSphere = true;
		}
		else if (argv[i] == (std::string) "--no-angle-labels") {
			i++;
			angleLabels = false;
		}
		else
		{
			i++;
		}
	}


	/////////////////////////////////////
	// Window and OpenGL Context creation
	/////////////////////////////////////

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES,4);
    window = glfwCreateWindow(1600, 900, "Simple Graph Renderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	/*	Initialize glew */
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if( GLEW_OK != error)
	{
		std::cout<<"-----\n"
				<<"The time is out of joint - O cursed spite,\n"
				<<"That ever I was born to set it right!\n"
				<<"-----\n"
				<<"Error: "<<glewGetErrorString(error);
		return false;
	}
	/* Apparently glewInit() causes a GL ERROR 1280, so let's just catch that... */
	glGetError();

	/* Intialize controls */
	glfwSetWindowSizeCallback(window,windowSizeCallback);
	glfwSetScrollCallback(window, Controls::mouseScrollFeedback);
	glfwSetKeyCallback(window, Controls::keyFeedback);
	/* Hide cursor */
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	////////////////////////////
	// Load graph data from file
	////////////////////////////

	std::vector<Node> nodes;
	std::vector<Edge> edges;

	std::vector<Node_RGB> nodes_rgb;
	std::vector<Edge_RGB> edges_rgb;
	std::vector<Triangle_RGB> triangles_rgb;

	std::vector<CollisionSphere> cSpheres;
	std::map<uint,uint> idMap;

	/* Decide which graph format to load */
	std::size_t filepath_length = filepath.length();
	std::string file_format(filepath.substr(filepath_length-2, 2));

	if(file_format == "gl")
		gff = GFF_GL;
	else if(file_format == "sg")
		gff = GFF_SG;
	else if(file_format == "aw")
		gff = GFF_RAW;

	switch (gff) {
	case GFF_GL:
		Parser::parseTxtGraphFile(filepath,nodes,edges);
		break;
	case GFF_SG:
		Parser::parseTxtTriangleGraphFile(filepath,nodes_rgb,edges_rgb,triangles_rgb);
		break;
	case GFF_RAW:
		Parser::parseTxtCollisionSpheresFile(filepath,cSpheres,idMap);
		break;
	default:
		std::cerr << "Unkown graph format" << std::endl;
		return -1;
	}

	/////////////////////////////////////////////////////////////////////
	// Creation of graphics resources, i.e. shader programs, meshes, etc.
	/////////////////////////////////////////////////////////////////////

	/*
	 * OpenGL Objects are usaully represented by handles.
	 * From my understanding, these are basically pointers/adresses to something
	 * the GPU (or GPU driver) owns.
	 *
	 * Instances of structs holding OpenGL handles are created within an additional
	 * scope, so that they are destroyed while the OpenGL context is still alive
	 */	
	{
		//GLenum glerror = glGetError();
		//std::cout<<glerror<<std::endl;

		/* Create renderable graph (mesh) */
		Graph lineGraph;
		if(gff == GFF_GL)
			lineGraph.addSubgraph(nodes,edges);

		/* Create renderable simple graph (mesh) */
		TriangleGraph simpleColouredGraph;
		if(gff == GFF_SG)
		{
			simpleColouredGraph.loadGraphData(nodes_rgb,edges_rgb,triangles_rgb);
			Controls::setActiveTriangleGraph(&simpleColouredGraph);
			glfwSetMouseButtonCallback(window, Controls::mouseButtonFeedback);
		}

		/* Create renderable collision spheres */
		CollisionSpheres collisionSpheres;
		if(gff == GFF_RAW)
		{
			collisionSpheres.loadData(cSpheres,idMap);
			Controls::setActiveCollisionSpheres(&collisionSpheres);
		}

		/* Example for creating polygons */
		//	Polygons polys;
		//	polys.addPolygon(std::vector<Node>({Node(50.0,5.0),Node(43.0,3.2),Node(40.0,5.0),Node(40.0,10.0),Node(45.0,15.0),Node(50.0,10.0)}), 0);
		//	polys.addPolygon(std::vector<Node>({Node(50.0,10.0),Node(45.0,15.0),Node(48.0,17.5)}), 2);
		//	polys.addPolygon(std::vector<Node>({Node(50.0,20.0),Node(45.0,25.0),Node(48.0,27.0)}), 1);
		//	polys.addPolygon(std::vector<Node>({Node( 90,-135 ), Node ( 0, -180 ), Node (0, -90 )/*, Node( 90,-135 )*/}), 2);

		/* Create a orbital camera */
		OrbitalCamera camera;
		camera.longitude = 0.0f;
		camera.latitude = 0.0f;
		camera.orbit = 5.0f;
		camera.near = 0.0001f;
		camera.far = 10.0f;
		camera.fovy = 30.0f * PI/180.0f;
		camera.aspect_ratio = 16.0f/9.0f;

		/* Make camera accessable in window callbacks */
		glfwSetWindowUserPointer(window,&camera);

		/* Create text labels */
		TextLabels labels;

		/* Create icons */
		Icons icons;

		//TODO FIND BUG: camera matrices have to be updated after label object creation....
		camera.updateViewMatrix();
		camera.updateProjectionMatrix();

		if (angleLabels) {
			for(int lon=-180; lon<=180 ; lon++)
				labels.addLabel(std::to_string(lon),0.0,(float)lon,0.25);

			for(int lat=-90; lat<=90 ; lat++)
				labels.addLabel(std::to_string(lat),(float)lat,0.0,0.25);
		}
		if (debugMode) {
			labels.addLabel("\"()*-_ßöäüÜÖÄ",5.0,5.0,0.25);
			icons.addIcon(Icons::ATM2,48.0,5.0,0.25);
			icons.addIcon(Icons::BUS,48.0,5.5,0.25);
			icons.addIcon(Icons::INFORMATION,48.0,6.0,0.25);
			icons.addIcon(Icons::PARKINGGARAGE,48.0,6.5,0.25);
			icons.addIcon(Icons::RESTAURANT,48.0,7.0,0.25);
			icons.addIcon(Icons::SIGHT2,48.0,7.5,0.25);
			icons.addIcon(Icons::TAXI,48.0,8.0,0.25);
			icons.addIcon(Icons::TELEPHONE,48.0,8.5,0.25);
			icons.addIcon(Icons::TOILETS,48.0,9.0,0.25);
			icons.addIcon(Icons::TOOLS,48.0,10.0,0.25);
			icons.addIcon(Icons::TRASH,48.0,10.5,0.25);
			icons.addIcon(Icons::WIFI,48.0,11.0,0.25);
			icons.addIcon(Icons::HOSPITAL,48.0,11.5,0.25);
			icons.addIcon(Icons::CINEMA,48.0,12.0,0.25);
			icons.addIcon(Icons::THEATER,48.0,12.5,0.25);
		}

		/* Create the debug sphere */
		DebugSphere db_sphere;


		//////////////////////////////////////////////////////
		// Render Loop - Each loop iteration renders one frame
		//////////////////////////////////////////////////////

		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		//glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			Controls::updateOrbitalCamera(window);

			/* update near/far clipping plane based on camera orbit */
			camera.near = 0.0001f * pow(camera.orbit,2.0f);
			camera.far = 2.0f * camera.orbit;
			camera.updateProjectionMatrix();

		    /* Render here */
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//set background colour
			glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			/* Draw debug sphere */
			if (!disableBgSphere) {
				db_sphere.draw(camera);
			}

			/* Draw polygons */
			//polys.draw(camera);

			/* Draw edges (i.e. streets) */
			float scale = std::min((0.0025f/(camera.orbit - 1.0f)),2.0f);

			if(gff == GFF_GL)
				lineGraph.draw( camera, scale );
			else if(gff == GFF_SG)
			{
				simpleColouredGraph.draw( camera, trianlge_transparency );
				simpleColouredGraph.pickingPass( camera );
			}
			else if(gff == GFF_RAW)
			{
				collisionSpheres.draw(camera);
				collisionSpheres.show_eliminations_factor = etd;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			/* Draw labels */
			labels.draw(camera);

			/* Draw icons */
			icons.draw(camera);

			// GeoBoundingBox bbox = camera.computeVisibleArea();
			//std::cout << std::fixed;
			//std::cout << std::setprecision(20);
			//std::cout<<"min latitude: "<<bbox.min_latitude<<" max latitude: "<<bbox.max_latitude<<std::endl;
			//std::cout<<"min longitude: "<<bbox.min_longitude<<" max longitude: "<<bbox.max_longitude<<std::endl;
			//std::cout<<"camera lon: "<<camera.longitude<<" "<<camera.latitude<<std::endl;

		    /* Swap front and back buffers */
		    glfwSwapBuffers(window);

		    /* Poll for and process events */
		    glfwPollEvents();
		}
	}

    glfwTerminate();
    return 0;
}
