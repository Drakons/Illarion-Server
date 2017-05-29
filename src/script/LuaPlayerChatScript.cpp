/*
 *  illarionserver - server for the game Illarion
 *  Copyright 2011 Illarion e.V.
 *
 *  This file is part of illarionserver.
 *
 *  illarionserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  illarionserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LuaPlayerChatScript.hpp"
#include "Character.hpp"
#include "character_ptr.hpp"

LuaPlayerChatScript::LuaPlayerChatScript()
    : LuaScript() {
}

LuaPlayerChatScript::LuaPlayerChatScript(const std::string &filename)
    : LuaScript(filename) {
}

LuaPlayerChatScript::~LuaPlayerChatScript() {}

void LuaPlayerChatScript::beforeReceiveText(Character *player, Character::talk_type tt, const std::string &message, Character *cc) {
    character_ptr fuse_player(player);
    character_ptr fuse_cc(cc);
    callEntrypoint("beforeReceiveText", fuse_player, (int)tt, message, fuse_cc);
}

std::string LuaPlayerChatScript::beforeSendText(Character *player, Character::talk_type tt, const std::string &message) {
    character_ptr fuse_player(player);
    return callEntrypoint<std::string>("beforeSendText", fuse_player, (int)tt, message);
}