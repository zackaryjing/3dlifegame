#pragma once
#include "Icosahedron.hpp"


class Sphere : public Icosahedron {
public:
    Sphere(float radius, int iteration = 4) : Icosahedron(1.0) {
        (void) radius;
        for (int i = 0; i < iteration; ++i) {
            VFVec3 newPoints = points;
            VIVec3 newFaceIndices;
            int pointCnt = newPoints.size();
            int faceIndicesCnt = faceIndices.size();
            newPoints.reserve(pointCnt * 2);
            newFaceIndices.reserve(faceIndicesCnt * 4);

            int index = pointCnt;
            // cout << "old_point_cnt: " << points.size() << endl;
            // cout << "old_face_indices_cnt: " << faceIndices.size() << endl;
            for (auto [indexA, indexB, indexC]: faceIndices) {
                auto vertexA = points[indexA];
                auto vertexB = points[indexB];
                auto vertexC = points[indexC];
                auto vertexD = times(normalize(add(vertexA, vertexB)),
                                     sphereCircumradius);
                auto vertexE = times(normalize(add(vertexB, vertexC)),
                                     sphereCircumradius);
                auto vertexF = times(normalize(add(vertexC, vertexA)),
                                     sphereCircumradius);
                newPoints.push_back(vertexD);
                newPoints.push_back(vertexE);
                newPoints.push_back(vertexF);
                int indexD = index++;
                int indexE = index++;
                int indexF = index++;

                newFaceIndices.emplace_back(indexA, indexD, indexF);
                newFaceIndices.emplace_back(indexB, indexE, indexD);
                newFaceIndices.emplace_back(indexC, indexF, indexE);
                newFaceIndices.emplace_back(indexF, indexD, indexE);
            }
            points = newPoints;
            faceIndices = newFaceIndices;
            // cout << "new_point_cnt: " << points.size() << endl;
            // cout << "new_face_indices_cnt: " << faceIndices.size() << endl;
        }
        // cout << "finish" << endl;
    }

    Model toModel() {
        return Model::genModel(points, faceIndices,
                               ModelNormalGenType::VERTEXBASED);
    }
};
