/** 
 * @file components.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief Bu bileşenler, oyun nesnelerinin temel özelliklerini ve davranışlarını tanımlar.
 *        Bu dosya, temel bileşenlerin ve donusum, hiz gibi özel bileşenlerin tanımlarını içerir.
 *        Bileşenler, oyun nesnelerinin özelliklerini ve davranışlarını tanımlamak için kullanılır.
 *        Bu, oyun nesnelerinin esnek ve modüler bir şekilde oluşturulmasını sağlar.
 *        Bu dosya, Entity-Component-System (ECS) mimarisinin temelini oluşturur.
 *        İnşallah ilerleyen yazılarımda, ECS mimarisine ya da ECS kütüphanelerine bir göz atarız.
 * @date 2025-05-31
 */
#pragma once

#include <memory>
#include <vector>

// Forward declarations
class Renderer;

/** 
 * @brief Temel gosterim ve benzeri islevler icin kullanabilecegimiz siniftir.
 */
class GraphicalObject;
class Component {
public:
    GraphicalObject* mOwner = nullptr;
    virtual ~Component() = default;
    virtual void Update(float deltaTime) {}
    virtual void Render(Renderer& renderer) {}
};

/** 
 * @brief Temel donusumlere yonelik bilgileri tutacak siniftir.
 */
class Transform : public Component {
public:
    float mX = 0.0F;
    float mY = 0.0F;
    float mRotation = 0.0F;
    float mScaleX = 1.0F;
    float mScaleY = 1.0F;
    
    Transform(float x = 0, float y = 0) : mX(x), mY(y) {}
};

/** 
 * @brief Hiza yonelik bilgileri tutacak siniftir.
 */
class Velocity : public Component {
public:
    float mVx = 0.0F;
    float mVy = 0.0F;
    
    Velocity(float vx = 0, float vy = 0) : mVx(vx), mVy(vy) {}
};

/** 
 * @brief Renderer sinifini kullanarak gorsel cizim islevlerini saglayacak olan RenderStrategy sinifidir.
 * 
 */
class RenderStrategy;

class RenderComponent 
    : public Component {
private:
    std::unique_ptr<RenderStrategy> mStrategy;

public:
    RenderComponent(std::unique_ptr<RenderStrategy> strategy);    
    void SetStrategy(std::unique_ptr<RenderStrategy> strategy);    
    void Render(Renderer& renderer) override;
};
