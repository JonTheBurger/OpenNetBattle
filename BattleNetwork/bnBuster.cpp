#include <random>
#include <time.h>

#include "bnBuster.h"
#include "bnBusterHit.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

#include "bnGear.h" 

#define COOLDOWN 40.0f/1000.0f

Buster::Buster(Field* _field, Team _team, bool _charged, int damage) : isCharged(_charged), Spell(_field, _team) {
  SetPassthrough(true);
  SetLayer(-100);

  cooldown = 0;

  hit = false;
  progress = 0.0f;

  if (isCharged) {
    hitHeight = 20.0f;
  }
  else {
    hitHeight = 0.0f;
  }

  random = 0;

  animationComponent = new AnimationComponent(this);
  this->RegisterComponent(animationComponent);

  if (_charged) {
    texture = TEXTURES.GetTexture(TextureType::SPELL_CHARGED_BULLET_HIT);
    animationComponent->SetPath("resources/spells/spell_charged_bullet_hit.animation");
    animationComponent->Reload();
    animationComponent->SetAnimation("HIT");
  } else {
    texture = TEXTURES.GetTexture(TextureType::SPELL_BULLET_HIT);
    animationComponent->SetPath("resources/spells/spell_bullet_hit.animation");
    animationComponent->Reload();
    animationComponent->SetAnimation("HIT");
  }
  setScale(2.f, 2.f);

  AUDIO.Play(AudioType::BUSTER_PEA, AudioPriority::HIGH);

  auto props = Hit::DefaultProperties;
  props.flags = props.flags & ~Hit::recoil;
  props.damage = damage;
  this->SetHitboxProperties(props);

  contact = nullptr;
  spawnGuard = false;
}

Buster::~Buster() {
}

void Buster::OnUpdate(float _elapsed) {
  GetTile()->AffectEntities(this);

  cooldown += _elapsed;
  if (cooldown >= COOLDOWN) {
    if (Move(GetDirection())) {
      AdoptNextTile();
      FinishMove();
      cooldown = 0;
    }
    else {
      this->Delete();
    }
  }

}

bool Buster::CanMoveTo(Battle::Tile * next)
{
  return true;
}

void Buster::OnDelete()
{
}

void Buster::Attack(Character* _entity) {
  if (_entity->Hit(this->GetHitboxProperties())) {
    hit = true;  

    hitHeight = (float)(std::floor(_entity->GetHeight()));

    if (!isCharged) {
      random = _entity->getLocalBounds().width / 2.0f;
      random *= rand() % 2 == 0 ? -1.0f : 1.0f;

      if (hitHeight > 0) {
        hitHeight = (float)(rand() % (int)hitHeight);
      }
    }
    else {
      hitHeight /= 2;
    }

    auto bhit = new BusterHit(GetField(), isCharged ? BusterHit::Type::CHARGED : BusterHit::Type::PEA);
    bhit->SetOffset({ random, GetHeight() + hitHeight });
    GetField()->AddEntity(*bhit, *this->GetTile());
  }

  if (hit) {
    this->Delete();
    AUDIO.Play(AudioType::HURT);
  }
}