/*! \brief Roll appears and attacks a random enemy 3 times 
 * 
 *  NOTE: The card summon system is going under major refactoring and this
 *  code will not be the same
 */

#pragma once

#include "bnSpell.h"
#include "bnAnimationComponent.h"

class CardSummonHandler;

class RollHeal : public Spell {
public:

  /**
   * @brief Adds itself to the field facing the opposing team based on who summoned it
   * 
   * Prepares animations callbacks
   * @param heal how much to heal the player with
   */
  RollHeal(CardSummonHandler* _summons, int heal);
  
  /**
   * @brief Deconstructor
   */
  virtual ~RollHeal();

  /**
   * @brief Updates the animation
   * @param _elapsed in seconds
   */
  virtual void OnUpdate(float _elapsed);
  
  /**
   * @brief Does not move
   * @param _direction ignored 
   * @return false
   */
  virtual bool Move(Direction _direction);

  /**
   * @brief Deals damage to enemy
   * @param _entity
   */
  virtual void Attack(Character* _entity);

  void DropHitbox(Battle::Tile* target);

private:
  int heal;
  int random;
  CardSummonHandler* summons;
  AnimationComponent* animationComponent;
};
