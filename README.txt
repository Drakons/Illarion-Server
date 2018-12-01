
  Illarion

Objectives

   Illarion is the online multiplayer roleplaying game that is developed and
   maintained by the Illarion e.V. This repository contains the server 
   application.

Details

   The application in this repository is the Illarion Server. The official
   repository is https://github.com/Illarion-eV/Illarion-Server. The lead
   developer's repository is https://github.com/vilarion/Illarion-Server. 
   It works together with the Illarion Client found at
   https://github.com/Illarion-eV/Illarion-Java.

Requirements [currently used in the build]

   Recommended: Debian
   GCC 6.3.0
   Make 4.1
   Automake 1.15
   Boost 1.68.0
   PostgreSQL 11.1
   libpqxx 4.0.1
   Lua 5.2.4
   Luabind 0.9.1 from here: https://github.com/oberon00/luabind

Build

   Configure luabind to create a shared library and compile luabind [cmake, make, make install].
   Optionally place the files under /usr/lib and /usr/include instead of /usr/local/lib and /usr/local/include.

   Run inside the illarion source directory:
   ./bootstrap
   ./configure
   make

   if you want to use multiple boost versions in parallel you should use the
   flag --layout=versioned to b2 on installing this version to make sure we link against
   the correct version
   
Run (will be updated soon)

   Create database illarion, database user illarion and user illarion
   Populate the database (use a dump file)
   Add script and map files
   Run the server: sudo -u illarion src/illarion setup/illarion.conf
   ______________________________________________________________________

   Last modified: Dec 01, 2018
