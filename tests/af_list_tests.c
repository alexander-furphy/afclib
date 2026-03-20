#include "af_list.h"
#include <assert.h>
#include <stdio.h>

void listTestLifetime(void) {
    List list = listCreate(sizeof(char));
    assert(!listIsInvalid(&list));

    listFree(&list);
    assert(listIsInvalid(&list));
}

void listTestModification(void) {
    List intList = listCreate(sizeof(int));

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
    assert(listContains(&intList, &toRemove));

    listFree(&intList);
}

void listTestIterator(void) {

}

void listTestCopy(void) {

}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    listTestLifetime();
    listTestModification();
    listTestIterator();
    listTestIterator();

    return 0;
}