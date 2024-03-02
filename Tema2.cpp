#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

struct Tank {
    glm::vec3 position;
    float hp;
};
std::vector<Tank> tanks;

struct Proiectil {
	glm::vec3 position;
	glm::vec3 direction;
	float speed;
	float time;
    bool canCollide;
    bool tankPunch;
    bool isPlayerProiectil;
    bool ok;
};
std::vector<Proiectil> proiectile;

//enum class MovementState {
//    GoingForward,
//    GoingBackward,
//    InPlaceRotationLeft,
//    InPlaceRotationRight,
//    // Alte stări...
//};

struct TankInamic {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 initialDirection;
    float speed;
    glm::vec3 color;
    glm::vec3 size;
    bool isMovingForward;
    float moveDistance;
    float movedDistance;
    float rotation;

    float rotationSpeed;     // Viteza de rotație
    float targetRotation;    // Rotația țintă
    bool isRotating;         // Dacă inamicul este în procesul de rotație
    float rotationTimer;     // Timpul până la următoarea rotație
    float moveTimer;         // Timpul până la următoarea mișcare
    bool isMoving;           // Dacă inamicul este în procesul de mișcare
    float cooldownTimer;     // Timpul până la următorul atac
    float turretRotation;    // Rotația turetei
    float cooldowncurrent;          // Timpul de cooldown

    glm::vec3 collisionTranslation; // Deplasarea necesară p
    float hp;
};
std::vector<TankInamic> tanksInamici;

struct Cladire {
	glm::vec3 position;
	glm::vec3 size;
    glm::vec3 color;
};
std::vector<Cladire> cladiri;

const float TANK_RADIUS = 0.6f;

bool CheckIntersectionjucator(const Cladire& cladire, const Tank& tank) {
    // Simplificat, presupune că toate obiectele sunt paralelipipede aliniate cu axele
    // și verifică suprapunerea pe fiecare axă
    float TANK_SIZE_X = 0.5f;
    float TANK_SIZE_Z = 0.5f;
    return (abs(cladire.position.x - tank.position.x) * 2 < (cladire.size.x + TANK_SIZE_X)) &&
        (abs(cladire.position.z - tank.position.z) * 2 < (cladire.size.z + TANK_SIZE_Z));
}


void Tema2::Init()
{
    /*renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));*/
    
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bazatank");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "bazatunului.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sina1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sina1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sina2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sina2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sinele");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sinele.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cap");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "capnou.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("proiectil");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "proiectil.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cladiri");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cladiri.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(0, 1, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    tankPosition = glm::vec3(0, 0, 0);
    tankRotation = 0;
    capRotation = 0;
    timpCooldownJucator = 1.0f;
    coolDownCurentJucator = 0.0f;
    timpTotal = 0.0f;
    gameOver = false;

    for (int i = 0; i < 20; i++) {
        TankInamic inamic;

        float positionX = (float)(rand() % 20) - 10.0f;
        float positionZ = (float)(rand() % 20) - 10.0f;
        float unghi = static_cast<float>(rand() / (float)RAND_MAX * 2 * glm::pi<float>());

        inamic.initialDirection = glm::vec3(cos(unghi), 0, -sin(unghi));
        inamic.position = glm::vec3(positionX, 0, positionZ);
        inamic.direction = inamic.initialDirection;
        inamic.speed = 2.0f;
        inamic.color = glm::vec3(0.5, 0.5, 0.5);
        inamic.size = glm::vec3(0.15f);
        inamic.isMovingForward = true;
        inamic.moveDistance = 10.0f;
        inamic.movedDistance = 0.0f;
        inamic.rotation = unghi;

        inamic.rotationSpeed = 0.5f; // sau o valoare aleatorie
        inamic.isRotating = false;
        inamic.rotationTimer = static_cast<float>(rand() % 5 + 1); // 1-5 secunde
        inamic.moveTimer = static_cast<float>(rand() % 5 + 1); // 1-5 secunde
        inamic.isMoving = true;
        inamic.cooldownTimer = 1.0f;
        inamic.cooldowncurrent = 0.0f;

		tanksInamici.push_back(inamic);
	}

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 30; i++) {
		Cladire cladire;

        cladire.position = glm::vec3(rand() % 40 - 40 / 2, 0, rand() % 40 - 40 / 2);
        cladire.size = glm::vec3(
            0.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.6f - 0.3f))),
            0.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.6f - 0.3f))),
            0.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.6f - 0.3f)))
        );
        cladire.color = glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // Culoare aleatorie

        bool interesecteaza = false;
        for (auto& tank : tanks) {
            if (CheckIntersectionjucator(cladire, tank)) {
				interesecteaza = true;
				break;
			}
		}

        if (!interesecteaza) {
            cladiri.push_back(cladire);
        }
        else {
            i--;
        }
		
	}   
}


inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
{
    return glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translateX, translateY, translateZ, 1.0f);
}



void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    float cameraDistance = 3.0f;
    float cameraHeight = 1.5f;

    if (!gameOver) {
		timpTotal += deltaTimeSeconds;
        if (timpTotal > 60.0f) {
			gameOver = true;
            window->Close();
        }
	}

    //glm::vec3 tankForward = glm::normalize(glm::vec3(sin(tankRotation), 0, cos(tankRotation)));

    //glm::vec3 cameraPosition = tankPosition - tankForward * cameraDistance;
    //cameraPosition.y += cameraHeight;

    //glm::vec3 cameraCenter = tankPosition;
    //camera->Set(cameraPosition, cameraCenter, glm::vec3(0, 1, 0));

    glm::mat4 modelMatrix1 = glm::mat4(1); // sina1
    glm::mat4 modelMatrix2 = glm::mat4(1); // sina2
    glm::mat4 modelMatrix3 = glm::mat4(1); // bazatank
    glm::mat4 modelMatrix4 = glm::mat4(1); // cap
    glm::mat4 modelMatrix5 = glm::mat4(1); // tun

    modelMatrix1 = Translate(tankPosition.z, tankPosition.y, -tankPosition.x);
    modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(0.15f));
    modelMatrix1 = glm::rotate(modelMatrix1, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix1 = glm::rotate(modelMatrix1, tankRotation, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["sina1"], shaders["LabShader"], modelMatrix1, glm::vec3(0.72, 0.72, 0.72));

    modelMatrix2 = Translate(tankPosition.z, tankPosition.y, -tankPosition.x);
    modelMatrix2 = glm::rotate(modelMatrix2, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.15f));
    modelMatrix2 = glm::rotate(modelMatrix2, tankRotation, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["sina2"], shaders["LabShader"], modelMatrix2, glm::vec3(0.72, 0.72, 0.72));

    modelMatrix3 = Translate(tankPosition.z, tankPosition.y, -tankPosition.x);
    modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(0.15f));
    modelMatrix3 = glm::rotate(modelMatrix3, RADIANS(90.0f), glm::vec3(0, 1, 0));
    modelMatrix3 = glm::rotate(modelMatrix3, tankRotation, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["bazatank"], shaders["LabShader"], modelMatrix3, glm::vec3(0.09, 0.22, 0.11));

    modelMatrix4 = Translate(tankPosition.z, tankPosition.y, -tankPosition.x);
    modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.15f));
    modelMatrix4 = glm::rotate(modelMatrix4, RADIANS(90.0f), glm::vec3(0, 1, 0));
    modelMatrix4 = glm::rotate(modelMatrix4, tankRotation, glm::vec3(0, 1, 0));
    modelMatrix4 = glm::rotate(modelMatrix4, capRotation, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["cap"], shaders["LabShader"], modelMatrix4, glm::vec3(0.5, 0.76, 0.42));

    modelMatrix5 = Translate(tankPosition.z, tankPosition.y, -tankPosition.x);
    modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.15f));
    modelMatrix5 = glm::rotate(modelMatrix5, RADIANS(90.0f), glm::vec3(0, 1, 0));
    modelMatrix5 = glm::rotate(modelMatrix5, tankRotation, glm::vec3(0, 1, 0));
    modelMatrix5 = glm::rotate(modelMatrix5, capRotation, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix5, glm::vec3(0.72, 0.72, 0.72));

    RenderProjectiles();

    for (auto& inamic : tanksInamici) {

        glm::vec3 positionAux = glm::vec3(-inamic.position.x, inamic.position.y, inamic.position.z);
        float distanta = glm::distance(inamic.position, tankPosition);

        // verifica daca jucatorul este in raza de actiune a inamicului
        if (distanta < 4) {
            glm::vec3 tankPositionAux;

            if (tankPosition.x < inamic.position.x && tankPosition.z > inamic.position.z) {
                tankPositionAux = glm::vec3(-inamic.position.x, inamic.position.y, inamic.position.z);
            }
            else if (tankPosition.x > inamic.position.x && tankPosition.z < inamic.position.z) {
                tankPositionAux = glm::vec3(inamic.position.x, inamic.position.y, inamic.position.z);
            }
            else if (tankPosition.x < inamic.position.x && tankPosition.z < inamic.position.z) {
                tankPositionAux = glm::vec3(inamic.position.x, inamic.position.y, -inamic.position.z);
            }
            else if (tankPosition.x > inamic.position.x && tankPosition.z > inamic.position.z) {
                tankPositionAux = glm::vec3(-inamic.position.x, inamic.position.y, -inamic.position.z);
            }

            glm::vec3 directieCatreJucator = glm::normalize(tankPosition - tankPositionAux);
            float unghiNecesar = atan2(directieCatreJucator.z, directieCatreJucator.x);

            // orientare tureta
            inamic.turretRotation = unghiNecesar;

            if (inamic.cooldowncurrent <= 0.0f) {
                inamic.cooldowncurrent = inamic.cooldownTimer;

                Proiectil proiectil;
                glm::vec3 pozitieProiectil = inamic.position;
                pozitieProiectil.x = -pozitieProiectil.x;
                proiectil.position = pozitieProiectil;
                if (tankPosition.x > inamic.position.x) {
                    if (tankPosition.z > inamic.position.z) {
                        proiectil.direction = glm::vec3(-cos(inamic.turretRotation), 0, sin(inamic.turretRotation));
                    }
                    else {
                        proiectil.direction = glm::vec3(-cos(inamic.turretRotation), 0, sin(inamic.turretRotation));
                    }
                    proiectil.ok = true;
				}
                else {
                    if (tankPosition.z > inamic.position.z) {
                        proiectil.direction = glm::vec3(cos(inamic.turretRotation), 0, sin(inamic.turretRotation));
                    }
                    else {
                        proiectil.direction = -glm::vec3(cos(inamic.turretRotation), 0, sin(inamic.turretRotation));
                    }
                }
                proiectil.speed = 1.0f;
                proiectil.time = 5.0f;
                proiectil.canCollide = false;
                proiectil.tankPunch = false;
                proiectil.isPlayerProiectil = false;
                proiectile.push_back(proiectil);
            }
        }

        if (coolDownCurentJucator > 0) {
            coolDownCurentJucator -= deltaTimeSeconds;
        }

        bool proiectilSters = false;
        //actualizeaza proiectilele
        for (int i = 0; i < proiectile.size() && !proiectilSters; i++) {
            proiectile[i].position += proiectile[i].direction * proiectile[i].speed * deltaTimeSeconds;
            proiectile[i].time -= deltaTimeSeconds;
            if (proiectile[i].time <= 0) {
                proiectile.erase(proiectile.begin() + i);
                i--;
                continue;
            }
            //coliziuni cu inamicii;
            if (proiectile[i].canCollide) {
                for (int j = 0; j < tanksInamici.size(); j++) {
                    glm::vec3 pozitieProiectil = proiectile[i].position;
                    glm::vec3 pozitieInamic = tanksInamici[j].position;
                    float distanta = glm::distance(pozitieProiectil, pozitieInamic);
                    if (distanta < 0.5 && proiectile[i].canCollide) {
                        tanksInamici[j].hp -= 10;
                        proiectile.erase(proiectile.begin() + i);
                        i--;
                        proiectilSters = true;
                        break;
                    }
                }
            }
            else {
                proiectile[i].canCollide = true;
            }

            // coliziune cu jucatorul
            if (!proiectilSters) {
                if (!proiectile[i].isPlayerProiectil) {
                    glm::vec3 pozitieProiectil = proiectile[i].position;
                    float distanta = glm::distance(pozitieProiectil, tankPosition);
                    if (distanta < 0.5 && proiectile[i].canCollide) {
                        coolDownCurentJucator = timpCooldownJucator;
                        proiectile.erase(proiectile.begin() + i);
                        i--;
                        proiectilSters = true;
                        break;
                    }
                }
            }


            // coliziuni cu cladiri
            if (!proiectilSters) {
                for (int j = 0; j < cladiri.size(); j++) {
                    if (glm::distance(proiectile[i].position, cladiri[j].position) < 0.5) {
                        proiectile.erase(proiectile.begin() + i);
                        i--;
                        proiectilSters = true;
                        break;
                    }
                }
            }
            proiectilSters = false;
        }

        if (inamic.cooldowncurrent > 0) {
			inamic.cooldowncurrent -= deltaTimeSeconds;
		}

        if (inamic.isRotating) {
            // Rotește inamicul
            inamic.rotation += inamic.rotationSpeed * deltaTimeSeconds;
            inamic.rotationTimer -= deltaTimeSeconds;

            if (inamic.rotationTimer <= 0) {
                inamic.isRotating = false;
                inamic.isMoving = true;
                inamic.moveTimer = static_cast<float>(rand() % 5 + 1);
                inamic.direction = glm::vec3(cos(inamic.rotation), 0, -sin(inamic.rotation));
            }
        }
        else if (inamic.isMoving) {
            // Mișcă inamicul înainte și înapoi
            float directionMultiplier = inamic.isMovingForward ? 1.0f : -1.0f;
            inamic.position += inamic.direction * inamic.speed * deltaTimeSeconds * directionMultiplier;
            inamic.movedDistance += inamic.speed * deltaTimeSeconds;

            if (inamic.movedDistance >= inamic.moveDistance) {
                inamic.isMovingForward = (rand() % 2) == 0;
                inamic.movedDistance = 0.0f;
                inamic.isMoving = false;
                inamic.isRotating = true;
                inamic.rotationTimer = static_cast<float>(rand() % 5 + 1);
            }
        }

        RenderScene();

    }



    // Coliziuni cu inamicii
    CheckAndResolveCollisions();

    RenderScene();


}


void Tema2::CheckAndResolveCollisions() {
    for (int i = 0; i < tanksInamici.size(); i++) {
        for (int j = i + 1; j < tanksInamici.size(); j++) {
            glm::vec3& pozitieTanc1 = tanksInamici[i].position;
            glm::vec3& pozitieTanc2 = tanksInamici[j].position;
            float distantaIntreTancuri = glm::distance(pozitieTanc1, pozitieTanc2);

            if (distantaIntreTancuri < 2 * TANK_RADIUS) {
                float P = (2 * TANK_RADIUS - distantaIntreTancuri);
                glm::vec3 directieDeplasare = glm::normalize(pozitieTanc1 - pozitieTanc2);

                pozitieTanc1 += directieDeplasare * P * 0.5f;
                pozitieTanc2 -= directieDeplasare * P * 0.5f;

            }
        }
    }

    // Verifică și rezolvă coliziunile între tancuri și clădiri
    for (auto& tank : tanksInamici) {
        for (auto& cladire : cladiri) {
            float distanta = glm::distance(tank.position, cladire.position);
            if (distanta < 2 * TANK_RADIUS) {
				float P = (2 * TANK_RADIUS - distanta);
				glm::vec3 directieDeplasare = glm::normalize(tank.position - cladire.position);

				tank.position += directieDeplasare * P * 0.5f;
			}
        }
    }

    //verificare coliziuni intre tankul meu si inamici
    for (auto& tank : tanksInamici) {
		float distanta = glm::distance(tank.position, tankPosition);
        if (distanta < 2 * TANK_RADIUS) {
			float P = (2 * TANK_RADIUS - distanta);
			glm::vec3 directieDeplasare = glm::normalize(tank.position - tankPosition);

			tank.position += directieDeplasare * P * 0.5f;
		}
	}
}

void Tema2::RenderProjectiles() {
    for (auto& projectile : proiectile) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        if (projectile.tankPunch) {
            modelMatrix = Translate(projectile.position.z, projectile.position.y, -projectile.position.x);
		}
        else if (projectile.canCollide && !projectile.ok) {
			modelMatrix = Translate(projectile.position.z, projectile.position.y, projectile.position.x);
		}
        else if (projectile.ok) {
            modelMatrix = Translate(projectile.position.z, projectile.position.y, projectile.position.x);
        }
        else {
			modelMatrix = Translate(projectile.position.z, projectile.position.y, -projectile.position.x);
		}
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f)); // Mărimea proiectilului
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    }
}

void Tema2::RenderScene() {
    // Randarea inamicilor
    for (auto& inamic : tanksInamici) {

        glUseProgram(shaders["LabShader"]->program);
        glUniform1f(glGetUniformLocation(shaders["LabShader"]->program, "hp"), inamic.hp);


        glm::mat4 modelMatrix1 = glm::mat4(1); // sina1
        glm::mat4 modelMatrix2 = glm::mat4(1); // sina2
        glm::mat4 modelMatrix3 = glm::mat4(1); // bazatank
        glm::mat4 modelMatrix4 = glm::mat4(1); // cap
        glm::mat4 modelMatrix5 = glm::mat4(1); // tun

        modelMatrix1 = Translate(inamic.position.z, inamic.position.y, -inamic.position.x);
        modelMatrix1 = glm::scale(modelMatrix1, inamic.size);
        modelMatrix1 = glm::rotate(modelMatrix1, RADIANS(270), glm::vec3(0, 1, 0));
        modelMatrix1 = glm::rotate(modelMatrix1, inamic.rotation, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["sina1"], shaders["LabShader"], modelMatrix1, glm::vec3(0.72, 0.72, 0.72));

        modelMatrix2 = Translate(inamic.position.z, inamic.position.y, -inamic.position.x);
        modelMatrix2 = glm::rotate(modelMatrix2, RADIANS(270), glm::vec3(0, 1, 0));
        modelMatrix2 = glm::scale(modelMatrix2, inamic.size);
        modelMatrix2 = glm::rotate(modelMatrix2, inamic.rotation, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["sina2"], shaders["LabShader"], modelMatrix2, glm::vec3(0.72, 0.72, 0.72));
        
        glm::vec3 InamicAux = glm::vec3(-inamic.position.x, inamic.position.y, inamic.position.z);
        modelMatrix3 = Translate(InamicAux.z, InamicAux.y, InamicAux.x);
        modelMatrix3 = glm::scale(modelMatrix3, inamic.size);
        modelMatrix3 = glm::rotate(modelMatrix3, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrix3 = glm::rotate(modelMatrix3, inamic.rotation, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["bazatank"], shaders["LabShader"], modelMatrix3, glm::vec3(0.27, 0.18, 0.003));

        modelMatrix4 = Translate(inamic.position.z, inamic.position.y, -inamic.position.x);
        modelMatrix4 = glm::scale(modelMatrix4, inamic.size);
        modelMatrix4 = glm::rotate(modelMatrix4, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrix4 = glm::rotate(modelMatrix4, inamic.rotation, glm::vec3(0, 1, 0));
        //modelMatrix4 = glm::rotate(modelMatrix4, inamic.turretRotation, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["cap"], shaders["LabShader"], modelMatrix4, glm::vec3(0.47, 0.36, 0.18));

        modelMatrix5 = Translate(inamic.position.z, inamic.position.y, -inamic.position.x);
        modelMatrix5 = glm::scale(modelMatrix5, inamic.size);
        modelMatrix5 = glm::rotate(modelMatrix5, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrix5 = glm::rotate(modelMatrix5, inamic.rotation, glm::vec3(0, 1, 0));
        //modelMatrix5 = glm::rotate(modelMatrix5, inamic.turretRotation, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix5, glm::vec3(0.72, 0.72, 0.72));

        
    }

    for (auto& cladire : cladiri) {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = Translate(cladire.position.z, cladire.position.y, -cladire.position.x);
		modelMatrix = glm::scale(modelMatrix, cladire.size);
		RenderSimpleMesh(meshes["cladiri"], shaders["LabShader"], modelMatrix, cladire.color);
	}
}



void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}



void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->program)
		return;

	// Render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	mesh->Render();
}  

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

    GLint loc = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(loc, materialShininess);

    loc = glGetUniformLocation(shader->program, "material_kd");  // componenta difuza
    glUniform1f(loc, materialKd);

    loc = glGetUniformLocation(shader->program, "material_ks");  // componenta speculara
    glUniform1f(loc, materialKs);

    loc = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(loc, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTimeSeconds, int mods)
{
    float speed = 2.0f; // viteza de miscare a tancului
    float rotationSpeed = 2.0f; // viteza de rotatie a tancului

    if (window->KeyHold(GLFW_KEY_W)) {
        tankPosition += glm::vec3(cos(tankRotation), 0, -sin(tankRotation)) * deltaTimeSeconds * speed;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        tankRotation += deltaTimeSeconds * rotationSpeed;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        tankPosition -= glm::vec3(cos(tankRotation), 0, -sin(tankRotation)) * deltaTimeSeconds * speed;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        tankRotation -= deltaTimeSeconds * rotationSpeed;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        //float sensivityOX = 0.001f;
        //float sensivityOY = 0.001f;

        //if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
        //    // Third-person camera rotation
        //    camera->RotateThirdPerson_OX(deltaY * sensivityOX);
        //    camera->RotateThirdPerson_OY(deltaX * sensivityOY);
        //}
        //else {
        //    // First-person camera rotation
        //    camera->RotateFirstPerson_OX(deltaY * sensivityOX);
        //    camera->RotateFirstPerson_OY(deltaX * sensivityOY);
        //}
    }
    else if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
    {
        // miscarea turetei
        float sensivityOX = 0.003f;
        capRotation -= deltaX * sensivityOX;

    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        if (coolDownCurentJucator > 0) {
			return;
		}
        
        coolDownCurentJucator = timpCooldownJucator;
        Proiectil p;
        glm::vec3 forwardDirection = glm::normalize(glm::vec3(cos(tankRotation + capRotation), 0, -sin(tankRotation + capRotation)));
        float offset = 0.7;
        glm::vec3 startPosition = tankPosition + forwardDirection * offset; // 1.0f este un offset pentru a începe de la vârful tunului
        startPosition.y += 0.35f; // 0.1f este un offset pentru a începe de la vârful tunului
        p.position = startPosition;
        p.direction = forwardDirection;
        p.speed = 2.0f;
        p.time = 5.0f;
        p.tankPunch = true;
        p.isPlayerProiectil = true;

        proiectile.push_back(p);
    }
    RenderProjectiles();

}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
