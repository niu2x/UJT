#ifndef __shaderSource__
#define __shaderSource__

#include <string>

#include "../refCount/Ref.h"

#define GLSL(src) "#version 400 core\n" #src

class ShaderSource: public Ref{
public:
	static ShaderSource * createWithStrings(
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &
	);
	static ShaderSource * createWithStrings(
		const char *, 
		const char *, 
		const char *, 
		const char *,
		const char *
	);

	const char *getVertexSource()const;
	const char *getTessCtrlSource()const;
	const char *getTessEvalSource()const;
	const char *getGeometrySource()const;
	const char *getFragmentSource()const;

protected:
	ShaderSource();
	bool initWithStrings(
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &
	);
	virtual ~ShaderSource(){}

private:
	std::string vertexSource;
	std::string tessCtrlSource;
	std::string tessEvalSource;
	std::string geometrySource;
	std::string fragmentSource;
};

#endif
