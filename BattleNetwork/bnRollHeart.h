/*! \brief Part of roll summon. Heart appears from the top of the screen slowly descending 
 * 
 * NOTE: The card summon system is going under major refactoring and this
 * code will not be the same
 * 
 * NOTE: This should just be an artifact since it is used as effect
 */

#pragma once

#include "bnSpell.h"
#include "bnCardSummonHandler.h"
#include "bnAnimationComponent.h"


class RollHeart : public Spell {
public:
  RollHeart(CardSummonHandler* _summons, int _heal);
  ~RollHeart();

  /**
   * @brief Descend and then heal the player
   * @param _elapsed
   */
  void OnUpdate(float _elapsed) override;
  
  /**
   * @brief does not move
   * @param _direction ignored
   * @return false
   */
  bool Move(Direction _direction) override;
  
  /**
   * @brief Does nothing
   * @param _entity
   */
  void Attack(Character* _entity) override; 

  /** 
  * @brief Does nothing/
  */
  void OnDelete() override;

private:
  int heal; /*!< How much to heal */
  float height; /*!< The start height of the heart */
  Character* caller; /*!< The character that used the card */
  CardSummonHandler* summons; /*!< The card summon system */
  AnimationComponent* animationComponent;
  bool doOnce; /*!< Flag to restore health once */
};
