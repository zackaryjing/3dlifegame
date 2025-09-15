#pragma once

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <numbers>
#include <vector>

#include "model/Model.hpp"
#include "utils/Type.hpp"
using std::tuple;
using std::vector;

using Type::VFVec3;
using Type::VIVec3;


class Icosahedron {
public:
    float cos36 = (sqrt(5.f) + 1.f) / 4.0f;
    float sin36 = sqrt(10.f - 2.0f * sqrt(5.0f)) / 4.0f;
    float tan36 = sqrt(5.0f - 2.0f * sqrt(5.0f));
    float radius36 = std::numbers::pi / 5.0f;
    float radius72 = 2.0f * std::numbers::pi / 5.0f;
    float edgeLength;
    float crossSectionInradius;
    float crossSectionCircumradius;
    float triangleAltitude;
    float midHeight; // truncated pentagonal bipyramid
    float pyramidAltitude;
    float sphereCircumradius;
    float sphereRadiusEdgeRatio;
    VIVec3 faceIndices = {
            {0, 1, 2},   {0, 2, 3},   {0, 3, 4},  {0, 4, 5},  {0, 5, 1}, //
            {1, 10, 2},  {2, 6, 3},   {3, 7, 4},  {4, 8, 5},  {5, 9, 1}, //
            {6, 2, 10},  {10, 1, 9},  {9, 5, 8},  {8, 4, 7},  {7, 3, 6}, //
            {11, 6, 10}, {11, 10, 9}, {11, 9, 8}, {11, 8, 7}, {11, 7, 6}};
    VFVec3 points;
    explicit Icosahedron(float edgeLength) : edgeLength(edgeLength) {
        crossSectionInradius = edgeLength / (2.0f * tan36);
        crossSectionCircumradius = edgeLength / (2.0f * sin36);
        triangleAltitude = edgeLength * sqrt(3.0f) / 2.0f;
        const float radiusDiff =
                crossSectionCircumradius - crossSectionInradius;
        midHeight = static_cast<float>(sqrt(
                triangleAltitude * triangleAltitude - radiusDiff * radiusDiff));
        pyramidAltitude = static_cast<float>(
                sqrt(edgeLength * edgeLength -
                     crossSectionCircumradius * crossSectionCircumradius));
        sphereCircumradius = midHeight / 2.0f + pyramidAltitude;
        sphereRadiusEdgeRatio = sphereCircumradius / edgeLength;
        points = getPoints();
    }

    VFVec3 getPoints() {
        VFVec3 res = {};

        res.emplace_back(0.0f, 0.0f, sphereCircumradius);
        const Type::FVec3 upperSidePoint(
                edgeLength / 2.0f, -crossSectionInradius, midHeight / 2.0f);
        for (int i = 0; i < 5; i++) {
            auto point =
                    rotateXY(upperSidePoint, static_cast<float>(i) * radius72);
            res.push_back(point);
        }
        const Type::FVec3 lowerSidePoint(
                edgeLength / 2.0f, crossSectionInradius, -midHeight / 2.0f);
        for (int i = 0; i < 5; i++) {
            auto point =
                    rotateXY(lowerSidePoint, static_cast<float>(i) * radius72);
            res.push_back(point);
        }
        res.emplace_back(0.0f, 0.0f, -sphereCircumradius);
        return res;
    }

    Model toModel() {
        // showVFVec3(points);
        return Model::genModel(points, faceIndices,
                               ModelNormalGenType::VERTEXBASED);
    }
};
