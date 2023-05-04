//
// Created by masterktos on 01.04.23.
//

#ifndef GLOOMENGINE_PREFAB_H
#define GLOOMENGINE_PREFAB_H

#include <memory>
#include <string>
#include "Components/Scripts/MusicPattern.h"

class GameObject;
class Instrument;

class Prefab {
public:
    // GameObjects
    static std::shared_ptr<GameObject> GetPlayer(std::string name="Player");
    static std::shared_ptr<GameObject> GetCube(std::string name="Cube");
    static std::shared_ptr<GameObject> GetDancingDude(std::string name="dood");
    // UI
    static std::shared_ptr<GameObject> GetShop(std::string name="Shop");
    static std::shared_ptr<GameObject> GetMainMenuScene(std::string name="MainMenuScene");
    // Instruments
    static std::shared_ptr<Instrument> GetInstrument(InstrumentName instrumentName);

    static std::vector<std::shared_ptr<GameObject> (*)(std::string)>& GetCharacters();
    static std::shared_ptr<GameObject> GetDefaultCharacter(std::string name);
};


#endif //GLOOMENGINE_PREFAB_H
