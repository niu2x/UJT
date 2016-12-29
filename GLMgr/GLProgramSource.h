#ifndef __GLProgramSource__
#define __GLProgramSource__

#include <string>

#include "../refCount/Ref.h"

#define GLSL(src) "#version 400 core\n" #src

class GLProgramSource: public Ref{
public:
	static GLProgramSource * createWithStrings(
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &
	);
	static GLProgramSource * createWithStrings(
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
	GLProgramSource();
	bool initWithStrings(
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &, 
		const std::string &
	);
	virtual ~GLProgramSource(){}

private:
	std::string vertexSource;
	std::string tessCtrlSource;
	std::string tessEvalSource;
	std::string geometrySource;
	std::string fragmentSource;
};

#endif
