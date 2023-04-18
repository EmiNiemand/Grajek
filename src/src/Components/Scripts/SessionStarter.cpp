#include "Components/Scripts/SessionStarter.h"

SessionStarter::SessionStarter(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

SessionStarter::~SessionStarter() {}

void SessionStarter::Start() {
    Component::Start();
}

void SessionStarter::ShowMenu() {

}

void SessionStarter::HideMenu() {

}

void SessionStarter::ChangeActiveButton(glm::vec2 moveVector) {

}

void SessionStarter::OnClick() {

}
