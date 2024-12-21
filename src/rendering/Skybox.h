#ifndef RATCARE_SKYBOX_H
#define RATCARE_SKYBOX_H
float skyboxVertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

unsigned int skyTex;
unsigned int cubemapVAO, cubemapVBO;
Shader cubemapShader;
std::string sky_faces[] = {"res/textures/skybox/xpos.png", "res/textures/skybox/xneg.png",
                           "res/textures/skybox/ypos.png", "res/textures/skybox/yneg.png",
                           "res/textures/skybox/zpos.png", "res/textures/skybox/zneg.png"};

void create_textures() {
    glGenTextures(1, &skyTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex);

    int width, height, nrChannels;
    unsigned char *data;

    for (unsigned int i = 0; i < 6; i++) {
        data = stbi_load(sky_faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void init_buffer_objects() {
    // generate VAO
    glGenVertexArrays(1, &cubemapVAO);
    // bind VAO (use it)
    glBindVertexArray(cubemapVAO);
    // generate coordVBO
    glGenBuffers(1, &cubemapVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    // copy vertex data to vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    // set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
}

void init_skybox(int hour) {

    cubemapShader = Shader("res/shaders/cubemap.vert", "res/shaders/cubemap.frag");
    cubemapShader.use();
    cubemapShader.setMat4("view", glm::mat4(glm::mat3(view)));

    double brightness = glm::sin(hour/8.0);
    spdlog::info("Sky brightness: {}", brightness);
    glm::vec4 skycolor = glm::vec4(brightness, brightness, .99, 1.0);
    cubemapShader.setVec4("skycolor", skycolor);

    create_textures();
    init_buffer_objects();
}

void draw_skybox() {
    cubemapShader.use();
    cubemapShader.setMat4("projection", projection);

    glDepthMask(GL_FALSE);
    glBindVertexArray(cubemapVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

#endif //RATCARE_SKYBOX_H
