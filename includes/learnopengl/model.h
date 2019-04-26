#ifndef MODEL_H
#define MODEL_H
#include <stdlib.h> 
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <learnopengl/mesh.h>
#include <learnopengl/shader.h>
#include <glm/gtx/spline.hpp>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
public:
    /*  Model Data */
    vector<Texture> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    //vector <glm::mat4> objs;
    //vector<vector <glm::mat4> > matriX;
    vector <glm::mat4> matrix;
    vector <glm::mat4> objs;
    //vector<vector <glm::mat4> > objs(2);
    vector <glm::vec3> transformations;
    glm::vec3 pinicial = glm::vec3(0.0f, 0.0f, 0.0f);
     glm::vec3 patual = pinicial;
    glm::vec3 escalainicial = glm::vec3(0.1f, 0.1f, 0.1f);
    double delta_t=0.6f;
    float t = 0.0f; //parâmetro da curva análogo ao t da reta
    double first_t = 0.0;
    double atual_t = 0.0; //tempo inicial
    int i=0,j,objeto_corrente=0;
    double temp;
    const double lapse = 5.0;
    bool playAnimation = false;
    double current_t = 0.0;
    double  delta_rot =0.0;
    float bezi =0.0f;
    float valores = 0.0f;
        float x=-3.0f,y=0.0f,z=0.0f;
         glm::mat4 escal;
    // matriX.push_back(glm::mat4(1.0f));
     int indi1,indi2;
     glm::quat MyQuaternion;
    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    double retornaDelta(){
        return delta_t;
    }
    void settempSpline(float t){
       valores = t/1000;
    }
        glm::vec3 retornaPositionObj(){
        return glm::vec3(objs[objeto_corrente][3][0],objs[objeto_corrente][3][1],objs[objeto_corrente][3][2]);
    }
    void b(){
                
           glm::vec3 b = catmullRom(
            glm::vec3(2.5f, -3.0f, 0.0f),
                glm::vec3(0.0f, -0.8f, 0.0f),
                glm::vec3(5.0f, -2.3f, -1.0f), //final
                glm::vec3(3.5, 3.0f, -1.0f),t); //aux
            objs[objeto_corrente][0][3] = b.x;
            objs[objeto_corrente][3][1] = b.y;
            objs[objeto_corrente][3][2] = b.z;
           t+= (valores);
        
    }
    void setvalor(float valor){
        valores = valor;
    }
    void zerat(){
        t=0.0f;
    }
    void bezier( ){
        glm::vec3 a(2.5f, -3.0f, 0.0f),
                b(0.0f, 0.0f, 0.0f),
                c(5.0f, -2.3f, -1.0f), //final
                d(3.5, 3.0f, -1.0f);
           if(t<=1.0f){
                glm::vec3 ponto =(float)pow(1-t, 3) * a + 
                    3 * (float)pow(1-t, 2) * t * b +
                    3 * (1-t) * (float)pow(t, 2) * c +
                    (float)pow(t, 3) * d;
                    objs[objeto_corrente][3][0]= ponto.x;
                    objs[objeto_corrente][3][1]= ponto.y;
                    objs[objeto_corrente][3][2] = ponto.z;
                    t+=valores;
           }
          

    }   
     void rodaemponto(glm::vec3 p,float temp){

        glm::vec3 posi = retornaPositionObj();

       //a (a⋅v) + cos (α) (v − a (a⋅v)) + sin (α) ⋅ (a × v)
        //
       // glm::vec3 v = glm::vec3(10.0f,0.0f,10.0f);
        //glm::vec3 k = glm::vec3(0.0f,0.0f,1.0f);
       // glm::vec3
        //glm::vec3 rot = 
       // glm::vec3 rot = p * cos(45.0f) + (k*p)*sin(45.0f) + k* (k*p)*(1 - cos(45.0f));
        //objs[objeto_corrente] =   glm::translate( objs[objeto_corrente],v);

    
      //  glm::vec3 EulerAngles(45.0f, 45.0f, 0.0f);
        //glm::vec3 rotated_point = posi + ( EulerAngles * ( p - posi ));
        //glm::vec3 convert = glm::radius(EulerAngles);
        // m_rotation = Quaternion.FromAxisAngle(p, radians) * m_rotation;  
       // MyQuaternion = glm::quat(EulerAngles);
        // MyQuaternion =  glm::gtx::quaternion::angleAxis((10.0f), posi);
        //glm::vec3 rotated_point = posi + ( EulerAngles * ( p - posi ));
       // MyQuaternion =  glm::angleAxis((45.0f), rotated_point);
   //  glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
   //  objs[objeto_corrente] = objs[objeto_corrente] * RotationMatrix * escal ;
        float radius = 10.0f;
         float camX = (sin(temp)*radius);
        float camZ = cos(temp) * radius;
          objs[objeto_corrente][3][0]= camX;
          objs[objeto_corrente][3][1]= p.y;
          objs[objeto_corrente][3][2] = camZ;
     }
    void rodanoeixo(int delta_rot,int eixo){
            if (delta_rot != 0) {
                float valor = (float)((0.8*2) / 2);
                if(eixo==1)
                  objs[objeto_corrente] = glm::rotate( objs[objeto_corrente], glm::radians(valor),  glm::vec3(1.0f, 0.0f, 0.0f));
                else if(eixo==2)
                  objs[objeto_corrente] = glm::rotate( objs[objeto_corrente], glm::radians(valor),  glm::vec3(0.0f, 1.0f, 0.0f));
                else if(eixo==3)
                    objs[objeto_corrente] =  glm::rotate( objs[objeto_corrente],glm::radians(valor),  glm::vec3(0.0f, 0.0f, 1.0f));
            }

    }
    void escala(float x,int verifica){
     // x = x + objs[objeto_corrente][0][0];
        if(verifica==0)
            objs[objeto_corrente] = glm::scale( objs[objeto_corrente], glm::vec3(x,x,x));
        else
            objs[objeto_corrente] = glm::scale( objs[objeto_corrente], glm::vec3(-x,-x,-x));
    }
    void adicionaObjeto(float t){
        atual_t= t;
                delta_t =0.0f;
                pinicial = glm::vec3(x,y, z);
                objs.push_back( glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f)), pinicial));
                matrix.push_back(glm::mat4(1.0f));
                 escal = glm::scale(escal, glm::vec3(0.1, 0.1, 0.1));  
                //transformations.push_back( pinicial);
                x += 0.8f;
                y+=0.8f;
                ++i;
    }
     glm::mat4 translacao(glm::mat4 model,glm::vec3 patual){
        return glm::translate(model,patual);

    }
    void translat(int temp){

                if (temp!=0) {
                    patual.x = (float)((0.1*temp) / 2);
                    patual.y = 0.0f;
                    patual.z = 0.0f;
                    objs[objeto_corrente] = glm::translate(objs[objeto_corrente],patual);
                    
                }
    }

   glm::vec3 calculoponto(){
    glm::vec3 aux;
    if(objeto_corrente>=0){
       glm::vec3 aux(objs[objeto_corrente][3][0],objs[objeto_corrente][3][1],objs[objeto_corrente][3][2]);
    }
    
    else
       glm::vec3 aux(0.0f,0.0f,0.0f);
    return aux;
   }
    void trocaObjetoMax(){
            atual_t= glfwGetTime();
            delta_t =0.0f;
            if(objeto_corrente < i)
                ++objeto_corrente;
            else
                objeto_corrente= i-1;
    }
    void trocaObjetoMin(){
        atual_t= glfwGetTime();
            delta_t =0.0f;
            if(objeto_corrente > 0)
                --objeto_corrente;
            else
                objeto_corrente=0;
    }
    int calculoDelta(){
        delta_t += glfwGetTime() - atual_t;
    }
    void rodaobjetoespecifico(){
        objs[objeto_corrente] = glm::rotate(objs[objeto_corrente], glm::radians(1.0f),  glm::vec3(0.0f, 0.f, 1.0f));
    }
    
    void desenha(Shader ourshader){
        if(i>0){
            for (j=0; j < objs.size(); ++j)
            {
              //  objs[j] = glm::scale(objs[j], glm::vec3(0.1f, 0.1f, 0.1f)); 
                ourshader.setMat4("model", objs[j]);
                //ourshader.setMat4("model2",matrix[j]);
                Draw(ourshader);

            }
        }
    }
    vector <glm::mat4> retornaMatriz(){
        return objs;
    }
    // draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    
private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif
