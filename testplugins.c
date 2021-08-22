#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "./plugins/infocalcfunc.h"

int main()
{
    void *handle1;
    void *handle2;
    int (*calc_func)(long, long, long *const);
//    char *error;

    char libformat[] = ".so";
    struct Info_calc_func *info_calc_func;

    DIR *pdir;
    struct dirent *pdirent;
    
    long a = 11;
    long b = 4;
    long c = 0;

    char *rez_search;

//    printf("strlen(libformat) = %ld, sizeof(libformat) = %ld\n", strlen(libformat),
//            sizeof(libformat[0]));

    if (NULL == (pdir = opendir("./plugins")))
    {
        perror("couldn't open './plugins'");
        exit(EXIT_FAILURE);
    }


    while (NULL != (pdirent = readdir(pdir)))
    {
        rez_search = strstr(pdirent->d_name, libformat);
        if (NULL != rez_search && 3 == strlen(rez_search))
        {
//            printf("strlen(rez_search) = %ld\n", strlen(rez_search));
            printf("%s\n", pdirent->d_name);
        }
    }

    closedir(pdir);

    handle1 = dlopen("./plugins/libfuncadd.so", RTLD_LAZY);
    if (!handle1)
    {
        fprintf(stderr, "Error dlopen(...add)%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    handle2 = dlopen("./plugins/libfuncsub.so", RTLD_LAZY);
    if (!handle2)
    {
        fprintf(stderr, "Error dlopen(...sub)%s\n", dlerror());
        exit(EXIT_FAILURE);
    }


    info_calc_func = dlsym(handle1, "info_calc_func");

    //calc_func = (int (*)(long, long, long *const))dlsym(handle, "my_add");

    calc_func = dlsym(handle1, info_calc_func->calc_func);

    (*calc_func)(a, b, &c);

    printf("Func name = %s  calc_func = %s\n",
            info_calc_func->name,
            info_calc_func->calc_func);

    printf("a = %ld  b = %ld  c = %ld\n", a, b, c);

    info_calc_func = dlsym(handle2, "info_calc_func");

    //calc_func = (int (*)(long, long, long *const))dlsym(handle, "my_add");

    calc_func = dlsym(handle2, info_calc_func->calc_func);

    (*calc_func)(a, b, &c);

    printf("Func name = %s  calc_func = %s\n",
            info_calc_func->name,
            info_calc_func->calc_func);

    printf("a = %ld  b = %ld  c = %ld\n", a, b, c);



    dlerror();
    dlclose(handle1);
    dlerror();
    dlclose(handle2);
}
