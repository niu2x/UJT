#include "ShaderSource.h"
#include "macro.h"

ShaderSource::ShaderSource():
vertexSource(""),
tessCtrlSource(""),
tessEvalSource(""),
geometrySource(""),
fragmentSource("")
{

}

ShaderSource *ShaderSource::createWithStrings(
	const std::string &__vertexSource, 
	const std::string &__tessCtrlSource, 
	const std::string &__tessEvalSource, 
	const std::string &__geometrySource, 
	const std::string &__fragmentSource
){
	ShaderSource *shaderSource = new (std::nothrow) ShaderSource();
	if(shaderSource && shaderSource->initWithStrings(
	    	__vertexSource,
			__tessCtrlSource,
			__tessEvalSource,
			__geometrySource,
			__fragmentSource
       )){
		shaderSource->autorelease();
		return shaderSource;
	}
	SAFE_DELETE(shaderSource);
	return nullptr;
}

ShaderSource *ShaderSource::createWithStrings(
	const char *__vertexSource, 
	const char *__tessCtrlSource, 
	const char *__tessEvalSource, 
	const char *__geometrySource, 
	const char *__fragmentSource
){
	return createWithStrings(
		std::string(__vertexSource ? __vertexSource : ""),
		std::string(__tessCtrlSource ? __tessCtrlSource : ""),
		std::string(__tessEvalSource ? __tessEvalSource : ""),
		std::string(__geometrySource ? __geometrySource : ""),
		std::string(__fragmentSource ? __fragmentSource : "")
	);
}

bool ShaderSource::initWithStrings(
	const std::string &__vertexSource, 
	const std::string &__tessCtrlSource, 
	const std::string &__tessEvalSource, 
	const std::string &__geometrySource, 
	const std::string &__fragmentSource
){
	vertexSource = __vertexSource;
	tessCtrlSource = __tessCtrlSource;
	tessEvalSource = __tessEvalSource;
	geometrySource = __geometrySource;
	fragmentSource = __fragmentSource;
	return true;
}

const char *ShaderSource::getVertexSource()const{
	if(vertexSource == std::string("")){
		return nullptr;
	}
	else{
		return vertexSource.c_str();
	}
}

const char *ShaderSource::getTessCtrlSource()const{
	if(tessCtrlSource == std::string("")){
		return nullptr;
	}
	else{
		return tessCtrlSource.c_str();
	}
}

const char *ShaderSource::getTessEvalSource()const{
	if(tessEvalSource == std::string("")){
		return nullptr;
	}
	else{
		return tessEvalSource.c_str();
	}
}

const char *ShaderSource::getGeometrySource()const{
	if(geometrySource == std::string("")){
		return nullptr;
	}
	else{
		return geometrySource.c_str();
	}
}

const char *ShaderSource::getFragmentSource()const{
	if(fragmentSource == std::string("")){
		return nullptr;
	}
	else{
		return fragmentSource.c_str();
	}
}
