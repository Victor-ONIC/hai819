#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "ShaderManager.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class GameEngine {
public:
    static GameEngine& getInstance() { //  Permet d'avoir une instance unique statique à la class
        static GameEngine instance;
        return instance;
    }

private:
    GameEngine();
    ~GameEngine();
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
    //ShaderManager m_SM = ShaderManager::getInstance;
};


#endif
