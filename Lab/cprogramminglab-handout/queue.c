/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"
#include "harness.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));

    /* What if malloc returned NULL? */
    if(q == NULL) return NULL; //如果内存分配失败，返回NULL

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */

    if (q == NULL) {
        return;
    }

    // 遍历并释放q链表的所有元素
    list_ele_t *current = q->head;
    while(current != NULL){
        list_ele_t *next = current->next;
        free(current->value);  // 首先释放字符串内存
        free(current);         // 然后释放链表节点
        current = next;
    }

    /* Free queue structure */
    free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    //如果q是NULL的错误处理
    if (q == NULL) {
        return false;  
    }

    // 创建新节点
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));

    // 内存分配失败的错误处理
    if (newh == NULL) {
        return false;
    }

    //开始手动计算字符串 s 的长度，并给 newh->value 分配对应的内存空间，然后手动复制
    // （1）手动计算字符串长度
    size_t length = 0; //因为后面的malloc()需要 size_t 类型的参数
    while (s[length] != '\0') {       
        length++;
    }

    // （2）为 newh->value 分配内存
    newh->value = malloc(length + 1); // +1 是为了 '\0'
    if (newh->value == NULL) {// 内存分配失败的错误处理
        free(newh); //退出函数之前，记得要释放本函数前面已创建的 newh 对象。
        return false;
    }

    // （3）手动复制字符串 s 到 newh->value
    for (size_t i = 0; i <= length; i++) {
        newh->value[i] = s[i];
    }


    newh->next = q->head;
    q->head = newh;//将q链表的头结点置为这个新节点

    //如果q链表的尾部节点之前为空（即本次插入的节点是链表的唯一节点）
    if (q->tail == NULL) {
        q->tail = newh;
    }

    q->size++;

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    
    //如果q是NULL的错误处理
    if (q == NULL) return false;  
    

    // 创建新节点
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));

    // 内存分配失败的错误处理
    if (newh == NULL) return false;


    //开始手动计算字符串 s 的长度，并给 newh->value 分配对应的内存空间，然后手动复制
    // （1）手动计算字符串长度
    size_t length = 0;
    while (s[length] != '\0') {       
        length++;
    }

    // （2）为 newh->value 分配内存
    newh->value = malloc(length + 1); // +1 是为了 '\0'
    if (newh->value == NULL) {// 内存分配失败的错误处理
        free(newh); //退出函数之前，记得要释放本函数前面已创建的 newh 对象。
        return false;
    }

    // （3）手动复制字符串 s 到 newh->value
    for (size_t i = 0; i <= length; i++) {
        newh->value[i] = s[i];
    }


    if(q->head == NULL){//表示q链表是空的
        q->head = newh;
        q->tail = newh;
    }else{
        q->tail->next = newh;
        q->tail = newh;
    }

    //注意：这个非常重要，很容易遗漏（不然无法通过qtest）
    q->tail->next = NULL;// 确保新的尾部元素的next指针为NULL

    q->size++;

    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */

    if(q == NULL || q->head == NULL){
        return false;
    }

    //保存链表头元素
    list_ele_t *temp = q->head;

    //更新链表头元素
    q->head = q->head->next;

    // 如果链表变成空了，更新尾部指针为空
    if (q->head == NULL) {
        q->tail = NULL;
    }

    // 如果提供了 buf 和有效的 bufsize，就要复制被删除的元素到 buf
    if (buf != NULL && bufsize > 0) {
        size_t i;
        for (i = 0; i < bufsize - 1 && temp->value[i] != '\0'; i++) {
            buf[i] = temp->value[i];
        }
        buf[i] = '\0'; // 确保 null 结尾
    }

    //注意：在 C 语言中，free(temp) 只会释放 temp 指向的内存块，并不会自动释放 temp 内部指向的任何其他动态分配的内存。
    free(temp->value); // 先释放指向的字符串内存
    free(temp);        // 然后释放结构体本身的内存

    q->size--;

    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */

    if (q == NULL) {
        return 0;  
    }
    
    return q->size; // 注意，这个函数要返回的是 size_t 类型
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */

    // 对于空队列或只有一个元素的队列无需反转
    if (q == NULL || q->head == NULL || q->head == q->tail) {
        return; 
    }

    //原地反转单链表：O(n)时间复杂度，O(1)空间

    //先把尾结点标记为头结点
    q->tail = q->head;

    list_ele_t *prev = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *next;
    // 开始遍历q链表，并反转每个元素的 next 指针
    while(cur != NULL){
        next = cur->next; // 保存下一个节点
        cur->next = prev;  // 反转当前节点的指针
        prev = cur;        // 更新pre为当前节点
        cur = next;       // 移动到下一个节点
    }

    // 更新队列头部指针
    q->head = prev;
}
