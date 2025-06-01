/** 
 * @file graphical-object-factory.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief 
 * @date 2025-05-31
 */
#pragma once

#include <memory>
#include <vector>

#include "components.h"
#include "renderer.h"

/** 
 * @brief GraphicalObject, oyun nesnelerini temsil eden ve bileşenleri yöneten sınıftır.
 */
class GraphicalObject {
private:
    std::vector<std::unique_ptr<Component>> mComponents;
    
public:
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        ptr->mOwner = this;
        mComponents.push_back(std::move(component));
        return ptr;
    }
    
    template<typename T>
    T* GetComponent() const {
        for (const auto& component : mComponents) {
            if (auto* ptr = dynamic_cast<T*>(component.get())) {
                return ptr;
            }
        }
        return nullptr;
    }
    
    void Update(float deltaTime);    
    void Render(Renderer& renderer);
};

/** 
 * @brief GraphicalObjectFactory, grafiksek nesneleri oluşturan fabrika sınıfıdır.
 *        Bu sınıf, farklı türdeki grafik nesnelerini (örneğin, dikdortgen ve daire) oluşturmak için kullanılabilir.
 */
class GraphicalObjectFactory {
public:
    static std::unique_ptr<GraphicalObject> CreateRectangle(float x, float y);
    static std::unique_ptr<GraphicalObject> CreateCircle(float x, float y);
    static std::unique_ptr<GraphicalObject> CreateTriangle(float x, float y);
};