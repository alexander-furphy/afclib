#include "af_list.h"
#include <assert.h>
#include <stdio.h>

void listTestLifetime(void) {
    List list = listCreate(sizeof(char), NULL);
    assert(!listIsInvalid(&list));

    listFree(&list);
    assert(listIsInvalid(&list));
}

bool intComparator(void* a, void* b) {
    return (*(int*)a) == (*(int*)b);
}

void listTestModification(void) {
    List intList = listCreate(sizeof(int), intComparator);

    int item = 5;
    listAdd(&intList, &item);
    item = 30;
    listAdd(&intList, &item);
    listAdd(&intList, &item);
    item = 3;
    listAdd(&intList, &item);
    assert(intList.count == 4);

    int check = 3;
    assert(listContains(&intList, &check));
    check = 5;
    assert(listContains(&intList, &check));

    int toRemove = 30;
    assert(listRemove(&intList, &toRemove));
    assert(intList.count == 3);
    assert(listContains(&intList, &toRemove));

    listFree(&intList);
}

void listTestIterator(void) {
    List intList = listCreate(sizeof(int), intComparator);
    for(int i = 0; i < 32; i++) {
        listAdd(&intList, &i);
    }
    assert(intList.count == 32);

    for(size_t i = 0; i < intList.count; i++) {
        int value;
        listGet(&intList, i, &value);
        assert(value == i);
    }

    listFree(&intList);
}

void listTestCopy(void) {
    List intList = listCreate(sizeof(int), intComparator);
    for(int i = 0; i < 32; i++) {
        listAdd(&intList, &i);
    }

    List intListCopy = listCopy(&intList);
    for(size_t i = 0; i < intListCopy.count; i++) {
        int value;
        listGet(&intListCopy, i, &value);
        assert(value == i);
    }
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    listTestLifetime();
    listTestModification();
    listTestIterator();
    listTestCopy();

    return 0;
}