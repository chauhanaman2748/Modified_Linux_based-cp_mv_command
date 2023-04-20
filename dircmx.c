#define _XOPEN_SOURCE 500
#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <ftw.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_EXT 6

char *src_path, *dst_path, *random_dir, *random_dir2, *extensions[MAX_EXT];
int copy = 0, move = 0, ext_count = 0;

void copy_folder(const char *src_folder, const char *dst_folder) {
  DIR *src_dir = opendir(src_folder);
  if (src_dir == NULL) {
    perror("Error opening source folder");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(src_dir)) != NULL) {
    if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      char src_path[1024];
      char dst_path[1024];
      snprintf(src_path, sizeof(src_path), "%s/%s", src_folder, entry->d_name);
      snprintf(dst_path, sizeof(dst_path), "%s/%s", dst_folder, entry->d_name);
      mkdir(dst_path, 0700);
      copy_folder(src_path, dst_path);
    } 
  }
  closedir(src_dir);
}

int mk(const char *dest_path){
    int j, len = strlen(dest_path);
    char subpath[100];
    for (j = 1; j < len; j++) {
        if (dest_path[j] == '/') {
            strncpy(subpath, dest_path, j);
            subpath[j] = '\0';

            int status = mkdir(subpath, 0700);
            if (status == -1 && errno != EEXIST) {
                printf("Error creating directory %s\n", subpath);
                return 1;
            }
        }
    }

    int status = mkdir(dest_path, 0700);
    if (status == -1 && errno != EEXIST) {
        printf("Error creating directory %s\n", dest_path);
        return 1;
    }
}



int copy_file(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    //printf("Inside nftw\n");
    char dst_file_path[1024];
    int i;

    if (tflag == FTW_F) {
        if (ext_count > 0) {
            int found = 0;
            for (i = 0; i < ext_count; i++) {
                char *ext = extensions[i];
                int ext_len = strlen(ext);
                int path_len = strlen(fpath);

                if (path_len > ext_len) {
                    if (strcmp(fpath + path_len - ext_len, ext) == 0) {
                        found = 1;
                        break;
                    }
                }
            }

            if (!found)
                return 0;
        }

        sprintf(dst_file_path, "%s/%s", dst_path, fpath + strlen(src_path));
        if (copy)
            link(fpath, dst_file_path);
        else if (move)
            rename(fpath, dst_file_path);
    }
    return 0;
}

int remove_directory(const char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;

        r = 0;
        while (!r && (p=readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2; 
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_directory(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }

    if (!r) {
        r = rmdir(path);
    }

    return r;
}

int main(int argc, char* argv[]) {
    char *op, *e1,*e2,*e3,*e4,*e5,*e6;

    if (argc < 4) {
        printf("Usage: dircmx [source_dir] [destination_dir] [options] <extension list>\n");
        return 1;
    }

    //printf("%d\n",argc);
    ext_count= argc-4;
    switch(ext_count){
        case 1: {
             e1 = (char *) malloc(strlen(argv[4]) + 1);
            if (e1 != NULL) {
                strcpy(e1, argv[4]);
                //printf("d_dir: %s\n", d_dir);
            } else {
                printf("Memory allocation failed.\n");
            }   
            break;
        }
        case 2: {
             e2 = (char *) malloc(strlen(argv[5]) + 1);
            if (e2 != NULL) {
                strcpy(e2, argv[5]);
                //printf("d_dir: %s\n", d_dir);
            } else {
                printf("Memory allocation failed.\n");
            }  
            break; 
        }
        case 3: {
             e3 = (char *) malloc(strlen(argv[6]) + 1);
            if (e3 != NULL) {
                strcpy(e3, argv[6]);
                //printf("d_dir: %s\n", d_dir);
            } else {
                printf("Memory allocation failed.\n");
            }
            break;   
        }
        case 4: {
             e4 = (char *) malloc(strlen(argv[7]) + 1);
            if (e4 != NULL) {
                strcpy(e4, argv[7]);
                //printf("d_dir: %s\n", d_dir);
            } else {
                printf("Memory allocation failed.\n");
            }
            break;   
        }
        case 5: {
             e5 = (char *) malloc(strlen(argv[8]) + 1);
            if (e5 != NULL) {
                strcpy(e5, argv[8]);
                //printf("d_dir: %s\n", d_dir);
            } else {
                printf("Memory allocation failed.\n");
            }
            break;   
        }
        case 6: {
             e6 = (char *) malloc(strlen(argv[9]) + 1);
            if (e6 != NULL) {
                strcpy(e6, argv[9]);
                //printf("d_dir: %s\n", d_dir);
            } else {
                printf("Memory allocation failed.\n");
            }
            break;   
        }
        default: break;
    }

    ext_count=0;
    for (int i = 4; i < argc; i++){
        if(i==4){
            extensions[ext_count++] = e1;
        }else if(i==5){
            extensions[ext_count++] = e2;
        }
        else if(i==6){
            extensions[ext_count++] = e3;
        }
        else if(i==7){
            extensions[ext_count++] = e4;
        }else if(i==8){
            extensions[ext_count++] = e5;
        }
        else if(i==9){
            extensions[ext_count++] = e6;
        }
        //printf("ext: %s\n",argv[i]);
    }


    //printf("argv[3]: %s\n",argv[3]);
    op = (char *) malloc(strlen(argv[3]) + 1);
    if (op != NULL) {
        strcpy(op, argv[3]);
        //printf("d_dir: %s\n", d_dir);
    } else {
        printf("Memory allocation failed.\n");
    }

   src_path = argv[1];
   dst_path = argv[2]; 

   //-------------------------------

    char arr[100];
    int ind=0;
    for(int j=0; src_path[j] != '\0'; j++){
        //printf("src_path[j]: %c, j=%d and ind=%d\n",src_path[j],j,ind);
        if(src_path[j] == '/'){
            ind=0;
            for(int i=0; i<strlen(arr); i++){
                arr[i]='\0';
            }
            //printf("\narr: %s, j=%d and ind=%d\n",arr,j,ind);
        }
        else{
            arr[ind]= src_path[j];
            arr[ind+1]='\0';
            ind=ind+1;
        }
       //printf("arr: %s, j=%d and ind=%d\n",arr,j,ind);
    }
    //printf("arr: %s\n",arr);

    dst_path = strcat(dst_path,"/");
    //printf("rand2: %s\n",dst_path);
    dst_path = strcat(dst_path,arr);
    //printf("rand2: %s\n",dst_path);

   // --------------------------

    random_dir = (char *) malloc(strlen(src_path) + 1);
    if (random_dir != NULL) {
        strcpy(random_dir, src_path);
        //printf("d_dir: %s\n", d_dir);
    } else {
        printf("Memory allocation failed.\n");
    }

   random_dir2 = (char *) malloc(strlen(dst_path) + 1);
    if (random_dir2 != NULL) {
        strcpy(random_dir2, dst_path);
        //printf("d_dir: %s\n", d_dir);
    } else {
        printf("Memory allocation failed.\n");
    }

  mk(random_dir2);
  copy_folder(random_dir, random_dir2);


    //printf("argv[3]: %s\n",argv[3]);
    //printf("op: %s\n",op);

    if (strcmp(op, "-cp") == 0)
        copy = 1;
    else if (strcmp(op, "-mv") == 0)
        move = 1;
    else {
        printf("Error: Invalid option %s\n", op);
        return 1;
    }
        
        

    if (nftw(src_path, copy_file, 20, FTW_PHYS) == -1) {
        perror("nftw");
        return 1;
    }


    if(copy==1 && ext_count==0){
        printf("Copied all items...!\n");
    }
    else if(copy==1 && ext_count!=0){
        printf("Copied selected items...!\n");
    }
    else if(move==1 && ext_count==0){
        if (remove_directory(src_path) != 0) {
            perror("remove_directory");
            return 1;
        }
        printf("Moved all items...!\n");
    }
    else{
        if (remove_directory(src_path) != 0) {
            perror("remove_directory");
            return 1;
        }
        printf("Moved selected items and deleted remaining items...!\n");
    }


  return 0;
}
