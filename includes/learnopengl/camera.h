#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/spline.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f; //Y
const float PITCH       =  0.0f; //X
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float control =0.0f;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    void angulocamera(glm::vec3 patual){
        std::cout << "angulo" << std::endl;
        glm::vec3 direct = glm::normalize(Position - patual);
        std::cout<<"X:" << direct.x <<" Y:"<< direct.y<<" Z:"<< direct.z << std::endl;
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    void bSpline(){
         glm::vec3 b;
        if (control < 1.0f) {         
            b = catmullRom(
            glm::vec3(2.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, -0.8f, 0.0f),
                glm::vec3(2.5f, -2.3f, -1.0f), //final
                glm::vec3(2.5, 1.2f, -1.0f),control); //aux
           control += 0.01;
        }
        Position = b;
       // glm::mat4 view =  GetViewMatrix();
        //return view;
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
       // std::cout<<"front x " << Front.x << "front y " << Front.y << "front z " << Front.z << std::endl;
        return glm::lookAt(Position, Position + Front, Up);

    }
    void linearAnimation(glm::vec3 ponto,float deltaTime){
        deltaTime = deltaTime / 500  ;
        float velocity = MovementSpeed * deltaTime;
        Position += (Right * velocity); 

    }
    glm::mat4 segue(glm::vec3 ponto){
       return glm::lookAt(Position, ponto, Up);

    }
        void aumentaZoom()
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= 0.5;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }
    void trocaPosition(glm::vec3 p)
    {
        Position = p;
    }
    void diminuiZoom()
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom += 0.5;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }
    glm::mat4 trocaLook(glm::vec3 ponto)
    {
        //Position -= Right + ponto; 
        //updateCameraVectors();
         return glm::lookAt(Position, ponto, Up);
    }
    void trocaFront(glm::vec3 novo){
        Position = novo;
        updateCameraVectors();

    }
    void rodaEixo(float t){
        
       /* glm::vec3 frontInicial, positionInicial,upInicial;
        frontInicial = Front;
        positionInicial = Position;
        upInicial = Up;
        float time_atual, delta=0.0;
         glm::mat4 rotacao;
         glm::vec4 novaposicao, novofront, novoup;
         //   delta = glfwGetTime() - t;
        rotacao = glm::rotate(glm::mat4(1),  glm::radians(90.0f),  glm::vec3(1.0f, 0.0f, 0.0f));
        novaposicao = glm::vec4(positionInicial, 1);
        novaposicao = novaposicao * rotacao;
        novofront = glm::vec4((positionInicial + frontInicial), 1);
        novofront = novofront * rotacao;
        novoup = glm::vec4((positionInicial + upInicial), 1);
        novoup = novoup * rotacao;

         Position = glm::vec3(novaposicao.x, novaposicao.y, novaposicao.z);
        Front = glm::vec3(novofront.x, novofront.y, novofront.z) - Position;
        Up = glm::vec3(novoup.x, novoup.y, novoup.z) - Position;
        //Position = r.InicialPosition;

        updateCameraVectors();*/
       //  Yaw   += t;
   // Pitch += t*2;

    float radius = 10.0f;
         float camX = (sin(t)*radius);
        //float camZ = cos(temp) * radius;
         //float camY =  sin(glm::radians(0.0f));
          Position.x = camX;
          //Front.x = camX;
    }
    glm::mat4 rodaemponto(float temp, glm::vec3 p){
         float radius = 10.0f;
         float camX = (sin(temp)*radius);
        float camZ = cos(temp) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX,0.0f,camZ), p,Up);
        return view;

    }
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }
    void AnimationCam(float temp,double duracao){
            double delta_t=temp;
            double final=0.0;
            float lF = 0.0f;
            float dT = 0.0f; 
            double first = glfwGetTime();
            while (final < duracao) {
                double atualt= glfwGetTime();
                final = atualt - first;
               float cF = glfwGetTime();
                dT = cF - lF;
                lF = cF;
                float velocity = MovementSpeed * dT;
                Position -= Front  * velocity;
               updateCameraVectors();
            }
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }
    
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif