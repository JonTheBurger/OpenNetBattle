/*! \brief Tiles represent where entities can move around on.
 * 
 *  \class Tile
 * 
 * Without a tile, an entity cannot exist in the battle field.
 * Tiles have states such as LAVA, ICE, GRASS, NORMAL, CRACKED, BROKEN, and EMPTY.
 * Each state may affect an entity that lands on it or prevent other entities 
 * from occupying them.
 * 
 * The tiles are responsible for updating everything that exists on it. To achieve this
 * each type of entity has divided into different buckets: artifacts, spells, characters, 
 * and obstacles. Spells can affect characters and obstacles can affect both spells and 
 * other characters while artifacts are seen as purely visual and don't affect others.
 * 
 * When entities move they adopt new tiles and should remove themselves from their
 * previous tile. The tile will remove the entity from its appropriate bucket.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
using sf::RectangleShape;
using sf::Sprite;
using std::vector;
using std::find;
using std::set;

class Entity;
class Spell;
class Character;
class Obstacle;
class Artifact;
class Field;

#include "bnTeam.h"
#include "bnMemory.h"
#include "bnTextureType.h"
#include "bnTileState.h"

namespace Battle {
  class Tile : public Sprite {
  public:
    friend class Entity;

    Tile(int _x, int _y);
    ~Tile();

    Tile(const Tile& rhs);
    Tile& operator=(const Tile& other);

    const TileState GetState() const;

    void SetField(Field* _field);

    int GetX() const;
    int GetY() const;

    Team GetTeam() const;
    void SetTeam(Team _team);

    float GetWidth() const;
    float GetHeight() const;

    void SetState(TileState _state);

    void RefreshTexture();

    bool IsWalkable() const;
    bool IsCracked() const;

    bool IsHighlighted() const;

    void AddEntity(Spell& _entity);
    void AddEntity(Character& _entity);
    void AddEntity(Obstacle& _entity);
    void AddEntity(Artifact& _entity);

    void RemoveEntityByID(int ID);
    bool ContainsEntity(Entity* _entity) const;

    void ReserveEntityByID(int ID);

    template<class Type> bool ContainsEntityType();
    void AffectEntities(Spell* caller);

    bool GetNextEntity(Entity*& out) const;

    void Update(float _elapsed);

    void SetBattleActive(bool state);

    std::vector<Entity*> FindEntities(std::function<bool(Entity*e)> query);

    // Todo: use sets to avoid duplicate entries
    vector<Artifact*> artifacts;
    vector<Spell*> spells;
    vector<Character*> characters;
    vector<Entity*> entities;

    set<int> reserved;
  private:
    int x;
    int y;
    Team team;
    TileState state;
    TextureType textureType;
    float elapsed;

    float width;
    float height;
    Field* field;
    float cooldown;
    float cooldownLength;
    bool hasSpell;
    bool isBattleActive;

    std::vector<long> taggedSpells;

    // Aux
    void AddEntity(Entity* _entity);
  };


  template<class Type>
  bool Tile::ContainsEntityType() {
    // std::cout << "len of entities is: " << entities.size() << "\n";

    for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
      if (dynamic_cast<Type*>(*it) != nullptr) {
        return true;
      }
    }

    return false;
  }
}