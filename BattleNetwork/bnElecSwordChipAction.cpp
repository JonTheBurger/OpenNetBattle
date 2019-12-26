#include "bnElecSwordChipAction.h"
#include "bnChipAction.h"
#include "bnSpriteSceneNode.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnBasicSword.h"

#define PATH "resources/spells/spell_elec_sword.png"
#define ANIM "resources/spells/spell_elec_sword.animation"

#define FRAME1 { 1, 0.05 }
#define FRAME2 { 2, 0.05 }
#define FRAME3 { 3, 0.3 }

#define FRAMES FRAME1, FRAME2, FRAME3

ElecSwordChipAction::ElecSwordChipAction(Character * owner, int damage) : ChipAction(owner, "PLAYER_SWORD", &attachment, "HILT"), attachmentAnim(ANIM) {
  this->damage = damage;

  this->OverrideAnimationFrames({ FRAMES });

  attachmentAnim.Reload();
  attachmentAnim.SetAnimation("DEFAULT");

  overlay.setTexture(*TextureResourceManager::GetInstance().LoadTextureFromFile(PATH));
  this->attachment = new SpriteSceneNode(overlay);
  this->attachment->SetLayer(-1);
}

ElecSwordChipAction::~ElecSwordChipAction()
{
}

void ElecSwordChipAction::Execute() {
  auto owner = GetOwner();
  owner->AddNode(this->attachment);

  attachmentAnim.Update(0, *this->attachment);

  // On attack frame, drop sword hitbox
  auto onFire = [this, owner]() -> void {
    BasicSword* b = new BasicSword(GetOwner()->GetField(), GetOwner()->GetTeam(), damage);
    auto props = b->GetHitboxProperties();
    props.aggressor = GetOwnerAs<Character>();
    props.element = Element::ELEC;
    props.flags |= Hit::stun;

    b->SetHitboxProperties(props);

    AUDIO.Play(AudioType::SWORD_SWING);

    GetOwner()->GetField()->AddEntity(*b, GetOwner()->GetTile()->GetX() + 1, GetOwner()->GetTile()->GetY());
  };


  this->AddAction(2, onFire);

  this->OnUpdate(0); // position to owner...
}

void ElecSwordChipAction::OnUpdate(float _elapsed)
{
  attachmentAnim.Update(_elapsed, *this->attachment);
  ChipAction::OnUpdate(_elapsed);
}

void ElecSwordChipAction::EndAction()
{
  this->GetOwner()->RemoveNode(attachment);
  GetOwner()->FreeComponentByID(this->GetID());
  delete this;
}
