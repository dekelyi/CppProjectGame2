#include "Player.h"
#include "Collectible.h"
#include "Room.h"

M_CODE Player::handle_collision(GameRoom* room, MapObject* other, Move& move)
{
	Player* p = dynamic_cast<Player*>(other);
	if (p && move.kind == Move::EVENT && std::find(moves.begin(), moves.end(), move) == moves.end()) {
		moves.remove_if([move](const Move& m) { return m.dir.is_same_direction(move.dir); });
		moves.push_back(move);
		return try_move(room, move) ? CAN_MOVE : CANT_MOVE;
	}
	else return MapObject::handle_collision(room, other, move);
}

/**
 * @brief Drop the currently held collectible into the provided room.
 *
 * If the player is carrying a collectible, this sets the collectible's
 * position to the player's current position plus an available adjacent
 * offset (determined by `getDumpPosition`), adds it to the room, draws it,
 * and clears the player's `collectible` pointer.
 *
 * @param room Pointer to the GameRoom where the collectible should be placed.
 *             Must be non-null.
 *
 * @notes
 * - If no collectible is held (`collectible == nullptr`) this function does nothing.
 * - The chosen drop position is the first empty adjacent cell (excluding
 *   the cell in the player's current `direction`). If no empty cell exists
 *   `getDumpPosition` returns `V(0,0)` and the collectible will be placed
 *   at the player's position.
 */
void Player::dump_collectible(GameRoom* room) {
	if (collectible == nullptr) return;
	collectible->setPosition(this->getPosition() + this->getDumpPosition(room));
	room->add_object(collectible);
	collectible = nullptr;
}


/**
 * @brief Determine a valid adjacent offset to drop a collectible.
 *
 * Considers the four cardinal offsets (up, down, left, right) but excludes
 * the player's current `direction` (so the player won't drop directly in
 * the direction they are facing). Iterates the remaining offsets and returns
 * the first one that refers to an empty cell in `room`.
 *
 * @param room Pointer to the GameRoom used to test for occupied cells.
 *             Must be non-null.
 * @return A `V` offset representing where to place the dropped collectible
 *         relative to the player's current position. Returns `V(0,0)` if no
 *         empty adjacent cell is found.
 */
V Player::getDumpPosition(GameRoom* room) {
	// FIXME: out of bounds
	std::vector<V> arr = { V(0,-1), V(0,1), V(-1,0), V(1,0) };
	//arr.erase(std::remove(arr.begin(), arr.end(), this->direction), arr.end());
	for (V v : arr) {
		V p = this->getPosition() + v;
		if (!p.is_out_of_bounds() && room->get_object_at(p) == nullptr)
			return v;
	}
	return V(0, 0);
}