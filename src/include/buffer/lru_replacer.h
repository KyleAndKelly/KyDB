//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.h
//
// Identification: src/include/buffer/lru_replacer.h
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex>  // NOLINT
#include <vector>
#include <memory>
#include <unordered_map>
#include <cassert>

#include "buffer/replacer.h"
#include "common/config.h"
#include "buffer/lru_linked_list.h"

namespace bustub {

struct lru_list_node {
  int val;
  std::shared_ptr<lru_list_node> prev;
  std::shared_ptr<lru_list_node>  next;
  lru_list_node(int x) : val(x), prev(nullptr), next(nullptr) {}
};

typedef std::shared_ptr<lru_list_node> list_node_ptr;


/**
 * LRUReplacer implements the Least Recently Used replacement policy.
 */
class LRUReplacer : public Replacer {
 public:
  /**
   * Create a new LRUReplacer.
   * @param num_pages the maximum number of pages the LRUReplacer will be required to store
   */
  explicit LRUReplacer(size_t num_pages);

  /**
   * Destroys the LRUReplacer.
   */
  ~LRUReplacer() override;

  auto Victim(frame_id_t *frame_id) -> bool override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  auto Size() -> size_t override;

 private:
  std::unordered_map<frame_id_t,list_node_ptr> lru_map;
  list_node_ptr lru_list_head;
  list_node_ptr lru_list_tail;
  int lru_list_size = 0;
  int max_list_size = 0;
  
};

}  // namespace bustub
