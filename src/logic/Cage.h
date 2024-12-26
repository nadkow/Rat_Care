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

    void addFood(const std::string& filename, int hp) {
        Node* baseNode = new Node();
        baseNode->translate(dist(rnd) * xsize, .0, dist(rnd) * zsize);
        Food* newfood = new Food(filename, baseNode, hp);
        foods.push_back(newfood);
    }

    void addFood(const std::string& filename, int hp, Node* pos) {
        Food* newfood = new Food(filename, pos, hp);
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

    void deleteFood(Food *ob) {
        auto it = std::remove(foods.begin(), foods.end(), ob);
        foods.erase(it, foods.end());
        delete ob;
    }

    int getFood(int req_hp) {
        // returns the amount of hp that is available <= requested hp
        // and subtracts hp from the chosen food object
        int collected_hp = 0;
        std::vector<Food*> empties;
        for (auto child : foods) {
            collected_hp += child->tryGet(req_hp-collected_hp);
            if (child->hp == 0) empties.push_back(child);
        }
        // clean up empty foods
        for (auto empty : empties) {
            deleteFood(empty);
        }
        return collected_hp;
    }

    void load() {
        YAML::Node cagefile = YAML::LoadFile("cage.yaml");
        if (cagefile["foods"]) {
            // load foods if present
            for (int i = 0; i < cagefile["foods"].size(); i++) {
                Node* basenode = new Node();
                basenode->translate(cagefile["foods"][i]["position"][0].as<double>(), cagefile["foods"][i]["position"][1].as<double>(), cagefile["foods"][i]["position"][2].as<double>());
                addFood(cagefile["foods"][i]["file"].as<std::string>(), cagefile["foods"][i]["hp"].as<int>(), basenode);
            }
        }
    }

    void save() {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "foods";
        out << YAML::Value << YAML::BeginSeq;

        for (Food* food : foods) {
            if (food->hp > 0){
                out << YAML::BeginMap;
                out << YAML::Key << "file" << YAML::Value << food->filename;
                out << YAML::Key << "hp" << YAML::Value << food->hp;
                glm::vec3 transl = food->position->getGlobalTranslation();
                out << YAML::Key << "position" << YAML::Value;
                out << YAML::Flow;
                out << YAML::BeginSeq << transl.x << transl.y << transl.z << YAML::EndSeq;
                out << YAML::EndMap;
            }
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;
        std::ofstream fout("cage.yaml", std::ios::trunc);
        fout << out.c_str();
        fout.close();
    }

private:
    std::vector<Food*> foods;
    std::vector<Object*> decor;
    unsigned int groundVAO, groundVBO;
    unsigned int groundTexture;
};

#endif //RATCARE_CAGE_H
