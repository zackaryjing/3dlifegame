#pragma once
#include <utility>
#include <vector>
#include "light/DirLight.hpp"
#include "light/PointLight.hpp"
#include "light/SpotLight.hpp"


class LightManager {
public:
    vector<unique_ptr<DirLight>> dirLights;
    vector<unique_ptr<PointLight>> pointLights;
    vector<unique_ptr<SpotLight>> spotLights;

    [[nodiscard]] vector<Animation *> getAnimation() const {
        vector<Animation *> animationList;
        for (const auto &light: pointLights) {
            if (light->lightTurning) {
                animationList.push_back(light->lightPosAnimation);
                animationList.push_back(light->lightColorAnimation);
            }
        }
        return animationList;
    }
    [[nodiscard]] vector<shared_ptr<Model>> getModel() const {
        vector<shared_ptr<Model>> modelList;
        cout << "size: " << spotLights.size() << " " << dirLights.size() << " "
             << pointLights.size() << endl;
        for (const auto &light: spotLights) {
            modelList.push_back(light->lightModel);
        }
        for (const auto &light: pointLights) {
            modelList.push_back(light->lightModel);
        }
        for (const auto &light: dirLights) {
            modelList.push_back(light->lightModel);
        }
        return modelList;
    }
    void drawLights(const glm::mat4 &view, const glm::mat4 &projection) const {
        for (const auto &light: dirLights) {
            light->drawLight(view, projection);
        }
        for (const auto &light: pointLights) {
            light->drawLight(view, projection);
        }
        for (const auto &light: spotLights) {
            light->drawLight(view, projection);
        }
    }
    void useLightUniform(const Shader &shader) const {
        for (const auto &light: spotLights) {
            light->useLightUniform(shader);
        }
        for (const auto &light: dirLights) {
            light->useLightUniform(shader);
        }
        for (const auto &light: pointLights) {
            light->useLightUniform(shader);
        }
    }

    LightManager() {
        // dirLights.emplace_back(make_unique<DirLight>(
        //         glm::vec3{0.2, 0.2, 0.2}, glm::vec3{0.0, 5.0, 0.0},
        //         glm::vec3{0.0, -1.0, 0.0}));
        for (int i = 0; i < 4; ++i) {
            pointLights.emplace_back(make_unique<PointLight>(
                    Rand::genVec3(0.7f, 1.0f), Rand::genVec3(-3.0f, 3.0f), i));
        }
        // spotLights.emplace_back(make_unique<SpotLight>());
    }
    template<typename DContainer, typename PContainer, typename SContainer>
    explicit LightManager(DContainer &&dir, PContainer &&point,
                          SContainer &&spot) {
        addLights(dirLights, std::forward<DContainer>(dir));
        addLights(pointLights, std::forward<PContainer>(point));
        addLights(spotLights, std::forward<SContainer>(spot));
    }

private:
    template<typename T, typename Container>
    void addLights(std::vector<std::unique_ptr<T>> &target,
                   Container &&source) {
        using ElementType = std::decay_t<decltype(*std::begin(
                std::declval<Container &>()))>;
        if constexpr (std::is_same_v<ElementType, std::unique_ptr<T>>) {
            for (auto &ptr: source) {
                target.push_back(std::move(ptr));
            }
        } else {
            using Concrete = ElementType;
            for (auto &obj: source) {
                target.push_back(std::make_unique<Concrete>(obj));
            }
        }
    }
};
