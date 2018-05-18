//
// Created by jeff2310 on 5/11/18.
//

#ifndef TILEGENERATOR_PRIORITYQUEUE_H
#define TILEGENERATOR_PRIORITYQUEUE_H

#include <vector>
#include <iterator>
#include <iostream>

using std::vector;
using std::less;

template<typename T, typename Sequence = vector<T>,
        typename Compare = less<T> >
class PriorityQueue {
    typedef unsigned int Size;
private:
    inline void adjust_heap(Size rootIndex) {
        iterator first = data.begin();
        Size holeIndex = rootIndex;
        Size smallerChild = 2 * holeIndex + 2;
        value_type value = *(first + rootIndex);
        // 左右子树中选择最小的进行上调
        while (smallerChild < size) {
            if (*(first + smallerChild) > *(first + (smallerChild - 1)))
                smallerChild--;
            // 整颗树无需调整，需要确保左右子树已经是一个堆
            if (value < *(first + smallerChild)) break;
            *(first + holeIndex) = *(first + smallerChild);
            holeIndex = smallerChild;
            smallerChild = 2 * holeIndex + 2;
        }
        // 没有右子树，那就找左子树
        if (smallerChild == size) {
            smallerChild--;
            if (value > *(first + smallerChild)) {
                *(first + holeIndex) = *(first + smallerChild);
                holeIndex = smallerChild;
            }
        }
        *(first + holeIndex) = value;
    }

    inline void make_heap() {
        iterator first = data.begin();
        if (size < 2) return;
        // last 节点的父节点
        Size rootIndex = (size - 2) / 2;
        //从叶子节点的父节点开始一层层建立子树（恰好与递归的回溯顺序相同）
        while(true) {
            adjust_heap(rootIndex);
            if(rootIndex==0) break;
            rootIndex--;
        }
    };

    inline void push_heap(Size holeIndex){
        Size parentIndex = (holeIndex-1) / 2;
        iterator first = data.begin();
        value_type value = *(first + holeIndex);
        while(holeIndex > 0){
            if(*(first+parentIndex) < value) break;
            *(first+holeIndex) = *(first+parentIndex);
            holeIndex = parentIndex;
            parentIndex = (holeIndex-1) / 2;
        }
        *(first + holeIndex) = value;
    }

    inline int pop_heap(Size rootIndex = 0){
        iterator first = data.begin(), last = first + size-1;
        // 交换第一个和最后一个，然后把最后一个下放。
        value_type temp = *last; *first = *last; *last = *first;
        size--;
        adjust_heap(rootIndex);
    }


protected:
    Sequence data;
    Compare compare;
    Size size;

public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::iterator iterator;
    typedef typename Sequence::const_reference const_reference;

    PriorityQueue() : data(), compare(), size(0){}

    explicit PriorityQueue(const Sequence &sequence)
            : data(sequence), compare(), size(sequence.size()) { make_heap(); }

    explicit PriorityQueue(Sequence &&sequence)
            : data(std::move(sequence)), compare(), size(sequence.size()) { make_heap(); }

    template<typename Allocator>
    explicit PriorityQueue(const Allocator& x): data(x), compare(), size(x.size()) { make_heap(); }

    iterator begin(){
        return data.begin();
    }
    iterator end(){
        return data.begin()+size;
    }

    inline bool empty(){
        return size == 0;
    }

    inline void push(const value_type& value){
        if(data.size() == size) {
            data.push_back(value);
        } else{
            iterator first = data.begin();
            *(first+size) = value;
        }

        push_heap(size);
        size++;
    }

    inline void pop(){
        // 先将顶部的元素放入最后一个叶子节点，然后size--以达到无视它的目的...
        pop_heap();
    }

    const_reference top(){
        return *(data.begin());
    }

    void find_and_remove(const value_type &value){
        Size index;
        iterator first = data.begin();
        for(index=0; index<size; index++){
            if(*(first+index) == value){
                break;
            }
        }
        if(index==size) return;
        pop_heap(index);
    }
};

#endif //TILEGENERATOR_PRIORITYQUEUE_H
