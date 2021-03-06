/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2020  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_SPAWN_H_1A86089E080846A9AE53ED12E7AE863B
#define FS_SPAWN_H_1A86089E080846A9AE53ED12E7AE863B

#include "tile.h"
#include "position.h"

class Monster;
class MonsterType;
class Npc;

struct spawnBlock_t
{
	spawnBlock_t(MonsterType* mType, uint32_t interval, Position pos, Direction direction) : mType(mType), interval(interval), pos(std::move(pos)), direction(direction) {}

	Monster* monster = nullptr;
	MonsterType* mType;
	int64_t lastSpawn = 0;
	uint32_t interval;
	Position pos;
	Direction direction;
};

class Spawn
{
	public:
		Spawn(Position pos) : centerPos(std::move(pos)) {}
		~Spawn();

		// non-copyable
		Spawn(const Spawn&) = delete;
		Spawn& operator=(const Spawn&) = delete;

		// moveable
		Spawn(Spawn&& rhs) noexcept {
			this->spawnMap = std::move(rhs.spawnMap);
			this->centerPos = std::move(rhs.centerPos);
			this->interval = rhs.interval;
			this->checkSpawnEvent = rhs.checkSpawnEvent;
		}
		Spawn& operator=(Spawn&& rhs) noexcept {
			this->spawnMap = std::move(rhs.spawnMap);
			this->centerPos = std::move(rhs.centerPos);
			this->interval = rhs.interval;
			this->checkSpawnEvent = rhs.checkSpawnEvent;
			return *this;
		}

		void scheduleSpawn(size_t spawnId, int32_t interval);
		bool addMonster(const std::string& name, const Position& pos, Direction dir, uint32_t interval);
		void removeMonster(Monster* monster);

		uint32_t getInterval() const {
			return interval;
		}
		void startup();

		void startSpawnCheck();
		void stopEvent();

	private:
		//map of creatures in the spawn
		std::vector<spawnBlock_t> spawnMap;
		size_t spawnIndex = 0;

		Position centerPos;
		uint32_t interval = 60000;
		uint32_t checkSpawnEvent = 0;

		static bool findPlayer(const Position& pos);
		bool spawnMonster(spawnBlock_t& sb, bool startup = false);
		void checkSpawn();
};

class Spawns
{
	public:
		static bool isInZone(const Position& centerPos, int32_t radius, const Position& pos);

		bool loadFromXml(const std::string& filename);
		void startup();
		void clear();

		bool isStarted() const {
			return started;
		}

	private:
		std::vector<Npc*> npcList;
		std::vector<Spawn> spawnList;
		std::string filename;
		bool loaded = false;
		bool started = false;
};

#endif
