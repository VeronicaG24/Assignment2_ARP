#include "./../include/processB_utilities.h"
#include <bmpfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct{
    int x, y;
}center;

int main(int argc, char const *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;
    center c;
    // Initialize UI
    init_console_ui();
    //creare bitmap locale

    //old center

    //shared memory
    const char* shm_name = "\bitmap";
    //1600x600x3 size della memoria condivisa matrice in cui copiare bitmap
    const int size=sizeof(int);
    int shm_fd;
    void *ptr;
    //open the shared memery
    shm_fd=shm_open(shm_name, O_RDONLY, 0666);
    if(shm_fd==-1){
        perror("B-error in open the shared memory:");
    }
    
    
    //pointer to reference the shared memory
    ptr= mmap(0, size,PROT_READ, MAP_SHARED,shm_fd,0);
    if(ptr<0){
        perror("B-error in mapping the shared memory:");
    }
    /*
    //to unmap the pointer
    if(mummap(ptr,size)==-1){
        perror("B-Can't unmap shared memory");
    };

    //to close the shared memory 
    if(shm_unlink(shm_fd)==-1){
        perror("B-Can't unlink shared memory");
    }; 
    */

    //semafori
    
    // Infinite loop
    while (TRUE) {

        // Get input in non-blocking mode
        int cmd = getch();
        
        //copia da shared memory a posizione locale

        // If user resizes screen, re-draw UI...
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }

        else {
            sleep(2);
            //controllare centro nuova bitmap
            int x=atoi(ptr);
            ptr += sizeof(int);
            int y=atoi(ptr);
            //printf("%d %d", c.x, c.y);
            ptr= mmap(0, size,PROT_READ, MAP_SHARED,shm_fd,0);
            if(ptr<0){
                perror("B-error in mapping the shared memory:");
            }
            //se diverso
            if(x!=c.x || y != c.y){
                //plotta nuovo punto e distanza dal vecchio
                //dist_x=x-c_old.x;
                //dist_y=y-c_old.y;
                //plot dist
                c.x=x;
                c.y=y;
            }
            
                
            mvaddch(y, x, '-');
            refresh();
        }
    }

    endwin();
    return 0;
}
