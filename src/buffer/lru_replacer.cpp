
//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

void remove_lru_list_node(list_node_ptr& target_node,list_node_ptr& head,list_node_ptr& tail)
{
    assert(target_node&&head&&tail);
    if(head == tail){
        head = tail = nullptr;
        return;
    }
    list_node_ptr prev_node = target_node->prev;
    list_node_ptr next_node = target_node ->next;
    if(target_node == tail){
        tail = prev_node;
        prev_node->next = next_node;
    }else if(target_node == head){
        head = next_node;
        next_node->prev = prev_node;
    }else{
        next_node->prev = prev_node;
        prev_node->next = next_node;
    }
    

};

void add_lru_list_node(list_node_ptr& target_node,list_node_ptr& head,list_node_ptr& tail)
{
    assert(target_node);
    if(head == nullptr){
        head = target_node;
        tail = head;
        return ;
    }

    target_node ->next = head;
    head ->prev =  target_node;
    head = target_node;
    

}

LRUReplacer::LRUReplacer(size_t num_pages) {
    max_list_size = num_pages;
    
}

LRUReplacer::~LRUReplacer() = default;

auto LRUReplacer::Victim(frame_id_t *frame_id) -> bool { 
    if(!lru_list_tail){
        return false;
    }
    *frame_id = lru_list_tail -> val;
    Pin(*frame_id);
    return true; 
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    if(lru_map.count(frame_id) == 0 ){
        return;
    }
    list_node_ptr target_node = lru_map.at(frame_id);
    assert(target_node);
    remove_lru_list_node(target_node,lru_list_head,lru_list_tail);
    lru_map.erase(frame_id);
    --lru_list_size;
    assert(lru_list_size>=0);

}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    if(lru_map.count(frame_id) != 0 ){
        return ;
    }
    list_node_ptr target_node(new lru_list_node(frame_id));
    lru_map.insert(std::make_pair(frame_id,target_node));
    add_lru_list_node(target_node,lru_list_head,lru_list_tail);
    ++lru_list_size;
    while(lru_list_size > max_list_size){
        remove_lru_list_node(lru_list_tail,lru_list_head,lru_list_tail);
        --lru_list_size;
    }

}

auto LRUReplacer::Size() -> size_t { 
    return lru_list_size; 
}

}  // namespace bustub
