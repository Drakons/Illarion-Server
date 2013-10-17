/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Statistics.hpp"
#include "version.hpp"
#include "db/ConnectionManager.hpp"
#include "db/SelectQuery.hpp"
#include "db/InsertQuery.hpp"
#include "db/DeleteQuery.hpp"
#include "db/Query.hpp"
#include "World.hpp"
#include "Logger.hpp"
#include <chrono>
#include <algorithm>

namespace Statistic {

Statistics *Statistics::instance = nullptr;

Statistics::Statistics() : statistics(TYPE_COUNT), startTimes(TYPE_COUNT) {
    for (int i = 0; i < TYPE_COUNT; ++i) {
        for (int j = 0; j < 50; ++j) {
            statistics[i].emplace_back();
        }
    }

    versionId = getVersionId();
    lastSaveTime = getMillisecondsSinceEpoch();
    load();
}

Statistics &Statistics::getInstance() {
    if (!instance) {
        instance = new Statistics();
    }
    
    return *instance;
}

void Statistics::startTimer(Type type) {
    int intType = static_cast<int>(type);

    if (intType >= 0 && intType < TYPE_COUNT) {
        startTimes[intType] = getMillisecondsSinceEpoch();
    }
}

void Statistics::stopTimer(Type type) {
    int intType = static_cast<int>(type);

    if (intType >= 0 && intType < TYPE_COUNT) {
        long now = getMillisecondsSinceEpoch();
        int duration = static_cast<int>(now - startTimes[intType]);
        startTimes[intType] = now;
        auto playersOnline = World::get()->Players.size();

        while (statistics[intType].size() <= playersOnline) {
            statistics[intType].emplace_back();
        }
        
        auto &data = statistics[intType][playersOnline];
        ++data.samples;
        data.time += duration;
        data.timeSquared += duration*duration;
        data.timeMin = std::min(data.timeMin, duration);
        data.timeMax = std::max(data.timeMax, duration);

        if (now - lastSaveTime > SAVE_INTERVAL) {
            lastSaveTime = now;
            save();
        }
    }
}

int Statistics::getVersionId() {
    using namespace Database;
    auto connection = ConnectionManager::getInstance().getConnection();

    SelectQuery query(connection);
    query.addColumn("versions", "version_id");
    query.addEqualCondition<std::string>("versions", "version_name", SERVER_VERSION);
    query.addServerTable("versions");

    auto result = query.execute();

    if (!result.empty()) {
        const auto &row = result.front();
        return row["version_id"].as<int>();
    }

    InsertQuery insQuery(connection);
    const auto nameColumn = insQuery.addColumn("version_name");
    insQuery.addServerTable("versions");
    insQuery.addValue<std::string>(nameColumn, SERVER_VERSION);
    insQuery.execute();

    Query idQuery(connection, "SELECT lastval();");
    auto idResult = idQuery.execute();
    const auto &row = idResult.front();
    return row["lastval"].as<int>();
}

void Statistics::load() {
    using namespace Database;

    SelectQuery query;
    query.addColumn("statistics", "stat_type");
    query.addColumn("statistics", "stat_players");
    query.addColumn("statistics", "stat_samples");
    query.addColumn("statistics", "stat_time");
    query.addColumn("statistics", "stat_time_squared");
    query.addColumn("statistics", "stat_time_min");
    query.addColumn("statistics", "stat_time_max");
    query.addEqualCondition<int>("statistics", "stat_version", versionId);
    query.addServerTable("statistics");

    auto result = query.execute();

    for (const auto &row : result) {
        int type = row["stat_type"].as<int>();
        int players_online = row["stat_players"].as<int>();
        auto &stats = statistics[type][players_online];
        stats.samples = row["stat_samples"].as<long long>();
        stats.time = row["stat_time"].as<long long>();
        stats.timeSquared = row["stat_time_squared"].as<long long>();
        stats.timeMin = row["stat_time_min"].as<int>();
        stats.timeMax = row["stat_time_max"].as<int>();
    }
}

void Statistics::save() {
    using namespace Database;

    auto connection = ConnectionManager::getInstance().getConnection();

    try {
        DeleteQuery deleteQuery(connection);
        deleteQuery.addEqualCondition<int>("statistics", "stat_version", versionId);
        deleteQuery.addServerTable("statistics");
        deleteQuery.execute();

        InsertQuery insertQuery(connection);
        const auto versionColumn = insertQuery.addColumn("stat_version");
        const auto typeColumn = insertQuery.addColumn("stat_type");
        const auto playersColumn = insertQuery.addColumn("stat_players");
        const auto samplesColumn = insertQuery.addColumn("stat_samples");
        const auto timeColumn = insertQuery.addColumn("stat_time");
        const auto timeSquaredColumn = insertQuery.addColumn("stat_time_squared");
        const auto timeMinColumn = insertQuery.addColumn("stat_time_min");
        const auto timeMaxColumn = insertQuery.addColumn("stat_time_max");
        insertQuery.addServerTable("statistics");

        int currentType = 0;
        for (const auto &types : statistics) {
            int players = 0;

            for (const auto &data : types) {
                if (data.samples > 0) {
                    insertQuery.addValue<int>(versionColumn, versionId);
                    insertQuery.addValue<int>(typeColumn, currentType);
                    insertQuery.addValue<int>(playersColumn, players);
                    insertQuery.addValue<long>(samplesColumn, data.samples);
                    insertQuery.addValue<long>(timeColumn, data.time);
                    insertQuery.addValue<long>(timeSquaredColumn, data.timeSquared);
                    insertQuery.addValue<int>(timeMinColumn, data.timeMin);
                    insertQuery.addValue<int>(timeMaxColumn, data.timeMax);
                }

                ++players;
            }

            ++currentType;
        }

        insertQuery.execute();
    } catch (std::exception &e) {
        Logger::error(LogFacility::Other) << "saving statistics caught exception: " << e.what() << Log::end;
        connection->rollbackTransaction();
    }

}

long Statistics::getMillisecondsSinceEpoch() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

}
