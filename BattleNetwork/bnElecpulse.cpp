#include <random>
#include <time.h>

#include "bnElecpulse.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnSharedHitBox.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

Elecpulse::Elecpulse(Field* _field, Team _team, int _damage) : Spell(field, _team) {
  this->SetLayer(0);
  SetPassthrough(true);
  this->SetElement(Element::ELEC);
  this->HighlightTile(Battle::Tile::Highlight::flash);
  setScale(2.f, 2.f);
  progress = 0.0f;

  damage = _damage;
  
  setTexture(TEXTURES.GetTexture(TextureType::SPELL_ELEC_PULSE));

  animation = new AnimationComponent(this);
  this->RegisterComponent(animation);
  animation->SetPath("resources/spells/elecpulse.animation");
  animation->Reload();

  auto onFinish = [this]() { this->Delete(); };
  animation->SetAnimation("PULSE", onFinish);
}

Elecpulse::~Elecpulse(void) {
}

void Elecpulse::OnSpawn(Battle::Tile & start)
{
    auto field = GetField();
    auto forward = GetField()->GetAt(start.GetX()+1, start.GetY());
    auto shared = new SharedHitbox(this);
    field->AddEntity(*shared, *forward);
}

void Elecpulse::OnUpdate(float _elapsed) {
  GetTile()->AffectEntities(this);

  setPosition(tile->getPosition()-sf::Vector2f(-80.0f, 40.0f));
}

bool Elecpulse::Move(Direction _direction) {
  return false;
}

void Elecpulse::Attack(Character* _entity) {
    long ID = _entity->GetID();

    if (std::find(taggedCharacters.begin(), taggedCharacters.end(), ID) != taggedCharacters.end())
        return; // we've already hit this entity somewhere

    Hit::Properties props;
    props.element = this->GetElement();
    props.flags = Hit::recoil | Hit::stun | Hit::drag;
    props.drag = (GetTeam() == Team::RED)? Direction::LEFT : Direction::RIGHT;
    props.damage = damage;

    _entity->Hit(props);
    _entity->SlideToTile(true);

    taggedCharacters.insert(taggedCharacters.begin(), _entity->GetID());
}

