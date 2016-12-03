// This is part of ronsor-libfindopt-simple.
// License: Ronsor Public License 3.0
#include <string.h>
char *findopt(char *shorty, char *longy, int hasparam, int argc, char *argv[]){
        int i;
        for(i=0;i<argc;i++){

                if(argv[i] != NULL && strlen(argv[i]) > 0 && (strcmp(argv[i], shorty) == 0 || strcmp(argv[i], longy) == 0)){
                        if ( hasparam != 1 ){
                        return "y";
                        } else {
                        return argv[i+1];
                        }
                }
                //printf("%s\n", argv[i]);
        }
        return "n";
}