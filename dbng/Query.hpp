//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _QUERY_HPP_
#define _QUERY_HPP_

#include <string>

#include "dbng/Connection.hpp"
#include "dbng/Result.hpp"

namespace Database {
class Query;

typedef Query *PQuery;

class Query {
private:
    PConnection dbConnection;
    std::string *dbQuery;

public:
    ~Query(void);

    Result execute();

    Query(const Query &org);
    Query(PConnection connection, const std::string &query);
private:
    Query(void);
};
}

#endif // _CONNECTION_HPP_
