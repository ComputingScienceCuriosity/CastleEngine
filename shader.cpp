/*#include "GL/glew.h"
#include "Shader.hpp"
#include "QMessageBox"


using namespace std;

 QMessageBox msgBox;

Shader* Shader::_instance = NULL;
Shader::Shader(){}

Shader* Shader::getInstance(){
    if(_instance == NULL){
        _instance = new Shader();
    }

    return _instance;
}

Shader::~Shader(void){}

void Shader::begin(const char* progVsh_path,const char* progFsh_path, ShaderRessources deleteStatus)
{

    GLhandleARB vertexShaderID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
    GLhandleARB fragmentShaderID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

    // Try Exceptions
    TryLoadingFile(progVsh_path);
    TryLoadingFile(progFsh_path);

    // Read shaders
    std::string sourceVsh = read_ShaderFile(progVsh_path);
    std::string sourceFsh = read_ShaderFile(progFsh_path);

    GLchar *ShaderSourceV[1];
    ShaderSourceV[0] = (GLchar *)sourceVsh.c_str();
    GLchar *ShaderSourceF[1];
    ShaderSourceF[0] = (GLchar *)sourceFsh.c_str();

    GLint	compileStatus = GL_FALSE;

    // Compile Vertex Shader
    if(vertexShaderID != 0)
    {
        glShaderSourceARB(vertexShaderID, 1,(const GLchar**) ShaderSourceV, NULL);
        glCompileShaderARB(vertexShaderID);
    }

    // Compiles the Vertex Shaders Program
    shader_CompilerLog(compileStatus, vertexShaderID);

    // Compile Fragment Shader
    glShaderSourceARB(fragmentShaderID, 1,(const GLchar**) ShaderSourceF, NULL);
    glCompileShaderARB(fragmentShaderID);

    // Compiles the Fragment Shaders Program
    shader_CompilerLog(compileStatus, fragmentShaderID);

    GLuint shaderProgramID = createProgram();

    // Attachement
    attachShader(shaderProgramID, vertexShaderID);
    attachShader(shaderProgramID, fragmentShaderID);

    // Linking
    linkProgram(shaderProgramID);
    // Delete Shader
    if(deleteStatus){
        deleteShader(vertexShaderID);
        deleteShader(fragmentShaderID);
    }

    useProgram (shaderProgramID);

    // Delete Program
    deleteProgram(shaderProgramID);
}

GLuint Shader::getShaderProg(const char* progVsh_path,const char* progFsh_path, ShaderRessources deleteStatus)
{

    GLhandleARB vertexShaderID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
    GLhandleARB fragmentShaderID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

    // Try Exceptions
    TryLoadingFile(progVsh_path);
    TryLoadingFile(progFsh_path);

    // Read shaders
    std::string sourceVsh = read_ShaderFile(progVsh_path);
    std::string sourceFsh = read_ShaderFile(progFsh_path);

    GLchar *ShaderSourceV[1];
    ShaderSourceV[0] = (GLchar *)sourceVsh.c_str();
    GLchar *ShaderSourceF[1];
    ShaderSourceF[0] = (GLchar *)sourceFsh.c_str();

    GLint	compileStatus = GL_FALSE;

    // Compile Vertex Shader
    if(vertexShaderID != 0)
    {
        glShaderSourceARB(vertexShaderID, 1,(const GLchar**) ShaderSourceV, NULL);
        glCompileShaderARB(vertexShaderID);
    }

    // Compiles the Vertex Shaders Program
    shader_CompilerLog(compileStatus, vertexShaderID);

    // Compile Fragment Shader
    glShaderSourceARB(fragmentShaderID, 1,(const GLchar**) ShaderSourceF, NULL);
    glCompileShaderARB(fragmentShaderID);

    // Compiles the Fragment Shaders Program
    shader_CompilerLog(compileStatus, fragmentShaderID);

    GLuint shaderProgramID = createProgram();

    // Attachement
    attachShader(shaderProgramID, vertexShaderID);
    attachShader(shaderProgramID, fragmentShaderID);

    // Linking
    linkProgram(shaderProgramID);
    // Delete Shader
    if(deleteStatus){
        deleteShader(vertexShaderID);
        deleteShader(fragmentShaderID);
    }

    //useProgram (shaderProgramID);

    // Delete Program
    /*deleteProgram(shaderProgramID);
    deleteProgram(shaderProgramID);

    return shaderProgramID;
}

void Shader::begin(ShaderRessources ressourceType,const char* progSh_path, ShaderRessources deleteStatus)
{
    GLhandleARB ShaderID;
    if(ressourceType == vertexShader)
        ShaderID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
    else if(ressourceType == pixelShader)
        ShaderID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

    // Try Exceptions
    TryLoadingFile(progSh_path);

    // Read shaders
    std::string sourceVsh = read_ShaderFile(progSh_path);

    GLchar *ShaderSourceV[1];
    ShaderSourceV[0] = (GLchar *)sourceVsh.c_str();

    GLint	compileStatus = GL_FALSE;

    // Compile Vertex Shader
    if(ShaderID != 0)
    {
        glShaderSourceARB(ShaderID, 1,(const GLchar**) ShaderSourceV, NULL);
        glCompileShaderARB(ShaderID);
    }

    // Compiles the Vertex Shaders Program
    shader_CompilerLog(compileStatus, ShaderID);

    GLuint shaderProgramID = createProgram();
    // Attachement
    attachShader(shaderProgramID, ShaderID);

    // Linking
    linkProgram(shaderProgramID);

    // Delete Shader
    if(deleteStatus){
        deleteShader(ShaderID);
    }
    // Using the Shader's current program
    useProgram(shaderProgramID);

    // Delete Program
    deleteProgram(shaderProgramID);
}

void Shader::end()
{
    //Use default program
    glUseProgramObjectARB( NULL );
}

void Shader::attachShader(GLuint shaderProgramID,GLhandleARB shaderID)
{
    if(shaderProgramID != 0 && shaderID != 0){
        glAttachShader(shaderProgramID, shaderID);
    } else {
        msgBox.setInformativeText("ERROR: No shader with associated key does exist!");

    }
}

void Shader::detachShader(GLuint shaderProgramID,GLhandleARB shaderID){

    if(shaderProgramID != 0 && shaderID != 0){
        glDetachShader(shaderProgramID, shaderID);
    } else {
        msgBox.setInformativeText("ERROR: No shader or shader-program with associated key does exist!");
    }

}

void Shader::resetProgram(){
    glUseProgramObjectARB(0);
}

void Shader::useProgram(GLuint shaderProgramID){

    if(shaderProgramID != 0){
        glUseProgramObjectARB(shaderProgramID);
    } else {
        msgBox.setInformativeText("ERROR: No shader-program does exist!");

    }
}

void Shader::linkProgram(GLuint shaderProgramID){

    if(shaderProgramID != 0){
        glLinkProgram(shaderProgramID);
    } else {
        msgBox.setInformativeText("ERROR: No shader-program does exist!");
    }

    GLint linkStatus;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkStatus);

    if(glGetError() != GL_NO_ERROR || linkStatus == GL_FALSE){

        GLsizei* length = new GLsizei;
        glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, length); //Get the length of the compilation log
        char* linkingLog = new char[*length];			 //Create the needed char array

        glGetProgramInfoLog(shaderProgramID, *length, NULL, linkingLog); //Get the compilation log

    }
}

void Shader::deleteProgram(GLuint shaderProgramID){

    if(shaderProgramID != 0){
        glDeleteProgram(shaderProgramID);
    } else {
        msgBox.setInformativeText("ERROR: No shader-program does exist!");
    }

}

void Shader::deleteShader(GLuint shaderID){

    if(shaderID != 0){
        glDeleteShader(shaderID);
    } else {
         msgBox.setInformativeText("ERROR: No shader-program does exist!");
    }
}

std::string Shader::read_ShaderFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::createProgram()
{
    return (glCreateProgramObjectARB());
}

void Shader::TryLoadingFile(const char* filepath)
{
    try{
        read_ShaderFile(filepath);
    } catch(std::exception e){
        msgBox.setInformativeText("ERROR: Could not load file from!");
    }

    glGetError();
}

void Shader::shader_CompilerLog(GLint compileStatus, GLhandleARB shaderObject)
{
    glGetObjectParameterivARB(shaderObject, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);


    if(!compileStatus)
    {
        glGetShaderiv(shaderObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &compileStatus);
        char* sLog = new char[compileStatus];
        glGetShaderInfoLog(shaderObject, compileStatus, NULL, sLog);
        //OUTPUT_DEBUG_STRING_INFO(sLog);
        delete [] sLog;
    }
}
*/
