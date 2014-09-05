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

#include "LuaLogoutScript.hpp"
#include "Character.hpp"
#include "character_ptr.hpp"

LuaLogoutScript::LuaLogoutScript(const std::string &filename)
    : LuaScript(filename) {
}

LuaLogoutScript::~LuaLogoutScript() {}

void LuaLogoutScript::onLogout(Character *cc) {
    character_ptr fuse_cc(cc);
    callEntrypoint("onLogout", fuse_cc);
}

