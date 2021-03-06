/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 *  for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *            az@pocketpcrussia.com 
 *  for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _omweather_database_hpp
#define _omweather_database_hpp 1
/*******************************************************************************/
#include <sqlite3.h>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "omweather-stations.hpp"
/*******************************************************************************/
typedef std::map<std::string,std::string> Item;
typedef std::vector<Item> Result;
int callback(void*, int, char**, char**);
/*******************************************************************************/
class Database{
    sqlite3 *db;
    Result *result;
    public:
        Database();
        ~Database();
        bool open(const std::string);
        void close();
        bool countries(CountriesList&);
        bool regions(int, RegionsList&);
};
/*******************************************************************************/
#endif
