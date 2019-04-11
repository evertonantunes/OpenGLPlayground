#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <shader.hpp>
#include <resource.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

//screen size
const int WIDTH  = 1280;
const int HEIGHT = 960;

//vertex array and vertex buffer object IDs
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

const int NUM_X = 40; //total quads on X axis
const int NUM_Z = 40; //total quads on Z axis

const float SIZE_X = 4; //size of plane in world space
const float SIZE_Z = 4;
const float HALF_SIZE_X = SIZE_X/2.0f;
const float HALF_SIZE_Z = SIZE_Z/2.0f;

//ripple displacement speed
const float SPEED = 2;

//ripple mesh vertices and indices
glm::vec3 vertices[(NUM_X+1)*(NUM_Z+1)];
const int TOTAL_INDICES = NUM_X*NUM_Z*2*3;
GLushort indices[TOTAL_INDICES];

static std::vector<glm::vec3> s_vertices;

//projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);

//camera transformation variables
int state = 0, oldX=0, oldY=0;
float rX=25, rY=-40, dist = -7;

//current time
float render_time = 0;

application::opengl::Shader shader;

std::vector<glm::vec3> get_vertices()
{
    std::vector<glm::vec3> result;
    for ( std::size_t j = 0; j <= NUM_Z; j++ )
    {
        for ( std::size_t i = 0; i <= NUM_X; i++ )
        {
            const auto a = ((static_cast<float>(i) / (NUM_X - 1)) * 2 - 1) * HALF_SIZE_X;
            const auto b = 0.0f;
            const auto c = ((static_cast<float>(j) / (NUM_Z - 1)) * 2 - 1) * HALF_SIZE_Z;
            result.push_back(glm::vec3{a, b, c});
        }
    }
    return result;
}

void OnInit()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.LoadFromString(GL_VERTEX_SHADER, application::resource::get("resource/shader.vert"));
    shader.LoadFromString(GL_FRAGMENT_SHADER, application::resource::get("resource/shader.frag"));
    shader.CreateAndLinkProgram();
    shader.Use();
    shader.AddAttribute("vVertex");
    shader.AddUniform("MVP");
    shader.AddUniform("time");
    shader.UnUse();

    //setup plane geometry
    //setup plane vertices
//    int count = 0;
    int i=0, j=0;
//    for( j=0;j<=NUM_Z;j++)
//    {
//        for( i=0;i<=NUM_X;i++)
//        {
//            vertices[count++] = glm::vec3(((float(i)/(NUM_X-1)) *2-1)* HALF_SIZE_X, 0, ((float(j)/(NUM_Z-1))*2-1)*HALF_SIZE_Z);
//        }
//    }

    s_vertices = get_vertices();

    //fill plane indices array
    GLushort* id=&indices[0];
    for (i = 0; i < NUM_Z; i++)
    {
        for (j = 0; j < NUM_X; j++)
        {
            int i0 = i * (NUM_X+1) + j;
            int i1 = i0 + 1;
            int i2 = i0 + (NUM_X+1);
            int i3 = i2 + 1;

            if ((j+i) % 2)
            {
                *id++ = i0; *id++ = i2; *id++ = i1;
                *id++ = i1; *id++ = i2; *id++ = i3;
            }
            else
            {
                *id++ = i0; *id++ = i2; *id++ = i3;
                *id++ = i0; *id++ = i3; *id++ = i1;
            }
        }
    }

    GL_CHECK_ERRORS;

    //setup plane vao and vbo stuff
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboVerticesID);
    glGenBuffers(1, &vboIndicesID);

    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);

    //pass plane vertices to array buffer object

    /*
     *  Como a função glBufferData sabe que o array é do tipo float ?
    */
    glBufferData(GL_ARRAY_BUFFER, s_vertices.size()*(sizeof(float)*3), &s_vertices[0], GL_STATIC_DRAW);
    GL_CHECK_ERRORS;

    // enable vertex attrib array for position
    glEnableVertexAttribArray(shader["vVertex"]);
    glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE,0,0);
    GL_CHECK_ERRORS;

    // pass the plane indices to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
    GL_CHECK_ERRORS;
}

void OnShutdown()
{
    shader.DeleteShaderProgram();
    glDeleteBuffers(1, &vboVerticesID);
    glDeleteBuffers(1, &vboIndicesID);
    glDeleteVertexArrays(1, &vaoID);
}

void OnMouseDown(int button, int s, int x, int y)
{
    if (s == GLUT_DOWN)
    {
        oldX = x;
        oldY = y;
    }

    if(button == GLUT_MIDDLE_BUTTON)
    {
        state = 0;
    }
    else
    {
        state = 1;
    }
}

void OnMouseMove(int x, int y) {
    if (state == 0)
        dist *= (1 + (y - oldY)/60.0f);
    else {
        rY += (x - oldX)/5.0f;
        rX += (y - oldY)/5.0f;
    }
    oldX = x; oldY = y;
    glutPostRedisplay();
}

void OnRender()
{
    render_time = glutGet(GLUT_ELAPSED_TIME)/1000.0f * SPEED;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glm::mat4 T=glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
    glm::mat4 Rx= glm::rotate(T, rX, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MV= glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP= P*MV;
    shader.Use();
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform1f(shader("time"), render_time);
    glDrawElements(GL_TRIANGLES,TOTAL_INDICES, GL_UNSIGNED_SHORT,0);
    shader.UnUse();
    glutSwapBuffers();
}

//idle event callback
void OnIdle()
{
    glutPostRedisplay();
}

void OnResize(int w, int h)
{
    //set the viewport size
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    //setup the projection matrix
    P = glm::perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}

int main( int argc, char **argv )
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitContextVersion (3, 3);
    glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Getting started with OpenGL 3.3");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    else
    {
        if (GLEW_VERSION_3_3)
        {
            std::cout << "Driver supports OpenGL 3.3\nDetails:" << std::endl;
        }
    }

    std::cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "\tVendor: "    << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer: "  << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tVersion: "   << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL: "      << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    OnInit();
    glutCloseFunc(OnShutdown);
    glutDisplayFunc(OnRender);
    glutMotionFunc(OnMouseMove);
    glutMouseFunc(OnMouseDown);
    glutReshapeFunc(OnResize);
    glutIdleFunc(OnIdle);
    glutMainLoop();

    return 0;
}
