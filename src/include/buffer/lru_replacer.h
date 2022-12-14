/**
 * lru_replacer.h
 *
 * Functionality: The buffer pool manager must maintain a LRU list to collect
 * all the pages that are unpinned and ready to be swapped. The simplest way to
 * implement LRU is a FIFO queue, but remember to dequeue or enqueue pages when
 * a page changes from unpinned to pinned, or vice-versa.
 */

#pragma once

#include "buffer/replacer.h"
#include "hash/extendible_hash.h"
#include <mutex>
#include <map>

namespace scudb {

template <typename T> class LRUReplacer : public Replacer<T> {
public:
  // do not change public interface
  LRUReplacer();

  ~LRUReplacer();

  void Insert(const T &value);

  bool Victim(T &value);

  bool Erase(const T &value);

  size_t Size();

private:
    struct DLinkNode{
        DLinkNode() = default;
        explicit DLinkNode(T data, DLinkNode *p = nullptr) : data(data), pre(p) {}
        T data;
        std::shared_ptr<DLinkNode> pre;
        std::shared_ptr<DLinkNode> next;
    };
    std::shared_ptr<DLinkNode> head;
    std::shared_ptr<DLinkNode> tail;
    size_t size;
    // ExtendibleHash<T, std::shared_ptr<DLinkNode>> table;
    std::mutex mutex;
    std::map<T, std::shared_ptr<DLinkNode>> table;
};

} // namespace scudb
