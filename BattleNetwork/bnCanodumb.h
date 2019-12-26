#pragma once
#include "bnAnimatedCharacter.h"
#include "bnMobState.h"
#include "bnAI.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"
#include "bnCounterHitPublisher.h"
#include "bnCanodumbIdleState.h"

/**
 * @class Canodumb
 * @author mav
 * @date 05/05/19
 * @brief Classic cannon enemy waits for opponent to be in view.
 * @warning Legacy code. Should update code.
 */
class Canodumb : public AnimatedCharacter, public AI<Canodumb> {
  friend class CanodumbIdleState;
  friend class CanodumbAttackState;
  friend class CanodumbCursor;

  float hitHeight;
  DefenseRule* virusBody;

public:
  using DefaultState = CanodumbIdleState;

  Canodumb(Rank _rank = Character::Rank::_1);

  virtual ~Canodumb();

  /**
   * @brief When health is low, deletes. Updates AI
   * @param _elapsed
   */
  virtual void OnUpdate(float _elapsed);

  virtual const float GetHeight() const;

  virtual const bool OnHit(const Hit::Properties props);
  virtual void OnDelete();
};