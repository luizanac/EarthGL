#pragma once

#include <array>

struct Vertex {
    vec3 m_Position;
    vec3 m_Color;
    vec2 m_UV;
};

class Primitives {
public:
    static uint LoadGeometry() {
        uint vbo, vao, ebo;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadIndices), &QuadIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, m_Position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, m_Color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, m_UV));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        return vao;
    }

    static void CreateSphereMesh(uint resolution, std::vector<Vertex> &vertices, std::vector<ivec3> &indices) {
        vertices.clear();
        indices.clear();

        const float invResolution = 1.0f / static_cast<float>(resolution - 1);

        for (uint uIndex = 0; uIndex < resolution; uIndex++) {
            const float u = uIndex * invResolution;
            const float theta = mix(0.0f, two_pi<float>(), u);

            for (uint vIndex = 0; vIndex < resolution; vIndex++) {
                const float v = vIndex * invResolution;
                const float phi = mix(0.0f, pi<float>(), v);

                vec3 vertexPosition{
                        cos(theta) * sin(phi),
                        sin(theta) * sin(phi),
                        cos(phi)
                };

                Vertex vertex{
                        vertexPosition,
                        vec3(1.0f, 1.0f, 1.0f),
                        vec2{1.0f - u, 1.0f - v}
                };

                vertices.push_back(vertex);
            }
        }

        for (int u = 0; u < resolution - 1; u++) {
            for (int v = 0; v < resolution - 1; v++) {
                auto p0 = u + v * resolution;
                auto p1 = u + 1 + v * resolution;
                auto p2 = u + (v + 1) * resolution;
                auto p3 = u + 1 + (v + 1) * resolution;

                indices.emplace_back(p3, p2, p0);
                indices.emplace_back(p1, p3, p0);
            }
        }
    }

    static uint LoadSphere(uint &numVertices, uint &numIndices) {
        std::vector<Vertex> vertices;
        std::vector<ivec3> indices;
        CreateSphereMesh(100, vertices, indices);
        numVertices = vertices.size();
        numIndices = indices.size();

        uint vbo, vao, ebo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(ivec3), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, m_Position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, m_Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, m_UV));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        return vao;
    }


    constexpr static float TriangleVertices[] = {
            -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
            1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
            0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0
    };

    constexpr static std::array<Vertex, 6> QuadVertices[] = {
            Vertex{vec3{-1.0, 1.0, 0.0},
                   vec3{1.0, 1.0, 1.0},
                   vec2{0.0, 1.0}},

            Vertex{vec3{1.0, 1.0, 0.0},
                   vec3{1.0, 1.0, 1.0},
                   vec2{1.0, 1.0}},

            Vertex{vec3{1.0, -1.0, 0.0},
                   vec3{1.0, 1.0, 1.0},
                   vec2{1.0, 0.0}},

            Vertex{vec3{-1.0, -1.0, 0.0},
                   vec3{1.0, 1.0, 1.0},
                   vec2{0.0, 0.0}},
    };

    constexpr static std::array<ivec3, 2> QuadIndices = {
            ivec3{0, 1, 3},
            ivec3{3, 1, 2}
    };
};
