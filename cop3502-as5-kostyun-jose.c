#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "leak_detector_c.h"

///Dr Gerber Functions
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    t--;

    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';  // ...clobber it by overwriting it with null, and...
        t--;        // decrement t *itself (i.e., change the pointed location)
    }
}

int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

///Gets the initial loop variable
int get_count(FILE *ifp){
    char buff[11];
    int count;

    get_next_nonblank_line(ifp, buff, 10);
    sscanf(buff, "%d", &count);

    return count;
}

///fills the dynamic array with integers from the input file
void fill_array(int *array, int count, FILE *ifp){
    char buff [11];
    for(int i = 0; i < count; ++i){
        get_next_nonblank_line(ifp, buff, 10);
        sscanf(buff, "%d", &array[i]);
    }
}

///Prints the array
void print_array(FILE *ofp, int *array, int count){
    for(int i = 0; i < count; ++i){
        if(i == count - 1)
            fprintf(ofp, "%d\n", array[i]);
        else
            fprintf(ofp, "%d ", array[i]);
    }
}

///Functions that swaps the values
void swap_positions(int *array, int child, int parent){
    int temp = array[parent];

    array[parent] = array[child];
    array[child] = temp;
}

///Siftsdown the integer to appropriate position
void siftdown(int *array, int last_index, int parent){
    int least = parent;
    int left_child = 2 * parent + 1;
    int right_child = 2 * parent + 2;

    if(left_child < last_index && array[least] < array[left_child])
        least = left_child;

    if(right_child < last_index && array[least] < array[right_child])
        least = right_child;

    if(least != parent){
        swap_positions(array, least, parent);
        siftdown(array, last_index, least);
    }
}

///heapifies the array
void heapify(FILE *ofp, int *array, int max){
    int last_parent = (max / 2) - 1;

    for(int i = last_parent; i >= 0; --i){
        siftdown(array, max, i);
        print_array(ofp, array, max);
    }
}

///removes the "root/first element" from the array
void remove_int(FILE *ofp, int *array, int max){
    while(max >= 1){
        swap_positions(array, max - 1, 0);
        fprintf(ofp, "%d\n", array[max- 1]);
        max--;
        int *temp = realloc(array, sizeof(int) * max);
        array = temp;

        int last_parent = (max / 2) - 1;
        for(int i = last_parent; i >= 0; --i)
            siftdown(array, max, i);

        print_array(ofp, array, max);
  }
}

int main(){
    atexit(report_mem_leak);
    FILE *ifp = fopen("cop3502-as5-input.txt", "r"), *ofp = fopen("cop3502-as5-output-kostyun-jose.txt", "w");

    int count = get_count(ifp);
    int *array = malloc(sizeof(int) * count);

    fill_array(array, count, ifp);
    print_array(ofp, array, count);
    heapify(ofp, array, count);
    print_array(ofp, array, count);
    remove_int(ofp, array, count);
    free(array);
    fclose(ofp);
    fclose(ifp);
    return 0;
}
