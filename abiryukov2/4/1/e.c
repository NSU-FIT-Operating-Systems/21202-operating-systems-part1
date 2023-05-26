#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void buffer_operations() {
    char *buffer = malloc(100);                             // i. выделяет на куче буфер (например, размером 100 байт);
    strcpy(buffer, "hello world");                          // ii. записывает в него какую-либо фразу (например, hello world);  
    printf("Buffer content: %s\n", buffer);                 // iii. выводит содержимое буфера;  
    free(buffer);                                           // iv. освобождает выделенную память;  

    printf("Buffer content after free: %s\n", buffer);      // v. снова выводит содержимое буфера;  

    char *buffer2 = malloc(100);                            // vi. выделяет еще один буфер;  
    strcpy(buffer2, "hello world");                         // vii. записывает в них какую-либо фразу (например, hello world);  
    printf("Buffer2 content: %s\n", buffer2);               // viii. выводит содержимое буфера;  

    char *mid_buffer = buffer2 + 50;                        // ix. перемещает указатель на середину буфера;  
    free(mid_buffer);                                       // x. освобождает память по этому указателю.  

    printf("Buffer2 content after free: %s\n", buffer2);    // xi. выводит содержимое буфера;  
}

int main() {
    buffer_operations();
    return 0;
}
