#pragma once

#include "components/simple_scene.h"
#include <random>
#include <thread>
#include <chrono>
#include <ctime>

namespace m1
{


    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void RenderScene();
        void RenderGreyStars();
        void RenderRomb();
        void RenderGrid();
        void RenderLife();
        void CollisionHexaProj();
        void CollisionHexaRomb();
        void RenderRombMouse();
        void RenderRombsToR();
        void RenderRombOnGrid(float deltatime);
        void RenderProjectiles(float deltaTimeSeconds);
        void RandomStar(float deltaTimeSeconds);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderEnemies(float deltaTimeSeconds);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY, translateXLine;
        float scaleX, scaleY;
        float angularStep;
        int currentEnemy;
       // std::uniform_real_distribution<int>randLine;
        float hx, hy;
        float randomAppearanceTime;
        float timeSinceLastHexagon;
        float timeSinceLastProjectile;
        float timeSinceLastStar;
        int lives;
        int draggedRhombusIndex;
        bool isDragging;
        float transrombX;
        float transrombY;
        int whichRomb;
        bool eliminStar;
        int ok;
        int ok2;
        int num;
        int mystar;
        float rhombusSide;
        int myromb;
        int whichIsStatic;
        int renderagain;
        float posProjX;
        float posProjY;
        int colorProj;
        bool goProj;
        float x1;
        float x2;
        float x3;
        bool rombdisappear;
        float s1x,s1y;
        float s2x, s2y;
        float s3x, s3y;
        bool disp1, disp2, disp3, disp4;
        int whichToDis;
        // TODO(student): If you need any other class variables, define them here.

    };

}   // namespace m1