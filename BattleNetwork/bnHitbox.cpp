#include <random>
#include <time.h>

#include "bnHitbox.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

Hitbox::Hitbox(Field* _field, Team _team, int _damage) : Spell(_field, _team) {
  hit = false;
  damage = _damage;

  auto props = Hit::DefaultProperties;
  props.flags |= Hit::impact;
  props.damage = _damage;
  SetHitboxProperties(props);

  callback = 0;
}

Hitbox::~Hitbox() {
}

void Hitbox::OnSpawn(Battle::Tile & start)
{

}

void Hitbox::OnUpdate(float _elapsed) {
  tile->AffectEntities(this);
  Delete();
}

bool Hitbox::Move(Direction _direction) {
  return false;
}

void Hitbox::Attack(Character* _entity) {
  if (_entity->Hit(GetHitboxProperties()) && callback) {
    callback(_entity);
  }
}

void Hitbox::AddCallback(decltype(callback) callback)
{
  Hitbox::callback = callback;
}

void Hitbox::OnDelete()
{
  Remove();
}