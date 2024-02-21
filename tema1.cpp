#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <random>
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;
/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
float RandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}
class Star {
public:
    float x;
    float y;
    int idx;
    float angle=0;
    bool fire;
    bool hit;
};
class Enemy {
public:
    int type;
    float x;
    float y;
    int hits;
    bool dis;
    float scaleX;
    float scaleY;
};
class Rhombus {
public:
    int idx;
    float x;
    float y;
    int cost;
    float scaleX;
    float scaleY;
    float time;
    bool dis;
    bool h;
    vector<Star> s;
    Rhombus(int idx, float x, float y, int cost) : idx(idx), x(x), y(y), cost(cost) {}

    Rhombus(int idx, float x, float y, int cost, float scaleX, float scaleY, float time, bool dis
    ) : idx(idx), x(x), y(y), cost(cost), scaleX(scaleX), scaleY(scaleY), time(time),
        dis(dis){}

    Rhombus(int idx, float x, float y, int cost, float time) : idx(idx), x(x), y(y), cost(cost),
                                                               time(time) {}
};
class GridSq{
public:
    float x;
    float y;
    int idx;

    GridSq(float x, float y, int idx) : x(x), y(y), idx(idx) {}
};

std::vector<Enemy> enemies;
std::vector<Rhombus> rombs;
std::vector<Rhombus> staticrombs;
std::vector<Rhombus> initialrombs;
std::vector<Star> pinkstars;
std::vector<Star> greystars;
std::vector<GridSq> grid;
vector<Star> projectiles;
Tema1::Tema1()
{
}



Tema1::~Tema1()
{
}


void Tema1::Init()
{
    lives=3;
    whichToDis=0;
    goProj=false;
    posProjX=0.0f;
    posProjY=0.0f;
    eliminStar=false;
    whichIsStatic=0;
    num=0;
    ok=0;
    mystar=0;
    myromb=0;
    whichRomb=0;
    renderagain=0;
    x1=0.0f;
    x2 =0.0f;
    x3 =0.0f;
    s1x=1;
    s2x=1;
    s3x=1;
    s1y=1;
    s2y=1;
    s3y=1;
    Rhombus rhombus1(1, 180.0f, 1200.0f,1); //porto
    Rhombus rhombus2(2, 540.0f, 1200.0f,2);//albastru
    Rhombus rhombus3(3, 900.0f, 1200.0f,2);//galben
    Rhombus rhombus4(4, 1260.0f, 1200.0f,3);//mov

    initialrombs.push_back(rhombus1);
    initialrombs.push_back(rhombus2);
    initialrombs.push_back(rhombus3);
    initialrombs.push_back(rhombus4);
    GridSq s1(0,0,1);
    GridSq s2(150,0,2);
    GridSq s3(150,0,3);
    GridSq s4(-150,150,4);
    GridSq s5(-150,0,5);
    GridSq s6(300,0,6);
    GridSq s7(0,150,7);
    GridSq s8(-150,0,8);
    GridSq s9(-150,0,9);
    grid.push_back(s1);
    grid.push_back(s2);
    grid.push_back(s3);
    grid.push_back(s4);
    grid.push_back(s5);
    grid.push_back(s6);
    grid.push_back(s7);
    grid.push_back(s8);
    grid.push_back(s9);

    draggedRhombusIndex = -1; // Set to -1 when not dragging any rhombus
    isDragging=false;
    transrombX=0;
    transrombY=0;
    timeSinceLastHexagon = 0.0f;
    timeSinceLastProjectile=0.0f;
    timeSinceLastStar =0.0f;
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;
    translateXLine=0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3((float)154/255, (float)66/255, (float)83/255), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3((float)141/255, (float)186/255, (float)127/255), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3((float)24/255, (float)41/255, (float)88/255));
    AddMeshToList(square3);
    // Example usage to render a rhombus
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f); // Define the center point
    rhombusSide = 90.0f; // Define the side length

    cx = (float)rhombusSide * cos(glm::radians(60.0f));
    cy = (float)rhombusSide* sin(glm::radians(60.0f));
    Mesh* rhombusMesh1 = object2D::CreateRhombus("rhombus1", center, rhombusSide, glm::vec3((float)253/255, (float)146/255, (float)87/255), true);
    AddMeshToList(rhombusMesh1);
    Mesh* rhombusMesh2 = object2D::CreateRhombus("rhombus2", center, rhombusSide, glm::vec3((float)80/255, (float)133/255, (float)188/255), true);
    AddMeshToList(rhombusMesh2);
    Mesh* rhombusMesh3 = object2D::CreateRhombus("rhombus3", center, rhombusSide, glm::vec3((float)255/255, (float)232/255, (float)124/255), true);
    AddMeshToList(rhombusMesh3);
    Mesh* rhombusMesh4 = object2D::CreateRhombus("rhombus4", center, rhombusSide, glm::vec3((float)176/255, (float)139/255, (float)232/255), true);
    AddMeshToList(rhombusMesh4);
    Mesh* star = object2D::CreateStar("star", center, 30, 60,glm::vec3((float)172/255, (float)188/255, (float)207/255), true);
    AddMeshToList(star);
    Mesh* star1 = object2D::CreateStar("star1", center, 30, 60,glm::vec3((float)253/255, (float)146/255, (float)87/255), true);
    AddMeshToList(star1);
    Mesh* star2 = object2D::CreateStar("star2", center, 30, 60,glm::vec3((float)80/255, (float)133/255, (float)188/255), true);
    AddMeshToList(star2);
    Mesh* star3 = object2D::CreateStar("star3", center, 30, 60,glm::vec3((float)255/255, (float)232/255, (float)124/255), true);
    AddMeshToList(star3);
    Mesh* star4 = object2D::CreateStar("star4", center, 30, 60,glm::vec3((float)176/255, (float)139/255, (float)232/255), true);
    AddMeshToList(star4);
    Mesh* hexasmol1= object2D::CreateHexagon("hexasmol1", center, 0.7f*rhombusSide, glm::vec3((float)128/255, (float)0/255, (float)128/255), true);
    AddMeshToList(hexasmol1);
    Mesh* hexabig1= object2D::CreateHexagon("hexabig1", center,1.6f*0.7f*rhombusSide , glm::vec3((float)176/255, (float)139/255, (float)232/255), true);
    AddMeshToList(hexabig1);
    Mesh* hexasmol2= object2D::CreateHexagon("hexasmol2", center, 0.7f*rhombusSide, glm::vec3((float)46/255, (float)62/255, (float)178/255), true);
    AddMeshToList(hexasmol2);
    Mesh* hexabig2= object2D::CreateHexagon("hexabig2", center, 0.7f*1.6f*rhombusSide, glm::vec3((float)80/255, (float)133/255, (float)188/255), true);
    AddMeshToList(hexabig2);
    Mesh* hexasmol3= object2D::CreateHexagon("hexasmol3", center, 0.7f*rhombusSide, glm::vec3((float)160/255, (float)114/255, (float)0/255), true);
    AddMeshToList(hexasmol3);
    Mesh* hexabig3= object2D::CreateHexagon("hexabig3", center, 0.7f*1.6f*rhombusSide, glm::vec3((float)255/255, (float)232/255, (float)124/255), true);
    AddMeshToList(hexabig3);
    Mesh* hexasmol4= object2D::CreateHexagon("hexasmol4", center, 0.7f*rhombusSide, glm::vec3((float)203/255, (float)93/255, (float)13/255), true);
    AddMeshToList(hexasmol4);
    Mesh* hexabig4= object2D::CreateHexagon("hexabig4", center, 1.6f*0.7f*rhombusSide, glm::vec3((float)253/255, (float)146/255, (float)87/255), true);
    AddMeshToList(hexabig4);
    Mesh* pinkstar = object2D::CreateStar("pinkstar", center, 30, 60,glm::vec3((float)255/255, (float)67/255, (float)168/255), true);
    AddMeshToList(pinkstar);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
void Tema1::RandomStar(float deltaTimeSeconds){
    timeSinceLastStar += deltaTimeSeconds;
    if (timeSinceLastStar >=randomAppearanceTime) {

        for (int i = 0; i < 3; i++) {
            timeSinceLastStar = 0.0f;
            float randomX = RandomFloat(0.0f, 1280.0f);
            float randomY = RandomFloat(0.0f, 720.0f);
            Star s;
            s.x = randomX;
            s.y = randomY;
            pinkstars.push_back(s);
        }
    }
    for (const auto& star : pinkstars) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(star.x, star.y);
        modelMatrix*= transform2D::Rotate(60);
        RenderMesh2D(meshes["pinkstar"], shaders["VertexColor"], modelMatrix);
    }
}
void Tema1::CollisionHexaRomb(){
    for(auto& hex : enemies){
        for(auto&r :staticrombs){
            float dist = glm::length(glm::vec2((hex.x - r.x), hex.y - r.y));
            if (dist <rhombusSide){
                r.h=true;
            }

        }
    }
}
void Tema1::CollisionHexaProj() {
    for (auto& hex : enemies) {

        for (int i= 0; i<projectiles.size(); i++) {

                if ((hex.type == 0 && projectiles[i].idx == 4) || (hex.type == 1 && projectiles[i].idx == 2) || (hex.type == 2 && projectiles[i].idx == 3) ||
                    (hex.type == 3 && projectiles[i].idx == 1)) {
                    float dist = glm::length(glm::vec2((hex.x - projectiles[i].x), hex.y - projectiles[i].y));
                    if (dist < 30 + rhombusSide) {
                        projectiles[i].hit = true;
                        projectiles.erase(projectiles.begin() + i);
                        i--;
                        hex.hits++;

                        cout << "Hexagon at (" << hex.x << ", " << hex.y << ") hit! Number of hits: " << hex.hits
                             << endl;

                        if (hex.hits == 3) {
                            hex.dis = true;
                            cout << "Hexagon at (" << hex.x << ", " << hex.y << ") disabled!" << endl;
                        }
                    }
                }

        }
    }
}


void Tema1::RenderEnemies(float deltaTimeSeconds) {

    lives =3;
    timeSinceLastHexagon += deltaTimeSeconds;

    if (timeSinceLastHexagon >=randomAppearanceTime) {
        timeSinceLastHexagon = 0.0f;
        int lane = rand() % 3;
        int enemyType = rand() % 4;
        Enemy e;
        e.type = enemyType;
        e.x=2700.0f;
        e.hits=0;
        if(lane == 0)
            e.y=145.0f;
        else if(lane == 1)
            e.y=440.0f;

        else if(lane == 2)
            e.y=737.0f;
        e.scaleX=1;
        e.scaleY=1;
        enemies.push_back(e);
    }


    for (auto& enemy : enemies) {
        enemy.x -= 100.0f * deltaTimeSeconds;
    }


    for ( int i=0; i<enemies.size(); i++) {
        if(enemies[i].x<=120){
            lives-=1;
        }

        switch (enemies[i].type) {
            case 0:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(enemies[i].x, enemies[i].y);
                if(enemies[i].dis){
                    if(enemies[i].scaleX<=0 || enemies[i].scaleY<=0){
                        enemies.erase(enemies.begin() + i);
                    }
                    enemies[i].scaleX-=15*deltaTimeSeconds/10;
                    enemies[i].scaleY-=15*deltaTimeSeconds/10;
                    modelMatrix *= transform2D::Scale(enemies[i].scaleX, enemies[i].scaleY);
                    cout<<"scalee"<<enemies[i].scaleX<<endl;
                }
                RenderMesh2D(meshes["hexasmol1"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexabig1"], shaders["VertexColor"], modelMatrix);
                break;
            case 1:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(enemies[i].x, enemies[i].y);
                if(enemies[i].dis){
                    enemies[i].scaleX-=15*deltaTimeSeconds/10;
                    enemies[i].scaleY-=15*deltaTimeSeconds/10;
                    modelMatrix *= transform2D::Scale(enemies[i].scaleX, enemies[i].scaleY);
                    cout<<"scalee"<<enemies[i].scaleX<<endl;
                    if(enemies[i].scaleX<=0 || enemies[i].scaleY<=0){
                        enemies.erase(enemies.begin() + i);
                    }
                }
                RenderMesh2D(meshes["hexasmol2"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexabig2"], shaders["VertexColor"], modelMatrix);
                break;
            case 2:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(enemies[i].x, enemies[i].y);
                if(enemies[i].dis){
                    enemies[i].scaleX-=15*deltaTimeSeconds/10;
                    enemies[i].scaleY-=15*deltaTimeSeconds/10;
                    modelMatrix *= transform2D::Scale(enemies[i].scaleX, enemies[i].scaleY);
                    cout<<"scalee"<<enemies[i].scaleX<<endl;
                    if(enemies[i].scaleX<=0 || enemies[i].scaleY<=0){
                        enemies.erase(enemies.begin() + i);
                    }
                }
                RenderMesh2D(meshes["hexasmol3"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexabig3"], shaders["VertexColor"], modelMatrix);
                break;
            case 3:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(enemies[i].x, enemies[i].y);
                if(enemies[i].dis){
                    enemies[i].scaleX-=15*deltaTimeSeconds/10;
                    enemies[i].scaleY-=15*deltaTimeSeconds/10;
                    modelMatrix *= transform2D::Scale(enemies[i].scaleX, enemies[i].scaleY);
                    cout<<"scalee"<<enemies[i].scaleX<<endl;
                    if(enemies[i].scaleX<=0 || enemies[i].scaleY<=0){
                        enemies.erase(enemies.begin() + i);
                    }
                }
                RenderMesh2D(meshes["hexasmol4"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexabig4"], shaders["VertexColor"], modelMatrix);
                break;

            default:

                break;
        }

    }
}
void Tema1::RenderRomb(){

    unsigned int vecSize = rombs.size();


    for(unsigned int i = 0; i < vecSize; i++)
    {
        switch(rombs[i].idx){
            case 1:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(rombs[i].x, rombs[i].y);
                RenderMesh2D(meshes["rhombus1"], shaders["VertexColor"], modelMatrix);
                break;
            case 2:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(rombs[i].x, rombs[i].y);
                RenderMesh2D(meshes["rhombus2"], shaders["VertexColor"], modelMatrix);
                break;
            case 3:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(rombs[i].x, rombs[i].y);
                RenderMesh2D(meshes["rhombus3"], shaders["VertexColor"], modelMatrix);
                break;
            case 4:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(rombs[i].x, rombs[i].y);
                RenderMesh2D(meshes["rhombus4"], shaders["VertexColor"], modelMatrix);
                break;
            default:

                break;
        }
    }

}
void Tema1::RenderRombOnGrid(float deltatime){
    for (int i=0; i<staticrombs.size();i++) {

        switch(staticrombs[i].idx ){
            case 1:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(staticrombs[i].x, staticrombs[i].y);
                if(staticrombs[i].dis || staticrombs[i].h){

                    staticrombs[i].scaleX-=15*deltatime/10;
                    cout<<"scale x"<<scaleX<<endl;
                    staticrombs[i].scaleY-=15*deltatime/10;
                    modelMatrix *= transform2D::Scale(staticrombs[i].scaleX,staticrombs[i].scaleY);
                    cout<<"am scalat"<<endl;
                    if(staticrombs[i].scaleX<=0 || staticrombs[i].scaleY<=0){
                        staticrombs.erase(staticrombs.begin() + i);

                    }
                }
                RenderMesh2D(meshes["rhombus1"], shaders["VertexColor"], modelMatrix);

                break;
            case 2:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(staticrombs[i].x, staticrombs[i].y);
                if(staticrombs[i].dis || staticrombs[i].h){

                    staticrombs[i].scaleX-=15*deltatime/10;
                    cout<<"scale x"<<scaleX<<endl;
                    staticrombs[i].scaleY-=15*deltatime/10;
                    modelMatrix *= transform2D::Scale(staticrombs[i].scaleX,staticrombs[i].scaleY);
                    cout<<"am scalat"<<endl;
                    if(staticrombs[i].scaleX<=0 || staticrombs[i].scaleY<=0){
                        staticrombs.erase(staticrombs.begin() + i);
                    }
                }
                RenderMesh2D(meshes["rhombus2"], shaders["VertexColor"], modelMatrix);

                break;
            case 3:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(staticrombs[i].x, staticrombs[i].y);
                if(staticrombs[i].dis || staticrombs[i].h){

                    staticrombs[i].scaleX-=15*deltatime/10;
                    cout<<"scale x"<<scaleX<<endl;
                    staticrombs[i].scaleY-=15*deltatime/10;
                    modelMatrix *= transform2D::Scale(staticrombs[i].scaleX,staticrombs[i].scaleY);
                    cout<<"am scalat"<<endl;
                    if(staticrombs[i].scaleX<=0 || staticrombs[i].scaleY<=0){
                        staticrombs.erase(staticrombs.begin() + i);
                    }
                }
                RenderMesh2D(meshes["rhombus3"], shaders["VertexColor"], modelMatrix);

                break;
            case 4:
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(staticrombs[i].x, staticrombs[i].y);
                if(staticrombs[i].dis || staticrombs[i].h){

                    staticrombs[i].scaleX-=15*deltatime/10;
                    cout<<"scale x"<<scaleX<<endl;
                    staticrombs[i].scaleY-=15*deltatime/10;
                    modelMatrix *= transform2D::Scale(staticrombs[i].scaleX,staticrombs[i].scaleY);
                    cout<<"am scalat"<<endl;
                    if(staticrombs[i].scaleX<=0 || staticrombs[i].scaleY<=0){
                        staticrombs.erase(staticrombs.begin() + i);
                    }
                }
                RenderMesh2D(meshes["rhombus4"], shaders["VertexColor"], modelMatrix);
                break;
            default:

                break;
        }
    }
}


void Tema1::RenderGrid(){
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(180, 40);
    modelMatrix *= transform2D::Scale(2, 2);
    for (const auto& s : grid) {
        modelMatrix*=transform2D::Translate(s.x,s.y);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);


    }
}


void Tema1::RenderScene(){
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 40);
    modelMatrix *= transform2D::Scale(1, 8.0f);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(180.0f, 1200.0f);
    RenderMesh2D(meshes["rhombus1"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(540.0f, 1200.0f);
    RenderMesh2D(meshes["rhombus2"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(900.0f, 1200.0f);
    RenderMesh2D(meshes["rhombus3"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1260.0f, 1200.0f);
    RenderMesh2D(meshes["rhombus4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(80, 1100);
    modelMatrix *= transform2D::Scale(2, 2);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(180, 0);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(180, 0);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(180, 0);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(120, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(475, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(545, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(830, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(900, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1195, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1265, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1335, 1070);
    modelMatrix*= transform2D::Rotate(60);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderGreyStars(){

    float space=0.0f;
    int i=1;

    for (auto& star : greystars) {

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(star.x, star.y);
        modelMatrix *= transform2D::Translate(space, 0);
        modelMatrix*= transform2D::Rotate(60);
        modelMatrix *= transform2D::Scale(0.5f, 0.5f);
        RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
        space+=70.0f;
        i++;
    }

}
void Tema1::RenderProjectiles(float deltaTimeSeconds){

    for(auto& r:staticrombs){

        if(r.time>=2.0f) {
            r.time = 0.0f;
            Star s;
            s.x = r.x;
            s.y = r.y;
            s.idx = r.idx;
            for (auto &enemy: enemies) {
                if (enemy.x>=120.0f && enemy.x <= 2700.0f) {
                    if ((enemy.type == 0 && s.idx == 4) || (enemy.type == 1 && s.idx == 2) || (enemy.type == 2 && s.idx == 3) ||
                        (enemy.type == 3 && s.idx == 1)) {
                        if ((130.0f <= s.y && s.y <= 340.0f && enemy.y == 145.0f) || (390.0f <= s.y && s.y<= 590.0f && enemy.y == 440.f) ||
                            (540.0f <= s.y && s.y <= 780.0f && enemy.y == 737.0f)) {
                            projectiles.push_back(s);
                        }
                    }
                }
            }
        }else {
            r.time +=deltaTimeSeconds;
        }

    }
    for(auto& p : projectiles){
        p.x +=200*deltaTimeSeconds;
        if(p.angle>2*M_PI){
            p.angle=0.0f;
        }else{
            p.angle+=5*deltaTimeSeconds;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix*= transform2D::Translate(p.x,p.y);
        modelMatrix*= transform2D::Rotate(p.angle);
        //modelMatrix *= transform2D::Scale(0.5f, 0.5f);
        switch(p.idx){
            case 1:
                RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
                break;
            case 2:
                RenderMesh2D(meshes["star2"], shaders["VertexColor"], modelMatrix);
                break;
            case 3:
                RenderMesh2D(meshes["star3"], shaders["VertexColor"], modelMatrix);
                break;
            case 4:
                RenderMesh2D(meshes["star4"], shaders["VertexColor"], modelMatrix);
                break;

            default:
                break;

        }

    }

}
void Tema1::Update(float deltaTimeSeconds)
{   //std::uniform_real_distribution<int> randLine(0, 2);
    if(lives==0){
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> randAppearanceTime(7.0f, 10.0f);
    randomAppearanceTime=randAppearanceTime(gen);
    // TODO(student): Update steps for translation, rotation and scale,
    translateX -=  deltaTimeSeconds * 150;
    RenderScene();
    RenderGrid();
    CollisionHexaProj();
    CollisionHexaRomb();
    RenderEnemies(deltaTimeSeconds);
    RenderRomb();
    RenderRombOnGrid(deltaTimeSeconds);
    RandomStar(deltaTimeSeconds);
    modelMatrix = glm::mat3(1);
    modelMatrix*= transform2D::Translate(1500, 1130);
    modelMatrix *= transform2D::Scale(1.6f, 1.6f);
    for(int i=0; i<lives; i++){
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(170, 0);
    }
    RenderGreyStars();
    RenderProjectiles(deltaTimeSeconds);


}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float yr=1440-2*mouseY;
    if(ok==1 && window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        //  int i=0;
        for (auto &romb: rombs) {
            if(romb.idx==whichRomb){
                cout<<"in which romb\n";
                romb.x = 2*mouseX;
                romb.y =yr;
            }
        }
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    renderagain=1;
    mystar=720-mouseY;
    myromb=1440-2*mouseY;

    int j=0;
    for (const auto &star: pinkstars) {
        if (button==1 && mouseX >= star.x/2 - 30.0f && mouseX <= star.x/2 + 30.0f &&
            mystar >= star.y/2 - 30.0f && mystar <= star.y/2 + 30.0f)
        {
            float randomX = 1495.0f;
            float randomY = 1110.0f;
            cout<<"stelute\n";
            Star newStar;
            newStar.x = randomX;
            newStar.y = randomY;
            greystars.push_back(newStar);
            pinkstars.erase(pinkstars.begin()+j);
        }j++;

    }


    unsigned int vecSize = greystars.size();
    for (auto &romb:initialrombs) {
//

        if (mouseX >= romb.x / 2 - 50.0f && mouseX <= romb.x / 2 + 50.0f &&
            mystar >= romb.y / 2 - 50.0f && mystar <= romb.y / 2 + 50.0f && romb.cost<=vecSize ) {
            //num=romb.cost;
            Rhombus r(romb.idx,mouseX * 2,myromb, romb.cost);
            cout<<"cost "<<romb.cost<<endl;
            eliminStar=true;
            rombs.push_back(r);
            whichRomb = romb.idx;
            cout << "in button press rombs\n";
            ok = 1;
            whichIsStatic=romb.idx;

        }
    }
    if(button==2) {
        for (auto &romb: staticrombs) {
//

            if (mouseX >= romb.x / 2 - 50.0f && mouseX <= romb.x / 2 + 50.0f &&
                mystar >= romb.y / 2 - 50.0f && mystar <= romb.y / 2 + 50.0f) {
                romb.dis = true;

            }
        }
    }
//dreapta 2 degete; stg unul sg
}




void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

    ok=0;


    for (int i=0; i<rombs.size(); i++) {

        if(whichIsStatic==rombs[i].idx){
           if(rombs[i].y<=290 && rombs[i].y>=0  && rombs[i].x>=170 &&rombs[i].x<=300){
               rombs[i].x=270;
               rombs[i].y=130;
           }
            if(rombs[i].y<=290 && rombs[i].y>=0  && rombs[i].x>=500 &&rombs[i].x<=630){
                rombs[i].x=570;
                rombs[i].y=130;
            }
            if(rombs[i].y<=290 && rombs[i].y>=0  && rombs[i].x>=850 &&rombs[i].x<=960){
                rombs[i].x=870;
                rombs[i].y=130;
            }
            if(rombs[i].y<=580 && rombs[i].y>=290 && rombs[i].x>=170 &&rombs[i].x<=300){
                rombs[i].x=270;
                rombs[i].y=430;
            }
            if(rombs[i].y<=580 && rombs[i].y>=290  && rombs[i].x>=500 &&rombs[i].x<=630){
                rombs[i].x=570;
                rombs[i].y=430;
            }
            if(rombs[i].y<=580 && rombs[i].y>=290  && rombs[i].x>=850 &&rombs[i].x<=960){
                rombs[i].x=870;
                rombs[i].y=430;
            }
            if(rombs[i].y>=590 && rombs[i].x>=170 &&rombs[i].x<=300){
                rombs[i].x=270;
                rombs[i].y=730;
            }
            if(rombs[i].y>=590&& rombs[i].x>=500 &&rombs[i].x<=630){
                rombs[i].x=570;
                rombs[i].y=730;
            }
            if(rombs[i].y>=590 && rombs[i].x>=850 &&rombs[i].x<=960){
                rombs[i].x=870;
                rombs[i].y=730;
            }
             float x=rombs[i].x;
             float y=rombs[i].y;
            Rhombus rh(rombs[i].idx,x,y,rombs[i].cost,1,1,0.0f,false);
            staticrombs.push_back(rh);
            cout<<"coord romb "<< x<<"si"<<y<<endl;
            posProjX=x;
            posProjY=y;
            colorProj=rh.idx;
            num=rombs[i].cost;
            rombs.erase(rombs.begin() + i);

        }

    }
    if(eliminStar) {
        goProj=true;
        eliminStar= false;
        if (num <= greystars.size()) {
            cout << "sterg " << num << "din " << greystars.size() << endl;
            greystars.erase(greystars.end() - num, greystars.end());
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}