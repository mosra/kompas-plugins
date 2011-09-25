/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "FilesystemCache.h"

#include "Utility/Directory.h"
#include "Utility/Endianness.h"

using namespace std;
using namespace Kompas::Utility;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

PLUGIN_REGISTER(Kompas::Plugins::FilesystemCache,
                "cz.mosra.Kompas.Core.AbstractCache/0.2")

bool FilesystemCache::initializeCache(const std::string& url) {
    if(!_url.empty()) finalizeCache();

    _url = url;
    string _file = Directory::join(_url, "index.kps");
    if(Directory::fileExists(_file)) {
        ifstream file(_file, ios::binary);
        if(!file.good()) {
            Error() << "Cannot open cache index" << _file;
            return false;
        }

        char* buffer = new char[4];

        /* Check file signature */
        file.get(buffer, 4);
        if(string(buffer) != "CCH") {
            Error() << "Unknown Kompas cache signature" << buffer << "in" << _file;
            return false;
        }

        /* Check file version */
        file.read(buffer, 1);
        if(buffer[0] != 1) {
            Error() << "Unsupported Kompas cache version" << buffer[0] << "in" << _file;
            return false;
        }

        /* Block size */
        file.read(buffer, 4);
        _blockSize = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));

        /* Block count */
        file.read(buffer, 4);
        _maxBlockCount = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));

        /* Count of all entries */
        file.read(buffer, 4);
        unsigned int count = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));
        _entries.reserve(count);

        /* Populate the hash table with entries */
        for(unsigned int i = 0; i != count; ++i) {
            if(!file.good()) {
                Error() << "Incomplete cache index" << _file;
                return false;
            }

            Entry* entry = new Entry();

            /* SHA-1, file size, key size and usage */
            file.read(reinterpret_cast<char*>(&entry->sha1), 20);
            file.read(buffer, 4);
            entry->size = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));
            file.read(buffer, 4);
            file.read(reinterpret_cast<char*>(&entry->usage), 1);

            /* Key */
            unsigned int keySize = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));
            char* key = new char[keySize];
            file.read(key, keySize);
            entry->key = string(key, keySize);
            delete[] key;

            /* Find hash of the file, if it already exists, increase usage count */
            unordered_map<Sha1::Digest, unsigned int>::iterator it = _files.find(entry->sha1);
            if(it != _files.end()) ++it->second;

            /* If it doesn't exist, add the hash and increase used size */
            else {
                _files.insert(pair<Sha1::Digest, unsigned int>(entry->sha1, 1u));
                _usedBlockCount += blockCount(entry->size);
            }

            /* Add entry to entries table */
            set(entry);
        }

        file.close();
    }

    Debug() << "Initialized cache with block size" << _blockSize << "B," << _maxBlockCount << "blocks, containing" << _entries.size() << "entries of size" << _usedBlockCount << "blocks.";

    return true;
}

void FilesystemCache::finalizeCache() {
    if(_url.empty()) return;

    string _file = Directory::join(_url, "index.kps");
    Directory::mkpath(Directory::path(_file));
    ofstream file(_file.c_str(), ios::binary);
    if(!file.good()) {
        Error() << "Cannot write cache index" << _file;

        /* Avoid memory leak */
        for(unordered_map<string, Entry*>::const_iterator it = _entries.begin(); it != _entries.end(); ++it)
            delete it->second;
        return;
    }

    unsigned int buffer;

    /* Write file signature, version, block size, block count and entry count */
    file.write("CCH", 3);
    file.write("\1", 1);
    buffer = Endianness::littleEndian(static_cast<unsigned int>(_blockSize));
    file.write(reinterpret_cast<const char*>(&buffer), 4);
    buffer = Endianness::littleEndian(static_cast<unsigned int>(_maxBlockCount));
    file.write(reinterpret_cast<const char*>(&buffer), 4);
    buffer = Endianness::littleEndian(static_cast<unsigned int>(_entries.size()));
    file.write(reinterpret_cast<const char*>(&buffer), 4);

    /* Foreach all entries and write them to index */
    if(_position != 0) {
        Entry* entry = _position;
        do {
            file.write(reinterpret_cast<const char*>(&entry->sha1), Sha1::DigestSize);
            buffer = Endianness::littleEndian<unsigned int>(entry->size);
            file.write(reinterpret_cast<const char*>(&buffer), 4);
            buffer = Endianness::littleEndian<unsigned int>(entry->key.size());
            file.write(reinterpret_cast<const char*>(&buffer), 4);
            file.write(reinterpret_cast<const char*>(&entry->usage), 1);
            file.write(entry->key.c_str(), entry->key.size());

            entry = entry->next;
            delete entry->previous;
        } while(entry != _position);
    }

    file.close();

    _position = 0;
    _usedBlockCount = 0;
    _entries.clear();
    _files.clear();
    _url.clear();
}

void FilesystemCache::setBlockSize(size_t size) {
    _blockSize = size;
    if(_entries.size() == 0) return;

    /* Rebuild files map */
    _files.clear();
    _usedBlockCount = 0;
    Entry* entry = _position;
    do {
        /* Find hash of the file, if it already exists, increase usage count */
        unordered_map<Sha1::Digest, unsigned int>::iterator it = _files.find(entry->sha1);
        if(it != _files.end()) ++it->second;

        /* If it doesn't exist, add the hash and increase used size */
        else {
            _files.insert(pair<Sha1::Digest, unsigned int>(entry->sha1, 1u));
            _usedBlockCount += blockCount(entry->size);
        }

        entry = entry->next;
    } while(entry != _position);
}

void FilesystemCache::purge() {
    Debug() << "Cleaning cache.";

    for(unordered_map<Sha1::Digest, unsigned int>::iterator it = _files.begin(); it != _files.end(); ++it)
        Directory::rm(fileUrl(it->first));

    _entries.clear();
    _files.clear();
    _position = 0;
    _usedBlockCount = 0;

    optimize();
}

void FilesystemCache::optimize() {
    size_t orphanEntryCount = 0;
    size_t orphanFileCount = 0;
    size_t orphanDirectoryCount = 0;

    /* Remove entries without files */
    if(_position != 0) {
        Entry* entry = _position;
        do {
            Entry* next = entry->next;

            /* If the file doesn't exist, remove entry */
            if(!Directory::fileExists(fileUrl(entry->sha1))) {
                ++orphanEntryCount;
                remove(_entries.find(entry->key));
            }

            entry = next;
        } while(_position == 0 || entry != _position);
    }

    /* Delete files which are not in the file table */
    Directory d(_url, Directory::SkipDotAndDotDot);
    for(Directory::const_iterator it = d.begin(); it != d.end(); ++it) {
        if(*it == "index.kps") continue;

        /* Subdirectory, open it and look */
        if(it->size() == 2) {
            string subdir = Directory::join(_url, *it);
            bool used = false;
            Directory sd(subdir, Directory::SkipDotAndDotDot);

            /* Remove unused files */
            for(Directory::const_iterator sit = sd.begin(); sit != sd.end(); ++sit) {
                if(sit->size() == 38) {
                    Sha1::Digest sha1 = Sha1::Digest::fromHexString(*it + *sit);
                    if(sha1 != Sha1::Digest() && _files.find(sha1) == _files.end()) {
                        if(Directory::rm(Directory::join(subdir, *sit)))
                            ++orphanFileCount;
                        continue;
                    }
                }

                used = true;
            }

            if(!used) {
                if(Directory::rm(subdir))
                    ++orphanDirectoryCount;
            }
        }
    }

    Debug() << "Optimization removed" << orphanEntryCount << "orphan entries," << orphanFileCount << "orphan files and" << orphanDirectoryCount << "orphan directories.";
}

string FilesystemCache::get(const std::string& key) {
    ++_getCount;

    /* Find the key in entry table */
    unordered_map<string, Entry*>::iterator eit = _entries.find(key);
    if(eit == _entries.end()) {
        Debug() << "Cache miss.";
        return string();
    }

    Entry* entry = eit->second;

    /* Hash found, open the file */
    string _file = fileUrl(entry->sha1);
    ifstream file(_file, ios::binary);
    if(!file.is_open()) {
        Error() << "Cannot open cached file" << _file;
        return string();
    }

    /* Increase usage count and return file contents */
    ++entry->usage;
    char* buffer = new char[entry->size];
    file.read(buffer, entry->size);
    string s(buffer, entry->size);
    delete[] buffer;

    ++_hitCount;
    Debug() << "Retrieved entry" << entry->sha1.hexString() << "from cache. Hit rate:" << ((double) _hitCount)/_getCount;

    return s;
}

bool FilesystemCache::set(const std::string& key, const std::string& data) {
    if(_url.empty()) return false;

    /* Hash of the data */
    Sha1::Digest sha1 = Sha1::digest(data);

    /* Add the entry */
    Entry* entry = new Entry();
    entry->key = key;
    entry->sha1 = sha1;
    entry->size = data.size();
    entry->usage = 0;

    /* Find the hash in file table, if it exists, increase usage count */
    unordered_map<Sha1::Digest, unsigned int>::iterator it = _files.find(sha1);
    if(it != _files.end()) {
        ++it->second;

        Debug() << "Entry" << sha1.hexString() << "already exists in cache.";

    /* If it doesn't exists, prepare free space and insert it */
    } else {
        if(!reserveSpace(data.size())) return false;

        _usedBlockCount += blockCount(data.size());
        _files.insert(pair<Sha1::Digest, unsigned int>(sha1, 1u));

        if(!Directory::mkpath(filePath(sha1))) return false;
        string _file = fileUrl(sha1);
        ofstream file(_file, ios::binary);
        if(!file.good()) {
            Error() << "Cannot write cache file" << _file;
            return false;
        }
        file.write(data.c_str(), data.size());
        file.close();

        Debug() << "Added entry" << sha1.hexString() << "to cache.";
    }

    set(entry);

    return true;
}

void FilesystemCache::set(Entry* entry) {
    /* Find the key in entry table and remove it if it already exists */
    unordered_map<string, Entry*>::iterator eit = _entries.find(entry->key);
    if(eit != _entries.end()) remove(eit);

    /* First item in the cache, initialize circular linked list */
    if(_position == 0) {
        entry->next = entry;
        entry->previous = entry;
        _position = entry;
    } else {
        entry->next = _position;
        entry->previous = _position->previous;
        _position->previous->next = entry;
        _position->previous = entry;
    }

    _entries.insert(pair<string, Entry*>(entry->key, entry));
}

void FilesystemCache::remove(const unordered_map<string, Entry*>::iterator& eit) {
    Entry* entry = eit->second;

    /* Disconnect the entry from circular linked list and remove it from the table */
    if(entry->next == entry) _position = 0;
    else {
        entry->next->previous = entry->previous;
        entry->previous->next = entry->next;
        if(_position == entry) _position = _position->next;
    }
    _entries.erase(eit);

    /* Find the hash in file table, decrease usage count */
    unordered_map<Sha1::Digest, unsigned int>::iterator it = _files.find(entry->sha1);

    /* If the file is not used anymore, remove it and decrease used size */
    if(it != _files.end() && --it->second == 0) {
        Directory::rm(fileUrl(entry->sha1));
        Directory::rm(filePath(entry->sha1));
        _files.erase(it);
        _usedBlockCount -= blockCount(entry->size);
    }

    Debug() << "Removed entry" << entry->sha1.hexString() << "from cache, freed" << blockCount(entry->size) << "blocks.";

    delete entry;
}

bool FilesystemCache::reserveSpace(int required) {
    /* If we need more space than is available, don't do anything */
    if(blockCount(required) > _maxBlockCount) {
        Error() << "Cannot reserve" << blockCount(required) << "blocks in cache of" << _maxBlockCount << "blocks.";
        return false;
    }

    /* Go through the cycle and exponentially decrease usage count */
    while(_usedBlockCount+blockCount(required) > _maxBlockCount) {
        _position->usage >>= 1;

        /* If the usage decreased to zero, remove the entry */
        if(_position->usage == 0) remove(_entries.find(_position->key));

        /* Otherwise advance to next entry */
        else _position = _position->next;
    }

    Debug() << "Reserved" << blockCount(required) << "blocks in cache of" << _maxBlockCount << "blocks.";

    return true;
}

string FilesystemCache::filePath(const Sha1::Digest& sha1) const {
    return Directory::join(_url, sha1.hexString().substr(0, 2));
}

string FilesystemCache::fileUrl(const Sha1::Digest& sha1) const {
    return Directory::join(filePath(sha1), sha1.hexString().substr(2));
}

}}
