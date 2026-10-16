#include <sys/types.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>



// #define SIZE 640
#define SIZE 320



namespace fs = std::filesystem;



class Shader {
    fs::file_time_type last_change_time;

    void compileShader()
    {
        printf("Compiling %s\n", path.c_str());

        std::ifstream shader_file(path);
        if (!shader_file) {
            printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n%s\n", path.c_str());
            return;
        }

        std::stringstream shader_stream;
        // read file's buffer contents into streams
        shader_stream << shader_file.rdbuf();
        // close file handlers
        shader_file.close();
        // convert stream into string
        std::string shader_code = shader_stream.str();
        const char* cstr_shader_code = shader_code.c_str();

        glShaderSource(shader, 1, &cstr_shader_code, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        }
    }

public:
    fs::path path;
    uint shader;

    Shader(fs::path path, uint shader) : path(path), shader(shader), last_change_time(fs::last_write_time(path)) {
        compileShader();
    }

    bool updateShader()
    {
        if (fs::file_time_type file_last_time_write = fs::last_write_time(path)
           ; last_change_time < file_last_time_write)
        {
            compileShader();
            last_change_time = file_last_time_write;
            return true;
        };

        return false;
    }
};

class ShaderProgram {
    fs::path shaderPath;

    void createShaderProgram(const Shader *shader)
    {
        printf("Creating shader program %d with shader %s\n", program, shader->path.c_str());

        static Shader vert_shader = { "./resources/vertex.vert", glCreateShader(GL_VERTEX_SHADER) };

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, shader->shader);
        glAttachShader(shaderProgram, vert_shader.shader);

        glLinkProgram(shaderProgram);

        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
        }

        // glDeleteShader(shader.shader);

        program = shaderProgram;
        shaderPath = shader->path;

        mouseLoc = glGetUniformLocation(program, "u_mouse");
        resolutionLoc = glGetUniformLocation(program, "u_resolution");
        timeLoc = glGetUniformLocation(program, "u_time");
    }

public:
    uint program;
    int mouseLoc;
    int resolutionLoc;
    int timeLoc;

    ShaderProgram(Shader shader) {
        createShaderProgram(&shader);
    }

    void updateShaderProgram(Shader *shader)
    {
        if (shaderPath != shader->path || shader->updateShader())
            createShaderProgram(shader);
    }
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int code, const char* description)
{
    printf("GLFW error %d: %s\n", code, description);
}



int main()
{
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize the library
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // This makes window floating in sway
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SIZE, SIZE, "Shader test", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwSetWindowSize(window, SIZE, SIZE);

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loading GLAD
    if (gladLoadGL(glfwGetProcAddress) == 0)
        return 1;


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    std::map<fs::path, Shader> shaders;
    for (auto &path : fs::directory_iterator("./resources/fragment"))
    {
        shaders.emplace(path.path(), Shader{ path.path(), glCreateShader(GL_FRAGMENT_SHADER) });
    }

    fs::path selectedShadersPath = shaders.begin()->first;

    ShaderProgram shaderProgram = shaders.at(selectedShadersPath);

    double mouse_x, mouse_y;

    double shaderUpdateTime = glfwGetTime();

    bool previousFrameSpaceState = GLFW_RELEASE;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && previousFrameSpaceState == GLFW_RELEASE)
        {
            printf("Space was pressed\n");

            bool setPathToNext = false;
            fs::path newSelectedShadersPath;
            for (auto & [path, shader] : shaders)
            {
                if (setPathToNext)
                {
                    printf("Switch to shader %s\n", path.c_str());

                    newSelectedShadersPath = path;
                    selectedShadersPath = path;
                    break;
                }

                if (path == selectedShadersPath)
                    setPathToNext = true;
            }
            if (newSelectedShadersPath.empty())
            {
                printf("Switch to shader %s\n", shaders.begin()->first.c_str());

                selectedShadersPath = shaders.begin()->first;
            }
        }
        previousFrameSpaceState = glfwGetKey(window, GLFW_KEY_SPACE);

        if (glfwGetTime() - shaderUpdateTime > 0.1) {
            shaderUpdateTime = glfwGetTime();
            for (auto &path : fs::directory_iterator("./resources/fragment"))
            {
                // https://stackoverflow.com/a/1939971
                // Ngl that looks ugly if I would implement my own std::map it would look better
                if (shaders.find(path.path()) == shaders.end())
                {
                    shaders.emplace(path.path(), Shader{ path.path(), glCreateShader(GL_FRAGMENT_SHADER) });
                }
            }
            if (not fs::exists(selectedShadersPath))
            {
                shaders.erase(selectedShadersPath);
                selectedShadersPath = shaders.begin()->first;
            }

            shaderProgram.updateShaderProgram(&shaders.at(selectedShadersPath));
        }

        glViewport(0, 0, SIZE, SIZE);

        // Render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // be sure to activate the shader
        glUseProgram(shaderProgram.program);

        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        glUniform2f(shaderProgram.mouseLoc,
            static_cast<float>(mouse_x),
            static_cast<float>(mouse_y));
        glUniform2f(shaderProgram.resolutionLoc,
            static_cast<float>(SIZE),
            static_cast<float>(SIZE));
        glUniform1f(shaderProgram.timeLoc, static_cast<float>(glfwGetTime()));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram.program);

    glfwTerminate();
    return 0;
}
