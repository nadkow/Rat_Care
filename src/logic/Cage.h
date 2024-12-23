#ifndef RATCARE_CAGE_H
#define RATCARE_CAGE_H

#include "Object.h"

const std::string beddingTexFiles[] = {"hemp.png", "lavender.jpg"};
float xsize = 4.;
float zsize = 4.;

// simple plane
float groundVerts[] = {
        // first triangle                // normals                     // texture
        -xsize, -0.1f, zsize, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        xsize, -0.1f, zsize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -xsize, -0.1f, -zsize, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // second triangle
        xsize, -0.1f, zsize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        xsize, -0.1f, -zsize, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -xsize, -0.1f, -zsize, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

class Cage {
public:
    //double half_xsize = xsize / 2.0;
    //double half_zsize = zsize / 2.0;

    void init() {
        // generate VAO
        glGenVertexArrays(1, &groundVAO);
        // bind VAO (use it)
        glBindVertexArray(groundVAO);
        // generate coordVBO
        glGenBuffers(1, &groundVBO);
        glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
        // copy vertex data to vbo
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerts), groundVerts, GL_STATIC_DRAW);
        // set vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // generate texture
        glGenTextures(1, &groundTexture);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        // set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load from file
        int width, height, nrChannels;
        std::string texPath = "res/textures/bedding/";
        texPath.append(beddingTexFiles[0]);
        unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            spdlog::error("Failed to load bedding texture");
        }
        stbi_image_free(data);
    }

    void addFood(std::string filename, int hp) {
        Food* newfood = new Food(filename, new Node(), hp);
        foods.push_back(newfood);
    }

    void draw() {
        // uses ratShader
        ratShader.setMat4("transform", rootNode.getTransform());
        glBindVertexArray(groundVAO);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        for (auto child : foods) {
            child->draw();
        }
    }

    int getFood(int req_hp) {
        // returns the amount of hp that is available <= requested hp
        // and subtracts hp from the chosen food object
        int collected_hp;
        for (auto child : foods) {
            collected_hp += child->tryGet(req_hp-collected_hp);
            if (child->hp == 0) {
                auto it = std::remove(foods.begin(), foods.end(), child);
                foods.erase(it, foods.end());
            }
        }
        return collected_hp;
    }

private:
    std::vector<Food*> foods;
    std::vector<Object*> decor;
    unsigned int groundVAO, groundVBO;
    unsigned int groundTexture;
};

#endif //RATCARE_CAGE_H
