#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct pair {
    uint32_t key;
    const char* value;
};

uint32_t binary_search(const struct pair arr[], int size, const char* targetValue); 
uint32_t binary_search(const struct pair arr[], int size, const char* targetValue) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        int cmp = strcmp(arr[mid].value, targetValue);
        if (cmp == 0) {
            return arr[mid].key;
        }

        if (cmp < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return UINT32_MAX; // Assuming UINT32_MAX is used as an indicator of not found
}

int main(void) {
    struct pair arr[] = {
        { 10, "Apple" },
        { 20, "Banana" },
        { 30, "Cherry" },
        { 40, "Date" },
        { 50, "Fig" }
    };
    int size = sizeof(arr) / sizeof(arr[0]);
    const char* targetValue = "Date";

    uint32_t resultKey = binary_search(arr, size, targetValue);

    if (resultKey == UINT32_MAX) {
        printf("Value \"%s\" not found in array\n", targetValue);
    } else {
        printf("Key for value \"%s\" is: %u\n", targetValue, resultKey);
    }

    return 0;
}

