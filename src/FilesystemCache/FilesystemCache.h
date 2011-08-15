#ifndef Kompas_Plugins_FilesystemCache_h
#define Kompas_Plugins_FilesystemCache_h
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

#include "AbstractCache.h"

#include <unordered_map>

#include "Utility/Sha1.h"

namespace Kompas { namespace Plugins {

/**
@brief Filesystem based cache

<p>Basic one-handed-clock cache implementation. Keys are stored in hash table,
files are stored under their SHA-1 hashes to prevent unneeded storage of the
same file multiple times.</p>

@section FilesystemCacheAlgorithm Cache algorithm
<p>Cache algorithm is extension of <em>one-handed-clock</em> algorithm. All
entries are in hashtable and also in cyclic linked list with pointer to
<em>head</em>. Each entry has its key, under which it can be retrieved, usage
counter and SHA-1 hash for searching the file with actual data. There is also
second hashtable, which counts how many entries use particular SHA-1 hash.</p>
<p>When an item is requested from cache, the entry is searched via its key in
hashtable. If found, its usage counter is increased by one and data of file
with given SHA-1 are returned.
<p>When an item is about to be added into cache, first the space for it must
be reserved. If there isn't enough space, the <em>head</em> pointer goes
through the cyclic linked list, divides each entry's usage counter by 2 and if
the usage counter is equal to zero, removes the entry and decreases usage
counter of given SHA-1 hash. If the hash usage count is equal to zero, the file
is removed and its size in blocks is substracted from used block count. If there
is enough free blocks, the new item is added to cache, if not, the
<em>head</em> pointer continues with another element until there is enough free
space.</p></p>

@section FilesystemCacheStorageFormat Data storage format
<p>Data are stored as index file @c index.kps and files in filesystem
hierarchy. The files are written right when the item is added to the cache,
the index is written on cache finalization.</p>

@subsection FilesystemCacheIndexFile Index file
<p>Index file @c index.kps contains information about all entries stored in the
cache. All values are <strong>Little-Endian</strong>, except the SHA-1 hash, of
course.</p>
<p>File structure:</p>
<table>
<tr>
<th>Byte</th>
<th>Value (type)</th>
<th>Description</th>
</tr>
<tr>
<td>0 - 2</td>
<td><tt>0x43 0x43 0x48</tt></td>
<td>File signature (characters <tt>CCH</tt>)</td>
</tr>
<tr>
<td>3</td>
<td><tt>0x01</tt></td>
<td>Version number (currently <tt>1</tt>)</td>
</tr>
<tr>
<td>4 - 7</td>
<td>unsigned int</td>
<td>Block size</td>
</tr>
<tr>
<td>8 - 11</td>
<td>unsigned int</td>
<td>Maximal block count (cache size in blocks)</td>
</tr>
<tr>
<td>12 - 15</td>
<td>unsigned int</td>
<td>Count of all entries</td>
</tr>
<tr>
<td>16 - end</td>
<td>data</td>
<td>Entries (see below)</td>
</tr>
</table>
<p>Each entry is saved into index in this format:</p>
<table>
<tr>
<th>Byte</th>
<th>Value (type)</th>
<th>Description</th>
</tr>
<tr>
<td>0 - 19</td>
<td>data</td>
<td>SHA-1 hash</td>
</tr>
<tr>
<td>20 - 23</td>
<td>unsigned int</td>
<td>File size in bytes</td>
</tr>
<tr>
<td>24 - 27</td>
<td>unsigned int</td>
<td>Key size</td>
</tr>
<tr>
<td>28</td>
<td>unsigned char</td>
<td>Usage counter</td>
</tr>
<tr>
<td>29 - n</td>
<td>data</td>
<td>Key</td>
</tr>
</table>

@section FilesystemCacheFiles Data files
<p>Each data file is stored under name of its SHA-1 hash. The first two
characters of (string representation of) the hash are directory name, remaining
38 characters are filename. For example the file with hash
<tt>f2bb90146807e08fcc15dab4e439f67a0cd3f9e7</tt> will be stored as
<tt>f2/bb90146807e08fcc15dab4e439f67a0cd3f9e7</tt>.</p>
*/
class FilesystemCache: public Core::AbstractCache {
    public:
        /** @copydoc Core::AbstractCache::AbstractCache */
        inline FilesystemCache(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): AbstractCache(manager, plugin), _blockSize(4096), _maxBlockCount(25600), _usedBlockCount(0), _getCount(0), _hitCount(0), _position(0) {}

        inline virtual ~FilesystemCache() { finalizeCache(); }

        inline virtual int features() const { return BlockBased; }

        virtual bool initializeCache(const std::string& url);
        void finalizeCache();

        inline virtual size_t blockSize() const { return _blockSize; }
        virtual void setBlockSize(size_t size);

        inline virtual size_t cacheSize() const { return _maxBlockCount*_blockSize; }
        inline virtual void setCacheSize(size_t size) {
            _maxBlockCount = blockCount(size-_blockSize+1);
            if(_entries.size() != 0) reserveSpace(0);
        }

        inline virtual size_t usedSize() const { return _usedBlockCount*_blockSize; }

        /**
         * @copydoc Core::AbstractCache::purge()
         * Removes all entries from the cache and calls @ref optimize().
         */
        virtual void purge();

        /**
         * @copydoc Core::AbstractCache::optimize()
         * Deletes only files and directories which look like SHA-1 hashes,
         * leaves anything else alone.
         */
        virtual void optimize();

    protected:
        virtual std::string get(const std::string& key);
        virtual bool set(const std::string& key, const std::string& data);

    private:
        struct Entry {
            std::string key;
            Utility::Sha1::Digest sha1;
            unsigned int size;
            unsigned char usage;
            Entry *previous, *next;
        };

        class Sha1Hash {
            public:
                inline size_t operator()(const Utility::Sha1::Digest& sha1) const {
                    return *reinterpret_cast<const size_t*>(sha1.byteArray());
                }
        };

        void set(Entry* e);
        void remove(const std::unordered_map<std::string, Entry*>::iterator& eit);

        bool reserveSpace(int required);

        inline unsigned int blockCount(unsigned int size) const {
            return size == 0 ? 0 : (size-1)/_blockSize+1;
        }

        std::string filePath(const Utility::Sha1::Digest& sha1) const;
        std::string fileUrl(const Utility::Sha1::Digest& sha1) const;

        std::string _url;
        size_t _blockSize, _maxBlockCount, _usedBlockCount, _getCount, _hitCount;
        std::unordered_map<std::string, Entry*> _entries;
        std::unordered_map<Utility::Sha1::Digest, unsigned int, Sha1Hash> _files;
        Entry* _position;
};

}}

#endif
